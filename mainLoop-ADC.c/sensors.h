#include <msp430.h>

// Function declarations
void initADC();
unsigned int readADC(char ADCChannel);
unsigned int readThermistor();      // Returns 1 if over-temp
unsigned int readThermocouple();    // Returns 1 if flame present
unsigned int readPot();             // Returns ADC setpoint

// Externs for ADC state tracking
extern volatile char ADCFinished;
extern volatile unsigned int ADCResult;