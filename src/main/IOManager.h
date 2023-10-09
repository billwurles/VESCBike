//#include "AssistController.h"

class IOManager {
  struct batteryData {
    float volt;
    float temp;
  };
  
  public:
    IOManager(VescComm comm, function<void(void)> levelUpFunc, function<void(void)> levelDownFunc);
    batteryData getBatteryData(int p, int s);

  private:
    VescComm vesc;

    float batteryPins[PARALLEL_PACK_NUM][SERIES_BAT_NUM][2]; // [0] is Voltage [1] is temperature

    void brakeSensorChange()
    void pressCapacitiveButton()
    void pressHornButton()
};
