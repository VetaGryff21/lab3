#include <Servo.h>
#include <LedControl.h>
#include "button.h"

#define BUTTON 2
#define SERVO 5

int timeHold = 0;
int timeReleased = 0;
int startPressed = 0;
int endPressed = 0;
unsigned long timeStep = 300;

Servo myservo;
int startPosition = 0;
int finishPosition = 90;
int power = 0;
const int displaysCount = 1;
const int dataPin = 12;
const int clkPin = 10;
const int csPin = 11;
Button button(BUTTON);
LedControl led = LedControl(dataPin, clkPin, csPin, displaysCount);

void setup() {
  // put your setup code here, to run once:
  
  myservo.attach(SERVO);
  myservo.write(startPosition);
  
  resetLed();
  
  Serial.begin(9600);
  Serial.println("Started!");
}

void loop() {
  // put your main code here, to run repeatedly:
  if(button.wasPressed())
  {
    startPressed = millis();
    while(button.hold())
    {
      updatePower();
    }
    endPressed = millis();
    timeHold = endPressed - startPressed;
    Serial.println("Kick it!");
    resetLed();
    kick(power);
    delay(1000);
  }
}

void updateLed(int power) {
  if(power == 0){
    resetLed();
  }
  led.setRow(0, power, B11111111);
}

void updatePower() {
  timeHold = millis()- startPressed;
  Serial.print(timeHold);
  Serial.print("   ");
  power = (timeHold / timeStep) % 8;
  updateLed(power);
  Serial.println(power);
}

void resetLed()
{
  led.shutdown(0, false);
  led.setIntensity(0, 5);
  led.clearDisplay(0);
}

void kick(int power)
{
  for(int i=0; i<=120; i++){
    myservo.write(i);
    delay(8 - power);
  }
  delay(100);
  for(int i=120; i>=0; i--){
    myservo.write(i);
    delay(8 - power);
  }
}
