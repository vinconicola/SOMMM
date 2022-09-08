void update_display(int page_mode, String string_1, String string_2)
{
  mex_1 = string_1;
  mex_2 = string_2;

  switch (page_mode)
  {
    case 0:
      ePaper.drawPaged(startup); // Modalità logo
      break;
    case 1:
      ePaper.drawPaged(communication); // Modalità comunicazione/errore
      break;
    case 2:
      httpCode = http.GET();
      String response = http.getString();
      payload = response.c_str();

      Serial.println(httpCode);
      Serial.println(payload);

      http.end();

      ePaper.drawPaged(table); // Modalità comunicazione/errore
      break;
  }
}

/**
   Modalità di startup dove viene visualizzato il logo som e iti g. marconi
   @ page_mode 0
*/
void startup()
{
  ePaper.eraseDisplay();
  ePaper.drawBitmap(gImage_marconi, 144, 50, 351, 82, GxEPD_BLACK);
  ePaper.drawBitmap(gImage_logo, 257, 180, 125, 55, GxEPD_BLACK);
}

/**
   Modalità di comunicazione utilizando le due stringhe globali mex_1 e mex_2
   @ page_mode 1
*/
void communication()
{
  ePaper.eraseDisplay();
  ePaper.drawBitmap(gImage_marconi, 144, 50, 351, 82, GxEPD_BLACK);
  ePaper.drawBitmap(gImage_logo, 257, 180, 125, 55, GxEPD_BLACK);

  const GFXfont *f = &FreeSans12pt7b;
  ePaper.setFont(f);
  ePaper.setTextColor(GxEPD_BLACK);

  ePaper.setCursor(100, 330);
  ePaper.println(mex_1);

  ePaper.setCursor(100, 360);
  ePaper.println(mex_2);
}

/**
   Modalità di visualizzazione dell'orario preso da marconi tt
   @ page_mode 2
*/
void table()
{

  /**
    !!!!PAURISSIMAAA!!!!!
    Provo a gestire l'interpretazione del mio json
  */

  const size_t bufferSize = 7 * JSON_ARRAY_SIZE(10) + 10 * JSON_OBJECT_SIZE(5) + 2 * JSON_OBJECT_SIZE(6) + 1050;
  DynamicJsonBuffer jsonBuffer(bufferSize);

  JsonObject &root = jsonBuffer.parseObject(payload);

  // Dati header

  const char *stanza = root["stanza"];            // "L145"
  const char *giorno = root["giorno"];            // "Venerdi', 23 Novembre 2018"
  int giorno_settimana = root["giornoSettimana"]; // 5
  int oraAttuale = root["oraAttuale"];            // 3

  if (httpCode == -1)
  {
    giorno = "Errore di connesione x(";
  }

  // ################################################################################################################
  // CREAZIONE DELL'OGGETTO CONTENENTE I DATI RIGUARDANTI A "OGGI"

  JsonArray &oggi = root["oggi"]; // Oggetto "oggi" contenente tutte le informazioni

  JsonObject &prima = oggi[0];
  JsonObject &seconda = oggi[1];
  JsonObject &terza = oggi[2];
  JsonObject &quarta = oggi[3];
  JsonObject &quinta = oggi[4];
  JsonObject &sesta = oggi[5];
  JsonObject &settima = oggi[6];
  JsonObject &ottava = oggi[7];
  JsonObject &nona = oggi[8];
  JsonObject &decima = oggi[9];

  const char *today_matrix[10][5] = {{prima["ora"], prima["prof1"], prima["prof2"], prima["mat"], prima["res"]},
    {seconda["ora"], seconda["prof1"], seconda["prof2"], seconda["mat"], seconda["res"]},
    {terza["ora"], terza["prof1"], terza["prof2"], terza["mat"], terza["res"]},
    {quarta["ora"], quarta["prof1"], quarta["prof2"], quarta["mat"], quarta["res"]},
    {quinta["ora"], quinta["prof1"], quinta["prof2"], quinta["mat"], quinta["res"]},
    {sesta["ora"], sesta["prof1"], sesta["prof2"], sesta["mat"], sesta["res"]},
    {settima["ora"], settima["prof1"], settima["prof2"], settima["mat"], settima["res"]},
    {ottava["ora"], ottava["prof1"], ottava["prof2"], ottava["mat"], ottava["res"]},
    {nona["ora"], nona["prof1"], nona["prof2"], nona["mat"], nona["res"]},
    {decima["ora"], decima["prof1"], decima["prof2"], decima["mat"], decima["res"]}
  };

  // ################################################################################################################
  // CREAZIONE DELL'OGGETTO CONTENENTE I DATI RIGUARDANTI A "SETTIMANA"

  JsonObject &settimana = root["settimana"]; // Oggetto "settima" contenente tutte le informazioni

  JsonObject &settimana_1 = settimana["1"];
  JsonObject &settimana_2 = settimana["2"];
  JsonObject &settimana_3 = settimana["3"];
  JsonObject &settimana_4 = settimana["4"];
  JsonObject &settimana_5 = settimana["5"];
  JsonObject &settimana_6 = settimana["6"];

  const char *settimana_matrix[6][6] = {{settimana_1["1"], settimana_1["2"], settimana_1["3"], settimana_1["4"], settimana_1["5"], settimana_1["6"]},
    {settimana_2["1"], settimana_2["2"], settimana_2["3"], settimana_2["4"], settimana_2["5"], settimana_2["6"]},
    {settimana_3["1"], settimana_3["2"], settimana_3["3"], settimana_3["4"], settimana_3["5"], settimana_3["6"]},
    {settimana_4["1"], settimana_4["2"], settimana_4["3"], settimana_4["4"], settimana_4["5"], settimana_4["6"]},
    {settimana_5["1"], settimana_5["2"], settimana_5["3"], settimana_5["4"], settimana_5["5"], settimana_5["6"]},
    {settimana_6["1"], settimana_6["2"], settimana_6["3"], settimana_6["4"], settimana_6["5"], settimana_6["6"]}
  };

  // ###########################################################################

  ePaper.eraseDisplay(); //Eliminiamo l'immmagine presente sul dispositivo

  // @@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@
  // Parte di destra

  ePaper.fillRect(335, 0, 305, 60, GxEPD_BLACK);
  ePaper.drawBitmap(gImage_logo, 500, 3, 125, 55, GxEPD_WHITE);
  ePaper.drawFastVLine(335, 0, 330, GxEPD_WHITE);
  ePaper.drawFastVLine(334, 330, 84, GxEPD_BLACK);

  const GFXfont *f = &FreeSans18pt7b;
  ePaper.setFont(f);
  ePaper.setTextColor(GxEPD_WHITE);

  ePaper.setCursor(363, 42);
  ePaper.println(stanza);

  // Disegno i separatori

  ePaper.fillRect(355, 115, 280, 2, GxEPD_BLACK);
  ePaper.fillRect(355, 170, 280, 2, GxEPD_BLACK);
  ePaper.fillRect(355, 225, 280, 2, GxEPD_BLACK);
  ePaper.fillRect(355, 280, 280, 2, GxEPD_BLACK);
  ePaper.fillRect(355, 335, 280, 2, GxEPD_BLACK);

  // Disegno le informazioni ora per ora

  int pos_y[6] = {100, 153, 205, 260, 315, 368};
  ePaper.setTextColor(GxEPD_BLACK);

  if (oraAttuale != 0 && oraAttuale < 5)
  { // mostro le prime 6 ore

    for (int j = 0; j < 6; j++)
    {
      f = &FreeSans9pt7b;
      ePaper.setFont(f);

      ePaper.setCursor(342, 90 + (53 * j) + j);
      ePaper.println(j + 1);

      if (j + 1 == oraAttuale)
      {
        ePaper.fillRect(340, pos_y[j], 15, 5, GxEPD_BLACK);
      }
      else
      {
        ePaper.drawRect(340, pos_y[j], 15, 5, GxEPD_BLACK);
      }

      f = &FreeSans9pt7b;
      ePaper.setFont(f);

      ePaper.setCursor(363, 110 + (53 * j) + j);
      ePaper.println(today_matrix[j][1]); // primo professore

      ePaper.setCursor(363, 85 + (53 * j) + j);
      ePaper.println(today_matrix[j][2]); // secondo professore

      f = &FreeSans9pt7b;
      ePaper.setFont(f);

      ePaper.setCursor(522, 110 + (53 * j) + j);
      ePaper.println(today_matrix[j][3]); // Materia

      f = &FreeSans18pt7b;
      ePaper.setFont(f);

      ePaper.setCursor(573, 110 + (53 * j) + j);
      ePaper.println(today_matrix[j][4]); // Classe
    }
  }
  if (oraAttuale != 0 && oraAttuale >= 5)
  { // mostro le ultime 6 ore

    for (int j = 4; j < 10; j++)
    {
      f = &FreeSans9pt7b;
      ePaper.setFont(f);

      ePaper.setCursor(342, 90 + (53 * (j - 4)) + j);
      if (j + 1 == 10) {
        ePaper.println('X');
      } else {
        ePaper.println((j) + 1);
      }

      if (j + 1 == oraAttuale)
      {
        ePaper.fillRect(340, pos_y[j - 4], 15, 5, GxEPD_BLACK);
      }
      else
      {
        ePaper.drawRect(340, pos_y[j - 4], 15, 5, GxEPD_BLACK);
      }

      f = &FreeSans9pt7b;
      ePaper.setFont(f);

      ePaper.setCursor(363, 110 + (53 * (j - 4)) + j - 4);
      ePaper.println(today_matrix[j][1]); // primo professore

      ePaper.setCursor(363, 85 + (53 * (j - 4)) + j - 4);
      ePaper.println(today_matrix[j][2]); // secondo professore

      f = &FreeSans9pt7b;
      ePaper.setFont(f);

      ePaper.setCursor(522, 110 + (53 * (j - 4)) + j - 4);
      ePaper.println(today_matrix[j][3]); // Materia

      f = &FreeSans18pt7b;
      ePaper.setFont(f);

      ePaper.setCursor(573, 110 + (53 * (j - 4)) + j - 4);
      ePaper.println(today_matrix[j][4]); // Classe
    }
  }
  else if (oraAttuale == 0 && httpCode != -1)
  { // Giornata terminata

    f = &FreeSans9pt7b;
    ePaper.setFont(f);

    ePaper.setCursor(363, 82);
    ePaper.println("Giornata scolastica");

    ePaper.setCursor(363, 107);
    ePaper.println("terminata");
  }

  // @@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@ò
  // Parte di sinistra

  ePaper.fillRect(0, 0, 335, 330, GxEPD_BLACK);

  // Scritte

  f = &FreeSans9pt7b;
  ePaper.setFont(f);
  ePaper.setTextColor(GxEPD_WHITE);

  // Indicatore di ore dei vari giorni

  String num_ore[6] = {"1", "2", "3", "4", "5", "6"};

  for (int i = 0; i < 6; i++)
  {
    ePaper.setCursor(8, 52 + (50 * i));
    ePaper.println(num_ore[i]);
  }

  // Nome dei giorni

  String gior_name[6] = {"LUN", "MAR", "MER", "GIO", "VEN", "SAB"};

  for (int i = 0; i < 6; i++)
  {
    ePaper.setCursor(33 + ((48 * i) + i * 2), 19);
    ePaper.println(gior_name[i]);
  }

  /*
    colonne = 30, 80, 130, 180 , 230,280
    righe = 25,75, 125, 175, 225,275
  */

  for (int j = 0; j < 6; j++)
  {
    for (int i = 0; i < 6; i++)
    {
      if (giorno_settimana == j + 1)
      {
        ePaper.drawBitmap(gImage_round, 30 + (50 * j), 25 + (50 * i), 45, 45, GxEPD_WHITE);
      }
      else
      {
        ePaper.fillRect(30 + (50 * j), 25 + (50 * i), 45, 45, GxEPD_WHITE);
      }
    }
  }

  f = &FreeSans9pt7b;
  ePaper.setFont(f);

  ePaper.setTextColor(GxEPD_BLACK);

  for (int i = 0; i < 6; i++)
  {
    for (int j = 0; j < 6; j++)
    {
      ePaper.setCursor(35 + (50 * j), 55 + (50 * i));
      ePaper.println(settimana_matrix[j][i]); // Settimana giorno per giorno
    }
  }

  // Parte sotto del giorno
  f = &FreeSans12pt7b;
  ePaper.setFont(f);

  ePaper.setCursor(8, 365);
  ePaper.println(giorno); // info giorno
}
