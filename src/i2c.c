#include "i2c.h"
#include "gpio.h"
#include "delay.h"

/**
 * A fast i2c bitbanged driver for the wch552
 * 
 * Author: Robert Hendriks (rr1993)
 */


// Forward declarations
static uint8_t i2c_write_byte(uint8_t byte);
static inline void i2c_start_condition();
static inline void i2c_stop_condition();

#define _I2C_HIGH_SPEED
#ifdef _I2C_HIGH_SPEED
// Optimized for my personal setup, you may need to remove or add NOPs here
#define DLY_us(n)  \
    __asm \
        nop \
        nop \
        nop \
        nop \
        nop \
        nop \
        nop \
    __endasm;

#else
    #include "delay.h"
#endif


// Public
void i2c_open(){
    PIN_output_OD(P16);
    PIN_output_OD(P17);
}

// Public
void i2c_close(){
    return;
}

// Public
uint8_t i2c_write(uint8_t __xdata* data, uint16_t datalen){
    uint8_t ret = I2C_NACK;

    // Send start condition
    PIN_low(P16);
    DLY_us(1);
    PIN_low(P17);

    // Write bytes
    for(register uint16_t i = 0; i < datalen; ++i){
        /* I2C_write_byte function but inlined !*/
        //FIXME allow clock stretching with a timeout

        // loop unrol
        PP16 = (*data & (0x80));
        PIN_high(P17);DLY_us(1);PIN_low(P17);
        PP16 = (*data & (0x40));
        PIN_high(P17);DLY_us(1);PIN_low(P17);
        PP16 = (*data & (0x20));
        PIN_high(P17);DLY_us(1);PIN_low(P17);
        PP16 = (*data & (0x10));
        PIN_high(P17);DLY_us(1);PIN_low(P17);
        PP16 = (*data & (0x08));
        PIN_high(P17);DLY_us(1);PIN_low(P17);
        PP16 = (*data & (0x04));
        PIN_high(P17);DLY_us(1);PIN_low(P17);
        PP16 = (*data & (0x02));
        PIN_high(P17);DLY_us(1);PIN_low(P17);
        PP16 = (*data & (0x01));
        PIN_high(P17);DLY_us(1);PIN_low(P17);

        // Release SDA
        PIN_high(P16);
        DLY_us(1);

        // Generate 9th bit for ack
        PIN_high(P17);
        DLY_us(1);
        if(PIN_read(P16)) goto stop; // invert because a 0 means ack on the i2c bus
        PIN_low(P16); //Take over SDA an keep it low so we can do a proper stop
        PIN_low(P17);

        /* I2C_write_byte function but inlined !*/

        data++;
    }
    ret = I2C_ACK;

stop:
    // stop condition
    PIN_high(P17);
    DLY_us(1);
    PIN_high(P16);

    return ret;
}

// Public
uint8_t i2c_write_slow(uint8_t* data, uint16_t datalen, uint8_t nostop){
    uint8_t ret = I2C_NACK;

    // Send start condition
    PIN_low(P16);
    DLY_us(1);
    PIN_low(P17);

    // Write bytes
    for(register uint16_t i = 0; i < datalen; ++i){
        /* I2C_write_byte function but inlined !*/
        //FIXME allow clock stretching with a timeout

        // loop unrol
        PP16 = (*data & (0x80));
        PIN_high(P17);DLY_us(1);PIN_low(P17);
        PP16 = (*data & (0x40));
        PIN_high(P17);DLY_us(1);PIN_low(P17);
        PP16 = (*data & (0x20));
        PIN_high(P17);DLY_us(1);PIN_low(P17);
        PP16 = (*data & (0x10));
        PIN_high(P17);DLY_us(1);PIN_low(P17);
        PP16 = (*data & (0x08));
        PIN_high(P17);DLY_us(1);PIN_low(P17);
        PP16 = (*data & (0x04));
        PIN_high(P17);DLY_us(1);PIN_low(P17);
        PP16 = (*data & (0x02));
        PIN_high(P17);DLY_us(1);PIN_low(P17);
        PP16 = (*data & (0x01));
        PIN_high(P17);DLY_us(1);PIN_low(P17);

        // Release SDA
        PIN_high(P16);
        DLY_us(1);

        // Generate 9th bit for ack
        PIN_high(P17);
        DLY_us(1);
        if(PIN_read(P16)) goto stop; // invert because a 0 means ack on the i2c bus
        PIN_low(P16); //Take over SDA an keep it low so we can do a proper stop
        PIN_low(P17);

        /* I2C_write_byte function but inlined !*/

        data++;
    }
    ret = I2C_ACK;

    if(nostop) return ret;

stop:
    // stop condition
    PIN_high(P17);
    DLY_us(1);
    PIN_high(P16);

    return ret;
}
