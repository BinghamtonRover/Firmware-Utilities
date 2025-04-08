#include "BURT_ErrorLed.h"

//Error pin -1 means that no pin is set
int errorPin = -1;

void turnOffLed() {
    if (errorPin == -1) return;
    digitalWrite(errorPin, LOW);
}

BurtErrorLed::BurtErrorLed(int pinNum) 
    : pinNum(pinNum), timer(1000, turnOffLed) {
    errorPin = pinNum;
}

void BurtErrorLed::setup(){
    pinMode(pinNum, OUTPUT);
    digitalWrite(pinNum, LOW);
}

void BurtErrorLed::update(){
    timer.update();
}

void BurtErrorLed::turnOn(){
    digitalWrite(pinNum, HIGH);
    timer.setup();
}

