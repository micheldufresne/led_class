#include <Arduino.h>
#include "led.h"
#include "rgbled.h"

LED maled(26, 0);
RGBLed triled(14, 12, 27, 255);

void essai()
{
  // Serial.print("eteint");
  // maled.extinction();
  Serial.println("Rouge");
  triled.setCouleur(255, 0, 0);
  // rouge
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
  Serial.println("Jaune");
  triled.setCouleur(0x211500);
  delay(2000);
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

void setup()
{
  Serial.begin(115200);
  delay(50);
  triled.begin();
  essai();
  delay(2000);
  Serial.println("Saisir un code couleur hexadécimal (par exemple AC122B).");
  // configure auto-extinction / fade si souhaité :

  triled.setAutoExtinction(false, 2000, 2000); // 1s then 200ms fade
}

void loop()
{
  static String s = "";

  if (Serial.available())
  {
    char c = Serial.read();
    if (c == '\n')
    {
      s = s + c;
      s.trim();
      Serial.print(c);

      if (s.length() == 6)
      {
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
    }
  }
}

/*    //led.allume();        // s'éteint tout seul
    Serial.println("meteo rdc");
    triled.setCouleur(0x5AD624); // reste allumée tant qu'on ne fait pas rgb.eteint()
    delay(2000);
    triled.setCouleur(0x3C7E1E);
    //Serial.println("allume");
    delay(2000);
    Serial.println("Méteo etg");
    triled.setCouleur(59, 156, 224);
    delay(2000);
    triled.setCouleur(30, 82, 118);
    delay(2000);
    Serial.println("Meteo sous sol");
    triled.setCouleur(141,89,37);
    delay(2000);
    Serial.println("Monchy");
    triled.setCouleur(226, 108, 20);
    delay(2000);
    Serial.println("Setup");
    triled.setCouleur(255, 20, 171);
    // Serial.println("Vert");
    delay(2000);
    triled.eteint();
    delay(2000);
}
    */