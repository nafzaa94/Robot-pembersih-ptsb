#include <ESP32Servo.h>

Servo myservo;

int servoPin = 18;

int irsensor1 = 34;
int irsensor2 = 35;

int valueirsensor1 = 0;
int valueirsensor2 = 0;

int state = 0;

void setup() {
  pinMode(irsensor1, INPUT);
  pinMode(irsensor2, INPUT);
  ESP32PWM::allocateTimer(0);
  ESP32PWM::allocateTimer(1);
  ESP32PWM::allocateTimer(2);
  ESP32PWM::allocateTimer(3);
  myservo.setPeriodHertz(50);// Standard 50hz servo
  myservo.attach(servoPin, 1000, 2000);

  myservo.write(0);

}

void loop() {
  valueirsensor1 = analogRead(irsensor1); //0 - 1023
  valueirsensor2 = analogRead(irsensor2);

  valueirsensor1 = map(valueirsensor1, 0, 1023, 10, 80);
  valueirsensor2 = map(valueirsensor2, 0, 1023, 10, 80);

  if (valueirsensor1 <= 40 && state == 0){
    myservo.write(60);
    delay(1000);
    myservo.write(0);
    state = 1;
    }

  else {
    state = 0;
    } 

}
