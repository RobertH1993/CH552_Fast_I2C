#ifndef _I2C_H
#define _I2C_H

#include <stdint.h>

#define I2C_ACK 1
#define I2C_NACK 0



void i2c_open();
void i2c_close();
uint8_t i2c_write_slow(uint8_t* data, uint16_t datalen, uint8_t nostop);
uint8_t i2c_write(uint8_t __xdata* data, uint16_t datalen);


#endif