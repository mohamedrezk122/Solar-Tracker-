#include <Servo.h>
#include <Wire.h>
#include <Adafruit_Sensor.h>
#include <Adafruit_TSL2561_U.h>
#include <SoftwareSerial.h>

int val =  123 ; 
SoftwareSerial myBlue(2, 4); // RX | TX

// 180 horizontal MAX
Servo horizontal; // horizontal servo
int servoh = 180;   // 90;     // stand horizontal servo

int servohLimitHigh = 180;
int servohLimitLow = 65;

// 65 degrees MAX
Servo vertical;
int servov = 45;

int servovLimitHigh = 80;
int servovLimitLow = 15;


// LDR pin connections

int ldrlt = A0; //LDR top left - BOTTOM LEFT    <--- BDG
int ldrrt = A1; //LDR top rigt - BOTTOM RIGHT
int ldrld = A2; //LDR down left - TOP LEFT
int ldrrd = A3; //ldr down rigt - TOP RIGHT


/*current Pins*/
const int currentPin = A4;
int sensitivity = 66;
int adcValue= 0;
int offsetVoltage = 2500;
int adcVoltage = 0;
int currentValue = 0;
float acPower = 0.0;
float thPower = 0.0;
float lightInt = 0.0;
float surfaceA = 1.0;
float eff = 1.0;



Adafruit_TSL2561_Unified tsl = Adafruit_TSL2561_Unified(TSL2561_ADDR_FLOAT, 12345);

void displaySensorDetails(void){

  sensor_t sensor;
  tsl.getSensor(&sensor);
}

void configureSensor(void){
  tsl.enableAutoRange(true);            /* Auto-gain ... switches automatically between 1x and 16x */

  // tsl.setIntegrationTime(TSL2561_INTEGRATIONTIME_13MS);      /* fast but low resolution */
  tsl.setIntegrationTime(TSL2561_INTEGRATIONTIME_101MS);  /* medium resolution and speed   */
}



void setup(){

  Serial.begin(9600);
  myBlue.begin(9600);
  horizontal.attach(9);
  vertical.attach(10);
  horizontal.write(180);
  vertical.write(45);

  pinMode(currentPin, INPUT);


  displaySensorDetails();

  configureSensor();

  delay(3000);
}

void curCalc(){
  adcValue = analogRead(currentPin);
  adcVoltage = (adcValue / 1024.0) * 5000;
  currentValue = ((adcVoltage - offsetVoltage) / sensitivity);
  return currentValue;
}


void loop(){

  int lt = analogRead(ldrlt);
  int rt = analogRead(ldrrt);
  int ld = analogRead(ldrld);
  int rd = analogRead(ldrrd);


  int dtime = 10;
  int tol = 50;

  int avt = (lt + rt) / 2;
  int avd = (ld + rd) / 2;
  int avl = (lt + ld) / 2;
  int avr = (rt + rd) / 2;

  int dvert = avt - avd;
  int dhoriz = avl - avr;



  if (-1*tol > dvert || dvert > tol){ // check if the diffirence is in the tolerance else change vertical angle
    if (avt > avd){
      servov = ++servov;

       if (servov > servovLimitHigh){
          servov = servovLimitHigh;
       }
    }

    else if(avt < avd){
      servov= --servov;

      if (servov < servovLimitLow){
        servov = servovLimitLow;
      }
    }

    vertical.write(servov);
  }

  if (-1*tol > dhoriz || dhoriz > tol){ // check if the diffirence is in the tolerance else change horizontal angle

    if (avl > avr){
      servoh = --servoh;
      if (servoh < servohLimitLow){
        servoh = servohLimitLow;
      }
    }
    else if (avl < avr){
      servoh = ++servoh;
       if (servoh > servohLimitHigh){
         servoh = servohLimitHigh;
       }
    }
    horizontal.write(servoh);
  }


  curCalc();

  sensors_event_t event;
  tsl.getEvent(&event);

  
  /* Display the results (light is measured in lux) */
  if (event.light){
    lightInt = event.light;
  }


  acPower = adcVoltage * currentValue;
  thPower = (lightInt*surfaceA)/105;
  eff = acPower/thPower;

  String str = String(currentValue) + "," + String(adcVoltage) + "," + String(acPower) + "," + String(eff) + "," + String(lightInt);
  
  myBlue.print(str);
  delay(500);
}
