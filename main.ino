// *********** TIPE 2019 *********** //
//                                   //
// ******* Lycée Saint Louis ******* //
//                                   //
// KAOUKEB Ryan -- GRENIER Alexandre //
//                                   //
// **** STABILISATEUR DE CAMERA **** //
//                                   //
// ********* Version 0.0.1 ********* //

// Inclusion des librairies //
#include <Wire.h> // Librairie pour le controle des ports analogiques
#include <ADXL345.h> // Librairie de l'accelerometre ADXL 345
#include <Servo.h> // Librairie de l'objet Servo

// Declaration des instances des objets
Servo interfaceMoteur ;  // Instance de Servo pour controler le moteur
ADXL345 adxl; // Instance de l'objet ADXL345

// Declaration des variables globales
float u = 0; // Variable post-filtre passe bas
float wo = 0.1; // Pulsation de coupure du filtre passe-bas (rad/s)
int pause = 20; // Temps de pause entre deux loop (ms)
float gain = 0.01 // Gain de l'asservissement


void setup()
{
  
  myservo.attach(9); 
  Serial.begin(115200);

  // Parametrage de l'accelerometre ADXL //
  // *********************************** //
  adxl.powerOn();

  //set activity/ inactivity thresholds (0-255)
  adxl.setActivityThreshold(75); //62.5mg per increment
  adxl.setInactivityThreshold(75); //62.5mg per increment
  adxl.setTimeInactivity(10); // how many seconds of no activity is inactive?
 
  //look of activity movement on this axes - 1 == on; 0 == off 
  adxl.setActivityX(1);
  adxl.setActivityY(1);
  adxl.setActivityZ(1);
 
  //look of inactivity movement on this axes - 1 == on; 0 == off
  adxl.setInactivityX(1);
  adxl.setInactivityY(1);
  adxl.setInactivityZ(1);
 
  //look of tap movement on this axes - 1 == on; 0 == off
  adxl.setTapDetectionOnX(0);
  adxl.setTapDetectionOnY(0);
  adxl.setTapDetectionOnZ(1);
 
  //set values for what is a tap, and what is a double tap (0-255)
  adxl.setTapThreshold(50); //62.5mg per increment
  adxl.setTapDuration(15); //625us per increment
  adxl.setDoubleTapLatency(80); //1.25ms per increment
  adxl.setDoubleTapWindow(200); //1.25ms per increment
 
  //set values for what is considered freefall (0-255)
  adxl.setFreeFallThreshold(7); //(5 - 9) recommended - 62.5mg per increment
  adxl.setFreeFallDuration(45); //(20 - 70) recommended - 5ms per increment
 
  //setting all interrupts to take place on int pin 1
  //I had issues with int pin 2, was unable to reset it
  adxl.setInterruptMapping( ADXL345_INT_SINGLE_TAP_BIT,   ADXL345_INT1_PIN );
  adxl.setInterruptMapping( ADXL345_INT_DOUBLE_TAP_BIT,   ADXL345_INT1_PIN );
  adxl.setInterruptMapping( ADXL345_INT_FREE_FALL_BIT,    ADXL345_INT1_PIN );
  adxl.setInterruptMapping( ADXL345_INT_ACTIVITY_BIT,     ADXL345_INT1_PIN );
  adxl.setInterruptMapping( ADXL345_INT_INACTIVITY_BIT,   ADXL345_INT1_PIN );
 
  //register interrupt actions - 1 == on; 0 == off  
  adxl.setInterrupt( ADXL345_INT_SINGLE_TAP_BIT, 1);
  adxl.setInterrupt( ADXL345_INT_DOUBLE_TAP_BIT, 1);
  adxl.setInterrupt( ADXL345_INT_FREE_FALL_BIT,  1);
  adxl.setInterrupt( ADXL345_INT_ACTIVITY_BIT,   1);
  adxl.setInterrupt( ADXL345_INT_INACTIVITY_BIT, 1);
  // *********************************** //
  
}

void loop()
{ 
  
	int x,y,z; // Composantes de l'acceleration   
	adxl.readXYZ(&x, &y, &z); // Lecture des valeurs x,y,z
  
	// Sortie de controle //
	Serial.print("U : ");
	Serial.print(u);
	Serial.print(" , X : ");
  Serial.println(x);
  // ****************** //

  // Filtrage passe-bas de x par la methode d'Euler
  u = u + (pause/1000)*(x/wo - u/wo);

  // Asservissement de la vitesse du moteur
  interfaceMoteur.write(90+gain*u); // 90 => Vitesse nulle
	
  delay(pause);
 
 
}
