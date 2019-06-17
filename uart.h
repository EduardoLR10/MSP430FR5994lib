#ifndef UART_H
#define UART_H

#include "macros.h"
#include <string.h>
#include <stdint.h>
#include <stdio.h>

#define _parity 1
#define _nonParity 0
#define _isEven 1
#define _isOdd 0
#define _isMSB 1
#define _isLSB 0
#define _is2Stops 1
#define _is1Stops 0
#define _isSync 1
#define _isnonSync 0

void UARTInit_A1(int parity, int isEven, int isMSB, int is2Stops, int mode, int isSync);
void UARTA1_stop();
void UARTA1_write_byte(uint8_t data);
void UARTA1_write_string(char* string, int end_string);
void UARTInit_A3(int parity, int isEven, int isMSB, int is2Stops, int mode, int isSync);
void UARTA3_stop();
void UARTA3_write_byte(uint8_t data);
void UARTA3_write_string(char* string, int end_string);

#endif // UART_H