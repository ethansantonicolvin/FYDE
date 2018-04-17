#include <Time.h>
#include <TimeLib.h>


#include "I2Cdev.h"
#include "MPU6050.h"
#include "Wire.h"

struct accelData{
  double accelX;
  double accelY;
  double accelZ;
};

#define LED_PIN 13
#define RANGESCALAR 2048.0
#define nMovingAvg 10
#define maxAllowedAccel 2

MPU6050 accelgyro;
struct accelData getAccelValues(void);
double movingAvgArr[nMovingAvg];
int sampleNumber = 0;
double movingAvg;
boolean ledState = false;
time_t t;

//****************************SETUP**********************************************


void setup() {
    //init onboard LED
    pinMode(LED_PIN, OUTPUT);
      
    // join I2C bus (I2Cdev library doesn't do this automatically)
    #if I2CDEV_IMPLEMENTATION == I2CDEV_ARDUINO_WIRE
        Wire.begin();
    #elif I2CDEV_IMPLEMENTATION == I2CDEV_BUILTIN_FASTWIRE
        Fastwire::setup(400, true);
    #endif

    Serial.begin(9600);
    Serial1.begin(9600);
    
    // initialize device
    //Serial.println("Initializing I2C devices...");
    accelgyro.initialize();

    // verify connection
    //Serial.println("Testing device connections...");
    //Serial.println(accelgyro.testConnection() ? "MPU6050 connection successful" : "MPU6050 connection failed");

    // setting offsets to calibrate sensor
    accelgyro.setXGyroOffset(34);
    accelgyro.setYGyroOffset(68);
    accelgyro.setZGyroOffset(12);
    accelgyro.setXAccelOffset(-1585);
    accelgyro.setYAccelOffset(629);
    accelgyro.setZAccelOffset(824);

    //reset the Blynk value to 0 to show no tampering
    BlynkWrite(6, 0, 0);
    BlynkWrite(49, 0, -1);
    BlynkWrite(50, 0, -1);
    BlynkWrite(51, 0, -1);
    BlynkWrite(52, 0, -1);
    BlynkWrite(53, 0, -1);
    BlynkWrite(54, 0, -1);
}

//*****************MAIN LOOP************************************

void loop() {
    
    struct accelData data = getAccelValues();
    //Serial.print(data.accelX);Serial.print("\t");
    //Serial.print(data.accelY);Serial.print("\t");
    //Serial.print(data.accelZ);Serial.print("\t");
    //Serial.print(getMagnitude(data));Serial.print("\t"); 

    if(sampleNumber < nMovingAvg){ // don't do moving average until we have 5 values
      movingAvgArr[sampleNumber] = getMagnitude(data);
      movingAvg = getMagnitude(data);
    }
    else{
      double sum = 0;
      movingAvgArr[sampleNumber % nMovingAvg] = getMagnitude(data);
      for(int i = 0; i < nMovingAvg; i++)
        sum += movingAvgArr[i];
      movingAvg = sum / nMovingAvg;
    }

    Serial.print(movingAvg);Serial.print("\n");
    
    if(movingAvg > maxAllowedAccel){
      sampleNumber = -1; //re-populate array for next 5 loops  
      
      //if there's tampering, output a 1 to Blynk and exit the program
      BlynkWrite(6, 1, 1);
      delay(2000);
      
      t = now();
      BlynkWrite(49, 0, year(t));
      BlynkWrite(50, 0, month(t));
      BlynkWrite(51, 0, day(t));
      BlynkWrite(52, 0, hour(t));
      BlynkWrite(53, 0, minute(t));
      BlynkWrite(54, 0, second(t));
      delay(2000);

      exit(0);
    }

   
    
    sampleNumber++;  
}


//****************FUNCTIONS*************************************

struct accelData getAccelValues(void){
    int x, y, z;
    double xG, yG, zG;
    accelgyro.getAcceleration(&x, &y, &z);

    xG = x / RANGESCALAR;
    yG = y / RANGESCALAR;
    zG = z / RANGESCALAR;

    struct accelData ret = {xG, yG, zG};
    return ret;    
}

double getMagnitude(struct accelData a){
  return sqrt(sq(a.accelX) + sq(a.accelY) + sq(a.accelZ));
}

void onboardLEDToggle(void){
  if(!ledState)
    digitalWrite(LED_PIN, HIGH);
  else
    digitalWrite(LED_PIN, LOW);
  ledState = !ledState;
}

void BlynkWrite(int pin, int intvalue, double floatvalue) {

    Serial1.print(pin);
    Serial1.print(",");
    Serial1.print(intvalue);
    Serial1.print(",");
    Serial1.print(floatvalue);
    Serial1.print("\n");
    delay(10);
}





