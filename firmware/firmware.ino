
/*
 * Arduino prg to drive D.U.P.A. device
 */

#include <Stepper.h>

const int stepsPerRevolution = 100;  // change this to fit the number of steps per revolution
// for your motor

// initialize the stepper library on pins 8 through 11:
Stepper myStepper(stepsPerRevolution, 8, 9, 10, 11);

void setup() {
  // set the speed at 60 rpm:
  myStepper.setSpeed(120);
  pinMode(4, INPUT); // krancowka A
  pinMode(3, INPUT); // krancowka B
  // initialize the serial port:
  Serial.begin(9600);
  pinMode(13, OUTPUT);
}

void loop() {
  int i, kroki;
  char bfr[50];
  // step(+) jedzie w krańcówkę A
  digitalWrite(13, digitalRead(3));

  // step one revolution  in one direction:
  Serial.println("do krancowki A");
  // for(i = 0; i != stepsPerRevolution/5; ++i) {
  kroki = 0;
  while(digitalRead(4) == LOW) {
    myStepper.step(5);
    kroki += 5;
  }
  delay(100);
  myStepper.step(-10);
  sprintf(bfr, "Zrobilem %d krokow", kroki);
  Serial.println(bfr);
  // myStepper.step(stepsPerRevolution);
  delay(1000);

  // step one revolution in the other direction:
  Serial.println("do krancowki B");
  //for(i = 0; i != stepsPerRevolution/5; ++i) {
  kroki = 0;
  while(digitalRead(3) == LOW) {
    myStepper.step(-5);
    kroki += 5;
  }
  delay(100);
  myStepper.step(10);
  sprintf(bfr, "Zrobilem %d krokow", kroki);
  Serial.println(bfr);
  // myStepper.step(-stepsPerRevolution);
  delay(1000);
}

