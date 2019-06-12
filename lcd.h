#ifndef LCD_H
#define LCD_H

#include <stdint.h>
#include <string.h>
#include <math.h>
#include "i2c.h"

void LCD_B2_inic(uint8_t addr);
void LCD_B2_aux(char data);
void B2_write_byte_LCD(char data);
void B2_turnOnBL_LCD();
void B2_turnOffBL_LCD();
void B2_sendPulse_LCD(uint8_t byte, int RS, int RW);
void B2_setCursorPosition_LCD(uint8_t position);
void B2_toFloatValue_LCD(float value);
void B2_toHex8Value_LCD(int value);
void B2_toHex12Value_LCD(int value);
void B2_toHex16Value_LCD(int value);
void B2_Dec8Value_LCD(int value);
void B2_Dec16Value_LCD(int value);
void B2_MatrixSetCursor_LCD(int line, int column);
void B2_SetFunction_LCD(int line, int fonttype);
void B2_clearDisplay_LCD();
void B2_goToInit_LCD();
void B2_write_letter_LCD(char data);
void B2_write_string_LCD(char* string);
void B2_write_error_LCD();
void B2_write_valid_string_LCD(char* string);

#endif // LCD_H