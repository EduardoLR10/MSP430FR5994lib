#include <msp430.h> 
#include <stdint.h>

#define SDABIT_B2 BIT0
#define SCLBIT_B2 BIT1

#define SDA_B2_IN   P7 ## IN
#define SDA_B2_OUT  P7 ## OUT
#define SDA_B2_DIR  P7 ## DIR
#define SDA_B2_REN  P7 ## REN
#define SDA_B2_SEL0  P7 ## SEL0
#define SDA_B2_SEL1  P7 ## SEL1

#define SCL_B2_IN   P7 ## IN
#define SCL_B2_OUT  P7 ## OUT
#define SCL_B2_DIR  P7 ## DIR
#define SCL_B2_REN  P7 ## REN
#define SCL_B2_SEL0  P7 ## SEL0
#define SCL_B2_SEL1  P7 ## SEL1

#define SDABIT_B1 BIT0
#define SCLBIT_B1 BIT1

#define SDA_B1_IN   P5 ## IN
#define SDA_B1_OUT  P5 ## OUT
#define SDA_B1_DIR  P5 ## DIR
#define SDA_B1_REN  P5 ## REN
#define SDA_B1_SEL0  P5 ## SEL0
#define SDA_B1_SEL1  P5 ## SEL1

#define SCL_B1_IN   P5 ## IN
#define SCL_B1_OUT  P5 ## OUT
#define SCL_B1_DIR  P5 ## DIR
#define SCL_B1_REN  P5 ## REN
#define SCL_B1_SEL0  P5 ## SEL0
#define SCL_B1_SEL1  P5 ## SEL1

#define BR10K 10
#define BR50K 50
#define BR100K 100

#define Tmp_d 2000000

void delay(long x);

void config_I2C_B2(int isMaster, uint8_t ownAddr, int whichslave){      //SDA = BIT0 --> SCL = BIT1

    SDA_B2_SEL0 |=  SDABIT_B2;    // Use dedicated module
    SDA_B2_SEL1 &=  ~(SDABIT_B2);
    SDA_B2_REN |=  SDABIT_B2;    // Resistor enable
    SDA_B2_OUT |=  SDABIT_B2;    // Pull-up
    SCL_B2_SEL0 |=  SCLBIT_B2;    // Use dedicated module
    SCL_B2_SEL1 &=  ~(SCLBIT_B2);
    SCL_B2_REN |=  SCLBIT_B2;    // Resistor enable
    SCL_B2_OUT |=  SCLBIT_B2;    // Pull-up

    UCB2CTLW0 = UCSWRST;    // UCSI B0 em ressete
    UCB2CTLW0 |= UCSYNC |     //Síncrono
             UCMODE_3 |     //Modo I2C
             UCSSEL_2;
    if(isMaster){
    	UCB2CTLW0 |= UCMST;
    	UCB2I2COA0 = ownAddr;
    }else{
    	if(whichslave == 1){
    		UCB2I2COA1 = UCOAEN | ownAddr;
    	}else if(whichslave == 2){
    		UCB2I2COA2 = UCOAEN | ownAddr;
    	}else if(whichslave == 3){
    		UCB2I2COA3 = UCOAEN | ownAddr;
    	}
    }
    UCB2BRW = BR10K;        // 10 kbps
    UCB2CTLW0 &= ~(UCSWRST);    //SMCLK
}

void B2_start_B1(uint8_t addr){
	UCB2I2CSA = addr;
	UCB2CTLW0 |= UCTR |
    			 UCTXSTT;
}

void B2_write_on_B1(char dado){
    while ( (UCB2IFG & UCTXIFG0) == 0);      //espera TXIFG
    if ( (UCB2IFG & UCTXIFG0) == UCNACKIFG)  //NACK?
        while(1);
    UCB2TXBUF = dado;
    while ( (UCB2IFG & UCTXIFG0) == 0);      //espera TX
    UCB2CTLW0 |= UCTXSTP;    //gera STOP
    while ( (UCB2CTLW0 & UCTXSTP) == UCTXSTP);
    delay(500);
}

void config_I2C_B1(int isMaster, uint8_t ownAddr, int whichslave){      //SDA = BIT0 --> SCL = BIT1

    SDA_B1_SEL0 |=  SDABIT_B1;    // Use dedicated module
    SDA_B1_SEL1 &=  ~(SDABIT_B1);
    SDA_B1_REN |=  SDABIT_B1;    // Resistor enable
    SDA_B1_OUT |=  SDABIT_B1;    // Pull-up
    SCL_B1_SEL0 |=  SCLBIT_B1;    // Use dedicated module
    SCL_B1_SEL1 &=  ~(SCLBIT_B1);
    SCL_B1_REN |=  SCLBIT_B1;    // Resistor enable
    SCL_B1_OUT |=  SCLBIT_B1;    // Pull-up

    UCB1CTLW0 = UCSWRST;    // UCSI B0 em ressete
    UCB1CTLW0 |= UCSYNC |     //Síncrono
             UCMODE_3 |     //Modo I2C
             UCSSEL_2;
    if(isMaster){
    	UCB1CTLW0 |= UCMST;
    	UCB1I2COA0 = ownAddr;
    }else{
    	if(whichslave == 1){
    		UCB1I2COA1 = UCOAEN | ownAddr;
    	}else if(whichslave == 2){
    		UCB1I2COA2 = UCOAEN | ownAddr;
    	}else if(whichslave == 3){
    		UCB1I2COA3 = UCOAEN | ownAddr;
    	}
    }
    UCB1BRW = BR100K;        // 10 kbps
    UCB1CTLW0 &= ~(UCSWRST);    //SMCLK
}

// int PCF_B2_read(void){
//     int dado;
//     UCB2CTL1 &= ~UCTR;  //Mestre RX
//     UCB2CTL1 |= UCTXSTT;    //Gerar START
//     while ( (UCB2CTL1 & UCTXSTT) == UCTXSTT);
//     UCB2CTL1 |= UCTXSTP;    //Gera STOP + NACK
//     while ( (UCB2IFG & UCRXIFG) == 0 ); //Espera RX
//     dado = UCB2RXBUF;
//     return dado;
// }

// Gerar START e STOP para colocar PCF em estado conhecido
void PCF_B2_STT_STP(uint8_t addr){
    int x=0;
    UCB2I2CSA = addr;        //Endereço Escravo

    while (x<5){
        UCB2CTLW0 |= UCTR    |   //Mestre TX
                    UCTXSTT;    //Gerar START
        while ( (UCB2IFG & UCTXIFG0) == 0);  //Esperar TXIFG=1
        UCB2CTLW0 |= UCTXSTP;                //Gerar STOP
        delay(200);
        if ( (UCB2CTLW0 & UCTXSTP) == 0)   break;   //Esperar STOP
        x++;
    }
    while ( (UCB2CTLW0 & UCTXSTP) == UCTXSTP);   //I2C Travado (Desligar / Ligar)
}

void delay(long limite){
    volatile long cont=0;
    while (cont++ < limite) ;
    P1OUT ^= BIT0;
}