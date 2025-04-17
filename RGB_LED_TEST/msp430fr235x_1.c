#include "intrinsics.h"
#include <PWM.h>
#include <msp430.h>

int main(void) {
  // Setup
  WDTCTL = WDTPW | WDTHOLD;

  initPWM();

  PM5CTL0 &= ~LOCKLPM5;
  while (1) {
    setRGB(0, 255, 0);
    setServo(500);
    __delay_cycles(1000000);
    setRGB(255, 0, 0);
    setServo(2500);
    __delay_cycles(1000000);
  }
}
