#include <Arduino.h>
#include "rgbled.h"

#define pinRouge 18
#define pinVert 19
#define pinBleu 23
#define niveauOff 255
RGBLed triled(pinRouge, pinVert, pinBleu, niveauOff);

void essai()
{
    Serial.println("Rouge");
    triled.setCouleur(255, 0, 0);
    delay(2000);
    Serial.println("Vert");
    triled.setCouleur(0, 255, 0);
    delay(2000);
    Serial.println("Bleu");
    triled.setCouleur(0, 0, 255);
    delay(2000);
    Serial.println("Rouge");
    triled.setCouleur(0xFF0000);
    delay(2000);
    Serial.println("Vert");
    triled.setCouleur(0x00FF00);
    delay(2000);
    Serial.println("Bleu");
    triled.setCouleur(0x0000FF);
    delay(2000);
    Serial.println("Extinction");
    triled.eteint();
    delay(2000);
    Serial.println("Rebleu");
    triled.setCouleur(0, 0, 255);    
    delay(2000);
    Serial.println("Auto extinction douce après 3 secondes et baisse pendant 1seconde");
    triled.setAutoExtinction(true,3000,1000); // auto extinction douce après 3s et pendant 1s
    Serial.println("Rouge");
    triled.setCouleur(0xFF0000);
    delay(3000+1000);
    Serial.println("Le rouge vient juste de s'éteindre");
    delay(2000);
    Serial.println("Jaune pendant 3s");
    triled.setCouleur(0x211500);
    delay(3000+1000);
    Serial.println("Fin du test des couleurs de base");
    delay(4000);
}

bool parseHexColor(const String &in, uint32_t &outColor)
{
    String s = in;
    s.trim();
    if (s.startsWith("0x") || s.startsWith("0X"))
        s = s.substring(2);
    if (s.startsWith("#"))
        s = s.substring(1);
    if (s.length() == 3)
    {
        String a = "";
        for (int i = 0; i < 3; i++)
        {
            char c = s.charAt(i);
            a += c;
            a += c;
        }
        s = a;
    }
    if (s.length() != 6)
        return false;
    for (int i = 0; i < 6; i++)
    {
        char c = s.charAt(i);
        if (!isHexadecimalDigit(c))
            return false;
    }
    outColor = (uint32_t)strtoul(s.c_str(), nullptr, 16);
    return true;
}

void chercheCouleur()
{
    static String s = "";
    static uint8_t r = 125;
    static uint8_t v = 125;
    static uint8_t b = 125;
    while (true)
    {
        if (Serial.available())
        {
            char c = Serial.read();
            if (c == 'r')
            {
                if (r < 255)
                    r++;
            } else if (c == 'e')
            {
                if (r > 0)
                    r--;
            } else if (c == 'b')
            {
                if (b > 0)
                    b--;
            } else if (c == 'n')
            {
                if (b < 255)
                    b++;
            } else if (c == 'c')
            {
                if (v > 0)
                    v--;
            } else if (c == 'v')
            {
                if (v < 255)
                    v++;
            }
            else if (c == '\n')
            {
                // afficher la couleur courante
                Serial.printf("Couleur actuelle : r=%u v=%u b=%u\n", r, v, b);
                
            }
   
            while (Serial.available())
                Serial.read(); // vider le buffer
            while (!Serial.available())
            {
                for (uint8_t v2 = max(0,v-10); v2 < min(255,v+10); v2++)
                {
                    triled.setCouleur(r, v2, b);
                    delay(20);
                }
            }
            Serial.printf("Couleur actuelle : r=%u v=%u b=%u\n", r, v, b);
            while (Serial.available())
                Serial.read(); // vider le buffer
        }
        
    }
}

void saisieCouleur()
{
    static String s = "";
    Serial.println("Maintenant à vous de saisir un code couleur hexadécimal (par exemple AC122B).");
    // configure auto-extinction / fade si souhaité :
    triled.setAutoExtinction(false); // 1s then 200ms fade
    while (true)
    {
        if (Serial.available())
        {
            char c = Serial.read();
            if (c == '\n')
            {
                s.trim();
                Serial.println(s);
                uint32_t color;
                if (parseHexColor(s, color))
                {
                    Serial.print("Parsed hex: 0x");
                    Serial.println(color, HEX);
                    triled.setCouleur(color);
                }
                else
                {
                    Serial.println("Code couleur invalide. Utilisez le format HHHHHH où H est un chiffre en hexa");
                }
                s = "";
            }
            else
            {
                s = s + c;
            }
        }
    }
}

void setup()
{
    Serial.begin(115200);
    Serial.println("Test de led.cpp ou ledbase.cpp ou rgbled.cpp");
    delay(50);
    triled.begin(); // démarrage du timer interne des leds
    essai();        // test des couleurs de base
    delay(2000);
    triled.setAutoExtinction(false); // 1s then 200ms fade
    //chercheCouleur();
    saisieCouleur();
    Serial.println("Fin du setup");
}

void loop()
{
}