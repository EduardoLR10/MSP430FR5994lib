#include "lcd.h"
#include <msp430.h> 

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
    delay(50);
    //B2_SetFunction_LCD(1, 0);
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
	B2_start_Slave(0x27);
    B2_write_on_Slave(data);
    B2_stop_Slave();
    delay(50);
}

void B2_requestRead_LCD(uint8_t addr){
    B2_sendPulse_LCD((addr & 0x7F), 0, 1);
}

// Function to turn ON backlight with no content
void B2_turnOnBL_LCD(){
    B2_write_byte_LCD(0x08);
}

// Function to turn OFF backlight with no content
void B2_turnOffBL_LCD(){
    B2_write_byte_LCD(0x00);
}

void B2_sendPulse_LCD(uint8_t byte, int RS, int RW){
    B2_write_byte_LCD((byte & 0xf0) | (RS & 0x01)|((RW & 0x01) << 1)| 0x08);
    B2_write_byte_LCD((byte & 0xf0) | (RS & 0x01)|((RW & 0x01) << 1)| 0x0C);
    B2_write_byte_LCD((byte & 0xf0) | (RS & 0x01)|((RW & 0x01) << 1)| 0x08);

    B2_write_byte_LCD(((byte << 4) & 0xf0) | (RS & 0x01)|((RW & 0x01) << 1)| 0x08);
    B2_write_byte_LCD(((byte << 4) & 0xf0) | (RS & 0x01)|((RW & 0x01) << 1)| 0x0C);
    B2_write_byte_LCD(((byte << 4) & 0xf0) | (RS & 0x01)|((RW & 0x01) << 1)| 0x08);
}


// Function to set Cursor to a custom position
void B2_setCursorPosition_LCD(uint8_t position){
    B2_sendPulse_LCD((position | 0x80), 0 , 0);
}

// Function to display float value
void B2_toFloatValue_LCD(float value){
    int aux = (value*1000);
    char thousand, hundred, ten, unity;
    thousand = (0x30+((aux/1000)%10));
    hundred = (0x30+((aux/100)%10));
    ten = (0x30+((aux/10)%10));
    unity = (0x30+(aux%10));
    B2_write_letter_LCD(thousand);
    B2_write_letter_LCD('.');
    B2_write_letter_LCD(hundred);
    B2_write_letter_LCD(ten);
    B2_write_letter_LCD(unity);
    B2_write_letter_LCD('V');
}

// Function to display hexadecimal value (0 < x < 255)
void B2_toHex8Value_LCD(int value){
    if(value < 0 || value > 255){
        B2_write_error_LCD();
        return;
    }
    char hex[2] = {'0','0'};
    int temp;
    int i = 0;
    if(value != 0){
        while(value != 0) {
            temp = value % 16;
            if( temp < 10){
                hex[i++] = temp + 48;
            }else{
                hex[i++] = temp + 55;
            }
            value /= 16;
        }
    }
    B2_write_letter_LCD('0');
    B2_write_letter_LCD('X');
    B2_write_letter_LCD(hex[1]);
    B2_write_letter_LCD(hex[0]);
}

// Function to display hexadecimal value ( 0 < x < 4095)
void B2_toHex12Value_LCD(int value){
    if(value < 0 || value > 4095){
        B2_write_error_LCD();
        return;
    }
    char hex[3] = {'0','0','0'};
    int temp;
    int i = 0;
    if(value != 0){
        while(value != 0) {
            temp = value % 16;
            if( temp < 10){
                hex[i++] = temp + 48;
            }else{
                hex[i++] = temp + 55;
            }
            value /= 16;
        }
    }
    B2_write_letter_LCD('0');
    B2_write_letter_LCD('X');
    B2_write_letter_LCD(hex[2]);
    B2_write_letter_LCD(hex[1]);
    B2_write_letter_LCD(hex[0]);
}


// Function to display hexadecimal value (0 < x < 65535)
void B2_toHex16Value_LCD(int value){
    if(value < 0 || value > 65535){
        B2_write_error_LCD();
        return;
    }
    char hex[4] = {'0','0','0','0'};
    int temp;
    int i = 0;
    if(value != 0){
        while(value != 0) {
            temp = value % 16;
            if( temp < 10){
                hex[i++] = temp + 48;
            }else{
                hex[i++] = temp + 55;
            }
            value /= 16;
        }
    }
    B2_write_letter_LCD('0');
    B2_write_letter_LCD('X');
    B2_write_letter_LCD(hex[3]);
    B2_write_letter_LCD(hex[2]);
    B2_write_letter_LCD(hex[1]);
    B2_write_letter_LCD(hex[0]);
}

// Function to display decimal value (0 < x < 255)
void B2_Dec8Value_LCD(int value){
    if(value < 0 || value > 255){
        B2_write_error_LCD();
        return;
    }
    char unity = (int)(value%10);
    char ten = (int)((value/10)%10);
    char hundred = (int)((value/100)%10);
    B2_write_letter_LCD(hundred+0x30);
    B2_write_letter_LCD(ten+0x30);
    B2_write_letter_LCD(unity+0x30);
}

// Function to display decimal value (0 < x < 65535)
void B2_Dec16Value_LCD(int value){
    if(value < 0 || value > 65535){
        B2_write_error_LCD();
        return;
    }
    char unity = (int)(value%10);
    char ten = (int)((value/10)%10);
    char hundred = (int)((value/100)%10);
    char thousand1 = (int)((value/1000)%10);
    char thousand2 = (int)((value/10000)%10);
    B2_write_letter_LCD(thousand1+0x30);
    B2_write_letter_LCD(thousand2+0x30);
    B2_write_letter_LCD(hundred+0x30);
    B2_write_letter_LCD(ten+0x30);
    B2_write_letter_LCD(unity+0x30);
}

// Function to set Cursor to a custom position in matrix language
void B2_MatrixSetCursor_LCD(int line, int column){
    int i;
    if((column < 0 || column > 15) || (line < 0 || line > 1)){
        B2_write_error_LCD();
        return;
    }
    int position = (column + (40*line));
    if(position > 40){
        B2_setCursorPosition_LCD(position+24);    
    }else{
        B2_setCursorPosition_LCD(position);
    }
}

// Function to set function in the display
void B2_SetFunction_LCD(int line, int fonttype){
    B2_sendPulse_LCD(0x20|(line&0x01)<<3|(fonttype&0x01)<<2, 0, 0);
}

// Function to clear display
void B2_clearDisplay_LCD(){
    B2_sendPulse_LCD(1, 0, 0);
}

// Function to set cursor to initial position
void B2_goToInit_LCD(){
    B2_sendPulse_LCD(2, 0, 0);
}

// Function to set CGRAM address
void B2_setCGRAM_LCD(uint8_t addr){
    B2_sendPulse(( 0x80 | (0x3F & addr)), 0, 0);
}

int B2_busy_LCD(){
    if(B2_read_Slave() & 0x80){
        return 1;
    }else{
        return 0;
    }
}

// Function to write a letter in the LCD display
void B2_write_letter_LCD(char data){
    uint8_t ascii_letter = data;                         // Pick ASCII value of the letter (char)
    B2_sendPulse_LCD(ascii_letter, 1, 0);
}

// Function to write a full string in the LCD display
void B2_write_string_LCD(char* string){
    B2_write_valid_string_LCD(string);           // Verify if the string is valid
}

// Function to write a ERROR! message in the LCD display
void B2_write_error_LCD(){
    B2_write_letter_LCD('E');
    B2_write_letter_LCD('R');
    B2_write_letter_LCD('R');
    B2_write_letter_LCD('O');
    B2_write_letter_LCD('R');
    B2_write_letter_LCD('!');
}

// Function to verify if the string is valid and if it is, write it in the LCD display
void B2_write_valid_string_LCD(char* string){
    int counter = 0;
    int howmany_letters = strlen(string);                // Count length of the string
    if(howmany_letters >= 17){                           // If size is larger than the 16x2 of the display size, put an error message
        B2_write_error_LCD();                            // Erro message
        return;
    }else{
        while(counter != howmany_letters){              // Write string with the string size
            B2_write_letter_LCD(string[counter]);       // Write letter in the LCD display
            counter++;
        }
    }
}
