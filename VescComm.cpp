
//TODO: Implement vesc communication

// 2D array for arbitrary # of PAS levels? 
// Speed calculation function
// Emergency stop UART function
// return data - speed, temps, wattage, input voltage


//TODO: reconfigure arduino example code for the full controller setup:
// gonna need the other functions from VescUart too probably, and LeoDJs new stuff


#include <VescUart.h>

/** Initiate VescUart class */
VescUart UART;

const int SWITCH_PIN = 7; // Digital output pin the physical switch is connected to
boolean prevState = false; // We only send data when the state of the switch changes
double speedFact = 0; // The factorial which when multipled with ERPM, returns the speed



void setup() {

  /** Setup UART port (Serial1 on Atmega32u4) */
  Serial1.begin(19200);

  while (!Serial1) {;}

  /** Define which ports to use as UART */
  UART.setSerialPort(&Serial1);

  pinMode( SWITCH_PIN, INPUT_PULLUP );

  UART.getMcConfValues(); // TODO: what if it doesn't get the values instantly?

  speedFact = ((double(UART.mcconf.motorPoles) / 2.0) * 60.0 *
            UART.mcconf.gearRatio) /
            (UART.mcconf.wheelDiameter * M_PI);
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