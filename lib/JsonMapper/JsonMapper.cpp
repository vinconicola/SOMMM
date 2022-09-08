#include "JsonMapper.hpp"

void json2array(DynamicJsonDocument &doc, const char *settimana_matrix[6][6], const char *today_matrix[10][5])
{

  // Oggetto con i dati della settimana in corso
  JsonObject settimana = doc["settimana"];

  for (JsonPair giorno : settimana)
  {
    int column = 0;
    int row = atoi(giorno.key().c_str()) - 1;
    // se supero le massime dimensioni dell'array salto
    if (row < 0 || row >= 6)
      continue;

    // "esporto" il valore di giorno come oggetto json
    for (JsonPair ora : giorno.value().as<JsonObject>())
    {
      // se supero le massime dimensioni dell'array mi fermo
      if (column >= 6)
        break;
      // salvo il puntatore nella matrice
      settimana_matrix[row][column] = ora.value();
      column++;
    }
  }

  // Oggetto con i dati del giorno in corso
  JsonArray oggi = doc["oggi"];

  int row = 0;
  for (JsonObject ora : oggi)
  {
    int column = 0;
    for (JsonPair pair : ora)
    {
      // se supero le massime dimensioni dell'array mi fermo
      if (column >= 5)
        break;
      // salvo il puntatore nella matrice
      today_matrix[row][column] = pair.value();
      column++;
    }
    // se supero le massime dimensioni dell'array mi fermo
    if (row >= 10)
      break;
    row++;
  }
}
