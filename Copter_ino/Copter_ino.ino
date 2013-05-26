#include <SoftwareSerial.h>
#include <Wire.h>
#include <Servo.h>
#include <PID_v1.h>
#include "Copter.h"
#include "MPU6050_API.h"

SoftwareSerial		bluetooth ( 10 , 11 );
QuadroCopter        copter;

bool				rollPidOn			= false;
double				rollPidP			= 2.3;
double				rollPidI			= 0.3;
double				rollPidD			= 1.35;
double				rollPidSetpoint		        = 0;
double                          rollPidInput;
double				rollPidOut0;
double				rollPidOut2;

PID				rollPID0 ( &rollPidInput , &rollPidOut0, &rollPidSetpoint , rollPidP , rollPidI , rollPidD , DIRECT );
PID				rollPID2 ( &rollPidInput , &rollPidOut2, &rollPidSetpoint , rollPidP , rollPidI , rollPidD , REVERSE );

void setup()
{

	//  Begin serial communication over bluetooth
	bluetooth.begin(9600);

	//  configure accelgyro
	accelgyroSetup();

	//  Bind pins to copter engines
	copter.createEngines(3,0,6,1);
	copter.createBalancer(ypr,ypr);

	//	Configure PIDs
	rollPID0.SetMode(AUTOMATIC);
	rollPID0.SetOutputLimits(1130,1300);
	rollPID0.SetSampleTime(10);
	rollPID2.SetMode(AUTOMATIC);
	rollPID2.SetOutputLimits(1130,1300);
	rollPID2.SetSampleTime(10);


}

void loop()
{
	if ( accelgyroUpdate() )
	{
		//  Update telemetry data
		accelgyroGetYawPitchRoll();
		//accelgyroGetAceleration();

		if ( rollPidOn )
		{
                    rollPidInput = ypr[2];
                    rollPID0.Compute();
                    rollPID2.Compute();
                    
                    copter.getEngine(0)->setSpeed(rollPidOut0);
                    copter.getEngine(2)->setSpeed(rollPidOut2);
		}
		Serial.print(rollPidP);Serial.print("\t");
		Serial.print(rollPidI);Serial.print("\t");
		Serial.print(rollPidD);Serial.print("\t\t");

		Serial.println(ypr[2]);

	}

	//  If a command was recieved over bluetooth
	if ( bluetooth.available() )
	{
		//  Send it to copter
		switch (bluetooth.read())
		{
			case COPTER_STOP:					copter.cmdStop(); break;
			case COPTER_LAUNCH:					copter.cmdLaunch();rollPidOn=true; break;
			case COPTER_RAISE:					copter.cmdRaise(); break;
			case COPTER_DESCEND:				        copter.cmdDescend(); break;
			case BALANCER_TOGGLE_ENABLED:		                copter.getBalancer()->toggleEnabled(); break;
			case PID_P_SHIFT: 
			{
				if ( bluetooth.read() == COPTER_STOP )
					rollPidP -= 0.05;
				else
					rollPidP += 0.05;

				rollPID0.SetTunings(rollPidP,rollPidI,rollPidD);
				rollPID2.SetTunings(rollPidP,rollPidI,rollPidD);
                                break;
			}
			case PID_I_SHIFT: 
			{
				if ( bluetooth.read() == COPTER_STOP )
					rollPidI -= 0.05;
				else
					rollPidI += 0.05;

				rollPID0.SetTunings(rollPidP,rollPidI,rollPidD);
				rollPID2.SetTunings(rollPidP,rollPidI,rollPidD);
                                break;
			}
			case PID_D_SHIFT: 
			{
				if ( bluetooth.read() == COPTER_STOP )
					rollPidD -= 0.05;
				else
					rollPidD += 0.05;

				rollPID0.SetTunings(rollPidP,rollPidI,rollPidD);
				rollPID2.SetTunings(rollPidP,rollPidI,rollPidD);
                                break;
			}
		}
	}

}
