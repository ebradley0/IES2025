#include "PWM.h"

void setRGB(char red, char green, char blue) {
  TB3CCR1 = red << 0 2;
  TB3CCR2 = green << 2;
  TB3CCR3 = blue << 2;
}

void initPWM() {

  // Setup timer
  TB3CCR0 = 1025 - 1; // PWM Period (1024)

  // RGB INIT
  P6DIR |= BIT0 | BIT1 | BIT2; // P6.0 and P6.1 and 6.2 output
  P6SEL0 |= BIT0 | BIT1 |
            BIT2; // P6.0, 6.1, and 6.2 to use the timer, set to 00 by default
  P6SEL1 &=
      ~(BIT0 | BIT1 | BIT2); // This is a redundancy check to ensure P6Sel1 is 0
                             // for our set pins that are using tb3.

  TB3CCTL1 = OUTMOD_3; // CCR1 reset/set
  TB3CCR1 = 100;       // CCR1 PWM duty cycle Red
  TB3CCTL2 = OUTMOD_3; // CCR2 reset/set
  TB3CCR2 = 0;         // CCR2 PWM duty cycle Green
  TB3CCTL3 = OUTMOD_3;
  TB3CCR3 = 100; // Blue

  // Servo
  TB1CCR0 = 20000 - 1;
  P2DIR |= BIT1;
  P2SEL0 |= BIT1;
  P2SEL1 &= ~BIT1; // Redundancy check to make sure sel1 is 0.
  TB1CCTL2 = OUTMOD_7;
  TB1CCR2 =
      500; // Servo is initialized to be closed, so no signal 500 to 2500 us

  // Cleanup
  TB3CTL = TBSSEL__SMCLK | MC__UP | TBCLR; // SMCLK, up mode, clear TBR
  TB1CTL = TBSSEL__SMCLK | MC__UP | TBCLR; // SMCLK, up mode, clear TBR
}

void setServo(int position) { TB1CCR2 = position; }
