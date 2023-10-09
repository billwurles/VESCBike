//TODO: I made the pin numbers for literally everything up, sort that out

/*---------------- PAS Levels User Definable -------------------*/ 

// Speed, Duty Cycle, Wattage, Amperage, -1 is no limit
// e.g. {25, -1, 500, -1} - 25 km/h speed limit, no duty cycle limit, 500 watt limit, no amperage limit

#define PAS_DEFAULT_LEVEL               4
#define PAS_LEVELS_ON_ROAD              3
#define PAS_LIMITS_ON_ROAD              (int[PAS_LEVELS_ON_ROAD][4]){ {25, -1, 60, -1},      {25, -1, 175, -1},    {25, -1, -250, -1} }
#define PAS_LEVELS_OFF_ROAD             
#define PAS_LIMITS_OFF_ROAD             (int[PAS_LEVELS_OFF_ROAD][4]){ {25, -1, 350, -1},    {30, -1, 750, -1},    {38, -1, 1000, -1},     {-1, -1, -1, -1} }

/*---------------- VESC Config User Definable ------------------*/ 

#define UART_PINS 1 //TODO: is this even neccesary

/*---------------- IO Config User Definable --------------------*/ 
#define BUTTON_PINS                     (int[]){13, 14, 15, 16} // TODO: this isn't goig to work really, apaprently you need a separate function for each pin
#define SECRET_PIN                      17
#define THROTTLE_REGEN_SWITCH           18
#define THROTTLE_RESISTANCE_SWTICH      19
#define PAS_ASSIST_UP_PIN               20
#define PAS_ASSIST_DOWN_PIN             21
#define BRAKE_SENSOR_PIN A20 // Hall sensor so prob needs analog pin?

/*---------------- Battery Config User Definable ---------------*/ 

#define BATTERY_MONITORING_ENABLED       true
#define PARALLEL_PACK_NUM                1  // Number of battery packs in parallel
#define SERIES_BAT_NUM                   3  // Number of batteries in series within a pack
#define BATTERY_VOLT_PINS                (int[]){A1, A2, A3} // ADC pins for check the voltage
#define BATTERY_TEMP_PINS                (int[]){A4, A5, A6}
#define VOLTAGE_CUT_OFF                  16.00 //TODO: Ensure this is a good voltage
#define VOLTAGE_RECOVERY                 17.77 //TODO: Ensure this is a good voltage to restart, doesn't matter much
#define TEMPERATURE_CUT_OFF              70.00 //TODO: Ensure this is a good temp (Deg C) - this was taken from a chinese bms
#define TEMPERATURE_RECOVERY             55.00 //TODO: Ensure this is a good temp (Deg C) - this was taken from a chinese bms

/*--------------------------------------------------------------*/ 




/*---------------- Non user definable config -------------------*/ // Generally speaking don't change any values under this smart guy
/*---------------- Battery Config ------------------------------*/

#define LOOPS_BEFORE_RECOVERY_CHECK   30 // How many loops will occur before overtemp / undervolt recovery is checked
#define LOOP_EXECUTION_DELAY_MS       50 // Delay between each loop execution

// THERMISTOR READING CONSTANTS ----/
#define THERMISTORNOMINAL               10000      
// temp. for nominal resistance (almost always 25 C)
#define TEMPERATURENOMINAL              25   
// how many samples to take and average, more takes longer
// but is more 'smooth'
#define NUMSAMPLES                      5
// The beta coefficient of the thermistor (usually 3000-4000)
#define BCOEFFICIENT                    3950
// the value of the 'other' resistor
#define SERIESRESISTOR                  10000  

// MULTIPLEXER CONSTANTS ----/
#define MUX_SIG_PIN A10
#define S0 21
#define S1 20
#define S2 19
#define S3 18
//----------------------------------------------------------------/


// rESCue info on voltage calibration: ---------------------------/

/**** Calibration / Calculation of VOLTAGE_DIVIDER_CONSTANT ****

To measure voltages between 0V and ~100V we need a voltage divider. I use
a voltage divider of a 470k and a 22k resistor.
The 470k is connected between the battery plus and the ESP32 GPIO (e.g. 36)
The 22k is connected between the battery plus and battery minus.
Since the ESP32 Pin also has an resistance, we have to measure and calculate the constant.

Take a power supply and connect it to the voltage divider. Set the voltage to a constant value 
in the upper range of your maximal battery voltage, e.g 50V. Than measure the voltage at the 
outside of the voltage divider. 
You now have to divide the constant voltage with the measured value, e.g

50.4V / 1.822V =  27.69

Do the same at the lower edge of your battery voltage, e.g. 40V

40V / 1.445V = 27.68

Now you have two values for the typical range of your battery. In this example it's 
a value of 27.69 as VOLTAGE_DIVIDER_CONSTANT is pretty ok.

You always have to do the calibration when changing the resistors or the ESP32.

****/