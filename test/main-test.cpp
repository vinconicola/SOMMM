#include <unity.h>
#include <JsonMapper.hpp>

void tabella(void)
{
    const char response[] = "{\"stanza\":\"A303\",\"giorno\":\"Domenica`, 15 Novembre\",\"giornoSettimana\":0,\"oraAttuale\":10,\"oggi\":[],\"settimana\":{\"1\":{\"1\":\"5AI\",\"2\":\"5AI\",\"3\":\"5AI\",\"4\":\"5AI\",\"5\":\"5AI\",\"6\":null,\"7\":null,\"8\":null,\"9\":null,\"10\":null},\"2\":{\"1\":null,\"2\":null,\"3\":null,\"4\":null,\"5\":\"5AI\",\"6\":\"5AI\",\"7\":null,\"8\":null,\"9\":null,\"10\":null},\"3\":{\"1\":\"5AI\",\"2\":\"5AI\",\"3\":\"5AI\",\"4\":\"5AI\",\"5\":\"5AI\",\"6\":\"5AI\",\"7\":null,\"8\":null,\"9\":null,\"10\":null},\"4\":{\"1\":null,\"2\":null,\"3\":null,\"4\":null,\"5\":null,\"6\":null,\"7\":null,\"8\":null,\"9\":null,\"10\":null},\"5\":{\"1\":\"5AI\",\"2\":\"5AI\",\"3\":\"5AI\",\"4\":\"5AI\",\"5\":\"5AI\",\"6\":null,\"7\":null,\"8\":null,\"9\":null,\"10\":null},\"6\":{\"1\":\"5AI\",\"2\":\"5AI\",\"3\":\"5AI\",\"4\":\"5AI\",\"5\":null,\"6\":null,\"7\":null,\"8\":null,\"9\":null,\"10\":null}}}";

    const size_t bufferSize = 7 * JSON_ARRAY_SIZE(10) + 10 * JSON_OBJECT_SIZE(5) + 2 * JSON_OBJECT_SIZE(6) + 1050;
    DynamicJsonDocument doc(bufferSize);

    DeserializationError error = deserializeJson(doc, response);
    if (error) // Se errore a elaborare json
    {
        TEST_FAIL_MESSAGE("Deserializzazione API fallita");
    }
    // Matrice di puntatori a stringhe con le info del giorno in corso
    const char *today_matrix[10][5];

    // Matrice di puntatori a stringhe con le info della settima in corso
    const char *settimana_matrix[6][6];

    json2array(doc, settimana_matrix, today_matrix);

    TEST_ASSERT_EQUAL_STRING_LEN("5AI", settimana_matrix[0][0], 20);
}

int main()
{
    UNITY_BEGIN();
    RUN_TEST(tabella);

    UNITY_END();
}
