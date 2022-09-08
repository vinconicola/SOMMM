#include "Page.hpp"

PageSystem::PageSystem(GxEPD2_GFX &cose) : display(cose) {}

void PageSystem::initDisplay()
{
    display.init(115200);
    SPI.end();                 // release standard SPI pins, e.g. SCK(18), MISO(19), MOSI(23), SS(5)
    SPI.begin(13, 12, 14, 15); // map and init SPI pins SCK(13), MISO(12), MOSI(14), SS(15)
}

void PageSystem::dithering(int sx, int sy, int w, int h, int percent, int size)
{
    switch (percent)
    {
    // 75% (DARK GREY)
    case (75):

        for (int y = sy; y < sy + h; y++)
        {
            for (int x = sx; x < sx + w; x++)
            {
                display.fillRect(x, y, size, size, (y % 2 == 0) ? ((x % 2 == 0) ? GxEPD_WHITE : GxEPD_BLACK) : GxEPD_BLACK);
            }
        }
        break;

    // 50% (MIDDLE GREY)
    case (50):

        for (int y = sy; y < sy + h; y++)
        {
            for (int x = sx; x < sx + w; x++)
            {
                display.fillRect(x, y, size, size, (y % 2 == 0) ? ((x % 2 == 0) ? GxEPD_WHITE : GxEPD_BLACK) : ((x % 2 == 0) ? GxEPD_BLACK : GxEPD_WHITE));
            }
        }
        break;

        // 25% (DARK GREY)
    case (25):

        for (int y = sy; y < sy + h; y++)
        {
            for (int x = sx; x < sx + w; x++)
            {
                display.fillRect(x, y, size, size, (y % 2 == 0) ? GxEPD_WHITE : ((x % 2 == 0) ? GxEPD_BLACK : GxEPD_WHITE));
            }
        }
        break;
    }
}

void PageSystem::not_school(String frase)
{
    display.setRotation(0);
    display.setFullWindow();
    display.firstPage();
    do
    {
        display.fillScreen(GxEPD_WHITE);
        display.setFont(&FreeSans18pt7b);
        display.setTextColor(GxEPD_BLACK);

        // LOGO SMALLL IN 3D
        display.drawBitmap(468, 5, gImage_s_shadow, 163, 45, GxEPD_BLACK); // shadow
        display.drawBitmap(468, 5, gImage_s_text, 163, 45, GxEPD_WHITE);   // text

        display.setCursor(55, 135);
        display.println(frase);
        display.setFont(&FreeSans12pt7b);
        display.setCursor(55, 170);
        display.println("Il team SOMMM vi augura una buona giornata!");

        display.setFont(&FreeSans9pt7b);

        display.setCursor(55, 200);
        display.println("developed by V. Annunziata, R. Bussola, F. Cucino ");

        display.drawBitmap(211, 277, gImage_dev, 224, 107, GxEPD_BLACK);

    } while (display.nextPage());
}

/**
 *  ---------------------------------------------------------------------------------
 *  Funzione che gestisce la schermata di accensione del SOMMM
 * 
 */
void PageSystem::startup(const char *version)
{
    display.setRotation(0);
    display.setFullWindow();
    display.firstPage();
    do
    {
        display.fillScreen(GxEPD_WHITE);
        dithering(0, 0, 640, 384, 25, 1);

        // LOGO IN 3D
        display.drawBitmap(145, 144, gImage_sommm_shadow, 350, 95, GxEPD_BLACK); //shadow
        display.drawBitmap(145, 144, gImage_sommm_text, 350, 95, GxEPD_WHITE);   //text

        display.setFont(&FreeSans12pt7b);
        display.setCursor(142, 297);
        display.setTextColor(GxEPD_BLACK);
        display.println("The new way to manage your time");

        display.setCursor(10, 25);
        display.println(version);

        //------shadow
        display.setCursor(140, 295);
        display.setTextColor(GxEPD_WHITE);
        display.println("The new way to manage your time");

    } while (display.nextPage());
}

/**
 * ---------------------------------------------------------------------------------------------
 *  Funzione per la comunicazione dell'access_point
 * 
 */
void PageSystem::access_point(String random_id)
{
    display.setRotation(0);
    display.setFullWindow();
    display.firstPage();
    do
    {
        display.fillScreen(GxEPD_WHITE);
        dithering(0, 0, 640, 384, 25, 1);

        // LOGO SMALLL IN 3D
        display.drawBitmap(468, 10, gImage_s_shadow, 163, 45, GxEPD_BLACK); // shadow
        display.drawBitmap(468, 10, gImage_s_text, 163, 45, GxEPD_WHITE);   // text

        // CHROME TAB (fatta di primitive; paurissima)

        dithering(175, 295, 145, 50, 50, 1);
        display.fillRect(0, 324, 640, 60, GxEPD_BLACK);         // base della scheda
        display.fillRoundRect(0, 285, 175, 60, 5, GxEPD_BLACK); // indicatore tab principlae
        display.fillRoundRect(25, 340, 450, 30, 100, GxEPD_WHITE);

        display.setFont(&FreeSans12pt7b);
        display.setCursor(35, 313);
        display.setTextColor(GxEPD_WHITE);
        display.print("SOMMM");

        display.setCursor(35, 363);
        display.setTextColor(GxEPD_BLACK);
        display.print("192.168.4.1");

        // Scritta Guida

        // Titolo

        display.setFont(&FreeSans18pt7b);
        display.setCursor(15, 110);
        display.setTextColor(GxEPD_BLACK);
        display.print("Configurazione");

        display.setFont(&FreeSans12pt7b);
        display.setCursor(15, 150);

        display.setTextColor(GxEPD_BLACK);

        display.print("Connettersi alla rete \"" + random_id + "\" e aprire il browser.");
        display.setCursor(15, 175);
        display.print("Digitare 192.168.4.1 e compilare i vari campi.");
        display.setCursor(15, 200);
        display.print("Premere Salva e aspettare la conferma dal device;");
        display.setCursor(15, 225);
        display.print("una volta ricevuta la conferma riavviare il sistema.");

    } while (display.nextPage());
}

/**
 * -----------------------------------------------------------------------------------------
 *    Funzione che gestisce la tabella orario del SOMMM
 */
void PageSystem::tabella(int giorno_settimana, int oraAttuale, const char *stanza, const char *giorno, const char *today_matrix[10][5], const char *settimana_matrix[6][6])
{

    display.setRotation(0);
    display.setFullWindow();
    display.firstPage();

    do
    {
        display.fillScreen(GxEPD_WHITE);

        // ---------------------------------------------------------------------
        //                    LATO DESTRO DEL DISPLAY
        //----------------------------------------------------------------------

        dithering(335, 0, 305, 60, 50, 1);

        // LOGO SMALLL IN 3D
        display.drawBitmap(468, 5, gImage_s_shadow, 163, 45, GxEPD_BLACK); // shadow
        display.drawBitmap(468, 5, gImage_s_text, 163, 45, GxEPD_WHITE);   // text

        // Scrivo la stanza

        display.setFont(&FreeSans18pt7b);
        display.setTextColor(GxEPD_WHITE);

        display.setCursor(363, 42);
        display.println(stanza);

        // Disegno i separatori

        display.fillRoundRect(355, 115, 280, 2, 10, GxEPD_BLACK);
        display.fillRoundRect(355, 170, 280, 2, 10, GxEPD_BLACK);
        display.fillRoundRect(355, 225, 280, 2, 10, GxEPD_BLACK);
        display.fillRoundRect(355, 280, 280, 2, 10, GxEPD_BLACK);
        display.fillRoundRect(355, 335, 280, 2, 10, GxEPD_BLACK);

        int pos_y[6] = {100, 153, 205, 260, 315, 368};
        display.setTextColor(GxEPD_BLACK);

        int offset = 0;
        if (oraAttuale > 4)
            offset = 4;

        for (int j = 0; j < 6; j++)
        {
            display.setFont(&FreeSans9pt7b);

            display.setCursor(342, 90 + 54 * j);
            display.println(j + offset + 1);

            if (j + 1 == oraAttuale)
                display.fillRect(340, pos_y[j], 15, 5, GxEPD_BLACK); // Evidenzio l'ora attuale
            else
                display.drawRect(340, pos_y[j], 15, 5, GxEPD_BLACK);

            display.setCursor(363, 110 + 54 * j);
            display.println(today_matrix[j + offset][1]); // primo professore

            display.setCursor(363, 85 + 54 * j);
            display.println(today_matrix[j + offset][2]); // secondo professore

            display.setCursor(522, 110 + 54 * j);
            display.println(today_matrix[j + offset][3]); // Materia

            display.setFont(&FreeSans18pt7b);

            display.setCursor(573, 110 + 54 * j);
            display.println(today_matrix[j + offset][4]); // Classe
        }

        // ---------------------------------------------------------------------
        //                    LATO SINISTRO DEL DISPLAY
        // ----------------------------------------------------------------------

        dithering(0, 0, 335, 331, 75, 1); // SFONDO GRIGIO 75%
        display.drawFastVLine(335, 0, 331, GxEPD_BLACK);
        display.drawFastVLine(335, 332, 53, GxEPD_BLACK);

        // GRIGLIA GIORNI

        display.setFont(&FreeSans12pt7b);
        display.setTextColor(GxEPD_WHITE);

        // Indicatore di ore dei vari giorni
        for (int ora = 0; ora < 6; ora++)
        {
            display.setCursor(7, 52 + (50 * ora));
            display.println(ora + 1);
        }

        // Nome dei giorni
        display.setFont(&FreeSans9pt7b);
        const char *gior_name[6] = {"LUN", "MAR", "MER", "GIO", "VEN", "SAB"};

        for (int giorno = 0; giorno < 6; giorno++)
        {
            display.setCursor(33 + 50 * giorno, 18);
            display.println(gior_name[giorno]);
        }

        // Disegno la griglia
        for (int giorno = 0; giorno < 6; giorno++)
        {
            if (giorno == giorno_settimana)
                continue;
            for (int ora = 0; ora < 6; ora++)
            {
                display.fillRoundRect(30 + (50 * giorno), 25 + (50 * ora), 47, 47, 5, GxEPD_BLACK);
                display.fillRoundRect(31 + (50 * giorno), 26 + (50 * ora), 45, 45, 5, GxEPD_WHITE);
            }
        }

        // Evidenzio il giorno corrente
        for (int ora = 0; ora < 6; ora++)
        {
            display.fillRoundRect(30 + (50 * giorno_settimana), 25 + (50 * ora), 47, 47, 5, GxEPD_WHITE);
            display.fillRoundRect(31 + (50 * giorno_settimana), 26 + (50 * ora), 45, 45, 5, GxEPD_BLACK);
        }

        // Scrivo la classe in ogni ora del giorno
        display.setFont(&FreeSans9pt7b);
        for (int i = 0; i < 6; i++)
        {
            for (int j = 0; j < 6; j++)
            {
                display.setCursor(35 + (50 * j), 55 + (50 * i));
                if (giorno_settimana == j)
                    display.setTextColor(GxEPD_WHITE);
                else
                    display.setTextColor(GxEPD_BLACK);
                display.println(settimana_matrix[j][i]); // Settimana giorno per giorno
            }
        }

        display.setTextColor(GxEPD_BLACK);

        // Parte sotto del giorno
        display.setFont(&FreeSans12pt7b);

        display.setCursor(8, 365);
        display.println(giorno); // info giorno

    } while (display.nextPage());
}

/**
 * -----------------------------------------------------------------------------------------
 *    Funzione che gestisce il reboot
 */
void PageSystem::reboot_page()
{

    display.setRotation(0);
    display.setFullWindow();
    display.firstPage();
    do
    {
        display.fillScreen(GxEPD_WHITE);
        dithering(0, 0, 640, 384, 25, 1);

        display.setTextColor(GxEPD_BLACK);
        display.setFont(&FreeSans12pt7b);
        display.setCursor(50, 180);
        display.println("Attendere la conferma dalla pagina web;");
        display.setCursor(50, 210);
        display.println("successivamente riavviare il dispositivo per");
        display.setCursor(50, 240);
        display.println("caricare le nuove impostazioni.");

        // LOGO IN 3D
        display.drawBitmap(50, 50, gImage_sommm_shadow, 350, 95, GxEPD_BLACK); //shadow
        display.drawBitmap(50, 50, gImage_sommm_text, 350, 95, GxEPD_WHITE);   //text

        display.fillRoundRect(550, 62, 65, 260, 10, GxEPD_WHITE);
        display.drawBitmap(550, 62, gImage_reboot, 65, 260, GxEPD_BLACK);

    } while (display.nextPage());
}

/**
 * -----------------------------------------------------------------------------------------
 *    Funzione che gestisce la pagina errore
 */
void PageSystem::error(String codice_errore)
{
    display.setRotation(0);
    display.setFullWindow();
    display.firstPage();
    do
    {
        display.fillScreen(GxEPD_WHITE);

        display.setTextColor(GxEPD_BLACK);
        display.setFont(&FreeSans12pt7b);

        dithering(0, 0, 640, 384, 25, 1);
        display.drawChar(140, 150, 'X', GxEPD_BLACK, GxEPD_WHITE, 4);
        display.drawChar(440, 150, 'X', GxEPD_BLACK, GxEPD_WHITE, 4);

        display.setCursor(80, 250);
        display.println("Ops! Sembra che qualcosa sia andato storto!");
        display.setCursor(100, 285);
        display.println(codice_errore);

    } while (display.nextPage());
};
