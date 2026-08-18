#include "RgbLedManager.h"

RgbLedManager::RgbLedManager() 
    : _boardPixel(1, BOARD_RGB_PIN, NEO_GRB + NEO_KHZ800),
      _ringPixels(RING_LEDS_NUM, RING_RGB_PIN, NEO_GRB + NEO_KHZ800) 
{
}

void RgbLedManager::begin() {
    _boardPixel.begin();
    _ringPixels.begin();
    _ringPixels.setBrightness(80U);
    clearAll();
}

void RgbLedManager::clearAll() {
    clearBoard();
    clearRing();
}

// ==========================================
// LED ON BOARD (GPIO4)
// ==========================================
void RgbLedManager::clearBoard() {
    _boardPixel.clear();
    _boardPixel.show();
}

void RgbLedManager::setBoardColor(uint8_t r, uint8_t g, uint8_t b) {
    _boardPixel.setPixelColor(0, _boardPixel.Color(r, g, b));
    _boardPixel.show();
}

void RgbLedManager::doubleFlash(uint8_t r, uint8_t g, uint8_t b, uint16_t onDurationMs, uint16_t offDurationMs) {
    setBoardColor(r, g, b);
    delay(onDurationMs);
    clearBoard();
    delay(offDurationMs);

    setBoardColor(r, g, b);
    delay(onDurationMs);
    clearBoard();
}

// ==========================================
// NEOPIXEL RING external (SDA)
// ==========================================
void RgbLedManager::clearRing() {
    _ringPixels.clear();
    _ringPixels.show();
}

void RgbLedManager::setRingColor(uint8_t r, uint8_t g, uint8_t b) {
    for (int i = 0; i < RING_LEDS_NUM; i++) {
        _ringPixels.setPixelColor(i, _ringPixels.Color(r, g, b));
    }
    _ringPixels.show();
}

void RgbLedManager::setRingPixel(uint8_t index, uint8_t r, uint8_t g, uint8_t b) {
    if (index < RING_LEDS_NUM) {
        _ringPixels.setPixelColor(index, _ringPixels.Color(r, g, b));
        _ringPixels.show();
    }
}

void RgbLedManager::ringSpin(uint8_t r, uint8_t g, uint8_t b, uint16_t speedMs, uint8_t cycles) {
    for (uint8_t c = 0; c < cycles; c++) {
        for (int i = 0; i < RING_LEDS_NUM; i++) {
            _ringPixels.clear();
            _ringPixels.setPixelColor(i, _ringPixels.Color(r, g, b));
            _ringPixels.show();
            delay(speedMs);
        }
    }
    clearRing();
}