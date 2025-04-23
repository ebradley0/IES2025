#include "msp430fr2355.h"
#include <GPIO.h>

void init_GPIO() {
  // Igniter - 5.4 output
  //  Pilot - 5.0 output
  // CallForHeat - 3.1
  // Output configuration
  P5OUT &= ~(BIT4 & BIT0);
  P5DIR |= BIT4; // Set pin 5.4 as output
  P5DIR |= BIT0; // Set pin 5.0 as output

  // Input Configuration, Interrupt enabled.
  P3OUT &= ~BIT1; // Clear port
  P3DIR &= ~BiT1;
}

void setPilot(bool open) {
  if (open) {
    P5OUT |= BIT0; // Open valve
  } else {
    P5OUT &= ~BIT0; // Shut valve
  }
}

void Igniter(bool on) {
  if (on) {
    P5OUT |= BIT4; // On
  } else {
    P5OUT &= ~BIT4; // Off
  }
}

int checkCallForHeat() {
  if (P3IN & BIT1) {
    return 1; // If the pin is high, return a 1, signifying the need to start
              // the heating process.
  } else {
    return 0; // Default return.
  }
}