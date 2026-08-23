#include "LedBase.h"

esp_timer_handle_t LedBase::timer = nullptr;
uint64_t LedBase::tick = 0;
LedBase *LedBase::instances[64];
int LedBase::instanceCount = 0;
bool LedBase::usedChannels[8] = {false};

LedBase::LedBase(){
    if (instanceCount < 64)
        instances[instanceCount++] = this;
    
    if (timer == nullptr)
    {
        //1er demande de création d'un timer
        //il est partagé par les différentes led.
        begin();
    }
}

void LedBase::bloque()
{
    bloquee = true;
}

void LedBase::debloque()
{
    bloquee = false;
}

int LedBase::allocateChannel()
{
    //chaque broche commandée en PWM doit disposer d'un canal propre 
    //renvoie un canal libre (max 8 en high speed)
    for (int i = 0; i < 8; i++)
    {
        if (!usedChannels[i])
        {
            usedChannels[i] = true;
            return i;
        }
    }
    return -1; // plus de canaux disponibles
}

void LedBase::setAutoExtinction(bool autoOFF0, uint16_t duree_ms, uint16_t fade_ms)
{
    autoOff = autoOFF0;

    if (autoOff)
    {
        dureeTicks = (duree_ms / TICK_MS);
        if (dureeTicks == 0)
            dureeTicks = 1;
        fadeDuration = fade_ms; //durée du passage à Off
    }
}

void LedBase::setAutoExtinction(bool autoOFF0)
{
    autoOff = autoOFF0;

    if (autoOff)
    {
        setAutoExtinction(true, 1000, 200); // défaut 1s puis 200ms de fade
    } else
    {
        setAutoExtinction(false,0,0); //0 pour durée et fade par défaut
    }   
}

void LedBase::begin() {
    const esp_timer_create_args_t args = {
        .callback = &LedBase::timerCallback,
        .arg = nullptr,
        .dispatch_method = ESP_TIMER_TASK,
        .name = "universal_led_timer"};

    esp_timer_create(&args, &timer);
    // esp_timer_start_periodic(timer, TICK_US);
    esp_timer_start_periodic(LedBase::timer, TICK_MS * 1000);
}

void LedBase::timerCallback(void *arg)
{
    //appelée régulièrement par le timer toutes les 50ms
    tick++;

    for (int i = 0; i < instanceCount; i++)
    {
        instances[i]->tickUpdate(tick); //à chaque instance de led, mets toi à jour
    }
}

bool LedBase::estEnFlash()
{
    if (!enFlash)
        return false;

    uint16_t m = millis() - flashDepuis;
    if (m < dureeFlash)
        return true;

    return false;
}

void LedBase::flash()
{
    enFlash = true;
    flashDepuis = millis();
    // la couleur sera précisée dans l'objet fils
}

void LedBase::setDureeFlash(uint16_t duree)
{
    dureeFlash=duree;
}