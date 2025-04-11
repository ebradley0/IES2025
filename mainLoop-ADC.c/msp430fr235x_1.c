//make/include files into this file
#include <msp430.h>
#include <rgbLED.h>
#include <system.h>


int main(void)
{

    //setup code (runs when processor resets)

    WDTCTL = WDTPW | WDTHOLD;

    rgbInit();




while(1){ //while we idle

    while(!CallForHeat()); //pulling example for call for heat

    Setignitor();
    turnOnPilotValve()
    Wait2seconds()
    checkflame()


    /*
    while loop below helps us keep three main functions to turn everything off as a single variable based on multiplt condtioions
    */

   // while((CallforHeat()) | (checkflame()) | (readBoilerTemp()<Threshold)); //while these conditions are true keep doing this "thing"

// another way of doing the while loop

while(1){

    cfh = CallForHeat;
    cf = checkflame;


    if(!CallforHeat()) {
        break;
    }

    if (checkflame()){
        Flame_extinguished = 1;
        break;

    }

    


}

    turnoffMainValve();
    turnOffPilotValve();
    turnOffIgniter();






    _bis_SR_register(CPUOFF); //interrupt example, stops cpu until interrupt is active






}





}


