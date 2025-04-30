#include <msp430.h>

// Function declarations
void initSAC0();
void initADC();

// ADC read funciton
unsigned int readADC(char ADCChannel);

// Thermistor funcitons
unsigned int readThermistor(); // Returns value of ADC
unsigned int isOverheat();     // Returns a 1 if condition is true

// Thermocouple Functions
unsigned int readThermocouple(); // Returns value of ADC
unsigned int isFlamePresent();   // Returns a 1 if condition is true

// Potentiometer Funcitons
unsigned int readPot(); // Returns ADC setpoint

// Externs for ADC state tracking
extern volatile char ADCFinished;
extern volatile unsigned int ADCResult;
