#include "RGBLed.h"
#include <Arduino.h>

RGBLed::RGBLed(uint8_t pinR0, uint8_t pinG0, uint8_t pinB0, uint8_t niveauOff0)
    : LedBase(niveauOff0), pinR(pinR0), pinG(pinG0), pinB(pinB0)

{
    // assigner un canal unique par broche quelle que soit la led
    canalR = LedBase::allocateChannel();
    canalG = LedBase::allocateChannel();
    canalB = LedBase::allocateChannel();
    pinMode(pinR, OUTPUT);
    pinMode(pinG, OUTPUT);
    pinMode(pinB, OUTPUT);

    niveauOn = 255-niveauOff0;

    if (canalR < 0 || canalG < 0 || canalB < 0)
    {
        Serial.println("Erreur: pas assez de canaux LEDC");
        return;
    }
    

    ledcSetup(canalR, PWM_FREQ, PWM_RES);
    ledcSetup(canalG, PWM_FREQ, PWM_RES);
    ledcSetup(canalB, PWM_FREQ, PWM_RES);

    ledcAttachPin(pinR, canalR);
    ledcAttachPin(pinG, canalG);
    ledcAttachPin(pinB, canalB);

    eteint();
    ready = true; // à laisser à la fin du constructeur
}

void RGBLed::eteint()
{
    if (bloquee)
        return; // on ignore toutes les commandes sauf debloque()
    etatR = etatG = etatB = niveauOff;
    if (!enFlash)
        appliquerEtat();

    active = false;
    fading = false;
}

void RGBLed::setCouleur(uint8_t r, uint8_t g, uint8_t b)
{
    if (bloquee)
        return;

    if (niveauOff == 0)
    {
        etatR = r;
        etatG = g;
        etatB = b;
    }
    else
    {
        etatR = 255 - r;
        etatG = 255 - g;
        etatB = 255 - b;
    }

    if (!enFlash)
        appliquerEtat();

    if (autoOff && !fading)
    {
        finTick = LedBase::tick + dureeTicks;
        active = true;
    }
}

void RGBLed::setCouleur(uint32_t hexColor)
{
    if (bloquee)
        return; // on ignore toutes les commandes sauf debloque()
    uint8_t r = (hexColor >> 16) & 0xFF;
    uint8_t g = (hexColor >> 8) & 0xFF;
    uint8_t b = hexColor & 0xFF;

    setCouleur(r, g, b); // réutilise la version existante
}

bool RGBLed::estEteint()
{
    return (etatR == niveauOff && etatG == niveauOff && etatB == niveauOff);
}

bool RGBLed::estAllumee()
{
    return (!estEteint());
}

void RGBLed::flash()
{
    LedBase::flash();
    ledcWrite(canalR, 255 - niveauOff);
    ledcWrite(canalG, 255 - niveauOff);
    ledcWrite(canalB, 255 - niveauOff);
}

void RGBLed::debutFade()
{
    fadeStartR = etatR;
    fadeStartG = etatG;
    fadeStartB = etatB;
}

void RGBLed::appliquerFade(float factor)
{
    int16_t deltaR = (int16_t)fadeStartR - (int16_t)niveauOff;
    int16_t deltaG = (int16_t)fadeStartG - (int16_t)niveauOff;
    int16_t deltaB = (int16_t)fadeStartB - (int16_t)niveauOff;

    uint8_t newR = niveauOff + deltaR * factor;
    uint8_t newG = niveauOff + deltaG * factor;
    uint8_t newB = niveauOff + deltaB * factor;

    etatR = newR;
    etatG = newG;
    etatB = newB;

    if (!enFlash)
        appliquerEtat();
}

void RGBLed::eteintPourTick()
{
    eteint();
}

void RGBLed::appliquerEtat()
{
    ledcWrite(canalR, etatR);
    ledcWrite(canalG, etatG);
    ledcWrite(canalB, etatB);
}