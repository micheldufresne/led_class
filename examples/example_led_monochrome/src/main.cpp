#include <Arduino.h>
#include "led.h"

//une led est connectée à la pin26 de l'esp32, son autre broche est au 3.3v
constexpr int8_t pinBlanche = 26; 
LED bl(pinBlanche, 255); //La led sera donc éteinte si on a 3.3v sur le pin26
//255 est le niveauOff

constexpr int8_t pinBoutonFlash = 14; //un bouton pour tester le flash

void setup()
{
    Serial.println("Test de led.cpp ou ledbase.cpp ou rgbled.cpp");

    bl.begin();  //à mettre une fois quelles que soient les led utilisées

    bl.setAutoExtinction(true, 300, 200); //une extinction auto après 300ms, le fade dure 200ms
    bl.setDureeFlash(2000); //si nécessaire le flash dure 2 secondes

    Serial.begin(115200);

    Serial.println("Blanc allume");
    bl.allume();
    delay(2000);
    Serial.println("Blanc eteint");
    bl.eteint();
    delay(2000);

    pinMode(pinBoutonFlash, INPUT_PULLDOWN);

    while (true)
    {
        if (millis() % 2000 == 1000 && !bl.estAllume())
        {
            bl.allume();
        }

        if (digitalRead(pinBoutonFlash) == HIGH && !triled.estEnFlash())
        {
            bl.flash();
            Serial.println("!");
        }
    }
}

void loop()
{
}