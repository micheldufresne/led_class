#include "LED.h"
#include <Arduino.h>
#define PWM_FREQ 5000
#define PWM_RES 8

LED::LED(uint8_t pin0, uint8_t niveauOff0)
    : LedBase(niveauOff0), pin(pin0)
{
    //recherche d'un canal dispo
    canal = LedBase::allocateChannel();
    if (canal < 0) {
        Serial.println("Erreur: plus de canaux LEDC disponibles");
        return;
    }
    pinMode(pin0, OUTPUT);
    ledcSetup(canal, PWM_FREQ, PWM_RES);
    ledcAttachPin(pin0, canal);

    eteint();
    ready = true; // à laisser à la fin du constructeur
}

void LED::eteint()
{
    if (bloquee) return; //on ignore toutes les commandes sauf debloque()
    etat = niveauOff;

    if (!enFlash)
        appliquerEtat();

    active = false;
    fading=false;    
}

void LED::allume()
{
    if (bloquee) return; // on ignore toutes les commandes sauf debloque()
    etat = niveauOn;

    if (!enFlash)
        appliquerEtat();

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



void LED::flash()
{
    LedBase::flash(); //on retient le moment du départ du flash
    ledcWrite(canal, niveauOn);
}


void LED::debutFade()
{
    fadeStart = etat;
}

void LED::appliquerFade(float factor)
{
    int16_t delta = (int16_t)fadeStart - (int16_t)niveauOff;
    uint8_t newV = niveauOff + delta * factor;
    etat = newV;
    
    if (!enFlash)
        appliquerEtat();
}

void LED::eteintPourTick()
{
    eteint();
}

void LED::appliquerEtat()
{
    ledcWrite(canal, etat);
}