#define VOLTAGE_CUT_OFF               16.00 //TODO: Ensure this is a good voltage
#define VOLTAGE_RECOVERY              17.77 //TODO: Ensure this is a good voltage to restart, doesn't matter much
#define TEMPERATURE_CUT_OFF           70.00 //TODO: Ensure this is a good temp (Deg C) - this was taken from a chinese bms
#define TEMPERATURE_RECOVERY          55.00 //TODO: Ensure this is a good temp (Deg C) - this was taken from a chinese bms

#define LOOPS_BEFORE_RECOVERY_CHECK   30 // How many loops will occur before overtemp / undervolt recovery is checked
#define LOOP_EXECUTION_DELAY_MS       50 // Delay between each loop execution
#define PARALLEL_PACK_NUM             2  // Number of battery packs in parallel
#define SERIAL_BAT_NUM                3  // Number of batteries in series within a pack

//THERMISTOR READING CONSTANTS -----------------------------------/
#define THERMISTORNOMINAL 10000      
// temp. for nominal resistance (almost always 25 C)
#define TEMPERATURENOMINAL 25   
// how many samples to take and average, more takes longer
// but is more 'smooth'
#define NUMSAMPLES 5
// The beta coefficient of the thermistor (usually 3000-4000)
#define BCOEFFICIENT 3950
// the value of the 'other' resistor
#define SERIESRESISTOR 10000   
int ledPin = A9;

//MULTIPLEXER CONSTANTS ------------------------------------------/
#define MUX_SIG_PIN A10
#define S0 21
#define S1 20
#define S2 19
#define S3 18
//----------------------------------------------------------------/
