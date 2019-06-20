#include "uart.h"
#include <msp430.h>

void UARTInit_A3(int parity, int isEven, int isMSB, int is2Stops, int mode, int isSync){

	SetFlag(UCA3CTLW0, UCSWRST);
	if(parity) SetFlag(UCA3CTLW0, UCPEN);
	if(!isEven) SetFlag(UCA3CTLW0, UCPAR);
	if(isMSB) SetFlag(UCA3CTLW0, UCMSB);
	if(is2Stops) SetFlag(UCA3CTLW0, UCSPB);
	if(mode == 0) SetFlag(UCA3CTLW0, UCMODE0);
	if(mode == 1) SetFlag(UCA3CTLW0, UCMODE1);
	if(mode == 2) SetFlag(UCA3CTLW0, UCMODE_2);
	if(mode == 3) SetFlag(UCA3CTLW0, UCMODE_3);
	if(isSync) SetFlag(UCA3CTLW0, UCSYNC);
	
	SetFlag(UCA3CTLW0, UCSSEL__SMCLK);

	UCA3BRW = 6;
	UCA3MCTLW = UCBRF_8 | UCOS16;

	// Output PIN
	SetPort(P6, DIR, 0);
	SetPort(P6, SEL0, 0);
	ClearPort(P6, SEL1, 0);

	// Input PIN
	ClearPort(P6, DIR, 1);
	SetPort(P6, SEL0, 1);
	ClearPort(P6, SEL1, 1);

	ClearFlag(UCA3CTLW0, UCSWRST);

    SetFlag(UCA3IE, UCRXIE);
}

void UARTInit_A1(int parity, int isEven, int isMSB, int is2Stops, int mode, int isSync){

	SetFlag(UCA1CTLW0, UCSWRST);
	if(parity) SetFlag(UCA1CTLW0, UCPEN);
	if(isEven) SetFlag(UCA1CTLW0, UCPAR);
	if(isMSB) SetFlag(UCA1CTLW0, UCMSB);
	if(is2Stops) SetFlag(UCA1CTLW0, UCSPB);
	if(mode == 0) SetFlag(UCA1CTLW0, UCMODE0);
	if(mode == 1) SetFlag(UCA1CTLW0, UCMODE1);
	if(mode == 2) SetFlag(UCA1CTLW0, UCMODE_2);
	if(mode == 3) SetFlag(UCA1CTLW0, UCMODE_3);
	if(isSync) SetFlag(UCA1CTLW0, UCSYNC);
	
	SetFlag(UCA1CTLW0, UCSSEL__SMCLK);

	UCA1BRW = 6;
	UCA1MCTLW = UCBRF_8 | UCOS16;

	// Output PIN
    SetPort(P2, DIR, 5);
    SetPort(P2, SEL0, 5);
    ClearPort(P2, SEL1, 5);

    // Input PIN
    ClearPort(P2, DIR, 6);
    SetPort(P2, SEL0, 6);
    ClearPort(P2, SEL1, 6);

	ClearFlag(UCA1CTLW0, UCSWRST);
    
    SetFlag(UCA1IE, UCRXIE);

}

void UARTA3_stop(){
  UCA3IE = 0;  // Stops all interrupts
  while(UCA3STATW & UCBUSY);  // Waits to finish all transactions
  SetFlag(UCA3CTLW0, UCSWRST);
}

void UARTA3_write_byte(uint8_t data){
  while(!(UCA3IFG & UCTXIFG) && (UCA3STATW & UCBUSY));  // Waits for TXBUF to be empty
  UCA3TXBUF = data;             // Sends data
}

void UARTA3_write_string(char* string, int end_string){
  int i;
  int size = strlen(string);
  for(i = 0; i < size; i++){
    UARTA3_write_byte(string[i]);
  }
  if(end_string){
    UARTA3_write_byte('\0');  // End string transmission
  }
}

void UARTA1_stop(){
  UCA1IE = 0;  // Stops all interrupts
  while(UCA1STATW & UCBUSY);  // Waits to finish all transactions
  SetFlag(UCA1CTLW0, UCSWRST);
}

void UARTA1_write_byte(uint8_t data){
  while(!(UCA1IFG & UCTXIFG) && (UCA1STATW & UCBUSY));  // Waits for TXBUF to be empty
  UCA1TXBUF = data;             // Sends data
}

void UARTA1_write_string(char* string, int end_string){
  int i;
  int size = strlen(string);
  for(i = 0; i < size; i++){
    UARTA1_write_byte(string[i++]);
  }
  if(end_string){
    UARTA1_write_byte('\0');  // End string transmission
  }
}
