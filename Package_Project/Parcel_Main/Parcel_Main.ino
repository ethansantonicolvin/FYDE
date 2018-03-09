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
#define maxAllowedAccel 8.5

MPU6050 accelgyro;
struct accelData getAccelValues(void);
double movingAvgArr[nMovingAvg];
int sampleNumber = 0;
double movingAvg;
boolean ledState = false;

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

    Serial.begin(38400);

    // initialize device
    Serial.println("Initializing I2C devices...");
    accelgyro.initialize();

    // verify connection
    Serial.println("Testing device connections...");
    Serial.println(accelgyro.testConnection() ? "MPU6050 connection successful" : "MPU6050 connection failed");

    // setting offsets to calibrate sensor
    accelgyro.setXGyroOffset(34);
    accelgyro.setYGyroOffset(68);
    accelgyro.setZGyroOffset(12);
    accelgyro.setXAccelOffset(-1585);
    accelgyro.setYAccelOffset(629);
    accelgyro.setZAccelOffset(824);
    
}

//*****************MAIN LOOP************************************

void loop() {
    struct accelData data = getAccelValues();
    //Serial.print(data.accelX);Serial.print("\t");
    //Serial.print(data.accelY);Serial.print("\t");
    //Serial.print(data.accelZ);Serial.print("\t");
    //Serial.print(getMagnitude(data));Serial.print("\t"); 

    if(sampleNumber < 5){ // don't do moving average until we have 5 values
      movingAvgArr[sampleNumber] = getMagnitude(data);
      movingAvg = getMagnitude(data);
    }
    else{
      movingAvgArr[sampleNumber % 5] = getMagnitude(data);
      movingAvg = (movingAvgArr[0] + movingAvgArr[1] + movingAvgArr[2] + movingAvgArr[3] + movingAvgArr[4])/5.0;        
    }

    Serial.print(movingAvg);Serial.print("\n");
    
    if(movingAvg > maxAllowedAccel){
      onboardLEDToggle();
      delay(2000);
      onboardLEDToggle();
      sampleNumber = -1; //re-populate array for next 5 loops  
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



