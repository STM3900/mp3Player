#include <Arduino.h>

// Bibliothèque à inclure au minimum
#include "SoftwareSerial.h"      // pour les communications series avec le DFplayer
#include "DFRobotDFPlayerMini.h" // bibliotheque pour le DFPlayer

// PIN qui serviront pour la communication série sur le WEMOS
SoftwareSerial mySoftwareSerial(2, 8); // RX, TX ( wemos D2,D8 ou 4,15 GPIO )  ou Tx,RX ( Dfplayer )
DFRobotDFPlayerMini myDFPlayer;        // init player

const int buttonPreviousPin = 7;
const int buttonPausePin = 6;
const int buttonNextPin = 5;

int buttonPreviousState = 0;
int buttonPauseState = 0;
int buttonNextState = 0;

bool isPressed = false;
bool isPaused = false;

void setup()
{
  mySoftwareSerial.begin(9600);
  Serial.begin(9600); // optionnel pour info dans terminal

  pinMode(buttonPreviousPin, INPUT);
  pinMode(buttonPausePin, INPUT);
  pinMode(buttonNextPin, INPUT);

  if (!myDFPlayer.begin(mySoftwareSerial))
  { // Utilisation de  softwareSerial pour communiquer
    Serial.println(F("Pb communication:"));
    Serial.println(F("1.SVP verifier connexion serie!"));
    Serial.println(F("2.SVP verifier SDcard !"));
    while (true)
      ;
  }
  Serial.println(F("DFPlayer Mini En ligne."));

  myDFPlayer.setTimeOut(500); // Définit un temps de time out sur la communication série à 500 ms

  //----Controle volume----
  myDFPlayer.volume(15); // Monte le volume à 18 ( valeur de 0 à 30 )
  // ---- indique d'utiliser le player de carte SD interne
  myDFPlayer.outputDevice(DFPLAYER_DEVICE_SD);

  // optionel , permet d'afficher quelques infos
  Serial.println(myDFPlayer.readFileCounts());        // Le nombre total de fichier mp3 sur la carte ( dossier inclus )
  Serial.println(myDFPlayer.readCurrentFileNumber()); // l'index courant

  // Joue le premier morceau de la liste
  myDFPlayer.play(1);
  myDFPlayer.enableLoop();
}

void loop()
{
  buttonPreviousState = digitalRead(buttonPreviousPin);
  buttonPauseState = digitalRead(buttonPausePin);
  buttonNextState = digitalRead(buttonNextPin);

  if (buttonPreviousState == HIGH && !isPressed)
  {
    Serial.println(F("Bouton previous allumé"));
    myDFPlayer.previous();
  }

  if (buttonPauseState == HIGH && !isPressed)
  {
    if (!isPaused)
    {
      Serial.println(F("Bouton pause allumé"));
      myDFPlayer.pause();
      isPaused = true;
    }
    else
    {
      Serial.println(F("Bouton play allumé"));
      myDFPlayer.start();
      isPaused = false;
    }
  }

  if (buttonNextState == HIGH && !isPressed)
  {
    Serial.println(F("Bouton next allumé"));
    myDFPlayer.next();
  }

  if (buttonPreviousState == HIGH || buttonPauseState == HIGH || buttonNextState == HIGH)
  {
    isPressed = true;
  }
  else
  {
    isPressed = false;
  }

  //  myDFPlayer.volume(30);
}