#pragma once
#include <Arduino.h>
#include <esp_timer.h>

#define TICK_MS 50
#define TICK_US (TICK_MS * 1000)
#define PWM_FREQ 5000
#define PWM_RES 8 // 8 bits → 0-255

class LedBase
{
    public:
        LedBase();
        virtual void tickUpdate(uint64_t tick) = 0; // appelée toutes les 50ms
        static void begin();
        void setAutoExtinction(bool autoOFF, uint16_t duree_ms, uint16_t fade_ms);
        void setAutoExtinction(bool autoOFF);
        void bloque();
        void debloque();
        virtual void flash();
        bool estEnFlash();
        void setDureeFlash(uint16_t duree);

    protected:    
        static void timerCallback(void *arg);
        static bool usedChannels[8]; // 8 canaux high-speed
        static int allocateChannel();
        bool bloquee = false;
        bool ready = false;
        uint16_t dureeTicks = 0;
        uint64_t finTick = 0; // tic de fin d'allumage
        bool active = false;
        static esp_timer_handle_t timer;
        static uint64_t tick;
        bool autoOff = false;
        static LedBase *instances[64];
        static int instanceCount;
        bool fading = false;
        uint32_t fadeStartTime = 0; // avec millis(), instant du démarrage de la décroissance
        uint32_t fadeDuration = 0;  // durée du fade en ms, 0=arrêt brutal sans fade

        // cas particulier du flash
        bool enFlash = false;
        uint32_t flashDepuis = 0;        // instant du début du flash
                                         // le flash est prioritaire sur toutes les autres commandes
                                         // après un flash le programme précédent se poursuit
        uint32_t dureeFlash = 500; // durée en ms d'un flash
    };
