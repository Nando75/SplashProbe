#ifndef RGB_LED_MANAGER_H
#define RGB_LED_MANAGER_H

#include "Arduino.h"
#include "CubeCell_NeoPixel.h"
#include "sys.h"

class RgbLedManager {
private:
    CubeCell_NeoPixel _boardPixel;
    CubeCell_NeoPixel _ringPixels;

public:
    RgbLedManager();
    void begin();
    void clearAll();

    // ==========================================
    // CONTROLLO LED SCHEDA (GPIO4)
    // ==========================================
    void setBoardColor(uint8_t r, uint8_t g, uint8_t b);
    void clearBoard();
    void doubleFlash(uint8_t r = 255, uint8_t g = 255, uint8_t b = 255, uint16_t onDurationMs = 100, uint16_t offDurationMs = 100);

    // ==========================================
    // CONTROLLO RING ESTERNO (SDA)
    // ==========================================
    void setRingColor(uint8_t r, uint8_t g, uint8_t b);
    void setRingPixel(uint8_t index, uint8_t r, uint8_t g, uint8_t b);
    void clearRing();
    
    // Giochi di luce per il Ring
    void ringSpin(uint8_t r, uint8_t g, uint8_t b, uint16_t speedMs = 40, uint8_t cycles = 1);
};

#endif