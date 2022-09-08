/**
 * 
 * 
 *             Smart Orario Management Marconi Magalini
 *                         5AI anno 2k18/2k19
 *           -------------------------------------------
 *                              TEAM 1
 *           -------------------------------------------
 *           *- Riccardo Bussola
 *           *- Cucino Federico
 *           *- Victor Annunziata
 * 
 *            Project : SOMMM x ESP32
 *           @@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@
 *                                                               
          _____                   _______                   _____                    _____                    _____          
         /\    \                 /::\    \                 /\    \                  /\    \                  /\    \         
        /::\    \               /::::\    \               /::\____\                /::\____\                /::\____\        
       /::::\    \             /::::::\    \             /::::|   |               /::::|   |               /::::|   |        
      /::::::\    \           /::::::::\    \           /:::::|   |              /:::::|   |              /:::::|   |        
     /:::/\:::\    \         /:::/~~\:::\    \         /::::::|   |             /::::::|   |             /::::::|   |        
    /:::/__\:::\    \       /:::/    \:::\    \       /:::/|::|   |            /:::/|::|   |            /:::/|::|   |        
    \:::\   \:::\    \     /:::/    / \:::\    \     /:::/ |::|   |           /:::/ |::|   |           /:::/ |::|   |        
  ___\:::\   \:::\    \   /:::/____/   \:::\____\   /:::/  |::|___|______    /:::/  |::|___|______    /:::/  |::|___|______  
 /\   \:::\   \:::\    \ |:::|    |     |:::|    | /:::/   |::::::::\    \  /:::/   |::::::::\    \  /:::/   |::::::::\    \ 
/::\   \:::\   \:::\____\|:::|____|     |:::|    |/:::/    |:::::::::\____\/:::/    |:::::::::\____\/:::/    |:::::::::\____\
\:::\   \:::\   \::/    / \:::\    \   /:::/    / \::/    / ~~~~~/:::/    /\::/    / ~~~~~/:::/    /\::/    / ~~~~~/:::/    /
 \:::\   \:::\   \/____/   \:::\    \ /:::/    /   \/____/      /:::/    /  \/____/      /:::/    /  \/____/      /:::/    / 
  \:::\   \:::\    \        \:::\    /:::/    /                /:::/    /               /:::/    /               /:::/    /  
   \:::\   \:::\____\        \:::\__/:::/    /                /:::/    /               /:::/    /               /:::/    /   
    \:::\  /:::/    /         \::::::::/    /                /:::/    /               /:::/    /               /:::/    /    
     \:::\/:::/    /           \::::::/    /                /:::/    /               /:::/    /               /:::/    /     
      \::::::/    /             \::::/    /                /:::/    /               /:::/    /               /:::/    /      
       \::::/    /               \::/____/                /:::/    /               /:::/    /               /:::/    /       
        \::/    /                 ~~                      \::/    /                \::/    /                \::/    /        
         \/____/                                           \/____/                  \/____/                  \/____/         V 2.0


                                  ./oo/-             `-+++/`                  /ydmdy:         
                                .so++omy           odhddmMN.                ymsohmNMy        
                                .`` .-/m-          N`   /mM+               .m:```:yNd        
                                `::`:-.+.          :.:`-+oy-               .:.:/:shm-        
                                  .`.-`-/` `         ```-`:s-`              `-`-:s+y-         
                              `.:::/--:::../+-    `  o-:/+o+-:/.          ``-///ohdy          
                            `+sdNm/-.:+ohdmNNds-+-  `y/-://-`:Nmo-   `..+syhmooydmmNy/-`      
                          .hdyhmNh:/hmNmddmddNmNds:..m::+y:ydNMMNo -syhNdhmdhhhoydNMMMmdo-`  
                          `yhhdhddhshhhyyymdhmNMMNNdh`dNMdoNMMMMMMNohhddmysyhhyNmmNNmNNmdmNh. 
                          sdhmmdddhyyhhdmNmydNNMMMMNh-yMMymMNmNMMMmNdmmNdsssydddhydddddddMNMm`
                        +hddNNmdmNmmmmm+shyNNMMMMMMh+yMMdMNMMNMMMNNmhdMmdhhhdNdhhshdmmMMMMMMo
                        -dhdNhssysyyhhhyoshdMmNMMMhdmmdMmNmsMs/MMMMMNdNMNNmdmdMdhhdNNNMMMMMMMo
                        +dhyyhmhhyyysysshshNMNNMMMomNNNNNmNmmdNMMMMMNNNMMNmmdmmhdmNMMMMMMNNMMo
                        -mmddhhmmmmmdddhddNMMNNMNNNMMMMMMMMMMMMMMMMMNMMMNmmdmmddmNNMNMMMNNmmNo
                        `:+dNNNmdmmmmNNMMMMMMMMMMNMMMMMMMMMMMMMMMMmddmNmyyyyhyyhyhmNMMMMNmmmo
                          `-yysssssosyhdmmNNmmNNhhNNNNNNNNNNNNNNNdmy/:/ymdhysssyyhhhdmNNmyyydo
 */

#include <Arduino.h>
#include <SPIFFS.h>

#include <ArduinoOTA.h>

#include <WiFi.h>
#include <HTTPClient.h>
#include <ESPAsyncWebServer.h>

#include <ArduinoJson.h>
#include <AsyncJson.h>

#include "page/Page.hpp"
#include <JsonMapper.hpp>

#include <mbedtls/md.h>

// FUNZIONI DEFINITE INIZIALMENTE PER POI ESSERE IMPLEMENTATE
void save_json(AsyncWebServerRequest *richiesta, JsonVariant &json);
void setup_server();
void tabella();                                           // funzione per il disegno della tabella principale
void log_error(String error_m);                           // funzione per il salvataggio di messaggi di log viisibile attraverso il webserver /error_log
void computeSha256(const char *payload, char *shaResult); // funzione che calcola l'hash SHA256 di una stringa

// DEFINIZIONE DELLE VARIABILI GLOBALI NECESSARIE AL SISTEMA
const char *version = "v2.3.0.0 x32";

//CREDENZIALI WEB
const char *www_username = "SOMMM";
const char *www_password = "laPasswordQui";

bool canRequest = false;

String aula_id = "";

#define SLEEP_SWITCH GPIO_NUM_33
unsigned long timeCounter = 0;
const unsigned long delay_time = 300000; // Intervallo di aggiornamento richiesta e display -> 5 minuti

RTC_DATA_ATTR char previousSha[32];

HTTPClient http;
AsyncWebServer server(1518); // settaggio server sulla porta 1518

GxEPD2_BW<GxEPD2_750, GxEPD2_750::HEIGHT> display(GxEPD2_750(15, 27, 26, 25));
PageSystem page(display);

void setup()
{

  // parte iniziale per la seriale
  Serial.begin(115200);
  Serial.println();
  Serial.println("SOMMM STARTUP");
  delay(100);

  page.initDisplay();
  if (esp_sleep_get_wakeup_cause() == ESP_SLEEP_WAKEUP_UNDEFINED)
    page.startup(version);

  pinMode(SLEEP_SWITCH, INPUT_PULLDOWN);

  // Monto il mio SPIFFS File System
  if (!SPIFFS.begin())
  {
    Serial.println("File system non montato ");
    page.error("Errore caricamento File System");
  }
  else
  {
    Serial.println("File system montato");
  }

  // Apro il file config.json in sola lettura
  File config_json_file = SPIFFS.open("/config.json", "r");

  String file_config = config_json_file.readStringUntil('\n'); // Leggo la prima riga del file

  // Creo buffer JSON per la lettura del file config.json
  DynamicJsonDocument jsonRead(1024);

  DeserializationError errorRead = deserializeJson(jsonRead, file_config);
  if (errorRead)
  {
    Serial.print("deserializeJson() line168 failed: ");
    Serial.println(errorRead.c_str());
    Serial.println("Impossibile leggere la configurazione");
    page.error("Errore lettura JSON configurazione");
    return;
  }
  else
  {
    Serial.println("Configurazione correttamente caricata");
    serializeJson(jsonRead, Serial);
    Serial.println("");
  }

  // vado a settare le variabili coi valori caricati dalla memoria

  File pswdFile = SPIFFS.open("/passwd.txt", "r");
  const String net_pswd = pswdFile.readStringUntil('\n');
  pswdFile.close();

  const String net_ssid = jsonRead["net_ssid"];
  const char *api_url = jsonRead["api_url"];
  aula_id = jsonRead["aula"].as<String>(); // Per API raggiungibile a /info

  // Solo se viene richiesto l'ip statico processo i dati
  JsonObject static_config = jsonRead["net_static"].as<JsonObject>();
  if (!static_config.isNull())
  {
    // Memorizzo le chiavi così la matrice sarà sempre nello stesso ordine
    const char *settingsKeys[] = {"net_ip", "net_sm", "net_dfgw", "net_dns"};

    // Copio le impostazioni in una matrice contenente nell'ordine
    // indirizzo ip, subnet mask, default gateway e dns
    uint8_t settings[4][4];
    for (int row = 0; row < 4; row++)
    {
      int column = 0;
      for (JsonVariant settingN : static_config[settingsKeys[row]].as<JsonArray>())
      {
        if (column >= 4)
          break;
        settings[row][column] = settingN.as<int>();
        column++;
      }
    }

    Serial.println("Configurazione statica...");
    IPAddress ip_addr(settings[0]);
    IPAddress sm_addr(settings[1]);
    IPAddress gw_addr(settings[2]);
    IPAddress dns_addr(settings[3]);

    Serial.println(ip_addr);
    Serial.println(sm_addr);
    Serial.println(gw_addr);
    Serial.println(dns_addr);

    if (!WiFi.config(ip_addr, gw_addr, sm_addr, dns_addr))
    { // Configurazione statica del web-server in caso di Client
      Serial.println("Errore configurazione statica");
    }
  }

  WiFi.mode(WIFI_STA);
  WiFi.begin(net_ssid.c_str(), net_pswd.c_str()); // Provo a eseguire una connessione con le credenziali che ho

  Serial.println("Connecting");

  const unsigned long start_c = millis();
  unsigned long counter = 0;

  while (WiFi.status() != WL_CONNECTED && counter < 40000) // Wait for connection
  {
    counter = millis() - start_c;
    Serial.println(counter);
    delay(1000);
    Serial.print(".");
  }

  Serial.println("");
  Serial.println(WiFi.macAddress());
  Serial.println(WiFi.getHostname());

  if (WiFi.status() == WL_CONNECTED)
  {
    // Connessione stabilita
    Serial.println(WiFi.localIP().toString().c_str());

    // Creo l'url per indirizzo API
    String http_address(api_url);
    http_address.concat("?stanza=");
    http_address.concat(aula_id);

    Serial.print("Richiesta settata su: "); // Stampo l'indirizzo
    Serial.println(http_address);

    canRequest = true; // Abilito l'invio del dato

    http.begin(http_address); // configuro e avvio http sul'url precedentemente dichiarato

    // Non configuro server/OTA se e' abilitato il deep sleep
    if (!digitalRead(SLEEP_SWITCH))
    {
      setup_server();

      // Port defaults to 3232
      ArduinoOTA.setPort(1815);

      // Hostname defaults to esp3232-[MAC]
      // ArduinoOTA.setHostname("myesp32");

      // No authentication by default
      ArduinoOTA.setPassword("laPasswordQui");

      // Password can be set with it's md5 value as well
      // MD5(admin) = 21232f297a57a5a743894a0e4a801fc3
      // ArduinoOTA.setPasswordHash("21232f297a57a5a743894a0e4a801fc3");

      ArduinoOTA
          .onStart([]() {
            String type;
            if (ArduinoOTA.getCommand() == U_FLASH)
              type = "sketch";
            else // U_SPIFFS
              type = "filesystem";

            // NOTE: if updating SPIFFS this would be the place to unmount SPIFFS using SPIFFS.end()
            SPIFFS.end();
            Serial.println("Start updating " + type);
          })
          .onEnd([]() {
            Serial.println("\nEnd");
          })
          .onProgress([](unsigned int progress, unsigned int total) {
            Serial.printf("Progress: %u%%\r", (progress / (total / 100)));
          })
          .onError([](ota_error_t error) {
            Serial.printf("Error[%u]: ", error);
            if (error == OTA_AUTH_ERROR)
              Serial.println("Auth Failed");
            else if (error == OTA_BEGIN_ERROR)
              Serial.println("Begin Failed");
            else if (error == OTA_CONNECT_ERROR)
              Serial.println("Connect Failed");
            else if (error == OTA_RECEIVE_ERROR)
              Serial.println("Receive Failed");
            else if (error == OTA_END_ERROR)
              Serial.println("End Failed");
          });

      ArduinoOTA.begin();
    }

    tabella();
    timeCounter = millis();
  }
  else
  {
    // Problemi di connessione (probabilmente rete non raggiungibile e/o settato)

    // Creo una stringa random e la aggiungo al device
    String random_id("SOMMM_");
    for (int i = 0; i < 3; i++)
      random_id.concat(char(random('a', 'z' + 1)));

    // avvio accesss Point
    WiFi.disconnect(true); // Disconnetto la wifi
    WiFi.mode(WIFI_AP);    // Wifi Mode accesss-Point

    WiFi.softAP(random_id.c_str(), "laPasswordQui"); // dichiaro i parametri del mio accesss point

    Serial.println("Access Point Mode");
    Serial.println(WiFi.softAPIP());
    page.access_point(random_id);

    setup_server();
  }

  config_json_file.close();
}

void loop()
{
  if (canRequest)
  {
    if (digitalRead(SLEEP_SWITCH))
    {
      esp_sleep_enable_ext0_wakeup(SLEEP_SWITCH, 0); // Abilito il risveglio tramite pin
      Serial.println("Vado a dormire...");
      ESP.deepSleep(delay_time * 1000); // Tempo in µs
    }

    ArduinoOTA.handle();

    if ((millis() - timeCounter) >= delay_time)
    {
      Serial.println("--------------------");
      tabella();              // Aggiorno il display
      timeCounter = millis(); // Azzerro il contatore
    }
  }
}

void tabella()
{
  // Invio la richiesta alle API
  int httpCode = http.GET();
  String response = http.getString();

  Serial.println(httpCode);
  Serial.println(response);

  http.end();

  if (httpCode < 0)
  {
    page.error("Errore di connessione, verifica la rete");
    log_error("Codice http -> " + String(httpCode));
    delay(5000);
    ESP.restart();
  }

  // TODO: Valutare e implementare, se possibile, un do-while sulla richiesta fino a quando httpCode > 0

  char shaResult[32];
  computeSha256(response.c_str(), shaResult); // Calcolo lo SHA256 della nuova risposta

  // Se corrisponde a quello precedente, esco
  if (!memcmp(previousSha, shaResult, 32))
  {
    Serial.println("Nessun cambiamento rispetto a prima");
    return;
  }
  memcpy(previousSha, shaResult, 32); // Altrimenti salvo il nuovo valore

  // Provo a gestire l'interpretazione del mio json
  const size_t bufferSize = 7 * JSON_ARRAY_SIZE(10) + 10 * JSON_OBJECT_SIZE(5) + 2 * JSON_OBJECT_SIZE(6) + 1050;
  DynamicJsonDocument doc(bufferSize);

  DeserializationError error = deserializeJson(doc, response);
  if (error) // Se errore a elaborare json
  {
    Serial.print("deserializeJson() line552 failed: ");
    Serial.println(error.c_str());
    log_error("Deserializzazione API fallita");
    delay(5000);
    ESP.restart();
  }

  // Dati header

  const char *stanza = doc["stanza"];            // "L145"
  const char *giorno = doc["giorno"];            // "Venerdi', 23 Novembre 2018"
  int giorno_settimana = doc["giornoSettimana"]; // 5
  giorno_settimana -= 1;
  int oraAttuale = doc["oraAttuale"]; // 3

  if (doc["oggi"].size() == 0)
  {
    page.not_school("Oggi non c'e` scuola, buon riposo ;P");
    return;
  }
  else if (oraAttuale == 0) // Giornata terminata
  {
    page.not_school("La giornata scolastica e` terminata.");
    return;
  }

  // Matrice di puntatori a stringhe con le info del giorno in corso
  const char *today_matrix[10][5] = {""};

  // Matrice di puntatori a stringhe con le info della settima in corso
  const char *settimana_matrix[6][6] = {""};

  json2array(doc, settimana_matrix, today_matrix);

  page.tabella(giorno_settimana, oraAttuale, stanza, giorno, today_matrix, settimana_matrix);
}

void save_json(AsyncWebServerRequest *richiesta, JsonVariant &json)
{
  if (!SPIFFS.begin())
  { // controllo di aver accesso al filesystem
    // Se viene visualizzato c'è un problema al filesystem
    richiesta->send(500, "text/plain", "Impossibile leggere la configurazione attualmente memorizzata"); // messaggio di callback per client web

    Serial.println("SPIFFS2 Mount failed");
    page.error("Errore caricamento File System");
    return;
  }
  else
    Serial.println("SPIFFS2 Mount succesfull");

  canRequest = false;

  if (!richiesta->authenticate(www_username, www_password))
    return richiesta->requestAuthentication();

  if (json["net_pswd"] != "")
  {
    // salvo la password su un file non accessibile dalla rete
    File pswdFile = SPIFFS.open("/passwd.txt", "w");
    pswdFile.printf("%s\n", json["net_pswd"].as<char *>());
    pswdFile.close();
  }
  json["net_pswd"] = "x"; // la password non deve essere esposta all'esterno

  File configFile = SPIFFS.open("/config.json", "w");
  serializeJson(json, configFile); // salvo la nuova configurazione
  serializeJson(json, Serial);     // stampo la nuova configurazione

  configFile.close();
  SPIFFS.end();

  // messaggio di callback per client web
  richiesta->send(200, "text/plain", "Salvataggio effettuato correttamente. Riavvia SOMMM.");
  delay(300);
  ESP.restart();
}

void log_error(String error_m)
{
  File log_file = SPIFFS.open("/log.txt", "a");

  const size_t f_size = log_file.size();
  Serial.printf("Dimensione file di log: %d\n", f_size);

  if (f_size > 1000 /* 1000 byte */)
  {
    log_file.close();
    log_file = SPIFFS.open("/log.txt", "w"); // sovrascrivo il contenuto del file
  }

  log_file.printf("%lu -> error: %s\n", millis(), error_m.c_str());
  log_file.close();
}

void setup_server()
{
  server.on("/info", HTTP_GET, [](AsyncWebServerRequest *request) {
    AsyncWebServerResponse *response = request->beginResponse(200, "text/plain", aula_id);
    response->addHeader("Access-Control-Allow-Origin", "*");
    request->send(response);
  });

  server.addHandler(new AsyncCallbackJsonWebHandler("/save", save_json));
  server.serveStatic("/", SPIFFS, "/html/").setDefaultFile("index.html");
  server.serveStatic("/error_log", SPIFFS, "/log.txt");
  server.serveStatic("/config", SPIFFS, "/config.json");
  server.begin(); // Faccio partire il server
}

void computeSha256(const char *payload, char *shaResult)
{
  mbedtls_md_context_t ctx;
  mbedtls_md_type_t md_type = MBEDTLS_MD_SHA256;

  const size_t payloadLength = strlen(payload);

  mbedtls_md_init(&ctx);
  mbedtls_md_setup(&ctx, mbedtls_md_info_from_type(md_type), 0);
  mbedtls_md_starts(&ctx);
  mbedtls_md_update(&ctx, (const unsigned char *)payload, payloadLength);
  mbedtls_md_finish(&ctx, (unsigned char *)shaResult);
  mbedtls_md_free(&ctx);
}
