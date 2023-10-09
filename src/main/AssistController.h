#include "VescComm.h"
#include "IOManager.h"
#include "config.h"

class AssistController {
  public:
    AssistController(VescComm comm, IOManager manager);
    void checkLevels(IOManager io)

    bool upButtonPressed();
    bool downButtonPressed();
    bool offRoadOn();
    bool offRoadOff();

  private:
    VescComm vesc;
    IOManager manager;
    bool offroad = false;

    int assistLevel = PAS_DEFAULT_LEVEL;
    int[] levelData = PAS_LIMITS_ON_ROAD;

};
