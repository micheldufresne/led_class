#include "RGBLed.h"


RGBLed::RGBLed(uint8_t pinR, uint8_t pinG, uint8_t pinB, uint8_t niveauOff)
    : pinR(pinR), pinG(pinG), pinB(pinB), niveauOff(niveauOff) {
    // assigner un canal unique par broche quelle que soit la led
    canalR = LedBase::allocateChannel();
    canalG = LedBase::allocateChannel();
    canalB = LedBase::allocateChannel();
    pinMode(pinR, OUTPUT);
    pinMode(pinG, OUTPUT);
    pinMode(pinB, OUTPUT);
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
    ready = true; // la LED est prête
}

void RGBLed::eteint()
{
    if (bloquee)
        return; // on ignore toutes les commandes sauf debloque()
    etatR = etatG = etatB = niveauOff;
    ledcWrite(canalR, niveauOff);
    ledcWrite(canalG, niveauOff);
    ledcWrite(canalB, niveauOff);
    //Serial.println(niveauOff);
    active = false;
    fading = false;
}


void RGBLed::setCouleur(uint8_t r, uint8_t g, uint8_t b)
{
    if (bloquee)
        return; // on ignore toutes les commandes sauf debloque()
    if (niveauOff==0) {
        etatR = r;
        etatG = g;
        etatB = b;
    } else {
        etatR = 255-r;
        etatG = 255-g;
        etatB = 255-b;
    }
    //Serial.printf("R=%u, V=%u, B=%u\n",etatR,etatG,etatB);
    ledcWrite(canalR, etatR);
    ledcWrite(canalG, etatG);
    ledcWrite(canalB, etatB);

    // si autoOFF est actif, recalculer fin du fade
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

void RGBLed::tickUpdate(uint64_t tick)
{
    if (bloquee)
        return; // on ignore toutes les commandes sauf debloque()
    if (!ready)
        return;
    if (autoOff && active && !fading && tick >= finTick)
    {
        if (fadeDuration > 0)
        {
            fading = true;
            fadeStartTime = millis();
            fadeStartR = etatR;
            fadeStartG = etatG;
            fadeStartB = etatB;
        }
        else
        {
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
            int16_t deltaR = (int16_t)fadeStartR - (int16_t)niveauOff;
            uint8_t newR = niveauOff + deltaR * factor;
            int16_t deltaG = (int16_t)fadeStartG - (int16_t)niveauOff;
            uint8_t newG = niveauOff + deltaG * factor;
            int16_t deltaB = (int16_t)fadeStartB - (int16_t)niveauOff;
            uint8_t newB = niveauOff + deltaB * factor;

            ledcWrite(canalR, newR);
            ledcWrite(canalG, newG);
            ledcWrite(canalB, newB);

            etatR = newR;
            etatG = newG;
            etatB = newB;
        }
    }
}

void RGBLed::flash()
{
    LedBase::flash();
    ledcWrite(canalR, 255 - niveauOff);
    ledcWrite(canalG, 255 - niveauOff);
    ledcWrite(canalB, 255 - niveauOff);
}
