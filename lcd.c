#include <msp430.h> 
#include <stdint.h>
#include <string.h>
#include "i2c.c"

int cursorPosition = 0;

// Initialize LCD in 4 bits mode
void LCD_B2_inic(uint8_t addr){

    B2_STT_STP(addr);                                    // Put LCD in a known state

    UCB2I2CSA = addr;                                    // Select slave's address
    SetFlag(UCB2CTLW0, UCTR);                            // Set as transmitter
    SetFlag(UCB2CTLW0, UCTXSTT);                         // Init Start
    while(CompareFlagEQ(UCB2IFG, UCTXIFG0, 0));          // Wait TXIFG0 (with master at I2COA0)
    UCB2TXBUF = 0;                                       // Slave's exit = 0;
    while(CompareFlagEQ(UCB2CTLW0, UCTXSTT, UCTXSTT));   // Wait Start
    if(CompareFlagEQ(UCB2IFG, UCTXIFG0, UCNACKIFG))      // NACK?
        while(1);                                        // If NACK do nothing

    // Start Initialization
    LCD_B2_aux(0);                                       // RS = RW = 0, BL=1
    delay(20000);
    LCD_B2_aux(3);                                       //3
    delay(10000);
    LCD_B2_aux(3);                                       //3
    delay(10000);
    LCD_B2_aux(3);                                       //3
    delay(10000);
    LCD_B2_aux(2);                                       //2

    // Enter 4 bits mode
    LCD_B2_aux(2);     LCD_B2_aux(8);                    //0x28
    LCD_B2_aux(0);     LCD_B2_aux(8);                    //0x08
    LCD_B2_aux(0);     LCD_B2_aux(1);                    //0x01
    LCD_B2_aux(0);     LCD_B2_aux(6);                    //0x06
    LCD_B2_aux(0);     LCD_B2_aux(0xF);                  //0x0F

    while(CompareFlagEQ(UCB2IFG, UCTXIFG0, 0));          // Wait TXIFG0 (with master at I2COA0)
    SetFlag(UCB2CTLW0, UCTXSTP);                         // Call Stop
    while(CompareFlagEQ(UCB2CTLW0, UCTXSTP, UCTXSTP));   // Wait Stop
    delay(500);
}

// Auxiliar for LCD's initialization (RS = RW = 0)
// *** Only for initialization ***
void LCD_B2_aux(char data){
    while(CompareFlagEQ(UCB2IFG, UCTXIFG0, 0));          // Wait TXIFG0 (with master at I2COA0)
    UCB2TXBUF = ((data<<4)&0XF0) | BIT_BL;               //PCF7:4 = dado;
    delay(50);
    while(CompareFlagEQ(UCB2IFG, UCTXIFG0, 0));          // Wait TXIFG0 (with master at I2COA0)
    UCB2TXBUF = ((data<<4)&0XF0) | BIT_BL | BIT_E;       //E=1
    delay(50);
    while(CompareFlagEQ(UCB2IFG, UCTXIFG0, 0));          // Wait TXIFG0 (with master at I2COA0)
    UCB2TXBUF = ((data<<4)&0XF0) | BIT_BL;               //E=0;
}

// Function to write in the LCD with UCB2 (must config B2 first)
void B2_write_byte_LCD(char data){
	UCB2I2CSA = 0x27;
    SetFlag(UCB2CTLW0, UCTR);                            // Set as transmitter
    SetFlag(UCB2CTLW0, UCTXSTT);                         // Init Start
    while(CompareFlagEQ(UCB2IFG, UCTXIFG0, 0));          // Wait TXIFG0 (with master at I2COA0)
    if(CompareFlagEQ(UCB2IFG, UCTXIFG0, UCNACKIFG))      // NACK?
        while(1);                                        // If NACK do nothing
    delay(500);
    UCB2TXBUF = data;                                    // Data into transmission buffer
    delay(500);
    while(CompareFlagEQ(UCB2IFG, UCTXIFG0, 0));          // Wait TXIFG0 (with master at I2COA0)
    delay(500);
    SetFlag(UCB2CTLW0, UCTXSTP);                         // Call Stop
    while(CompareFlagEQ(UCB2CTLW0, UCTXSTP, UCTXSTP));   // Wait Stop
    delay(500);
}

void B2_changeCursorDisplay_LCD(uint8_t howmany, uint8_t isCursor, uint8_t direction){
    while(howmany){
        uint8_t Dbyte = (0x01 << 4) | ((((isCursor << 3) & 0x08) | ((direction << 2) & 0x04)) & 0x0C);
        B2_cursorDisplayShift_Dnibble_LCD(Dbyte);
        B2_cursorDisplayShift_Dnibble_LCD(Dbyte << 4);
        howmany--;
    }
}

void B2_cursorDisplayShift_Dnibble_LCD(uint8_t Dnibble){
    B2_write_byte_LCD(((Dnibble & 0xf0) | 0x08));       
    B2_write_byte_LCD(((Dnibble & 0xf0) | 0x0C));        // Send first nibble signal
    B2_write_byte_LCD(((Dnibble & 0xf0) | 0x08));
}

void B2_write_letter_LCD(char data){
    uint8_t ascii_letter = data;                         // Pick ASCII value
    B2_write_byte_LCD(((ascii_letter & 0xf0) | 0x09));       
    B2_write_byte_LCD(((ascii_letter & 0xf0) | 0x0D));        // Send first nibble signal
    B2_write_byte_LCD(((ascii_letter & 0xf0) | 0x09));
    B2_write_byte_LCD((((ascii_letter & 0x0f) << 4)| 0x09));
    B2_write_byte_LCD((((ascii_letter & 0x0f) << 4)| 0x0D));  // Send second nibble signal
    B2_write_byte_LCD((((ascii_letter & 0x0f) << 4)| 0x09));       
}

void B2_write_string_LCD(char* string, int line){
    int i;
    switch(line){
        case 0:
            B2_write_valid_string_LCD(string);
        break;
        case 1:
            for(i = 0; i < 40; i++){
                B2_write_letter_LCD(' ');
            }
            B2_write_valid_string_LCD(string);
        break;
        default:
            B2_write_error_LCD();
    }
}

void B2_write_error_LCD(){
    B2_write_letter_LCD('E');
    B2_write_letter_LCD('R');
    B2_write_letter_LCD('R');
    B2_write_letter_LCD('O');
    B2_write_letter_LCD('R');
    B2_write_letter_LCD('!');
}

void B2_write_valid_string_LCD(char* string){
    int counter = 0;
    int howmany_letters = strlen(string);
    if(howmany_letters >= 17){
        B2_write_error_LCD();
        return;
    }else{
        while(counter != howmany_letters){
            B2_write_letter_LCD(string[counter]);
            counter++;
        }
    }
}
