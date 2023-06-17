#include <Arduino.h>
#include <Logger.h>
#include "config.h"

#include "BleServer.h"


HardwareSerial vesc(2);
BleServer *bleServer = new BleServer();


void setup() {
    vesc.begin(VESC_BAUD_RATE, SERIAL_8N1, VESC_RX_PIN, VESC_TX_PIN, false);
#ifdef CANBUS_ONLY
    bleServer->init(canbus->stream, canbus);
#else
    bleServer->init(&vesc);
}

void loop() {
    if (AppConfiguration::getInstance()->config.sendConfig) {
        bleServer->sendConfig();
        AppConfiguration::getInstance()->config.sendConfig = false;
    }

    // call the VESC UART-to-Bluetooth bridge
#ifdef CANBUS_ENABLED
    bleServer->loop(&vescData, loopTime, maxLoopTime);
#else
    bleServer->loop();
}