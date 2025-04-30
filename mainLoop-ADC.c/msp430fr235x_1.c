#include <msp430.h>
#include <sensors.h>

#define PILOT_VALVE_PIN BIT0

int main(void) {
  WDTCTL = WDTPW | WDTHOLD; // Stop watchdog timer

  initSAC0(); // Initialize SAC Internal Amplifier for thermocouple
  initADC();  // Initialize ADC system

  __enable_interrupt(); // Enable global interrupts (required for ADC ISR)

  // configuring pilot valve
  P1DIR |= PILOT_VALVE_PIN;  // Set as output
  P1OUT &= ~PILOT_VALVE_PIN; // Start with valve OFF

  // Igniter
  P5OUT &= ~BIT4;
  P5DIR |= BIT4;

  PM5CTL0 &= ~LOCKLPM5;

  while (1) {

    unsigned int pot = readPot();
    unsigned int tc = readThermocouple();
    unsigned int tm = readThermistor();

    if (isOverheat()) {
      P5OUT |= BIT4;
    } else {
      P5OUT &= ~BIT4;
    }

    __delay_cycles(50000); // Short delay (~50ms)
  }
}