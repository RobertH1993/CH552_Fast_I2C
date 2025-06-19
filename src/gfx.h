#pragma once
#include <stdint.h>

/**
 * A small and fast GFX library for monochromic oled displays on the wch552
 * 
 * Author: Robert Hendriks (rr1993)
 */

typedef struct {
    uint8_t width, height;
    uint8_t __xdata *data;
} vbuffer_t;

/**
 * @brief Draws text to the screen.
 * 
 * @param buf Videobuffer to write text to.
 * 
 * @param text Zero terminated string to draw.
 * 
 * @warning This function does no bounds checking.
 */
void gfx_draw_text(vbuffer_t* buf, uint8_t y, uint8_t x, char* text);

/**
 * @brief Draws a pixel to the screen.
 */
void gfx_draw_pixel(vbuffer_t* buf, uint8_t y, uint8_t x);

/**
 * @brief Draws a circle to the screen.
 */
void gfx_draw_circle(vbuffer_t* buf, uint8_t y, uint8_t x, uint8_t radius);

/**
 * @brief Inverts the video buffer, turning all leds that are on, off, and vice versa
 */
void gfx_effect_invert(vbuffer_t* buf);

/**
 * @brief Sets all data inside the videobuffer to byte
 * 
 * @param byte The byte to reset the whole videobuffer to
 */
void gfx_clear(vbuffer_t* display, uint8_t byte);