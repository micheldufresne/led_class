# Pour info

## Historique du projet
Au départ de ce projet minimaliste, je souhaitais simplement tester et mettre en pratique la programmation Orientée Objet en C++.
Comme j'utilisais régulièrement des leds, j'avais de nombreux projets qui contenaient des bouts de code identiques. Pour que chaque évolution dans la gestion de mes leds puisse se répercuter à l'ensemble de mes projets, 
j'ai choisi de les déposer sur GitHub.
Ce dépôt contient 3 classes permettant de standardiser l'usage des leds dans mes différents projets.
Outre l'allumage et l'extinction, on pourra changer les couleurs, la luminosité,commander des dégradés, une extinction automatique.

## Un détail qui a son importance

Un timer est utilisé ce qui a pour effet de n'avoir rien à ajouter dans le loop. L'extinction peut démarrer automatiquement après un délai à choisir. La vitesse de la décroissance lumineuse est gérée automatiquement.
Dernière conséquence, toutes ces actions sont asynchrones et ne sont donc pas perturbées par des delay() ou par une fonction gourmande en temps comme une requête http par exemple.

## Quelles LEDs ?

Il peut s'agir de simples leds ou de led rgb. Pour toutes ces leds, il est possible de les commander en mettant la broche (ou les) broches à l'état haut ou à l'état bas, ce choix s'effectue dans le constructeur de la led. On peut donc utiliser des led rgb à anode commune.
Evidemment, il faudra mettre la résistance adaptée entre chaque broche du microcontrôleur et la led pour ne pas endommager le composant. Les leds utilisées sont des leds classiques en 3.3v ou 5v.

## Exemple d'initialisation :

syntaxe : LED(uint8_t pin, uint8_t niveauOff = LOW);

>- #include "led.h"
>- LED L1(12,0) ou LED L1(12,LOW)
>- LED L2(14,255)       

L1 est connectée à la pin 12 et elle est éteinte lorsque la broche est au niveau 0 et sera donc allumée si la broche est au niveau 255
L2 est connectée à la pin 14, elle est éteinte lorsque la broche est au niveau 255 et allumée au niveau 0

Dans le setup :
>- L1.begin();

Ou tout autre led créée : L2.begin() convient. Un seul begin() est nécessaire même si plusieurs leds sont commandées. Le 1er appel active le timer, les appels suivants seront simplement ignorés.

### Pour les led RGB

syntaxe : RGBLed(uint8_t pinR, uint8_t pinG, uint8_t pinB, uint8_t niveauOff)

Exemples :
>- #include "rgbled.h"
>- RGBLed L1(12,14,27,0) ou RGBLed L1(12,14,27,LOW)
>- RGBLed L2(12,14,27,255) 

L1 est connectée aux broches 12, 14 et 27 commandant respectivement le rouge, le vert et le bleu. L1 sera éteinte si toutes les broches sont à 0, L2 sera éteinte si toutes les broches sont à 255


## Une fois créées

### Appliquée à une led simple à 2 broches :

Syntaxe : void allume();

Exemple :
>- M.allume()    allume la led M

### Appliquée à une led RGB :

Syntaxe : void setCouleur(uint8_t r, uint8_t g, uint8_t b);

Le triplet (r,g,b) donne la couleur désirée. Attention, les valeurs sont à choisir en supposant que niveauOff = 0. Si niveauOff est à 255, la correction (le complément à 255) se fera automatiquement.

>- Q.setCouleur(255,0,0);  //la led passe au rouge si noveauOff=0
>- Q.setCouleur(0,255,255);  //la led passe au rouge si noveauOff=255
>- Q.setCouleur(0, 125, 255); //La led Q passe à l'orange si niveauOff=0

Syntaxe : void setCouleur(uint32_t hexColor);

Le paramètre hexcolor à donner sous la forme 0x...... modifie la couleur de la led. Comme précédemment, il faut considérer que le niveauOff=0, si ce n'est pas le cas, la correction est faite automatiquement.

>- Q.setCouleur(0x00FF00);  //la led passe au vert si niveauOff=0
>- Q.setCouleur(0xFF00FF); //La led Q passe au vert si niveauOff=255

### Appliquée aux deux types de led :

Syntaxe : void eteint();

Exemple :
>- P.eteint()    éteint la led P

Syntaxe : void setAutoExtinction(bool autoOFF0, uint16_t duree_ms, uint16_t fade_ms)

>- Si autoOFF0 est à true, la led s'éteint automatiquement après duree_ms millisecondes. La valeur de fade_ms est la durée de la décroissance progressive de la lumière.
>- Si autoOff est à false, les deux paramètres suivants sont ignorés.

Exemples :
>- L.setAutoExtinction(true, 100, 1000);

Après toute demande d'allumage qui suivra cette commande, la led s'éteindra automatiquement après 100ms et mettra 1000ms pour s'éteindre complètement.
Si la commande L.eteint() est exécutée, la led s'arrête tout de suite cependant.

>- L.setAutoExtinction(true, 0, 1000);

La prochaine instruction L.allume() sera automatiquement suivi d'un arrêt progressif de la led durant 1s

>- L.setAutoExtinction(false, 0, 0);

Désormais, l'arrêt automatique est désactivé, la led ne s'éteindra que si on exécute la commande L.eteint()

## Limites

>- L'ESP32 est limité à 16 canaux PWM : 8 high speed et 8 autres moins rapides
>- J'ai fixé à 50ms le délai entre deux changements d'états. Ce qui fait que la temporisation avant l'extinction automatique est déterminée à 50ms près. De même la décroissance lumineuse est calculée toutes les 50ms. A l'usage, cette précision semble bien suffisante.
>- C'est le même timer qui régule le fonctionnement de toutes les leds créées avec ce dépôt.

