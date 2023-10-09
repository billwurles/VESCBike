#include "IOManager.h"

// TODO: write iomanager

// read only variables for all input data, bools / floats (or double?)


IOManager::IOManager(VescComm comm, function<void(void)> levelUpFunc, function<void(void)> levelDownFunc){
  vesc = vesccomm;
  pinMode(PAS_ASSIST_UP_PIN, INPUT_PULLUP);
  attachInterrupt(digitalPinToInterrupt(PAS_ASSIST_UP_PIN), levelUpFunc, CHANGE)
  pinMode(PAS_ASSIST_DOWN_PIN, INPUT_PULLUP);
  attachInterrupt(digitalPinToInterrupt(PAS_ASSIST_DOWN_PIN), levelDownFunc, CHANGE)

  pinMode(BRAKE_SENSOR_PIN, INPUT_PULLUP);
  attachInterrupt(digitalPinToInterrupt(BRAKE_SENSOR_PIN), brakeSensorChange, CHANGE)
  pinMode(SECRET_PIN, INPUT_PULLUP);
  attachInterrupt(digitalPinToInterrupt(SECRET_PIN), brakeSensorChange, CHANGE)

  pinMode(THROTTLE_REGEN_SWITCH, OUTPUT);
  pinMode(THROTTLE_RESISTANCE_SWTICH, OUTPUT);

  // Initialize Battery pack
  //TODO: if volt pins != temp pins len throw error - could have alt mode where volts + temps separate 

  for(int i = 0, p = 0, s = 0; i < BATTERY_VOLT_PINS; i++) {
    p = i / SERIES_BAT_NUM;
    s = i - (p * SERIES_BAT_NUM);

    pinMode(BATTERY_VOLT_PINS[i], INPUT_PULLUP);
    pinMode(BATTERY_TEMP_PINS[i], INPUT_PULLUP);
    batteryPins[p][s][0] = BATTERY_VOLT_PINS[i];
    batteryPins[p][s][1] = BATTERY_TEMP_PINS[i];
  }

}

IOManager::brakeSensorChange(){
  // check if rising or falling
  // if engaged - disable motor, switch throttle to regen
  // if disengaged - reenable motor, switch regent back to throttle
}

IOManager::pressCapacitiveButton(){
  // initiate some kind of timer
  // if held for ~3 seconds, and then horn button is pressed, enable off road mode
}

IOManager::pressHornButton(){
  // initiate some kind of timer
  // if held for ~3 seconds, and then another button is pressed, enable off road mode
}

float[] IOManager::getBatteryData(int p, int s){
  float data[2];
  data[0] = readVoltage(batteryPins[p][s][0]); // Check voltage for each battery in pack
  data[1] = readTemperature(batteryPins[p][s][1]); // Check temps for each battery in pack

  return data;
}
    /*  
            Inputs:
        interrupts: Brake Sensors, handlebar buttons, secret capacitive touch button?
        polled: battery data

            Outputs:
        throttle resistance switch
        throttle / regen switch,

        attach interrupts to pins defined in config.h
    */


}

Batt loop() {
}

// TODO: functions to toggle switches

// interrupt functions to update data - if instant response is required it may be neccesary to trigger the desired function from this context, not main
// could just have stuff return out of function when instant response is required?



float readVoltage(int pin){
  int data = analogRead(pin);
  float voltage = data / 40.92; // TODO: what is this dumb magic number
  printVoltage(pin, voltage);
  return voltage;
}

//float readTemperatureOLD(int pin){
//  int data = analogRead(pin);
//  float temperature = 0.00; // Figure out how to read a thermistor
//  printTemperature(pin, temperature);
//  return temperature;
//}

float readTemperature(int pin){
  uint8_t i;
  float average;
  int samples[NUMSAMPLES];

  // take N samples in a row, with a slight delay
  for (i=0; i< NUMSAMPLES; i++) {
   samples[i] = analogRead(pin);
//   Serial.print("RAW READING: ");
//   Serial.println(samples[i]);
   delay(10);
  }
  
  // average all the samples out
  average = 0;
  for (i=0; i< NUMSAMPLES; i++) {
     average += samples[i];
  }
  average /= NUMSAMPLES;

//  Serial.print("Average analog reading "); 
//  Serial.println(average);
  
  // convert the value to resistance
  average = 1023 / average - 1;
  average = SERIESRESISTOR / average;
//  Serial.print("Thermistor resistance "); 
//  Serial.println(average);
  
  float steinhart;
  steinhart = average / THERMISTORNOMINAL;     // (R/Ro)
  steinhart = log(steinhart);                  // ln(R/Ro)
  steinhart /= BCOEFFICIENT;                   // 1/B * ln(R/Ro)
  steinhart += 1.0 / (TEMPERATURENOMINAL + 273.15); // + (1/To)
  steinhart = 1.0 / steinhart;                 // Invert
  steinhart -= 273.15;                         // convert absolute temp to C

  return steinhart;
//  Serial.print("Temperature "); 
//  Serial.print(steinhart);
//  Serial.println(" *C");
}

void printVoltage(int pin, float voltage){
    Serial.print("Pin ");
    Serial.print(pin);
    Serial.print(" Voltage: ");
    Serial.print(voltage); 
    Serial.println("v");
}

void printTemperature(int pin, float temperature){
    Serial.print("Pin ");
    Serial.print(pin);
    Serial.print(" Temperature: ");
    Serial.print(temperature); 
    Serial.println("°C");
}
