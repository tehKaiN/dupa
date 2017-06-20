
/*
 * Arduino prg to drive D.U.P.A. device
 */

#include <Stepper.h>

// //////////////////////////////////////////////////////////// DEFINES & CONSTS

// Serial-relevant
const uint8_t s_ubSerialBfrMaxLength = 150;
bool s_bSerialRxReady = false;
char s_szSerialBfr[s_ubSerialBfrMaxLength];
uint8_t s_ubSerialBfrLength = 0;

// Stepper-relevant
const int stepsPerRevolution = 100;
Stepper myStepper(stepsPerRevolution, 8, 9, 10, 11);

// Sensor-relevant
#define SENSOR_PIN A1

// //////////////////////////////////////////////////////////// SENSOR FNS

/**
 * Measures voltage on sharp distance sensor.
 * @return Measured voltage, in 0..1023
 */
int measureSensor(void) {
  return analogRead(SENSOR_PIN);
}

/**
 * Does gets average distance measurment from given amount of measures.
 * Will work for up to 65 samples.
 * @param sampleCount Number of samples to be measured for avg calc.
 * @return            Averaged measurement on 
 */
int measureAveraged(uint8_t sampleCount) {
  uint8_t i;
  uint16_t sum = 0;
  for(i = sampleCount; i--;) {
    delay(5);
    sum += measureSensor();
  }
  return sum/sampleCount;
}

// //////////////////////////////////////////////////////////// SERIAL FNS

/**
 * Serial read event handler.
 */
void serialEvent() {
  // Read message from PC app
  while(Serial.available() && !s_bSerialRxReady) {
    char c = (char)Serial.read();
    
    if(c != '\n') {
      s_szSerialBfr[s_ubSerialBfrLength] = c;
      ++s_ubSerialBfrLength;      
    }
    else {
      s_bSerialRxReady = true;
      s_szSerialBfr[s_ubSerialBfrLength] = '\0';
      ++s_ubSerialBfrLength;      
    }
  }
}

void serialProcessCmd(void) {
  if(!s_bSerialRxReady)
    return;
  if(!strcmp(s_szSerialBfr, "read")) {
    doScan(false);
  }
  else if(!strcmp(s_szSerialBfr, "readBetter")) {
    doScan(true);
  }
  else {
    Serial.print("Invalid cmd!\n");
  }
  s_bSerialRxReady = false;
  s_ubSerialBfrLength = 0;
}

// //////////////////////////////////////////////////////////// READOUT

/**
 * Does full scanning.
 */
void doScan(bool bidir) {
  int i, steps, maxSteps;
  uint16_t samples[400];
  char txBfr[50];

  // Make sure you're on A
  // Serial.println("Going to A");
  while(digitalRead(4) == LOW) {
    myStepper.step(1);
  }
  
  steps = 0;

  // Go to B doing measurements
  // Serial.println("Measuring while going to B");
  while(digitalRead(3) == LOW) {
    samples[steps] = measureAveraged(2);
    myStepper.step(-1); // step(-) makes movement towards B
    steps += 1;
  }

  maxSteps = steps;
  delay(100);

  if(bidir) {
    // Go to A doing measurements
    // Serial.println("Measuring while going to A");
    while(digitalRead(4) == LOW) {
      myStepper.step(1);
      steps -= 1;
      samples[steps] = (samples[steps] + measureAveraged(2)) / 2;
    }
  }
  else {
    // Go to A without measurements
    // Serial.println("Going to A");
    while(digitalRead(4) == LOW)
      myStepper.step(1);
    steps = 0;
  }

  // Make some steps towards B so we won't press A continuously.
  myStepper.step(-10);
  myStepper.step(1);
  myStepper.step(0);

  // Send data
  for(i = 0; i != maxSteps; ++i) {
    sprintf(txBfr, "%d;%u", i, samples[i]);
    Serial.println(txBfr);
    delay(10);
  }
  Serial.print("end\n");
}

// //////////////////////////////////////////////////////////// ARDUINO MAIN

void setup() {
  myStepper.setSpeed(120);
  pinMode(4, INPUT); // krancowka A
  pinMode(3, INPUT); // krancowka B
  // initialize the serial port:
  Serial.begin(9600);
  pinMode(13, OUTPUT);
  s_bSerialRxReady = false;
  s_ubSerialBfrLength = 0;
}

void loop() {
  serialProcessCmd();
}
