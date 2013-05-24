#ifndef MPU6050API
#define MPU6050API
// I2Cdev and MPU6050 must be installed as libraries, or else the .cpp/.h files
// for both classes must be in the include path of your project
#include "I2Cdev.h"

#include "MPU6050_6Axis_MotionApps20.h"
//#include "MPU6050.h" // not necessary if using MotionApps include file

// class default I2C address is 0x68
// specific I2C addresses may be passed as a parameter here
// AD0 low = 0x68 (default for SparkFun breakout and InvenSense evaluation board)
// AD0 high = 0x69

/* =========================================================================
   NOTE: In addition to connection 3.3v, GND, SDA, and SCL, this sketch
   depends on the MPU-6050's INT pin being connected to the Arduino's
   external interrupt #0 pin. On the Arduino Uno and Mega 2560, this is
   digital I/O pin 2.
 * ========================================================================= */

/* =========================================================================
   NOTE: Arduino v1.0.1 with the Leonardo board generates a compile error
   when using Serial.write(buf, len). The Teapot output uses this method.
   The solution requires a modification to the Arduino USBAPI.h file, which
   is fortunately simple, but annoying. This will be fixed in the next IDE
   release. For more info, see these links:

   http://arduino.cc/forum/index.php/topic,109987.0.html
   http://code.google.com/p/arduino/issues/detail?id=958
 * ========================================================================= */
 
 // uncomment "OUTPUT_READABLE_QUATERNION" if you want to see the actual
// quaternion components in a [w, x, y, z] format (not best for parsing
// on a remote host such as Processing or something though)
//#define OUTPUT_READABLE_QUATERNION

// uncomment "OUTPUT_READABLE_EULER" if you want to see Euler angles
// (in degrees) calculated from the quaternions coming from the FIFO.
// Note that Euler angles suffer from gimbal lock (for more info, see
// http://en.wikipedia.org/wiki/Gimbal_lock)
//#define OUTPUT_READABLE_EULER

// uncomment "OUTPUT_READABLE_YAWPITCHROLL" if you want to see the yaw/
// pitch/roll angles (in degrees) calculated from the quaternions coming
// from the FIFO. Note this also requires gravity vector calculations.
// Also note that yaw/pitch/roll angles suffer from gimbal lock (for
// more info, see: http://en.wikipedia.org/wiki/Gimbal_lock)
//#define OUTPUT_READABLE_YAWPITCHROLL

// uncomment "OUTPUT_READABLE_REALACCEL" if you want to see acceleration
// components with gravity removed. This acceleration reference frame is
// not compensated for orientation, so +X is always +X according to the
// sensor, just without the effects of gravity. If you want acceleration
// compensated for orientation, us OUTPUT_READABLE_WORLDACCEL instead.
//#define OUTPUT_READABLE_REALACCEL

// uncomment "OUTPUT_READABLE_WORLDACCEL" if you want to see acceleration
// components with gravity removed and adjusted for the world frame of
// reference (yaw is relative to initial orientation, since no magnetometer
// is present in this case). Could be quite handy in some cases.
//#define OUTPUT_READABLE_WORLDACCEL

#define LED_PIN 13 // (Arduino is 13, Teensy is 11, Teensy++ is 6)
bool blinkState = false;

// MPU control/status vars
bool dmpReady = false;  // set true if DMP init was successful
uint8_t mpuIntStatus;   // holds actual interrupt status byte from MPU
uint8_t devStatus;      // return status after each device operation (0 = success, !0 = error)
uint16_t packetSize;    // expected DMP packet size (default is 42 bytes)
uint16_t fifoCount;     // count of all bytes currently in FIFO
uint8_t fifoBuffer[64]; // FIFO storage buffer

// orientation/motion vars
Quaternion q;           // [w, x, y, z]         quaternion container
VectorInt16 aa;         // [x, y, z]            accel sensor measurements
VectorInt16 aaReal;     // [x, y, z]            gravity-free accel sensor measurements
VectorInt16 aaWorld;    // [x, y, z]            world-frame accel sensor measurements
VectorFloat gravity;    // [x, y, z]            gravity vector
float euler[3];         // [psi, theta, phi]    Euler angle container
float ypr[3];           // [yaw, pitch, roll]   yaw/pitch/roll container and gravity vector
float copterAccel[3];
// indicates whether MPU interrupt pin has gone high
volatile bool mpuInterrupt = false;     

MPU6050 accelgyro;



// ================================================================
// ===               INTERRUPT DETECTION ROUTINE                ===
// ================================================================


void			dmpDataReady ( void )
{
    mpuInterrupt = true;
}



// ================================================================
// ===                      INITIAL SETUP                       ===
// ================================================================

void			accelgyroSetup( void )
{
      // configure LED for output
    pinMode(LED_PIN, OUTPUT);
    digitalWrite(LED_PIN,LOW);
   // join I2C bus (I2Cdev library doesn't do this automatically)
    Wire.begin();

    // initialize serial communication
    // (115200 chosen because it is required for Teapot Demo output, but it's
    // really up to you depending on your project)
    //Serial.begin(115200);
    Serial.begin(38400);
    while (!Serial); // wait for Leonardo enumeration, others continue immediately

    // NOTE: 8MHz or slower host processors, like the Teensy @ 3.3v or Ardunio
    // Pro Mini running at 3.3v, cannot handle this baud rate reliably due to
    // the baud timing being too misaligned with processor ticks. You must use
    // 38400 or slower in these cases, or use some kind of external separate
    // crystal solution for the UART timer.

    // initialize device
    Serial.println(F("Initializing I2C devices..."));
    accelgyro.initialize();

    // verify connection
    Serial.println(F("Testing device connections..."));
    Serial.println(accelgyro.testConnection() ? F("MPU6050 connection successful") : F("MPU6050 connection failed"));

    // wait for ready
   /* Serial.println(F("\nSend any character to begin DMP programming and demo: "));
    while (Serial.available() && Serial.read()); // empty buffer
    while (!Serial.available());                 // wait for data
    while (Serial.available() && Serial.read()); // empty buffer again
*/
    // load and configure the DMP
    Serial.println(F("Initializing DMP..."));
    devStatus = accelgyro.dmpInitialize();
    
    // make sure it worked (returns 0 if so)
    if (devStatus == 0) {
        // turn on the DMP, now that it's ready
        Serial.println(F("Enabling DMP..."));
        accelgyro.setDMPEnabled(true);

        // enable Arduino interrupt detection
        Serial.println(F("Enabling interrupt detection (Arduino external interrupt 0)..."));
        attachInterrupt(0, dmpDataReady, RISING);
        mpuIntStatus = accelgyro.getIntStatus();

        // set our DMP Ready flag so the main loop() function knows it's okay to use it
        Serial.println(F("DMP ready! Waiting for first interrupt..."));
        dmpReady = true;
        digitalWrite(LED_PIN,HIGH);

        // get expected DMP packet size for later comparison
        packetSize = accelgyro.dmpGetFIFOPacketSize();
    } else {
        // ERROR!
        // 1 = initial memory load failed
        // 2 = DMP configuration updates failed
        // (if it's going to break, usually the code will be 1)
        Serial.print(F("DMP Initialization failed (code "));
        Serial.print(devStatus);
        Serial.println(F(")"));
    }


}



// ================================================================
// ===                    MAIN PROGRAM LOOP                     ===
// ================================================================

bool			accelgyroUpdate ( void )
{

  // if programming failed, don't try to do anything
  if (!dmpReady)
  {
    return false;
  }

    // wait for MPU interrupt or extra packet(s) available
    while (!mpuInterrupt && fifoCount < packetSize)
	{
        // other program behavior stuff here
        // .
        // .
        // .
        // if you are really paranoid you can frequently test in between other
        // stuff to see if mpuInterrupt is true, and if so, "break;" from the
        // while() loop to immediately process the MPU data
        // .
        // .
        // .
    }

    // reset interrupt flag and get INT_STATUS byte
    mpuInterrupt = false;
    mpuIntStatus = accelgyro.getIntStatus();

    // get current FIFO count
    fifoCount = accelgyro.getFIFOCount();

    // check for overflow (this should never happen unless our code is too inefficient)
    if ((mpuIntStatus & 0x10) || fifoCount == 1024)
	{
        // reset so we can continue cleanly
        accelgyro.resetFIFO();
        Serial.println(F("FIFO overflow!"));
		
		return false;
    }
    // otherwise, check for DMP data ready interrupt (this should happen frequently)
	else if (mpuIntStatus & 0x02)
	{
        // wait for correct available data length, should be a VERY short wait
        while (fifoCount < packetSize) fifoCount = accelgyro.getFIFOCount();

        // read a packet from FIFO
        accelgyro.getFIFOBytes(fifoBuffer, packetSize);
        
        // track FIFO count here in case there is > 1 packet available
        // (this lets us immediately read more without waiting for an interrupt)
        fifoCount -= packetSize;
		
     
		
		return true;

    }
	
	return false;
	
}



// ================================================================
// ===                    INTERFACE FUNCTIONS                   ===
// ================================================================

VectorInt16*	accelgyroGetAceleration ( void )
{

	// display initial world-frame acceleration, adjusted to remove gravity
	// and rotated based on known orientation from quaternion
	accelgyro.dmpGetQuaternion(&q, fifoBuffer);
	accelgyro.dmpGetAccel(&aa, fifoBuffer);
	accelgyro.dmpGetGravity(&gravity, &q);
	accelgyro.dmpGetLinearAccel(&aaReal, &aa, &gravity);
	accelgyro.dmpGetLinearAccelInWorld(&aaWorld, &aaReal, &q);
  
	copterAccel[0] = aaWorld.x;
	copterAccel[1] = aaWorld.y;
	copterAccel[2] = aaWorld.z;
	
	return &aaWorld;

}

float*			accelgyroGetYawPitchRoll ( void )
{

	// display Euler angles in degrees
	accelgyro.dmpGetQuaternion(&q, fifoBuffer);
	accelgyro.dmpGetGravity(&gravity, &q);
	accelgyro.dmpGetYawPitchRoll(ypr, &q, &gravity);
	
	ypr[0] *= 180/M_PI;
	ypr[1] *= 180/M_PI;
	ypr[2] *= 180/M_PI;
  
	return ypr;
  
}

#endif
