#include "SYSTEM.h"
#include "msp430fr2355.h"

int main(void)
{
    //Setup Code (Runs when processor resets)

    WDTCTL = WDTPW | WDTHOLD;

    PM5CTL0 &= ~LOCKLPM5;

    while(1)
    {
        //setIgnitor
        //turnonPilotValve
        //Delay 2 seconds
        //CheckFlame
        //If flame, release gas


        //while(CallforHeat() || checkflame() || (readBoiler < threshhold) ); /Stalls while any of these states are active, this leaves everything open

        //shutDownHeatSys() -- Turns off main valve, pilot valve, and Ignitor

        // Loop

        //Idle, any functionality will be done via interrupts.
        __bis_SR_Register(CPUOFF);
    }
}
