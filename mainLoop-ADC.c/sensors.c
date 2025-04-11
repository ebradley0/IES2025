#include <sensors.h>

//for thermocouple, thermistor and potentiometer (3 sensors that takes a physical element into digital; ADC modules)

//learning how to take a signal reading; refer to adc reference code that is provided adc_12_01

unsigned int readThermistor() { // pin 1.6 (A6); refer to datatsheet on how to connect/configure adc pins

    unsigned int result;


    result = readADC(6); //common function that each device will likely share
    
  
}

unsigned int readThermocouple(){ // pin 1.3 (A3)
    unsigned int result;
    
    result = readADC(3);

}



   void initADC(){ //only have to write it one
        //Configure ADC Pins
        P1SEL0 |= BIT3 | BIT6 | BIT5;
        P1SEL1 |= BIT3 | BIT6 | BIT5;


        //configure adc core; going between user guide and datasheet; can look at macros that are useful for this configuration
        ADCCTL0 |= ADCSHT_2 | ADCON;                             // ADCON, S&H=16 ADC clks
        ADCCTL1 |= ADCSHP;                                       // ADCCLK = MODOSC; sampling timer
        ADCCTL2 &= ~ADCRES;                                      // clear ADCRES in ADCCTL
        ADCCTL2 |= ADCRES_2;                                     // 12-bit conversion results
                                    
       



    }

unsigned int readPOT(){ // pin 1.5 (A5)

    unsigned int result;
    
    result = readADC(5);

}

unsigned int readADC(char Channel){ //need to return a 12-bit number to read temperature; need interrupts to be enabled

    swicth(Channel){ //case statments to target each based on input


        case 3: ADCMCTL0 |= ADCINCH_3;
            break;
        case 5: ADCMCTL0 |= ADCINCH_5;
            break;
        case 6: ADCMCTL0 |= ADCINCH_6;
            break;
        default: ADCMCTL0 |= ADCINCH_3;
            break;

    }



    ADCCTL0 |= ADCENC | ADCSC; 
    while(ADCFinished != 1);  //as long as function does not = to 1, it'll constantly check it
    return ADCResult;

    // ADC interrupt service routine
}

#pragma vector=ADC_VECTOR
__interrupt void ADC_ISR(void)
{
    switch(__even_in_range(ADCIV,ADCIV_ADCIFG))
    {
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
            ADCResult = ADCMEM0;
            ADCFinished = 1;
            break;
        default:
            break;
    }
}
