// Initialisation du module du servomoteur pour pouvoir le contrôler
#include <Servo.h>
Servo servo;
 
float distance = 0; // On initialise la valeur "distance" en float, pour pouvoir la calculer avec des virgules.
byte distance2; // On initialise la valeur "distance2" en octet, pour pouvoir la transmettre par bluetooth.
 
int angle = 115; // On initialise la variable angle avec 135° sur le servomoteur, l'angle qui nous permet d'avoir des roues directionnelles droites.
 
void setup() {
  Serial.begin(9600); // On ouvre le port Serial pour la communication au moniteur de séries et au module Bluetooth
  servo.attach(7, 544, 2400); // On initialise le servomoteur sur le pin numéro 6. 544 et 2400 sont des valeurs pour l'angle minimum et maximum du servomoteur, propres à chacun.
  servo.write(angle); // On place le servomoteur à une position angulaire de angle° pour l'initaliser
}
 
void loop() {
  int erreur; // Je définie la variable erreur pour pouvoir la modifier plus facilement plus tard
  int correction_p; // Je définie la variable correction_p pour pouvoir la modifier plus facilement plus tard
  int amplification = 1; // Je définie la variable amplification à 1 pour pouvoir la modifier plus facilement plus tard (aucune amplification nécessaire lors de nos essais. On le laisse à 1).
  int pos_servo;// Je définie la variable pos_servo pour pouvoir la modifier plus facilement plus tard
  // Toutes ces variables nous permettent de mieux comprendre notre calcul visuellement et pouvoir modifier chaque paramètre plus facilement.
 
  // Calculs :
  distance = analogRead(0); // On récupère la tension du capteur au pin analogique n°0
  distance = distance * 5; // On multiplie cette valeur par 5
  distance = distance / 1023; // On divise par 1023
  distance = distance - 2.44; // On soustrait 2.44
  distance = distance / (0 - 0.28); // On divise par -0.28
  distance = distance * 10; // On multiplie le résultat par 10 pour l'avoir en millimètres et pas centimètres
  //Serial.println(distance); // On envoit la distance au moniteur de série lors de nos essais
 
  // Consigne = 28mm
  erreur = 28 - distance; // On soustrait la distance à la consigne
  correction_p = erreur * amplification; // On doit déterminer l'amplification avec des essais
  pos_servo = 115 + correction_p; // plus ou moins correction....
  // 115 -> angle auquel les roues sont droites. 
  // + correction, car correction peut être positif OU négatif

  Serial.println(pos_servo); // On envoit la correction lors de nos essais
 
  /*
   * Avec notre crémaillère et notre système de train avant, le servomoteur ne peux pas tourner à plus de 145° ni en-dessous de 103°
   * à cause des biellettes qui à ces angles là sont bloqué par le support du servomoteur. Donc, pour éviter de surcharger le servomoteur et de 
   * le casser, nous le bloquons à cet angle pour le préserver (de plus, si on le laisse aller plus loin, la roue dentée glisse sur l'axe du servomoteur
   * et ça fausse après les corrections, et puis c'est inutile de le laisser tourner plus loin sans que les roues puissent tourner du coup...) 
   */
 
  if (pos_servo > 135) { // Si la correction est supérieure à 135°
    servo.write(135); // On bloque la rotation (raison spécifiée plus haut)
  } else if (pos_servo < 100) { // Si la correction est inférieure à 100°
    servo.write(100); // On bloque la rotation (raison spécifiée plus haut)
  } else { // Si c'est entre les deux
    servo.write(pos_servo); // On corrige normalement
  }
  delay(50); // On attends 50 millisecondes pour ne pas perturber le servomoteur
}
