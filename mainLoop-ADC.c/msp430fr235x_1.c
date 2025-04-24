#include <msp430.h>
#include "sensors.h"


int main(void) {
    WDTCTL = WDTPW | WDTHOLD;        // Stop watchdog timer


    initADC();                       // Initialize ADC system

    while (1) {
        int overTemp = readThermistor();      // Returns 1 if over-temp
        int flamePresent = readThermocouple(); // Returns 1 if flame present

        if (flamePresent && !overTemp) {
            //print heating cycle
        } else {
            //Print shutodnw
        }

        __delay_cycles(50000); // Short delay (~50ms)
    }
}
