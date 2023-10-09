#include <crc.h>
#include <VescUart.h>
#include <datatypes.h>
#include <buffer.h>

#include "VescComm.h"
#include <VescUart.h>
#include <Arduino.h>

//TODO: Implement vesc communication

// 2D array for arbitrary # of PAS levels? 
// Speed calculation function
// Emergency stop UART function
// return data - speed, temps, wattage, input voltage


//TODO: reconfigure arduino example code for the full controller setup:
// gonna need the other functions from VescUart too probably, and LeoDJs new stuff

/** Initiate VescUart class */

VescComm::VescComm(){
  /** Setup UART port (Serial1 on Atmega32u4) */
  VescUart uart;
  Serial1.begin(19200);

  while (!Serial1) {;}

  /** Define which ports to use as UART */
  UART.setSerialPort(&Serial1);
  UART.getMcConfValues(); // TODO: what if it doesn't get the values instantly?

  speedFact = ((double(UART.mcconf.motorPoles) / 2.0) * 60.0 *
            UART.mcconf.gearRatio) /
            (UART.mcconf.wheelDiameter * M_PI);
}

bool VescComm::disableMotor(){
  this.setAssistLevel(0, 0, 0, 0);
}
bool VescComm::enableMotor(){
  this.setAssistLevel(mcconfSave[0], mcconfSave[1], mcconfSave[2], mcconfSave[3]);
}
bool VescComm::pollData(){
  return UART.getVescValues()
}

bool VescComm::setAssistLevel(float speed, float current, float watt, float duty){
  mcconfSave = {speed, current, watt, duty};
  if(speed > -1){
    uart.mcconf.erpmMax = speed * speedFact;
  }
  if(current > -1){
    uart.mcconf.current = current;
  }
  if(watt > -1){
    uart.mcconf.watt = watt;
  }
  if(duty > -1){
    uart.mcconf.duty = duty;
  }
  UART.setMcConfValues(); 
}

void loop() {



  int switchState = digitalRead(SWITCH_PIN);

  if (switchState == HIGH) {
    if(!prevState) { // Previous state was LOW, so we change the vesc settings
        /** Set the Maximum ERPM to a very large number to effectively unrestrict the VESC */
      UART.mcconf.erpmMax = 1500000000;

        /** Call the function setMcConfValues to send the current MCCONF values to the VESC */
      UART.setMcConfValues();
      
      prevState = true; // Set state to current switch state to prevent further UART calls
    }
  } else {
    if(prevState) { // Previous state was HIGH, so we change the vesc settings
        /** Set the Maximum ERPM to a number that will limit it's road speed to 25 KM/h */
      UART.mcconf.erpmMax = 25 * speedFact;

        /** Call the function setMcConfValues to send the current MCCONF values to the VESC */
      UART.setMcConfValues();
      
      prevState = false; // Set state to current switch state to prevent further UART calls
    }
  }

}
