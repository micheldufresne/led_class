#include "LED.h"

#define PWM_FREQ 5000
#define PWM_RES 8

LED::LED(uint8_t pin, uint8_t niveauOff)
    : pin(pin), niveauOff(niveauOff)
{
    //recherche d'un canal dispo
    canal = LedBase::allocateChannel();
    if (canal < 0) {
        Serial.println("Erreur: plus de canaux LEDC disponibles");
        return;
    }
    ledcSetup(canal, PWM_FREQ, PWM_RES);
    ledcAttachPin(pin, canal);

    eteint();
    niveauOn = (niveauOff == LOW ? 255 : LOW);
}

void LED::eteint()
{
    if (bloquee) return; //on ignore toutes les commandes sauf debloque()
    {etat = niveauOff;
    ledcWrite(canal, niveauOff);
    active = false;
    fading=false;}
    
}

void LED::allume()
{
    if (bloquee) return; // on ignore toutes les commandes sauf debloque()
    etat = niveauOn;
    ledcWrite(canal, niveauOn);
    active = true;    
    if (autoOff)
    {
        finTick = LedBase::tick + dureeTicks; //calcul du moment d'arrêt de la led, démarrage du fade
    }
}

bool LED::estEteint()
{
    return (etat == niveauOff);
}

bool LED::estAllume()
{
    return (etat == niveauOn);
}

void LED::tickUpdate(uint64_t tick) override
{
    if (estEnFlash())
        return; // on ignore tout

    if (bloquee) return; // led bloquée, on n'agit pas
    if (autoOff && active && !fading && tick >= finTick)
    {
        if (fadeDuration > 0)
        {
            fading = true;
            fadeStartTime = millis();
            fadeStart = etat;
            
        } else {
            eteint();
        }
    }
    // fade progressif basé sur millis()
    if (fading)
    {
        uint32_t elapsed = millis() - fadeStartTime;
        if (elapsed >= fadeDuration)
        {
            eteint();
        }
        else
        {
            float factor = 1.0f - (float)elapsed / fadeDuration; // de 1 à 0
            int16_t delta = (int16_t)fadeStart - (int16_t)niveauOff;
            uint8_t newV = niveauOff + delta * factor;
            
            ledcWrite(canal, newV);

            etat = newV;
        }
    }
}

void LED::flash()
{
    LedBase::flash();
    ledcWrite(canal, niveauOn);
}
