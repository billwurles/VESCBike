#include "VescComm.h"
#include "config.h"
#include "IOManager.h"

class BatteryMonitor {
  public:
    BatteryMonitor(VescComm vesc);

    void updateBatteryData(float[][][] data);
    
    void setInputVoltage(int voltage);
    bool checkStatus();
    bool getStatus();
    float[][][] getData();
    int getTempStatus();
    int getVoltStatus();

  private:
    VescComm vesc;
    IOManager io;

    int highestTemp;
    int inputVoltage;
    
    bool underVolt;
    bool overTemp;


    float batteries[PARALLEL_PACK_NUM][SERIES_BAT_NUM][2]; // [0] is Voltage [1] is temperature
};