#ifndef I2C_H
#define I2C_H

#include <stdint.h>
#include "macros.h"

void delay(long x);
void config_I2C_B2(int isMaster, uint8_t ownAddr, int whichslave);
void B2_start_B1(uint8_t addr);
void B2_write_on_B1(char data);
void B2_stop_B1();
void config_I2C_B1(int isMaster, uint8_t ownAddr, int whichslave);
int B2_read(void);
void B2_STT_STP(uint8_t addr);

#endif // I2C_H