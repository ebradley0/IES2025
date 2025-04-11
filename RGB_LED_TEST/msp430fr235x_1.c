#include "intrinsics.h"
#include <msp430.h>
#include <RGBLED.h>
int main(void)
{
    //Setup

    initRGB();

    while(1)
    {
      setRGB(255, 0, 0);
      __delay_cycles(1000000);
      setRGB(0, 255,  0);
      __delay_cycles(1000000);
    }
}
