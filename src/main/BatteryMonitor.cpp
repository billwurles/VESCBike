#include "BatteryMonitor.h"
#include "IOManager.h""

/*  

    TODO: implement battery monitoring  

    Will now be getting battery input data from IOManager for voltages & temps, 
    full input voltage will be got from VescComm, 
    needs to send back signal to kill the bike when voltages or temps are out of bounds, as well as alerts beforehand

*/

BatteryMonitor::BatteryMonitor(VescComm vesccomm, IOManager manager){
  vesc = vesccomm;
  io = manager;
}

void BatteryMonitor::updateBatteryData(int p, int s, float[] data){
  batteries[p][s] = data;
}


void BatteryMonitor::setInputVoltage(int voltage){
  inputVoltage = voltage
}

bool BatteryMonitor::checkStatus(IOManager io){
  for(int p = 0; p < PARALLEL_PACK_NUM; p++) {
    for(int s = 0; s < SERIES_BAT_NUM; s++) {
      batteries[p][s] = io.getBatteryData[p, s];
      
      float voltage = batteries[p][s][0]; // Check voltage for each battery in pack
      if(!underVolt){
        if(voltage < VOLTAGE_CUT_OFF){
          vesc.disableMotor();
          return false;
        } // TODO: Check if each series battery is dangerously different, e.g. 18v vs 21v (0.5v a good safe difference?)
      } else {
        if(voltage > VOLTAGE_RECOVERY){
          vesc.enableMotor();
        }
      }
      
      float temperature = batteries[p][s][1]; // Check temps for each battery in pack
      if(!overTemp){
        if(temperature < TEMPERATURE_CUT_OFF){ 
          vesc.disableMotor();
          return false;
        }
      } else {
        if(temperature > TEMPERATURE_RECOVERY){
          vesc.enableMotor();
        }
      }
    }
  }
}


//TODO: reconfigure old arduino battery code for the new ESP32 controller:



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
  // Maybe pass VescComm into this in constructor?
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
  for(int i = 0; i < SERIES_BAT_NUM; i++) {
    
    float voltage = readVoltage(voltPin[i]); // Check voltage for each battery in pack
    if(!underVolt){
      if(voltage < VOLTAGE_CUT_OFF){
        turnOffPack();
        return false;
      } // TODO: Check if each series battery is dangerously different, e.g. 18v vs 21v (0.5v a good safe difference?)
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
