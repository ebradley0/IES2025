#include "sensors.h"

volatile char ADCFinished = 0;
volatile unsigned int ADCResult = 0;
unsigned int FLAME_THRESHOLD =
    8; // Adjust based on flame signal ADC value; looking for volatage from
       // range of 0 ~ 0.8mV since that is the max at what this thermocouple
       // reads

/*
----------------------Thermistor Functions-----------------------
----------------------------------------------------------------------------------------------------------------
    First set of functions are used for Thermistor

    First Function "readThermistor" is taking a reading from thermistor; meant
to monitor if overheating occurs; PIN 1.6

    Second Funciton "isOverheat" compares reading taking from the first
function, to the setpoint measured by potentiometer; returns 1 if true;
-----------------------------------------------------------------------------------------------------------
*/

unsigned int readThermistor() { return readADC(6); }

unsigned int isOverheat() {

  unsigned int thermADC = readThermistor();
  unsigned int potADC = readPot();
  return (thermADC > potADC) ? 1 : 0;
}

/*
-----------------------ThermoCouple Functions--------------------------
----------------------------------------------------------------------------------------------------------------------
Next function is used to implement Thermocouple; PIN 1.3

Takes in reading through onboard amplifier and compares it to flame_threshold
value to determine Flame_Present

Needs Function to read Thermocouple and return value; readThermocouple();

Also needs a function to compare reading to flame_threshold and return a 1 or 0
to main.c based on result; isFlamePresent();
----------------------------------------------------------------------------------------------------------------------
*/

// Reading ADC value from thermocouple
unsigned int readThermocouple() {

  unsigned int result = readADC(3); // P1.3 = A3
  return result;
}

// Return 1 if flame is present, 0 if not; uses value returned from reading
// ThermocoupleADC
unsigned int isFlamePresent() {

  return (readThermocouple() > FLAME_THRESHOLD) ? 1 : 0;
}

/*
------------------------Potentiometer Functions------------------------------
------------------------------------------------------------------------------------------------------------

Next function is used to read setpoint value determined by potentiometer;
Potentiometer on PIN 1.5

------------------------------------------------------------------------------------------------------------
*/

unsigned int readPot() {

  return readADC(5); // P1.5 = A5
}

/*
---------------------SAC Amplifier Initialization-------------------------
-----------------------------------------------------------------------------------------------------------
To use the onboard amplifier, we need to initialize with referenced to its
assigned pin and thermocouple, since signal can be fairly low

In the following initSAC function for Amplifier usage, here are
the following MACROS and their intended purpose...

PMUXEN: Enable positive input mux (P1.3 = OA0+ selected)
NMUXEN: Enable internal negative input for PGA feedback
NSEL_1: Configure for PGA mode (non-inverting)
OAPM: Low-power mode (ideal for slow thermocouple signals)

this sets up the path: P1.3 -> opamp -> output on P1.1

MSEL_2: tells SAC to use op-amp as non-inverting
-----------------------------------------------------------------------------------------------------------
*/

void initSAC0() {

  // Configure pins for SAC0 OA function (P1.1, P1.3)
  P1SEL0 |= BIT1 | BIT3;
  P1SEL1 |= BIT1 | BIT3;

  // SAC0 OA (Op-Amp) for Non-inverting PGA mode
  SAC0OA = NMUXEN | PMUXEN | PSEL_0 |
           NSEL_1; // Positive input P1.3 OA0+, negative internally tied for PGA
  SAC0OA |= OAPM;  // Low-speed, low-power mode

  // Set PGA gain
  SAC0PGA =
      GAIN0 | GAIN2 | MSEL_2; // Non-inverting PGA, Gain = 17 (GAIN0 + GAIN2 =
                              // Gain of 17); this is by default but can be
                              // modified if signal is too small or saturating

  // Enable SAC0 and the op-amp
  SAC0OA |= SACEN | OAEN;
}

/*
---------------------ADC Initialization--------------------
-----------------------------------------------------------------------------------------------------------

To use the ADC peripheral, we need to first initialize it and configure what is
needed from it

Macros:

ADCSHT_2:	Sample-and-hold time = 16 ADC clocks ~	Needed for slower or
high-impedance inputs like thermistors or op-amp outputs ADCON:	    ADC ON
~  Powers up the ADC module ADCSHP:	    Sample-and-hold pulse mode ~ Enables
use of internal sampling timer (no external trigger or timer needed) ADCRES_2:
ADC resolution = 12-bit (0–4095)	 ~  Full resolution required for precise
flame/thermistor thresholding ADCRES:	    Bit mask to clear old resolution
~  Ensures the new setting is applied cleanly ADCIE0	    Enables interrupt
for end of conversion (ADCIFG flag) ~ Required if using an interrupt-based ADC
read, as readADC() function does
------------------------------------------------------------------------------------------------------------
*/

void initADC() {

  // Configure ADC Pins
  P1SEL0 |= BIT3 | BIT5 | BIT6;
  P1SEL1 |= BIT3 | BIT5 | BIT6;

  // Configure ADC core
  ADCCTL0 |= ADCSHT_2 | ADCON; // ADC ON, S&H = 16 ADC clocks
  ADCCTL1 |= ADCSHP;           // Use sampling timer
  ADCCTL2 &= ~ADCRES;
  ADCCTL2 |= ADCRES_2; // 12-bit resolution
  ADCIE |= ADCIE0;     // Enable ADC interrupt
}

unsigned int readADC(char ADCChannel) {

  ADCCTL0 &= ~ADCENC;      // Disable ADC before changing channel
  ADCMCTL0 &= ~ADCINCH_15; // Clear previous channel selection

  switch (ADCChannel) {
  case 3:
    ADCMCTL0 |= ADCINCH_3;
    break;
  case 5:
    ADCMCTL0 |= ADCINCH_5;
    break;
  case 6:
    ADCMCTL0 |= ADCINCH_6;
    break;
  default:
    ADCMCTL0 |= ADCINCH_3;
    break; // Default to thermocouple
  }

  ADCFinished = 0;
  ADCCTL0 |= ADCENC | ADCSC; // Start conversion

  // Timeout function: Wait for ADCFinished to be set by ISR ~ lets us know if
  // we endlessly stuck
  int timeout = 500000; // ~timeout window (tunable)

  while (ADCFinished != 1 && --timeout)
    ; // Waiting for result; if loop is constantly freezing or stuck, the
      // interrupt didnt fire

  if (timeout == 0) {
    return 0xFFFF; // Timeout: ADC never completed
  }

  return ADCResult;
}

/*
-----------------Interrupt for ADC---------------------
-----------------------------------------------------------------------------------------------------------
 ADC_ISR() - ADC Interrupt Service Routine

 Purpose: This ISR is triggered when the ADC completes a conversion or when
other ADC-related events (errors or threshold crossings) occur. In this project,
the ISR is used to:
   - Capture the 12-bit conversion result into ADCResult
   - Complete signal from ADCFinished flag

  Only one case (ADCIV_ADCIFG) is used, but all cases are available


   Interrupt Vector Cases (ADCIV):

    ADCIV_NONE: No interrupt pending (default state).
    ADCIV_ADCOVIFG: ADC overflow interrupt. Triggered if a new conversion starts
before the previous result is read. Not expected in single-conversion mode.
    ADCIV_ADCTOVIFG: ADC conversion time overflow. Triggered if conversion takes
too long (rare). ADCIV_ADCHIIFG: ADCIV_ADCLOIFG: ADCIV_ADCINIFG:ADC window
comparator interrupts.Triggered if conversion result is above, below, or inside
configured threshold.Not used unless window comparator is enabled.
    ADCIV_ADCIFG:End-of-conversion interrupt.Signals that ADCMEM0 contains a
valid result.

 ----------------------------------------------------------------------------------------------------------
 */

#pragma vector = ADC_VECTOR
__interrupt void ADC_ISR(void) // interrupt
{
  switch (__even_in_range(ADCIV, ADCIV_ADCIFG)) {
  case ADCIV_NONE:
    break;
  case ADCIV_ADCOVIFG:
    break;
  case ADCIV_ADCTOVIFG:
    break;
  case ADCIV_ADCHIIFG:
    break;
  case ADCIV_ADCLOIFG:
    break;
  case ADCIV_ADCINIFG:
    break;
  case ADCIV_ADCIFG:
    ADCResult = ADCMEM0; // how we are going to see reading
    ADCFinished =
        1; // changes to 1, thus ending conversion sequence for said readin
    break;
  default:
    break;
  }
}

/*
-------------------Side Notes---------------

we do not need a timer!
    --using polling loop in main.c
    --manually taking samples


*/
