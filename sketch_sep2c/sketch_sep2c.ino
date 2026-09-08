#include <LiquidCrystal.h>
#define ECHO_PIN  3
#define TRIGGER_PIN 4
#define RS_PIN A5 
#define E_PIN A4
#define D4 6
#define D5 7
#define D6 8
#define D7 9

LiquidCrystal lcd(RS_PIN, E_PIN, D4, D5, D6, D7);

 unsigned long startTime = micros();
 unsigned long timeDelay = 60000;

volatile unsigned long pulseBegin;
volatile unsigned long pulseEnd;
volatile bool flag = false;

void triggerSensor(){
  digitalWrite(TRIGGER_PIN,LOW);
  delayMicroseconds(2);
  digitalWrite(TRIGGER_PIN, HIGH);
  delayMicroseconds(10);
  digitalWrite(TRIGGER_PIN, LOW);

}
double readPulseForUltraSonicDistance(){
  double pulseDuration = pulseEnd - pulseBegin;
  double distance = pulseDuration/58.0;
  return distance;

}


void setup() {
  // put your setup code here, to run once:
  Serial.begin(115200);
  lcd.begin(16,2);
  lcd.print("Hello World!");
  pinMode(ECHO_PIN,INPUT);
  pinMode(TRIGGER_PIN,OUTPUT);
  
  attachInterrupt(digitalPinToInterrupt(ECHO_PIN),
                                        echoPinInterrupt,
                                        CHANGE);

}

void echoPinInterrupt()
{
  if(digitalRead(ECHO_PIN)==HIGH){
    pulseBegin=micros();
  }
  else{
    pulseEnd=micros();
    flag=true;
  }
}
void loop() {
  // put your main code here, to run repeatedly:
  unsigned long endTime = micros();
  if(endTime-startTime>timeDelay){
    startTime+=timeDelay;
    triggerSensor();
  }
  if(flag){
    flag=false;
    double distance = readPulseForUltraSonicDistance();
    lcd.setCursor(0,0);
    lcd.print("Rate: ");
    lcd.print(1/((double)timeDelay/1000000));
    lcd.print(" Hz");
    lcd.setCursor(0,1);
    lcd.print("Distance:");
    lcd.print(distance, 1);
    lcd.print(" cm");

  }

}
