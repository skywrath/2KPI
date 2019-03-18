// *********** TIPE 2019 *********** //
//                                   //
// ******* Lycée Saint Louis ******* //
//                                   //
// KAOUKEB Ryan -- GRENIER Alexandre //
//                                   //
// **** STABILISATEUR DE CAMERA **** //
//                                   //
// ********* Version 0.1.1 ********* //

// Inclusion des librairies //
#include <Wire.h> // Librairie pour le controle des ports analogiques
#include <Servo.h> // Librairie de l'objet Servo

// Declaration des instances des objets
Servo interfaceMoteur ;  // Instance de Servo pour controler le moteur

// Declaration des variables globales
float u = 0; // Variable post-filtre passe bas
float wo = 0.1; // Pulsation de coupure du filtre passe-bas (rad/s)
float pause = 20; // Temps de pause entre deux loop (ms)
float gain = 0.01; // Gain de l'asservissement
const int MPU_addr=0x68;  // Adresse I2C du MPU-6050
float AcX; // Composante sur X


void setup()
{
  
  interfaceMoteur.attach(9);

  // Parametrage de l'accelerometre ADXL //
  // *********************************** //
  Wire.begin();
  Wire.beginTransmission(MPU_addr);
  Wire.write(0x6B);  // PWR_MGMT_1 register
  Wire.write(0);     // set to zero (wakes up the MPU-6050)
  Wire.endTransmission(true);
  // *********************************** //

  Serial.begin(115200);
}

void loop()
{ 

  Wire.beginTransmission(MPU_addr);
  Wire.write(0x3B);  // starting with register 0x3B (ACCEL_XOUT_H)
  Wire.endTransmission(false);
  Wire.requestFrom(MPU_addr,14,true);  // request a total of 14 registers
  AcX=Wire.read()<<8|Wire.read();  // 0x3B (ACCEL_XOUT_H) & 0x3C (ACCEL_XOUT_L)    
  
	// Sortie de controle //
	Serial.print("U : ");
	Serial.print(u);
	Serial.print(" , X : ");
  Serial.println(AcX/100);
  // ****************** //

  // Filtrage passe-bas de x par la methode d'Euler
  u = u +(pause/1000)*(AcX/(100*wo) - u/wo);

  // Asservissement de la vitesse du moteur
  interfaceMoteur.write(90+gain*u); // 90 => Vitesse nulle
	
  delay(pause);
 
 
}
