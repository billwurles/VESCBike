class VescComm {
    struct vescData { // ideally only VescComm can edit this info, quick and dirty for now
        // --- dataPackage:
        float avgMotorCurrent;
        float avgInputCurrent;
        float dutyCycleNow;
        float rpm;
        float inpVoltage;
        float ampHours;
        float ampHoursCharged;
        float wattHours;
        float wattHoursCharged;
        long tachometer;
        long tachometerAbs;
        float tempMosfet;
        float tempMotor;
        float pidPos;
        // uint8_t id;
        // mc_fault_code error;  // TODO: useful for future -- full vesc error definition, just use vescError bool for now
        bool vescError; // if mc_fault_Code != 0 then error has occurred

        // --- McConf data package:
        float currentMin;
        float currentMax;
        float erpmMin;
        float erpmMax;
        float dutyMin;
        float dutyMax;
        float wattMin;
        float wattMax;
        float inCurrentMin;
        float inCurrentMax;
        int motorPoles;
        float gearRatio;
        float wheelDiameter;

        // --- ADC data package (throttle / regen levels)
        int throttle; //ADC1
        int regen; //ADC2
        // int32_t decodedLevel;   // 0-1000000
        // int32_t voltage;        // µV?
        // int32_t decodedLevel2;
        // int32_t voltage2;
	};

    public:
        bool disableMotor();
        bool enableMotor();
        bool pollData();
        bool setAssistLevel(float speed, float current, float watt, float duty);
        
        vescData data;

    private:
        VescUart uart;
        double speedFact; // The factorial which when multipled with ERPM, returns the speed
        float[4] mcconfSave

};