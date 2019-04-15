// *********** TIPE 2019 *********** //
//                                   //
// ******* Lycée Saint Louis ******* //
//                                   //
// KAOUKEB Ryan -- GRENIER Alexandre //
//                                   //
// **** STABILISATEUR DE CAMERA **** //
//                                   //
// ********* Version 0.2.0 ********* //

// Inclusion des librairies //
#include <Wire.h> // Librairie pour le controle des ports analogiques
#include <Servo.h> // Librairie de l'objet Servo

// Declaration des instances des objets
Servo interfaceMoteur ;  // Instance de Servo pour controler le moteur

// Declaration des variables globales
float u = 0; // Variable post-filtre passe bas
float tempsCaracteristique = 0.1; // Pulsation de coupure du filtre passe-bas (rad/s)
float pause = 10; // Temps de pause entre deux loop (ms)
float gain = 0.1; // Gain de l'asservissement
const int MPU_addr=0x68;  // Adresse I2C du MPU-6050
float AcX,AcY,AcZ; // Composante sur X
float marge = 1; 


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
  AcY=Wire.read()<<8|Wire.read();  // 0x3D (ACCEL_YOUT_H) & 0x3E (ACCEL_YOUT_L)
  AcZ=Wire.read()<<8|Wire.read();  // 0x3F (ACCEL_ZOUT_H) & 0x40 (ACCEL_ZOUT_L)    
  
	// Sortie de controle //
	Serial.print("U : ");
	Serial.print(u);
	Serial.print(" , X : ");
  Serial.println(AcY/100);
  // ****************** //

  // Filtrage passe-bas de x par la methode d'Euler
  u = u +(pause/1000)*(AcY/(100*tempsCaracteristique) - u/tempsCaracteristique);

  /*if(-1*(marge/2) < gain*u < (marge/2))
  {
    u = 0;
  };*/

  // Asservissement de la vitesse du moteur
  interfaceMoteur.write(float(90-gain*u)); // 90 => Vitesse nulle
    Serial.print(90-gain*u);
    Serial.print(90-gain*AcY);
  delay(pause);
 
 
}
