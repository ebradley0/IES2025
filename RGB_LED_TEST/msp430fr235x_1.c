#include "intrinsics.h"
#include <PWM.h>
#include <msp430.h>

int main(void) {
  // Setup

  initPWM();

  while (1) {
    setRGB(255, 0, 0);
    setServo(1);
    __delay_cycles(1000000);
    setRGB(0, 255, 0);
    setServo(0);
    __delay_cycles(1000000);
  }
}
