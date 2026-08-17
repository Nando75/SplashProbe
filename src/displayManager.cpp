#include "DisplayManager.h"

// Costruttore con lista di inizializzazione corretta:
DisplayManager::DisplayManager() 
    : epd2_driver(EDP_CS, EDP_DC, EDP_RST, EDP_BUSY),  // Inizializza il driver con i PIN
      display(epd2_driver)                              // Passa l'istanza del driver al display
{
    // Eventuale codice aggiuntivo del costruttore
}


void DisplayManager::init() {
      display.init(115200,true,50,false);
      display.setRotation(1);
      clear();
}

void DisplayManager::clear() {
    display.clearScreen();
}




void DisplayManager::UpdateScreen(float airTemp, float waterTemp, float battVoltage, int battPercentage) {
    // Imposta la rotazione orizzontale (1 o 3 a seconda del senso in cui hai ruotato il display)
    display.setRotation(1); 
    display.setFullWindow();

    int screenW = display.width();   // 250 px
    int screenH = display.height();  // 122 px

    int margin = 4;
    int gap = 4;
    int cornerRadius = 6;

    // Dimensioni dei 3 Box affiancati
    int boxH = screenH - (margin * 2); // 114 px di altezza per tutti i box
    
    int boxW_batt = 44; // Box 3 (Batteria): più stretto sulla destra
    int boxW_temp = (screenW - (margin * 2) - (gap * 2) - boxW_batt) / 2; // ~95 px ciascuno

    // Coordinate X d'inizio dei 3 box
    int x1 = margin;                            // AIR
    int x2 = x1 + boxW_temp + gap;              // WATER
    int x3 = x2 + boxW_temp + gap;              // BATTERY
    int y  = margin;

    int16_t tbx, tby;
    uint16_t tbw_num, tbh_num, tbw_c, tbh_c;

    display.firstPage();
    do {
        // --- 0. SFONDO GENERALE NERO ---
        display.fillScreen(GxEPD_BLACK);

        // ====================================================
        // Lambda / Helper locale per formattare il valore °C
        // ====================================================
        auto drawDegreeVal = [&](float tempVal, int boxX, int boxW) {
            String valStr = String(tempVal, 1);
            String cStr = (dataMode == Celsius) ? "C" : "F";

            display.setFont(&Seven_Segment20pt7b);
            display.setTextColor(GxEPD_BLACK);

            // Calcolo ingombri
            display.getTextBounds(valStr, 0, 0, &tbx, &tby, &tbw_num, &tbh_num);
            display.getTextBounds(cStr, 0, 0, &tbx, &tby, &tbw_c, &tbh_c);

            int circleRadius = 3; 
            int gap = 3;
            
            int totalW = tbw_num + gap + (circleRadius * 2) + gap + tbw_c;
            int startX = boxX + (boxW - totalW) / 2;
            int baseBaselineY = y + 70; // Posizionato bene in basso nel box

            // 1. Numero
            display.setCursor(startX, baseBaselineY);
            display.print(valStr);

            // 2. Cerchio Gradi
            int circleCenterX = startX + tbw_num + gap + circleRadius + 2;
            int circleCenterY = baseBaselineY - tbh_num + circleRadius + 4;

            display.drawCircle(circleCenterX, circleCenterY, circleRadius, GxEPD_BLACK);
            display.drawCircle(circleCenterX, circleCenterY, circleRadius - 1, GxEPD_BLACK);

            // 3. Lettera 'C' o 'F'

            int cX = circleCenterX + circleRadius + gap;
             display.setFont(&Seven_Segment7pt7b);
            display.setCursor(cX, baseBaselineY);
            display.print(cStr);
        };

        // ====================================================
        // 1. BOX AIR TEMP
        // ====================================================
        display.fillRoundRect(x1, y, boxW_temp, boxH, cornerRadius, GxEPD_WHITE);
        display.drawRoundRect(x1 + 2, y + 2, boxW_temp - 4, boxH - 4, cornerRadius - 1, GxEPD_BLACK);
        display.drawRoundRect(x1 + 3, y + 3, boxW_temp - 6, boxH - 6, cornerRadius - 2, GxEPD_BLACK);

        // Titolo AIR
        String airTitle = String(airText[Lang]);
        display.setFont(&FreeSansBold9pt7b);
        display.setTextColor(GxEPD_RED);
        display.getTextBounds(airTitle, 0, 0, &tbx, &tby, &tbw_num, &tbh_num);
        display.setCursor(x1 + (boxW_temp - tbw_num) / 2 - tbx, y + 25);
        display.print(airTitle);

        // Valore Temperatura
        drawDegreeVal(airTemp, x1, boxW_temp);

        // ====================================================
        // 2. BOX WATER TEMP
        // ====================================================
        display.fillRoundRect(x2, y, boxW_temp, boxH, cornerRadius, GxEPD_WHITE);
        display.drawRoundRect(x2 + 2, y + 2, boxW_temp - 4, boxH - 4, cornerRadius - 1, GxEPD_BLACK);
        display.drawRoundRect(x2 + 3, y + 3, boxW_temp - 6, boxH - 6, cornerRadius - 2, GxEPD_BLACK);

        // Titolo WATER
        String waterTitle = String(waterText[Lang]);
        display.setFont(&FreeSansBold9pt7b);
        display.setTextColor(GxEPD_RED);
        display.getTextBounds(waterTitle, 0, 0, &tbx, &tby, &tbw_num, &tbh_num);
        display.setCursor(x2 + (boxW_temp - tbw_num) / 2 - tbx, y + 25);
        display.print(waterTitle);

        // Valore Temperatura
        drawDegreeVal(waterTemp, x2, boxW_temp);

        // ====================================================
        // 3. BOX BATTERIA VERTICALE
        // ====================================================
        battPercentage = constrain(battPercentage, 0, 100);
        uint16_t battBoxColor = (battPercentage <= 20) ? GxEPD_RED : GxEPD_BLACK;

        display.fillRoundRect(x3, y, boxW_batt, boxH, cornerRadius, GxEPD_WHITE);
        display.drawRoundRect(x3 + 2, y + 2, boxW_batt - 4, boxH - 4, cornerRadius - 1, battBoxColor);
        display.drawRoundRect(x3 + 3, y + 3, boxW_batt - 6, boxH - 6, cornerRadius - 2, battBoxColor);

        // --- ICONA BATTERIA VERTICALE ---
        int batW = 20;  // Larghezza sagoma
        int batH = 42;  // Altezza sagoma
        int batPinH = 4; // Polo positivo in alto
        
        int batX = x3 + (boxW_batt - batW) / 2;
        int batY = y + 12;

        // Polo positivo (Punta in alto)
        display.fillRect(batX + (batW - 8) / 2, batY, 8, batPinH, GxEPD_BLACK);
        
        // Corpo esterno batteria
        int bodyY = batY + batPinH;
        display.drawRect(batX, bodyY, batW, batH, GxEPD_BLACK);
        display.drawRect(batX + 1, bodyY + 1, batW - 2, batH - 2, GxEPD_BLACK);

        // Segmenti Verticali (dal basso verso l'alto)
        int activeSegments = (battPercentage + 10) / 25;
        activeSegments = constrain(activeSegments, 0, 4);

        int segW = batW - 6; // 14px
        int segH = 7;
        int segGap = 2;

        for (int i = 0; i < 4; i++) {
            // i=0 è il segmento più in basso, i=3 quello più in alto
            int segX = batX + 3;
            int segY = bodyY + batH - 3 - (i + 1) * segH - (i * segGap);

            if (i < activeSegments) {
                uint16_t segColor = (battPercentage <= 20) ? GxEPD_RED : GxEPD_BLACK;
                display.fillRect(segX, segY, segW, segH, segColor);
            } else {
                display.drawRect(segX, segY, segW, segH, GxEPD_BLACK);
            }
        }

        // --- SCRITTE PERC % E VOLT (Sotto la batteria) ---
        float vVal = (battVoltage > 15.0f) ? (battVoltage / 1000.0f) : battVoltage;
        
        String pctStr = String(battPercentage) + "%";
        String voltStr = String(vVal, 1) + "V";

        display.setFont(&Seven_Segment7pt7b);
        display.setTextColor(battBoxColor);

        // Percentuale (Centrata)
        display.getTextBounds(pctStr, 0, 0, &tbx, &tby, &tbw_c, &tbh_c);
        display.setCursor(x3 + (boxW_batt - tbw_c) / 2 - tbx, bodyY + batH + 20);
        display.print(pctStr);

        // Volt (Centrati sotto la percentuale)
        display.getTextBounds(voltStr, 0, 0, &tbx, &tby, &tbw_c, &tbh_c);
        display.setCursor(x3 + (boxW_batt - tbw_c) / 2 - tbx, bodyY + batH + 38);
        display.print(voltStr);

        // ====================================================
        // 4. BANNER LOW BATTERY (Se tensione <= 3.3V)
        // ====================================================
        if (vVal <= 3.3f) {
            String lowBattMsg = String(lowBatt[Lang]);
            int alertW = 150;  // Larghezza banner
            int alertH = 22;   // Altezza banner
            int alertRadius = 5;
            
            // Centrato orizzontalmente nell'area delle temperature (x1 e x2)
            int alertX = x1 + ((x2 + boxW_temp - x1) - alertW) / 2;
            int alertY = y + boxH - alertH - 10; // Posizionato in basso dentro i box

            // Box nero principale
            display.fillRoundRect(alertX, alertY, alertW, alertH, alertRadius, GxEPD_BLACK);
            
            // Bordi interni bianchi (doppio bordo)
            display.drawRoundRect(alertX + 1, alertY + 1, alertW - 2, alertH - 2, alertRadius - 1, GxEPD_WHITE);
            display.drawRoundRect(alertX + 2, alertY + 2, alertW - 4, alertH - 4, alertRadius - 2, GxEPD_WHITE);

            // Testo BIANCO
            display.setFont(&Seven_Segment7pt7b);
            display.setTextColor(GxEPD_WHITE);




            display.getTextBounds(lowBattMsg, 0, 0, &tbx, &tby, &tbw_c, &tbh_c);
            
            // Centratura del testo nel banner
            int textX = alertX + (alertW - tbw_c) / 2 - tbx;
            int textY = alertY + (alertH - tbh_c) / 2 - tby;
            
            display.setCursor(textX, textY);
            display.print(lowBattMsg);
        }



    } while (display.nextPage());

    display.hibernate();
}






/*
//DRAW VERTICAL SCREEN:
void DisplayManager::UpdateScreen(float airTemp, float waterTemp, float battVoltage, int battPercentage) {
    display.setFullWindow();

    int w = display.width();   // 122
    int h = display.height();  // 250

    int marginX = 6;
    int boxW = w - (marginX * 2); // 110 px
    int cornerRadius = 6;

    // Altezze dei box
    int boxH_temp = 72; // Riquadri Temperatura
    int boxH_batt = 50; // Riquadro Batteria ridotto

    // Posizioni Y abbassate (più in alto numericamente per lasciare spazio SOTTO)
    int y1 = 4;                  // Box AIR
    int y2 = y1 + boxH_temp + 5; // Box WATER (~95px)
    int y3 = y2 + boxH_temp + 5; // Box BATT (~186px, termina a ~232px lasciando ~18px neri sotto)

    int16_t tbx, tby;
    uint16_t tbw_num, tbh_num, tbw_c, tbh_c;

    display.firstPage();
    do {
        // --- 0. SFONDO GENERALE NERO ---
        display.fillScreen(GxEPD_BLACK);

        // ====================================================
        // Lambda / Helper locale per formattare il valore °C con cerchio a doppio bordo
        // ====================================================
        auto drawDegreeVal = [&](float tempVal, int boxY) {
            String valStr = String(tempVal, 1);
            String cStr = (dataMode == Celsius) ? "C" : "F";

            display.setFont(&Seven_Segment20pt7b);
            display.setTextColor(GxEPD_BLACK);

            // Calcolo ingombri per centratura
            display.getTextBounds(valStr, 0, 0, &tbx, &tby, &tbw_num, &tbh_num);
            display.getTextBounds(cStr, 0, 0, &tbx, &tby, &tbw_c, &tbh_c);

            int circleRadius = 3;  // Raggio cerchietto gradi
            int gap = 5;           // Spazio tra numero, cerchietto e 'C'
            
            // Larghezza totale = Numero + Spazio + Cerchio(diametro) + Spazio + Lettera C
            int totalW = tbw_num + gap + (circleRadius * 2) + gap + tbw_c;
            int startX = marginX + (boxW - totalW) / 2;
            int baseBaselineY = boxY + 60;

            // 1. Disegno del numero
            display.setCursor(startX, baseBaselineY);
            display.print(valStr);

            // 2. Disegno Cerchio Gradi con Doppio Bordo
            int circleCenterX = startX + tbw_num + gap + circleRadius;
            int circleCenterY = baseBaselineY - tbh_num + circleRadius + 2; // Allineato in alto rispetto al numero

            display.drawCircle(circleCenterX, circleCenterY, circleRadius, GxEPD_BLACK);       // Bordo esterno
            display.drawCircle(circleCenterX, circleCenterY, circleRadius - 1, GxEPD_BLACK);   // Bordo interno (Doppio bordo)

            // 3. Disegno della 'C'
            int cX = circleCenterX + circleRadius + gap;
            display.setCursor(cX, baseBaselineY);
            display.print(cStr);
        };

        // ====================================================
        // 1. BOX AIR TEMP
        // ====================================================
        display.fillRoundRect(marginX, y1, boxW, boxH_temp, cornerRadius, GxEPD_WHITE);
        display.drawRoundRect(marginX + 2, y1 + 2, boxW - 4, boxH_temp - 4, cornerRadius - 1, GxEPD_BLACK);
        display.drawRoundRect(marginX + 3, y1 + 3, boxW - 6, boxH_temp - 6, cornerRadius - 2, GxEPD_BLACK);

        // Titolo "AIR" (Centrato)
        String airTitle = String(airText[Lang]);

        display.setFont(&FreeSansBold9pt7b);
        display.setTextColor(GxEPD_RED);
        display.getTextBounds(airTitle, 0, 0, &tbx, &tby, &tbw_num, &tbh_num);
        display.setCursor(marginX + (boxW - tbw_num) / 2 - tbx, y1 + 22);
        display.print(airTitle);
        // Valore Temperatura Air con °C centrato
        drawDegreeVal(airTemp, y1);
        // ====================================================
        // 2. BOX WATER TEMP
        // ====================================================
        display.fillRoundRect(marginX, y2, boxW, boxH_temp, cornerRadius, GxEPD_WHITE);
        display.drawRoundRect(marginX + 2, y2 + 2, boxW - 4, boxH_temp - 4, cornerRadius - 1, GxEPD_BLACK);
        display.drawRoundRect(marginX + 3, y2 + 3, boxW - 6, boxH_temp - 6, cornerRadius - 2, GxEPD_BLACK);

        // Titolo "WATER" (Centrato)
        String waterTitle = String(waterText[Lang]);
        display.setFont(&FreeSansBold9pt7b);
        display.setTextColor(GxEPD_RED);
        display.getTextBounds(waterTitle, 0, 0, &tbx, &tby, &tbw_num, &tbh_num);
        display.setCursor(marginX + (boxW - tbw_num) / 2 - tbx, y2 + 22);
        display.print(waterTitle);

        // Valore Temperatura Water con °C centrato
        drawDegreeVal(waterTemp, y2);

       // ====================================================
        // 3. BOX BATTERIA (Icona in alto + Percentuale e Volt sotto)
        // ====================================================
        battPercentage = constrain(battPercentage, 0, 100);
        uint16_t battBoxColor = (battPercentage <= 20) ? GxEPD_RED : GxEPD_BLACK;

        // Disegno Sfondo e Bordo del Box
        display.fillRoundRect(marginX, y3, boxW, boxH_batt, cornerRadius, GxEPD_WHITE);
        display.drawRoundRect(marginX + 2, y3 + 2, boxW - 4, boxH_batt - 4, cornerRadius - 1, battBoxColor);
        display.drawRoundRect(marginX + 3, y3 + 3, boxW - 6, boxH_batt - 6, cornerRadius - 2, battBoxColor);

        // Prepara il valore in Volt (gestisce sia millivolt es. 3700 sia volt es. 3.7)
        float vVal = (battVoltage > 15.0f) ? (battVoltage / 1000.0f) : battVoltage;

        // Prepara la stringa unica: "20%, 3.7 V"
        String battStr = String(battPercentage) + "%, " + String(vVal, 1) + " V";

        // Usiamo FreeSansBold9pt7b per supportare la virgola, % e la V
        display.setFont(&FreeSansBold9pt7b);
        display.getTextBounds(battStr, 0, 0, &tbx, &tby, &tbw_c, &tbh_c);

        // Geometria Sagoma Batteria
        int batW = 52;
        int batH = 20;
        int batPinW = 4;                // Polo positivo
        int totalBatW = batW + batPinW; // 55px larghezza totale icona

        // Spazio verticale tra l'icona batteria e la scritta sotto
        int vGap = 1; 
        int totalGroupH = batH + vGap + tbh_c; // Altezza totale del gruppo (Icona + Spazio + Testo)

        // Calcolo Y di partenza per centrare verticalmente l'intero gruppo nel box
        int startY = y3 + (boxH_batt - totalGroupH) / 2 + 4;

        // ----------------------------------------------------
        // 1. ICONA BATTERIA (Centrata orizzontalmente in alto)
        // ----------------------------------------------------
        int batX = marginX + (boxW - totalBatW) / 2;
        int batY = startY;

        // Disegno Corpo esterno e Polo positivo
        display.drawRect(batX, batY, batW, batH, GxEPD_BLACK);
        display.drawRect(batX + 1, batY + 1, batW - 2, batH - 2, GxEPD_BLACK);
        display.fillRect(batX + batW, batY + 4, batPinW, 12, GxEPD_BLACK);

        // Disegno Tacche Interne
        int activeSegments = (battPercentage + 10) / 25;
        activeSegments = constrain(activeSegments, 0, 4);
        int segW = 8;
        int segH = batH - 6;
        int segGap = 3;

        for (int i = 0; i < 4; i++) {
            int segX = batX + 4 + i * (segW + segGap);
            int segY = batY + 3;

            if (i < activeSegments) {
                uint16_t segColor = (battPercentage <= 20) ? GxEPD_RED : GxEPD_BLACK;
                display.fillRect(segX, segY, segW, segH, segColor);
            } else {
                display.drawRect(segX, segY, segW, segH, GxEPD_BLACK);
            }
        }

        // ----------------------------------------------------
        // 2. TESTO PERCENTUALE + VOLT (Centrato sotto l'icona)
        // ----------------------------------------------------
        int textX = marginX + (boxW - tbw_c) / 2 - tbx + 12;
        int textY = batY + batH + vGap - tby; // Offset -tby necessario per i font Adafruit_GFX

        display.setTextColor(battBoxColor);
        display.setCursor(textX, textY);
        display.setFont(&Seven_Segment7pt7b);
        display.print(battStr);


    } while (display.nextPage());

    display.hibernate();
}
*/