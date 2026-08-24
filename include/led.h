#pragma once
#include "LedBase.h"
#include <Arduino.h>

class LED : public LedBase //classe fille de LedBase
{
public:
    LED(uint8_t pin0, uint8_t niveauOff0 = LOW);

    void allume();
    void eteint();
    bool estEteint();
    bool estAllume();
    void flash() override;

private: //utilisable uniquement dans LED et pas dans les classes filles éventuelles

    uint8_t pin;    
    uint8_t etat = 0; //valeur de la broche à tout instant
    uint8_t canal;
    // fade variables
    uint8_t fadeStart = 0; //avec millis(), instant de début du fade
    
    void appliquerEtat() override; //
    void debutFade() override;
    void appliquerFade(float factor) override;
    void eteintPourTick() override;
};
