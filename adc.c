#include <msp430.h>

void config_adc(){
    ADC12CTL0  &= ~ADC12ENC;        //Desabilitar para configurar

    ADC12CTL0 = ADC12SHT0_3 |       //ADC tempo amostragem ADCMEM[0-7]
                ADC12ON;            //Ligar ADC

    ADC12CTL1 = ADC12SHS_0       |  // Selecionar ADC12SC
                ADC12SHP         |  // S/H usar timer
                ADC12DIV_0       |  //Clock ADC Divisor = 1
                ADC12SSEL_0      |  //Clock ADC = SMCLK
                ADC12CONSEQ_3;      //Modo Sequência de canais repetido

    ADC12CTL3 = ADC12CSTARTADD_0;  //Resultado em ADC12MEM0

    ADC12CTL2 = ADC12RES_2;         // Resolução 12-bit

    ADC12MCTL0 = ADC12INCH_2 |       //(P6.0) A0 = Canal 0
                 ADC12VRSEL_0;      //VR+ = AVCC e VR- = AVSS

    ADC12MCTL1 = ADC12EOS    |      //Fim de sequência
                 ADC12INCH_3 |       //(P6.0) A0 = Canal 0
                 ADC12VRSEL_0;      //VR+ = AVCC e VR- = AVSS

    ADC12IER0 = ADC12IE1;

    P1SEL0 |= BIT2;
    P1SEL1 |= BIT2;
    
    P1SEL0 |= BIT3;
    P1SEL1 |= BIT3;
}
