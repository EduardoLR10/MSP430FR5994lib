#include "i2c.h"
#include <msp430.h> 

// Function to configure UCB2
void config_I2C_B2(int isMaster, uint8_t ownAddr, int whichslave){

    SetPort(P7, SEL0, 0);                                // Select SDA
    ClearPort(P7, SEL1, 0);                              // Select SDA
    SetPort(P7, REN, 0);                                 // Resistor
    SetPort(P7, OUT, 0);                                 // Pull-up

    SetPort(P7, SEL0, 1);                                // Select SCL
    ClearPort(P7, SEL1, 1);                              // Select SCL   
    SetPort(P7, REN, 1);                                 // Resistor
    SetPort(P7, OUT, 1);                                 // Pull-up

    UCB2CTLW0 = UCSWRST;                                 // B2 set reset
    SetFlag(UCB2CTLW0, UCSYNC);                          // Sync
    SetFlag(UCB2CTLW0, UCMODE_3);                        // Mode I2C
    SetFlag(UCB2CTLW0, UCSSEL_2);                        // SMCLK
    
    if(isMaster){                                        // Master case
        SetFlag(UCB2CTLW0, UCMST);                       // Master Mode
    	UCB2I2COA0 = ownAddr;                            // Select master's own address
    }else{                                               // Slave case
    	if(whichslave == 1){                             // I2COA1
    		UCB2I2COA1 = UCOAEN | ownAddr;               // Slave enable and put slave's address
    	}else if(whichslave == 2){                       // I2COA2
    		UCB2I2COA2 = UCOAEN | ownAddr;               // Slave enable and put slave's address
    	}else if(whichslave == 3){                       // I2COA3
    		UCB2I2COA3 = UCOAEN | ownAddr;               // Slave enable and put slave's address
    	}
    }
    UCB2BRW = BR10K;                                     // Select baud-rate
    ClearFlag(UCB2CTLW0, UCSWRST);                       // B2 clear reset
}

// Funciton to start transmission between B1(Slave) and B2(Master)
void B2_start_B1(uint8_t addr){
	UCB2I2CSA = addr;                                    // Select slave's address
    SetFlag(UCB2CTLW0, UCTR);                            // Set as transmitter
    SetFlag(UCB2CTLW0, UCTXSTT);                         // Init Start
}

// Function to write from B2(Master) to B1(Slave)
void B2_write_on_B1(char data){
    while(CompareFlagEQ(UCB2IFG, UCTXIFG0, 0));          // Wait TXIFG0 (with master at I2COA0)
    if(CompareFlagEQ(UCB2IFG, UCTXIFG0, UCNACKIFG))      // NACK?
        while(1);                                        // If NACK do nothing
    UCB2TXBUF = data;                                    // Data into transmission buffer
}

void B2_stop_B1(){
    while(CompareFlagEQ(UCB2IFG, UCTXIFG0, 0));          // Wait TXIFG0 (with master at I2COA0)
    SetFlag(UCB2CTLW0, UCTXSTP);                         // Call Stop
    while(CompareFlagEQ(UCB2CTLW0, UCTXSTP, UCTXSTP));   // Wait Stop
}

// Function to configure UCB1
void config_I2C_B1(int isMaster, uint8_t ownAddr, int whichslave){

    SetPort(P5, SEL0, 0);                                // Select SDA
    ClearPort(P5, SEL1, 0);                              // Select SDA
    SetPort(P5, REN, 0);                                 // Resistor
    SetPort(P5, OUT, 0);                                 // Pull-up

    SetPort(P5, SEL0, 1);                                // Select SCL
    ClearPort(P5, SEL1, 1);                              // Select SCL
    SetPort(P5, REN, 1);                                 // Resistor
    SetPort(P5, OUT, 1);                                 // Pull-up

    UCB1CTLW0 = UCSWRST;                                 // B1 set reset
    SetFlag(UCB1CTLW0, UCSYNC);                          // Sync
    SetFlag(UCB1CTLW0, UCMODE_3);                        // Mode I2C
    SetFlag(UCB1CTLW0, UCSSEL_2);                        // SMCLK
    
    if(isMaster){                                        // Master case
        SetFlag(UCB1CTLW0, UCMST);                       // Master Mode
        UCB1I2COA0 = ownAddr;                            // Select master's own address
    }else{                                               // Slave case
        if(whichslave == 1){                             // I2COA1
            UCB1I2COA1 = UCOAEN | ownAddr;               // Slave enable and put slave's address
        }else if(whichslave == 2){                       // I2COA2
            UCB1I2COA2 = UCOAEN | ownAddr;               // Slave enable and put slave's address
        }else if(whichslave == 3){                       // I2COA3
            UCB1I2COA3 = UCOAEN | ownAddr;               // Slave enable and put slave's address
        }
    }
    UCB1BRW = BR10K;                                     // Select baud-rate
    ClearFlag(UCB1CTLW0, UCSWRST);                       // B1 clear reset
}

// Function to read from UCB2(Master)
int B2_read(void){
    int data;
    ClearFlag(UCB2CTLW0, UCTR);                          // Set as receiver 
    SetFlag(UCB2CTLW0, UCTXSTT);                         // Init Start
    while(CompareFlagEQ(UCB2CTLW0, UCTXSTT, UCTXSTT));   // Wait Start
    SetFlag(UCB2CTLW0, UCTXSTP);                         // Call Stop
    while (CompareFlagEQ(UCB2IFG, UCRXIFG0, 0));         // Wait RXIFG0 (with master at I2COA0) 
    data = UCB2RXBUF;                                    // Save data
    return data;                                         // Return data
}

// Put Slave in a known state
void B2_STT_STP(uint8_t addr){
    int x = 0;
    UCB2I2CSA = addr;                                    // Slave Address

    while (x < 5){
        SetFlag(UCB2CTLW0, UCTR);                        // Set as transmitter
        SetFlag(UCB2CTLW0, UCTXSTT);                     // Init Start
        while (CompareFlagEQ(UCB2IFG, UCTXIFG0, 0));     // Wait TXIFG0 (with master at I2COA0)
        SetFlag(UCB2CTLW0, UCTXSTP);                     // Call Stop
        delay(200);
        if (CompareFlagEQ(UCB2CTLW0, UCTXSTP, 0)){
            break;                                       // Wait Stop
        }
        x++;
    }
    while (CompareFlagEQ(UCB2CTLW0, UCTXSTP, UCTXSTP));  // I2C Locked Up
}

// Function to put delay between things
void delay(long limite){
    volatile long cont=0;
    while (cont++ < limite);
}
