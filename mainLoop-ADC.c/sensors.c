#include "sensors.h"

volatile char ADCFinished = 0;
volatile unsigned int ADCResult = 0;
unsigned int FLAME_THRESHOLD 1000;  // Adjust based on flame signal ADC value

unsigned int readThermistor() {
    unsigned int thermADC = readADC(6); // P1.6 = A6
    unsigned int potADC = readADC(5);   // P1.5 = A5
    return thermADC;
    return (thermADC > potADC) ? 1 : 0; // Over-temp = 1
}

unsigned int readThermocouple() {
    unsigned int result = readADC(3); // P1.3 = A3
    return result;
    return (result > FLAME_THRESHOLD) ? 1 : 0; // Flame present = 1
}

unsigned int readPot() {
    return readADC(5); // P1.5 = A5
}

void initADC() {
    // Configure ADC Pins
    P1SEL0 |= BIT3 | BIT5 | BIT6;
    P1SEL1 |= BIT3 | BIT5 | BIT6;

    // Configure ADC core
    ADCCTL0 |= ADCSHT_2 | ADCON;     // ADC ON, S&H = 16 ADC clocks
    ADCCTL1 |= ADCSHP;               // Use sampling timer
    ADCCTL2 &= ~ADCRES;
    ADCCTL2 |= ADCRES_2;             // 12-bit resolution
    ADCIE |= ADCIE0;                 // Enable ADC interrupt
}

unsigned int readADC(char ADCChannel) {
    ADCCTL0 &= ~ADCENC;              // Disable ADC before changing channel
    ADCMCTL0 &= ~ADCINCH_15;         // Clear previous channel selection

    switch(ADCChannel) {
        case 3: ADCMCTL0 |= ADCINCH_3; break;
        case 5: ADCMCTL0 |= ADCINCH_5; break;
        case 6: ADCMCTL0 |= ADCINCH_6; break;
        default: ADCMCTL0 |= ADCINCH_3; break; // Default to thermocouple
    }

    ADCFinished = 0;
    ADCCTL0 |= ADCENC | ADCSC;       // Start conversion
    while (!ADCFinished);            // Wait for result
    return ADCResult;
}

 // ADC interrupt service routin

#pragma vector=ADC_VECTOR
__interrupt void ADC_ISR(void) //interrupt 
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
