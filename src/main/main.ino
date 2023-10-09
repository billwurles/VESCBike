#include <Arduino.h>
#include "config.h"
#include "VescComm.h"
#include "AssistController.h"
#include "BatteryMonitor.h"
#include "IOManager.h"
#include "WebGUI.h"

VescComm vesc;
AssistController assist;
BatteryMonitor battery;
IOManager io;
WebGUI gui;

void setup() {
    // setup VescComm, BatteryMonitor, IOManager, WebGUI
    // TODO: low battery = low power mode
    vesc = new VescComm();
    assist = new AssistController(vesc);
    io = new IOManager(vesc, assist);
    battery = new BatteryMonitor(vesc, io);
    gui = new WebGUI(vesc, battery, io);
}

void loop() {
    battery.checkStatus(); // Checks temperatures and voltages, stops motor if out of bounds
    assist.checkLevels(io);

    /*
        TODO: figure this stuff out:

        - whether I should be writing this in pure c++ or arduino flavours
        - where are PAS levels stored / selected, I think just in their own class?
        - should IOManager interrupts just directly call the function
        - how to handle the multi button timed secret function
        - should probably learn how c++ works again lol what are header files
    
    */

   // poll VESC data, battery data, PAS level
   // update gui (prob on changes)


}