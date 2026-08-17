#ifndef DISPLAY_MANAGER_H
#define DISPLAY_MANAGER_H
#include <sys.h>
#include <GxEPD2_3C.h> // <- IMPORTANTE: Include per display 3 Colori (Black/White/Red)
#include <epd3c/GxEPD2_213_Z98c.h> // Driver per 2.13" Tricolore (o la versione Z98c)
#include <Fonts/FreeSansBold9pt7b.h>
#include <Seven_Segment20pt7b.h>
#include <Seven_Segment7pt7b.h>
#define ENABLE_GxEPD2_GFX 0


class DisplayManager {
private:
    GxEPD2_213_Z98c epd2_driver;
    GxEPD2_3C<GxEPD2_213_Z98c, GxEPD2_213_Z98c::HEIGHT> display;

public:
    DisplayManager();
    void init();
    void clear();
    void UpdateScreen(float airTemp, float waterTemp, float battVoltage, int battPercentage);




        
};

#endif // DISPLAY_MANAGER_H