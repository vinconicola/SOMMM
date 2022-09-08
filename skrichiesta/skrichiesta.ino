/*
   ##########################################################################

            Smart Orario Management Marconi Magalini
                          5AI anno 2k18/2k19
            -------------------------------------------
                               TEAM 1
            -------------------------------------------

 *           *- Riccardo Bussola
 *           *- Cucino Federico
 *           *- Victor Annunziata

             Project : SOM^3

             @@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@
          _____                   _______                   _____
         /\    \                 /::\    \                 /\    \
        /::\    \               /::::\    \               /::\____\
       /::::\    \             /::::::\    \             /::::|   |
      /::::::\    \           /::::::::\    \           /:::::|   |
     /:::/\:::\    \         /:::/~~\:::\    \         /::::::|   |
    /:::/__\:::\    \       /:::/    \:::\    \       /:::/|::|   |
    \:::\   \:::\    \     /:::/    / \:::\    \     /:::/ |::|   |
  ___\:::\   \:::\    \   /:::/____/   \:::\____\   /:::/  |::|___|______
  /\   \:::\   \:::\    \ |:::|    |     |:::|    | /:::/   |::::::::\    \
  /::\   \:::\   \:::\____\|:::|____|     |:::|    |/:::/    |:::::::::\____\
  \:::\   \:::\   \::/    / \:::\    \   /:::/    / \::/    / ~~~~~/:::/    /
  \:::\   \:::\   \/____/   \:::\    \ /:::/    /   \/____/      /:::/    /
  \:::\   \:::\    \        \:::\    /:::/    /                /:::/    /
   \:::\   \:::\____\        \:::\__/:::/    /                /:::/    /
    \:::\  /:::/    /         \::::::::/    /                /:::/    /
     \:::\/:::/    /           \::::::/    /                /:::/    /
      \::::::/    /             \::::/    /                /:::/    /
       \::::/    /               \::/____/                /:::/    /
        \::/    /                 ~~                      \::/    /
         \/____/                                           \/____/     v 1.0

   ##########################################################################
*/

// Librerie per la gestione del display e-paper
#include <GxEPD.h>
#include <GxGDEW075T8/GxGDEW075T8.cpp> // 7.5" b/w 640x384

// Librerie per rete
#include <ESP8266WiFi.h>
#include <WiFiClient.h>
#include <ESP8266WebServer.h>
#include <ESP8266HTTPClient.h>

// Libreria per JSON su arduino
#include <ArduinoJson.h>

#include "FS.h"

// Includo immagini precaricate
#include "images.h"

// Font di varia grandezza per il display
#include <Fonts/FreeSans9pt7b.h>
#include <Fonts/FreeSans12pt7b.h>
#include <Fonts/FreeSans18pt7b.h>
#include <Fonts/FreeSansBold9pt7b.h>

#include <GxIO/GxIO_SPI/GxIO_SPI.cpp>
#include <GxIO/GxIO.cpp>

ESP8266WebServer server(1518); //settaggio server sulla porta 1518

// Seguendo il datasheet della ESP8266 driver board di waveshare
GxIO_Class io(SPI, /*CS=D15*/ 15, /*DC=D4*/ 4, /*RST=D5*/ 5);
GxEPD_Class ePaper(io, 5, 16 /*RST=D5*/ /*BUSY=D16*/);

bool request = 0;

String mex_1 = "";        // riga comunicazione 1
String mex_2 = "";        // riga comunicazione 2
const char *payload = ""; // payload come variabile globale
int oraAttuale = 1;       // sisema di switch per le giornate con + di 6 ore
int httpCode = 0;

const char *www_username = "SOMMM";
const char *www_password = "laPasswordQui";

// Dichiarazione di variabili in parte poi caricate da config.js

const char *net_ssid = "";
const char *net_pswd = "";
const char *api_url = "";
const char *aula = "";
String aula_id = "";

int delay_time = 600000; // Intervallo di aggiornamento richiesta e display
bool static_config = 0; // static or DHCP

int ip[4], dns[4], default_gw[4], subnet_m[4];

String getData, Link, file_config;

HTTPClient http;

// Dichiaro funzioni da implementare

void update_display(int page_mode, String string_1, String string_2);
void save_json();

void setup()
{

  Serial.begin(115200); // Inizializzo la seriale

  ePaper.init(115200); // setto la seriale per la diagnostica del display
  //ePaper.setRotation(-1);

  update_display(0, "", ""); // Modalità iniziale accensione del display

  // Monto il mio SPIFFS File System

  if (!SPIFFS.begin())
  {
    Serial.println("File system non montato ");
    update_display(1, "Errore File System", ":X");
  }
  else
  {
    Serial.println("File system montato");
  }

  // Dichiaro che il file da aprire è il config.json

  File config_json_file = SPIFFS.open("/config.json", "r");

  file_config = config_json_file.readStringUntil('\n'); // questo è il nostro config.js salvato su stringa

  // Creo buffer JSON per la lettura del file config.json
  DynamicJsonDocument jsonRead(1024);
  
  DeserializationError errorRead = deserializeJson(jsonRead, file_config);
  if (errorRead)
  {
    Serial.print("deserializeJson() line142 failed: ");
    Serial.println(errorRead.c_str());
    Serial.println("Impossibile leggere la configurazione");
    update_display(1, "Errore di lettura json", ":X");
    return;
  }
  else
  {
    Serial.println("Configurazione correttamente caricata");
    serializeJson(jsonRead, Serial);
    Serial.println("");
  }

  // vado a settare le variabili coi valori caricati dalla memoria

  net_ssid = jsonRead["net_ssid"];
  net_pswd = jsonRead["net_pswd"];
  api_url = jsonRead["api_url"];
  aula = jsonRead["aula"];
  aula_id = String(aula); // Per API raggiungibile a /info

  static_config = jsonRead["net_static"];

  ip[0] = jsonRead["net_ip_0"];
  ip[1] = jsonRead["net_ip_1"];
  ip[2] = jsonRead["net_ip_2"];
  ip[3] = jsonRead["net_ip_3"];

  default_gw[0] = jsonRead["net_dfgw_0"];
  default_gw[1] = jsonRead["net_dfgw_1"];
  default_gw[2] = jsonRead["net_dfgw_2"];
  default_gw[3] = jsonRead["net_dfgw_3"];

  subnet_m[0] = jsonRead["net_sm_0"];
  subnet_m[1] = jsonRead["net_sm_1"];
  subnet_m[2] = jsonRead["net_sm_2"];
  subnet_m[3] = jsonRead["net_sm_3"];

  dns[0] = jsonRead["net_dns_0"];
  dns[1] = jsonRead["net_dns_1"];
  dns[2] = jsonRead["net_dns_2"];
  dns[3] = jsonRead["net_dns_3"];

  if (static_config)
  {
    Serial.println("Configurazione statica....");
    IPAddress ip_addr(ip[0], ip[1], ip[2], ip[3]);
    IPAddress dns_addr(dns[0], dns[1], dns[2], dns[3]);
    IPAddress gw_addr(default_gw[0], default_gw[1], default_gw[2], default_gw[3]);
    IPAddress sm_addr(subnet_m[0], subnet_m[1], subnet_m[2], subnet_m[3]);

    Serial.println(ip_addr);
    Serial.println(gw_addr);
    Serial.println(sm_addr);
    Serial.println(dns_addr);

    if (!WiFi.config(ip_addr, gw_addr, sm_addr, dns_addr))
    { // Configurazione statica del web-server in caso di Client
      Serial.println("Errore configurazione statica");
    }
  }

  WiFi.mode(WIFI_STA);
  WiFi.begin(net_ssid, net_pswd); // Provo a eseguire una connessione con le credenziali che ho

  Serial.print("Connecting");

  long start_c = millis();
  long counter = 0;
  long soglia = 300000; // soglia di controllo per passare il AP (default 25s)

  while (WiFi.status() != WL_CONNECTED) // Wait for connection
  {
    counter += millis() - start_c;
    Serial.println(counter);
    if (counter < soglia)
    {
      delay(1000);
      Serial.print(".");
    }
    else
    {
      break;
    }
  }
  Serial.println("");
  Serial.println(WiFi.macAddress());


  if (WiFi.status() == WL_CONNECTED)
  {
    // Connessione stabilita
    Serial.println(WiFi.localIP().toString().c_str());

    // Setto http sull'indirizzo del mio server

    String http_address = String(api_url) + "?stanza=" + String(aula); // Creo l'url per indirizzo API

    Serial.print("Richiesta settata su: "); // Stampo l'indirizzo
    Serial.println(http_address);

    request = 1; // Abilito l'invio del dato

    http.begin(http_address); // configuro e avvio http sul'url precedentemente dichiarato

    // Dichiaro la struttura del mio filesystem in modo da caricare i file archiviati con SPIFFS
    server.on("/info", []() {
      return server.send(200, "text/plain", aula_id);
    });
    server.on("/save", save_json);
    server.serveStatic("/img", SPIFFS, "/img");
    server.serveStatic("/css", SPIFFS, "/css");
    server.serveStatic("/", SPIFFS, "/index.html");
    server.begin(); //Faccio partire il server

    update_display(2, "", ""); // Mostro l'avvenuto successo della connesione su display e do informazioni utili all'utente
  }
  else
  {
    // Problemi di connessione (probabilmente rete non raggiungibile e/o settato), avvio Access Point
    WiFi.disconnect(true); // Disconnetto la wifi
    WiFi.mode(WIFI_AP); // Wifi Mode Access-Point

    WiFi.softAP("SOMMM", "laPasswordQui"); // dichiaro i parametri del mio access point

    Serial.println("Access Point Mode");
    Serial.println(WiFi.softAPIP());
    update_display(1, "AP SOMMM", "192.168.4.1"); // 192.168.4.1 + porta default
    // Avviso con modalità 1 il display che ho creato un access point

    server.on("/info", []() {
      return server.send(200, "text/plain", aula_id);
    });
    server.on("/save", save_json);
    server.serveStatic("/img", SPIFFS, "/img");
    server.serveStatic("/css", SPIFFS, "/css");
    server.serveStatic("/", SPIFFS, "/index.html");
    server.begin(); // Faccio partire il server
  }
}

long time_start = millis();

void loop()
{
  server.handleClient();

  if (request)
  {
    if (delay_time >= 1000)
    { // verifica se l'update_s è almeno maggiore di 1s
      if ((millis() - time_start) >= delay_time)
      {
        // mando la richiesta
        Serial.println("--------------------");
        update_display(2, "", "");
        time_start = millis(); // azzerro il contatore
      }
    }
  }
}

void save_json()
{
  if (!SPIFFS.begin()) // controllo di aver accesso al filesystem
  {
    // Se viene visualizzato c'è un problema al filesystem
    server.send(500, "text/plain", "Impossibile leggere la configurazione attualmente memorizzata"); // messaggio di callback per client web
    Serial.println("SPIFFS2 Mount failed");
    return;
  }
  else
  { // File system correttamente caricato
    Serial.println("SPIFFS2 Mount succesfull");
  }

  File file_conf_saved = SPIFFS.open("/config.json", "r"); // Apro il file in modalità lettura

  DynamicJsonDocument json(1024); // creo secondo buffer
  String conf_read_file = file_conf_saved.readStringUntil('\n');
  
  DeserializationError errorConf = deserializeJson(json, conf_read_file);
  if (errorConf)
  {
    server.send(500, "text/plain", "Impossibile leggere la configurazione attualmente memorizzata"); // messaggio di callback per client web
    Serial.print("deserializeJson() line327 failed: ");
    Serial.println(errorConf.c_str());
    Serial.println("Impossibile leggere la configurazione");
    return;
  }
  else
  {
    Serial.println("Configurazione attuale caricata correttamente");
    serializeJson(json, Serial);
    Serial.println("");
  }

  // #######################################################
  // controllo e salvataggio dei dati in caso di cambiamento
  // #######################################################

  if (server.arg("net_ssid") != "")
  { // ssid
    json["net_ssid"] = server.arg("net_ssid");
  }

  if (server.arg("net_pswd") != "")
  { // password
    json["net_pswd"] = server.arg("net_pswd");
  }

  if (server.arg("api_url") != "")
  { // codice dispositivo
    json["api_url"] = server.arg("api_url");
  }

  if (server.arg("aula") != "")
  { // url server con api
    json["aula"] = server.arg("aula");
  }

  if (server.arg("net_static") != "")
  { // 1-0 abilita configurazione statica
    json["net_static"] = server.arg("net_static");
  }

  if (server.arg("net_ip_0") != "")
  { // ip[0]
    json["net_ip_0"] = server.arg("net_ip_0");
  }

  if (server.arg("net_ip_1") != "")
  { // ip[1]
    json["net_ip_1"] = server.arg("net_ip_1");
  }

  if (server.arg("net_ip_2") != "")
  { // ip[2]
    json["net_ip_2"] = server.arg("net_ip_2");
  }

  if (server.arg("net_ip_3") != "")
  { // ip[3]
    json["net_ip_3"] = server.arg("net_ip_3");
  }

  if (server.arg("net_dns_0") != "")
  { // dns[0]
    json["net_dns_0"] = server.arg("net_dns_0");
  }

  if (server.arg("net_dns_1") != "")
  { // dns[1]
    json["net_dns_1"] = server.arg("net_dns_1");
  }

  if (server.arg("net_dns_2") != "")
  { // dns[2]
    json["net_dns_2"] = server.arg("net_dns_2");
  }

  if (server.arg("net_dns_3") != "")
  { // dns[3]
    json["net_dns_3"] = server.arg("net_dns_3"); 
  }

  request = 0;

  delay(100); // aspetto che tutto sia correttamente settato e poi scrivo

  if (!server.authenticate(www_username, www_password))
  {
    return server.requestAuthentication();
  }

  File save = SPIFFS.open("/config.json", "w"); // Apro il file in modalità scrittura

  delay(200);
  serializeJson(json, save);   // salvo la nuova configurazione
  serializeJson(json, Serial); // stampo la nuova configurazione

  server.send(200, "text/plain", "Salvataggio effettuato correttamente. Riavvia SOMMM appena led rosso spento"); // messaggio di callback per client web
  Serial.println("");
  Serial.println("Riavvia il dispositivo appena led rosso spento");
  update_display(1, "Salvataggio effettuato", "Riavvia SOMM appena led rosso spento"); // Riavvio dispositivo

  // N.B. No reboot lato software per problemi successivi a stack di memoria, riavvio hardware
}
