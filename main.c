#include <stdint.h>


// Libraries
#include "src/system.h"                   // system functions
#include "src/delay.h"                    // for delays
#include "src/prng.h"
#include "i2c.h"
#include "src/gfx.h"

typedef struct MovingCircle{
  int16_t x, y;
  uint8_t radius;
  int8_t vx, vy; // Speed vector
} MovingCircle;

#ifdef SMALL_OLED
#define I2C_BUFFER_SIZE 514
uint8_t __xdata i2c_buffer[514] = {
  0x78, 0x00,
  0xA8, 0x3F, // Multiplex ration
  0x8D, 0x14, // Set DC-DC enable
  0x20, 0x00, // Set horizontal addressing
  0xC8, 0xA1, // Flip screen
  0xDA, 0x02, // Set com pins
  0x81, 0xFF, // Set contrast
  0XD3, 0x20, // Set Display offset
  0x22, 0x00, 0x03, // Set number of pages, 0x00 -> 0x03 is 4 pages aka 32 bits in vertical addressing mode
  0xAF,        // Turn display on
};
#else
#define I2C_BUFFER_SIZE 1024
uint8_t __xdata i2c_buffer[1024] = {
  0x78, 0x00,
  0xA8, 0x3F, // Multiplex ration
  0x8D, 0x14, // Set DC-DC enable
  0x20, 0x00, // Set horizontal addressing
  0xC8, 0xA1, // Flip screen
  0xDA, 0x12, // Set com pins
  0x81, 0xFF, // Set contrast
  0XD3, 0x00, // Set Display offset
  0xAF,        // Turn display on
};
#endif

#define NUMBER_OF_CIRCLES 16
MovingCircle __idata circles[NUMBER_OF_CIRCLES];

void update_circles(){
  for(uint8_t i = 0; i < NUMBER_OF_CIRCLES; i++){
    circles[i].x += circles[i].vx;
    circles[i].y += circles[i].vy;

    if(circles[i].x + circles[i].radius >= 127){
      circles[i].x = 127 - circles[i].radius;
      circles[i].vx = -circles[i].vx;
    }else if(circles[i].x - circles[i].radius <= 0){
      circles[i].x = 0 + circles[i].radius;
      circles[i].vx = -circles[i].vx;
    }if(circles[i].y + circles[i].radius >= 63){
      circles[i].y = 63 - circles[i].radius;
      circles[i].vy = -circles[i].vy;
    }else if(circles[i].y - circles[i].radius <= 16){
      circles[i].y = 16 + circles[i].radius;
      circles[i].vy = -circles[i].vy;
    }

  }
}

void main(void) {
  // Setup
  CLK_config();                           // configure system clock
  CLK_external();
  DLY_ms(5);                              // wait for clock to stabilize

  i2c_open();

  // Init the display
#ifdef SMALL_OLED
  i2c_write(i2c_buffer, 20);
#else
  i2c_write(i2c_buffer, 17);
#endif

  // i2c_buffer[0] contains the device addr
  i2c_buffer[0] = 0x78; // Set device address
  i2c_buffer[1] = 0x40; // Set data command
  vbuffer_t video_buffer;
#ifdef SMALL_OLED
  video_buffer.height = 32;
  video_buffer.width = 128;
  video_buffer.data = &i2c_buffer[2];
#else
  video_buffer.height = 64;
  video_buffer.width = 128;
  video_buffer.data = i2c_buffer;
#endif

  // Init circles
  for(uint8_t i = 0; i < NUMBER_OF_CIRCLES; i++){
    circles[i].vx = prng_max(3) + 3;
    circles[i].vy = prng_max(3) + 3;
    circles[i].radius = 2;
    circles[i].x = prng_max(100) + 16;
    circles[i].y = prng_max(50) + 16;
  }


  
  while(1){
    // Draw to video buffer
    gfx_clear(&video_buffer, 0x00);
    gfx_draw_text(&video_buffer, 4, 16, "RR1993 OLED Demo");


    for(uint8_t  i = 0; i < NUMBER_OF_CIRCLES; i++){
      gfx_draw_circle(&video_buffer, circles[i].y, circles[i].x, circles[i].radius);
    }

    // Send to display
    uint8_t startbytes[2] = {0x78, 0x40};
    i2c_write_slow(startbytes, 2, 1);
    i2c_write(i2c_buffer, I2C_BUFFER_SIZE);

    // Update animation
    update_circles();
  }
}
