#pragma once
#include "LedBase.h"

class LED : public LedBase
{
public:
    // ajout du niveauOff dans le constructeur (par défaut LOW)
    LED(uint8_t pin, uint8_t niveauOff = LOW);

    void allume();
    void eteint();
    bool estEteint();
    bool estAllume();
private:
    void tickUpdate(uint64_t tick) override;

    uint8_t pin;
    uint8_t niveauOff; // niveau de tension de la broche quand la led est éteinte
    uint8_t niveauOn;  // niveau de tension de la broche quand la led est allumée
    uint8_t etat = 0; //valeur de la broche à tout instant
    uint8_t canal;
    // fade variables
    uint8_t fadeStart = 0; //avec millis(), instant de début du fade
};
