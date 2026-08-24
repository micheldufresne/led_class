#pragma once
#include "LedBase.h"

class RGBLed : public LedBase
{
public:
    RGBLed(uint8_t pinR0, uint8_t pinG0, uint8_t pinB0, uint8_t niveauOff0 = 0);

    void eteint();
    // on suppose que niveauOff =0 pour les deux méthodes ci-dessous
    void setCouleur(uint8_t r, uint8_t g, uint8_t b);  
    void setCouleur(uint32_t hexColor); // hexcolor de la forme : 0x10A0FF
    bool estEteint();
    bool estAllumee();
    void flash() override;

private : 
    uint8_t pinR, pinG, pinB;
    //uint8_t niveauOff; //niveau de tension de la broche quand la led est éteinte
    //uint8_t niveauOn; // niveau de tension de la broche quand la led est allumée
    uint8_t etatR = 0, etatG = 0, etatB = 0; //valeur des couleurs
    uint8_t canalR, canalG, canalB; // canaux PWM
    uint16_t fadeStartR = 0, fadeStartG = 0, fadeStartB = 0;

    void appliquerEtat() override;
    void debutFade() override;
    void appliquerFade(float factor) override;
    void eteintPourTick() override;
};

