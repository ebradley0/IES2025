#include "intrinsics.h"
#include <msp430.h>
#include <RGBLED.h>
int main(void)
{
    //Setup

    initRGB();

    while(1)
    {
      setRGB(char 255, char 0, char 0);
      __delay_cycles(1000000);
    }
}
