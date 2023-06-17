

/*  

    TODO: implement battery monitoring  

    Will now be getting battery input data from IOManager for voltages & temps, 
    full input voltage will be got from VescComm, 
    needs to send back signal to kill the bike when voltages are too low, as well as alerts beforehand

*/


//TODO: reconfigure old arduino battery code for the new ESP32 controller:

float readVoltage(int pin){
  int data = muxAnalogRead(pin);
  float voltage = data / 40.92;
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

class Pack {
  public:
    Pack(int PackId, int mosPin, int v1, int v2, int v3, int t1, int t2, int t3);
    bool checkStatus();
    bool getStatus();
    int getTempStatus();
    int getVoltStatus();
    int getId(); // Don't use int for ID, use somat else man
  private:
    int id;
    void turnOffPack();
    void turnOnPack();
    
    bool underVolt;
    bool overTemp;
    
    int mosfetPin;
    int voltPin[3];
    int tempPin[3];
};

Pack::Pack(int packId, int mosPin, int v1, int v2, int v3, int t1, int t2, int t3) {
  id = packId;
  mosfetPin = mosPin;
  voltPin[0] = v1;
  voltPin[1] = v2;
  voltPin[2] = v3;
  tempPin[0] = t1;
  tempPin[0] = t2;
  tempPin[0] = t3;
  
  pinMode(mosfetPin, OUTPUT);
  turnOffPack();
  for(int pin : voltPin){
    pinMode(pin, INPUT);
  }
  for(int pin : tempPin){
    pinMode(pin, INPUT);
  }
}

int Pack::getId(){
  return id;
}

void Pack::turnOffPack(){
  digitalWrite(mosfetPin, LOW);   // turn the Battery Pack off (LOW is the voltage level)
//  packStatus = false;
}

void Pack::turnOnPack(){
  digitalWrite(mosfetPin, HIGH);   // turn the Battery Pack on (HIGH is the voltage level)
//  packStatus = true;
}

bool Pack::getStatus(){
  return underVolt && overTemp;
}

bool Pack::checkStatus(){
  for(int i = 0; i < SERIAL_BAT_NUM; i++) {
    
    float voltage = readVoltage(voltPin[i]); // Check voltage for each battery in pack
    if(!underVolt){
      if(voltage < VOLTAGE_CUT_OFF){
        turnOffPack();
        return false;
      }
    } else {
      if(voltage > VOLTAGE_RECOVERY){
        turnOnPack();
      }
    }
    
    float temperature = readTemperature(tempPin[i]); // Check temps for each battery in pack
    if(!overTemp){
      if(temperature < TEMPERATURE_CUT_OFF){ 
        turnOffPack();
        return false;
      }
    } else {
      if(temperature > TEMPERATURE_RECOVERY){
        turnOnPack();
      }
    }
  }
  return true;
}

Pack packs[] = {
  Pack(1, A0, 0, 1, 2, 3, 4, 5),
  Pack(2, A1, 6, 7, 8, 9, 10, 11)
}; //TODO: ensure all pins set to correct pin on arduino and multiplexer

void setup() {
  Serial.println("Starting Up");
  pinMode(ledPin, OUTPUT); // LED Pin setup
  digitalWrite(ledPin, HIGH);
  
  bool startupReady = false;
  bool allOK = true;
  
  for(Pack pack : packs){
      Serial.print("Pack ");
      Serial.print(pack.getId());
    if (pack.checkStatus()) {
      Serial.println(" OK");
      startupReady = true;
    } else {
      Serial.println(" Fail");
      allOK = false;
    }
  }
  
  if (startupReady && allOK){
    Serial.println("Pack all OK, starting loop...");
    blinkLEDAllOK();
  } else if (startupReady && !allOK) {
    Serial.println("Partial OK status, starting loop...");
    // Blink LED to show partial pack ready - can use, but not with all packs
  } else {
    // Blink LED to show total startup failure, and exit
    Serial.println("Pack failure, exiting...");
    exit(0);
  }
}

int loopCount = 0;
void loop() {
  bool packsEnabled = false;
  for(Pack pack : packs){
    if(pack.getStatus()) {
      pack.checkStatus();
      if (!pack.getStatus()) {
        Serial.print("Pack ");
        Serial.print(pack.getId());
        Serial.println(" Fail");
      } else {
        Serial.print("Pack ");
        Serial.print(pack.getId());
        Serial.println(" OK");
      }
    } else if (loopCount == LOOPS_BEFORE_RECOVERY_CHECK) {
        Serial.println("Runnning Reenable check");
      pack.checkStatus();
      loopCount = 0;
    }
    packsEnabled = pack.getStatus(); // If this is false, all packs are disabled
  }
  if(!packsEnabled){
    
    // Add LED Blinking as in startup
//    exit(0);
  }
  loopCount += 1;
  delay(LOOP_EXECUTION_DELAY_MS);
}

void blinkLEDAllOK(){
    digitalWrite(7, LOW);   
    delay(50);
    digitalWrite(7, HIGH);   
    delay(250);
    digitalWrite(7, LOW);  
    delay(50);
    digitalWrite(7, HIGH);   
    delay(250);
    digitalWrite(7, LOW);   
}

void blinkLEDPartialOK(){
    digitalWrite(7, LOW);   
    delay(50);
    digitalWrite(7, HIGH);   
    delay(150);
    digitalWrite(7, LOW);   
    delay(150);
    digitalWrite(7, HIGH);   
    delay(150);
    digitalWrite(7, LOW);   
    delay(150);
    digitalWrite(7, LOW);   
}

void blinkLEDUnderVolt(){
  
}

void blinkLEDOverTemp(){
  
}

int muxChannel[16][4]={
  {0,0,0,0}, //channel 0
  {1,0,0,0}, //channel 1
  {0,1,0,0}, //channel 2
  {1,1,0,0}, //channel 3
  {0,0,1,0}, //channel 4
  {1,0,1,0}, //channel 5
  {0,1,1,0}, //channel 6
  {1,1,1,0}, //channel 7
  {0,0,0,1}, //channel 8
  {1,0,0,1}, //channel 9
  {0,1,0,1}, //channel 10
  {1,1,0,1}, //channel 11
  {0,0,1,1}, //channel 12
  {1,0,1,1}, //channel 13
  {0,1,1,1}, //channel 14
  {1,1,1,1}  //channel 15
};

int muxAnalogRead(int pin){
  int controlPin[] = {S0, S1, S2, S3};

  //loop through the 4 sig
  for(int i = 0; i < 4; i ++){
    digitalWrite(controlPin[i], muxChannel[pin][i]);
  }

  //read the value at the SIG pin
  int val = analogRead(MUX_SIG_PIN);
  return val;
  
//  //return the value
//  float voltage = (val * 5.0) / 1024.0;
//  return voltage;
}
