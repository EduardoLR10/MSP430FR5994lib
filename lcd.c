#include <msp430.h> 
#include <stdint.h>
#include "i2c.c"

#define BIT_BL BIT3
#define BIT_E BIT2
#define BIT_RW BIT1
#define BIT_RS BIT0

// Incializar LCD modo 4 bits
void LCD_B2_inic(uint8_t addr){

    PCF_STT_STP(addr);      //Colocar PCF em estado conhecido

    // Preparar I2C para operar
    UCB2I2CSA = addr;    //Endereço Escravo
    UCB2CTLW0 |= UCTR    |   //Mestre TX
                UCTXSTT;    //Gerar START
    while ( (UCB2IFG & UCTXIFG0) == 0);          //Esperar TXIFG=1
    UCB2TXBUF = 0;                              //Saída PCF = 0;
    while ( (UCB2CTLW0 & UCTXSTT) == UCTXSTT);   //Esperar STT=0
    if ( (UCB2IFG & UCNACKIFG) == UCNACKIFG)    //NACK?
                while(1);

    // Começar inicialização
    LCD_B2_aux(0);     //RS=RW=0, BL=1
    delay(20000);
    LCD_B2_aux(3);     //3
    delay(10000);
    LCD_B2_aux(3);     //3
    delay(10000);
    LCD_B2_aux(3);     //3
    delay(10000);
    LCD_B2_aux(2);     //2

    // Entrou em modo 4 bits
    LCD_B2_aux(2);     LCD_B2_aux(8);     //0x28
    LCD_B2_aux(0);     LCD_B2_aux(8);     //0x08
    LCD_B2_aux(0);     LCD_B2_aux(1);     //0x01
    LCD_B2_aux(0);     LCD_B2_aux(6);     //0x06
    LCD_B2_aux(0);     LCD_B2_aux(0xF);   //0x0F

    while ( (UCB2IFG & UCTXIFG0) == 0)   ;          //Esperar TXIFG=1
    UCB2CTLW0 |= UCTXSTP;                           //Gerar STOP
    while ( (UCB2CTLW0 & UCTXSTP) == UCTXSTP)   ;   //Esperar STOP
    delay(50);
}

// Auxiliar inicialização do LCD (RS=RW=0)
// *** Só serve para a inicialização ***
void LCD_B2_aux(char dado){
    while ( (UCB2IFG & UCTXIFG0) == 0);              //Esperar TXIFG=1
    UCB2TXBUF = ((dado<<4)&0XF0) | BIT_BL;          //PCF7:4 = dado;
    delay(50);
    while ( (UCB2IFG & UCTXIFG0) == 0);              //Esperar TXIFG=1
    UCB2TXBUF = ((dado<<4)&0XF0) | BIT_BL | BIT_E;  //E=1
    delay(50);
    while ( (UCB2IFG & UCTXIFG0) == 0);              //Esperar TXIFG=1
    UCB2TXBUF = ((dado<<4)&0XF0) | BIT_BL;          //E=0;
}

void B2_write_LCD(char dado){
	UCB2I2CSA = 0x27;
	UCB2CTLW0 |= UCTR |
    			 UCTXSTT;
    while ( (UCB2IFG & UCTXIFG0) == 0);      //espera TXIFG
    if ( (UCB2IFG & UCTXIFG0) == UCNACKIFG)  //NACK?
        while(1);
    delay(500);
    UCB2TXBUF = dado;
    delay(500);
    while ( (UCB2IFG & UCTXIFG0) == 0);      //espera TX
    delay(500);
    UCB2CTLW0 |= UCTXSTP;    //gera STOP
    while ( (UCB2CTLW0 & UCTXSTP) == UCTXSTP);
    delay(500);
}

// void B2_write_LCD(char dado){
// 	UCB2I2CSA = 0x27;
// 	UCB2CTLW0 |= UCTR |
//     			 UCTXSTT;
//     while ( (UCB2IFG & UCTXIFG0) == 0);      //espera TXIFG
//     UCB2TXBUF = dado;
//     while ( (UCB2CTLW0 & UCTXSTT) == UCTXSTT);
//     if ( (UCB2IFG & UCTXIFG0) == UCNACKIFG)  //NACK?
//         while(1);
//     while ( (UCB2IFG & UCTXIFG0) == 0);      //espera TX
//     UCB2CTLW0 |= UCTXSTP;    //gera STOP
//     while ( (UCB2CTLW0 & UCTXSTP) == UCTXSTP);
//     delay(500);
// }
