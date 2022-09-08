#ifndef __PAGE_H__
#define __PAGE_H__

#include <Arduino.h>

#define ENABLE_GxEPD2_GFX 1
#include <GxEPD2_BW.h>

// Font di varia grandezza per il display
#include <Fonts/FreeSans9pt7b.h>
#include <Fonts/FreeSans12pt7b.h>
#include <Fonts/FreeSans18pt7b.h>
#include <Fonts/FreeSansBold9pt7b.h>

#include "images.h"

class PageSystem
{
private:
    GxEPD2_GFX &display;

public:
    PageSystem(GxEPD2_GFX &cose);

    void initDisplay();

    void dithering(int sx, int sy, int w, int h, int percent, int size);

    /**
     * ---------------------------------------------------------------------------------
     * Funzione per mostrare quando non c'è scuola
     */
    void not_school(String frase);

    /**
     *  ---------------------------------------------------------------------------------
     *  Funzione che gestisce la schermata di accensione del SOMMM
     * 
     */
    void startup(const char *version);

    /**
     * ---------------------------------------------------------------------------------------------
     *  Funzione per la comunicazione dell'access_point
     * 
     */
    void access_point(String random_id);

    /**
     * -----------------------------------------------------------------------------------------
     *    Funzione che gestisce la tabella orario del SOMMM
     */
    void tabella(int giorno_settimana, int oraAttuale, const char *stanza, const char *giorno, const char *today_matrix[10][5], const char *settimana_matrix[6][6]);

    /**
     * -----------------------------------------------------------------------------------------
     *    Funzione che gestisce il reboot
     */
    void reboot_page();

    /**
     * -----------------------------------------------------------------------------------------
     *    Funzione che gestisce la pagina errore
     */
    void error(String codice_errore);
};

#endif
