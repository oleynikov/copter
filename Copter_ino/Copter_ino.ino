#include <SoftwareSerial.h>
#include <Arduino.h>
#include <Servo.h>
#include <Wire.h>
#include "Copter.h"
#include "MPU6050_API.h"

SoftwareSerial          bluetooth(10,11);
QuadroCopter            copter;

void setup()
{

  //  Begin serial communication over bluetooth
  bluetooth.begin(9600);
  
  //  configure accelgyro
  accelgyroSetup();

  //  Bind pins to copter engines
  copter.createEngines(3,5,6,9);
  
}

void loop()
{

  if ( accelgyroUpdate() )
  {
  
    VectorInt16*  aceleration = accelgyroGetAceleration();
    float*        yawPitchRoll = accelgyroGetYawPitchRoll();
  
    //  Output aceleration
    /*
    Serial.print("Aceleration:\t");
    Serial.print(aceleration->x);Serial.print("\t");
    Serial.print(aceleration->y);Serial.print("\t");
    Serial.print(aceleration->z);Serial.print("\t");
    */
    //  Output YRP
    //Serial.print(yawPitchRoll[0]);Serial.print("\t");
    Serial.print(yawPitchRoll[1]);Serial.print("\t");
    Serial.print(yawPitchRoll[2]);Serial.print("\t");
	
    //	Output current speeds of engines
    Serial.print(copter.getEngine(0)->getSpeed()); Serial.print('\t');
    Serial.print(copter.getEngine(1)->getSpeed()); Serial.print('\t');
    Serial.print(copter.getEngine(2)->getSpeed()); Serial.print('\t');
    Serial.println(copter.getEngine(3)->getSpeed());
	
  }
  
  //  If a command was recieved over bluetooth
  if ( bluetooth.available() )
  {
    
	//  Send it to copter
    switch (bluetooth.read())
    {
	
      case Copter::ENGINES_STOP:	copter.stopAllEngines(); break;
	  
      case Copter::ENGINES_ARM:		copter.armAllEngines(); break;
	  
      case Copter::COPTER_RAISE:	copter.accelerateAllEngines(); break;
	  
      case Copter::COPTER_DESCEND:	copter.slowAllEngines(); break;
	  
    }

  }

}
