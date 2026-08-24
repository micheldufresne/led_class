#pragma once
#include "Arduino.h"
#include "esp_timer.h"

#define TICK_MS 50
#define TICK_US (TICK_MS * 1000)
#define PWM_FREQ 5000
#define PWM_RES 8 // 8 bits → 0-255

class LedBase
{

public: //tout ce qui suit est utilisable dans LedBas, les classes filles et tout autre
    LedBase(const LedBase &) = delete;
    LedBase &operator=(const LedBase &) = delete;

    explicit LedBase(uint8_t niveauOff0 = LOW);

    static void begin();
    void setAutoExtinction(bool autoOFF, uint16_t duree_ms, uint16_t fade_ms);
    void setAutoExtinction(bool autoOFF);
    void bloque();
    void debloque();
    virtual void flash();  //virtual = la méthode sera redéfinie éventuellement, va avec le override de flash dans LED et RGBLed
    bool estEnFlash();
    void setDureeFlash(uint16_t duree);

protected: //tout ce qui suit est utilisable dans LedBase et les classes filles de LedBase
    static void timerCallback(void *arg);
    static bool usedChannels[8]; // 8 canaux high-speed
    static int allocateChannel();
    
    bool bloquee = false;
    bool ready = false;
    uint64_t finTick = 0; // tic de fin d'allumage
    uint16_t dureeTicks = 0;

    uint8_t niveauOff;
    uint8_t niveauOn;
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
    uint32_t flashDepuis = 0;  // instant du début du flash
                               // le flash est prioritaire sur toutes les autres commandes
                               // après un flash les ordres envoyés à la led se poursuivent
    uint32_t dureeFlash = 500; // durée par défaut d'un flash en ms

    void tickUpdate(uint64_t tick); // appelée toutes les .. ms
    virtual void debutFade() = 0;   // le =0 rend obligatoire la redéfinition, en l'absence debutFade reste abstraite et impossible de créer d'objet de cette classe.
    virtual void appliquerFade(float factor) = 0;
    virtual void eteintPourTick() = 0;
    virtual void appliquerEtat() = 0;
    };
