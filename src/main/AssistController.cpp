#include "AssistController.h"

AssistController::AssistController(VescComm comm, IOManager manager){
    vesc = comm;
    io = manager;
}

void AssistController::checkLevels()){
    
}

float upButtonPressTime;
bool AssistController::upButtonPressed(){
    // check time pressed
    if(assistLevel < sizeof(assistLevels)){
        if(vesc.setAssistLevel(levelMap[assistLevel][0],levelMap[assistLevel][1],levelMap[assistLevel][2],levelMap[assistLevel][3)){
            assistLevel++;
        }
        
    }
}

float dowmButtonPressTime;
bool AssistController::downButtonPressed(){
    // check time pressed
    if(assistLevel > 1){
        if(vesc.setAssistLevel(levelMap[assistLevel][0],levelMap[assistLevel][1],levelMap[assistLevel][2],levelMap[assistLevel][3)){
            assistLevel++;
        }
        
    }
}