#ifndef __JSON2ARRAY_H__
#define __JSON2ARRAY_H__

#include <ArduinoJson.h>

void json2array(DynamicJsonDocument &doc, const char *settimana_matrix[6][6], const char *today_matrix[10][5]);
#endif
