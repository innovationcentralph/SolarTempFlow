#include <OneWire.h>
#include <DallasTemperature.h>
#include <LiquidCrystal_I2C.h>

#include "DHT.h"
#include "SystemConfig.h"

OneWire oneWire(ONE_WIRE_BUS);
DallasTemperature surfaceTemp(&oneWire);
LiquidCrystal_I2C lcd(0x27,20,4);
DHT ambientTemp(DHTPIN, DHTTYPE);

double flowRate;    //This is the value we intend to calculate.
volatile int count; //This integer needs to be set as volatile to ensure it updates correctly during the interrupt process.

uint32_t updateMillis = 0;

void setup() {

  Serial.begin(9600);
  
  lcd.init();                  
  lcd.backlight();

  surfaceTemp.begin();
  ambientTemp.begin();

  pinMode(FLOW_SENSE_PIN, INPUT_PULLUP);           
  attachInterrupt(1, Flow, RISING);  //Configures interrupt 0 (pin 2 on the Arduino Uno) to run the function "Flow"

  pinMode(MOTOR_CONTROL_PIN, OUTPUT);
  
  lcd.clear();
  lcd.setCursor(0,0);
  lcd.print("   System  Monitor  ");
  lcd.setCursor(0,1);
  lcd.print("A Temp:");
  lcd.setCursor(0,2);
  lcd.print("S Temp:");
  lcd.setCursor(0,3);
  lcd.print("  Flow:");

  updateMillis = millis();

}

void loop() {

  // Read Temperature Difference
  float surfaceTemperature, ambientTemperature;

  surfaceTemperature = readSurfaceTemp();
  ambientTemperature = readAmbientTemp();

  if(ambientTemperature == 0 || ambientTemperature == 0){
    // do nothing
  }

  else{

    float tempDiff = surfaceTemperature - ambientTemperature;
    
    if(tempDiff < TH_1){
      Serial.println("WATER OFF");
      analogWrite(MOTOR_CONTROL_PIN, 0);
    }
    else if(tempDiff >= TH_1 && tempDiff < TH_2){
      Serial.println("WATER FLOW 1");
      analogWrite(MOTOR_CONTROL_PIN, 100);
    }
    else if(tempDiff >= TH_2 && tempDiff < TH_3){
      Serial.println("WATER FLOW 2");
      analogWrite(MOTOR_CONTROL_PIN, 130);
    }
    else if(tempDiff >= TH_3 && tempDiff < TH_4){
      Serial.println("WATER FLOW 3");
      analogWrite(MOTOR_CONTROL_PIN, 160);
    }
    else if(tempDiff >= TH_4 && tempDiff < TH_5){
      Serial.println("WATER FLOW 4");
      analogWrite(MOTOR_CONTROL_PIN, 190);
    }
    else if(tempDiff >= TH_5){
      Serial.println("WATER FLOW 5");
      analogWrite(MOTOR_CONTROL_PIN, 220);
    }
  }

  //Get flow rate
  float flowRate = getFlowRate();
  
  if(millis() - updateMillis >  3000){

    lcd.setCursor(0,0);
    lcd.print("  System  Monitor!  ");
    lcd.setCursor(0,1);
    lcd.print("A Temp: " + String(ambientTemperature, 1) + "C  ");
    lcd.setCursor(0,2);
    lcd.print("S Temp: " + String(surfaceTemperature, 1) + "C  ");
    lcd.setCursor(0,3);
    lcd.print("  Flow: " + String(flowRate) + "L/m  ");
  
    updateMillis = millis();
  }
  
}
