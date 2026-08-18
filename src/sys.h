#pragma once
#include <cstdint>

enum DataMode {
    Celsius,
    Fahrenheit
};

// Stati della Macchina a Stati
enum SystemState {
    STATE_READ_SENSORS,
    STATE_SLEEP
};

//translations
typedef enum {
    it,
    en,
    es,
    cn
} Language;


//EPAPER DISPLAY:
#define DISPLAY_SIZE 213
#define EDP_RST GPIO1
#define EDP_DC GPIO2
#define EDP_CS GPIO3
#define EDP_BUSY GPIO5
#define EDP_SCK SCK
#define EDP_MOSI MOSI
#define EDP_MISO MISO
#define EDP_FREQ 6000000
//Temperature sensors:
#define PIN_DS18 GPIO0
//Leds 
#define BOARD_RGB_PIN   GPIO4
#define RING_RGB_PIN    SDA
#define RING_LEDS_NUM   12


#define SENSORS_EVERY_N_CYCLES 6
//inline const uint32_t SLEEP_DURATION_MS = 5 * 60 * 1000UL; //un minuto
inline const uint32_t SLEEP_DURATION_MS = 5*60* 1000UL; //mezzo minuto

inline Language Lang = en;
inline DataMode dataMode = Fahrenheit;
inline bool debug = false;
inline SystemState currentState = STATE_READ_SENSORS;
inline const char* airText[] = {"ARIA", "AIR", "AIRE"};
inline const char* waterText[] = {"ACQUA", "WATER", "AGUA"};
inline const char* lowBatt[] = {"Batteria Scarica", "Low Battery", "Batería baja"};