#include <SoftwareSerial.h>
#include <Arduino.h>
#include <Wire.h>
#include <Servo.h>
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
  copter.createBalancer(ypr);
  
}

void loop()
{

  if ( accelgyroUpdate() )
  {
    //  Update YPTR data
    accelgyroGetYawPitchRoll();

    //  Update copter's telemetry
    copter.updateTelemetry();

    //  Output YRP
    Serial.print(ypr[1]);Serial.print("\t");
    Serial.print(ypr[2]);Serial.print("\t\t\t");
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
      case COPTER_STOP:		copter.cmdStop(); break;
      case COPTER_ARM:		copter.cmdArm(); break;
      case COPTER_RAISE:	copter.cmdRaise(); break;
      case COPTER_DESCEND:	copter.cmdDescend(); break;
    }
  }

}
