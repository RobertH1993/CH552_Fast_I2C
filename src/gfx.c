#include "gfx.h"
#include "font.h"

// Public
void gfx_draw_text(vbuffer_t* buf, uint8_t y, uint8_t x, char* text){
    uint8_t *vbuf = buf->data + ((y/8)*buf->width) + x;
    uint8_t *text_ptr = text;
    uint8_t bit_offset = y % 8;

    while(*text_ptr != 0x00){
        const uint8_t *font = &ssd1306xled_font6x8[(*text_ptr-32) * 6];
        for(uint8_t i = 0; i < 6; i++){
            if(!bit_offset){ // We only need to write to one byte because the y aligns on an 8 bit value
                *vbuf = *font;
            }else{
                uint8_t high_mask = ~((1 << bit_offset) - 1);
                *vbuf |= ((*font << bit_offset) & high_mask); // upper half of the text
                *(vbuf + (buf->width)) |= ((*font >> (8 - bit_offset)) & ~high_mask); // Write lower half of the text
            }
            vbuf++;font++;
        }
        text_ptr++;
    }

}

// Public
void gfx_draw_circle(vbuffer_t* buf, uint8_t y1, uint8_t x1, uint8_t radius){
    // Midpoint circle algorithm
    int8_t x, y = 0;
    int8_t p = 1 - radius;
    y = radius;


    while(x < y){
        if(p<0){
            p += 2 * x + 1;
        }else{
            y--;
            p += 2 * (x-y) + 1;
        }

        gfx_draw_pixel(buf, y1 + y, x1 + x);
        gfx_draw_pixel(buf, y1 + y, x1 - x);
        gfx_draw_pixel(buf, y1 - y, x1 + x);
        gfx_draw_pixel(buf, y1 - y, x1 - x);

        gfx_draw_pixel(buf, y1 + x, x1 + y);
        gfx_draw_pixel(buf, y1 + x, x1 - y);
        gfx_draw_pixel(buf, y1 - x, x1 + y);
        gfx_draw_pixel(buf, y1 - x, x1 - y);

        x++;
    }


}

// Public
inline void gfx_draw_pixel(vbuffer_t* buf, uint8_t y, uint8_t x){
    *(buf->data + ((y/8)*buf->width) + x) |= (1 << y % 8);
}

// Public
void gfx_effect_invert(vbuffer_t* buf){
    uint8_t* ptr = buf->data;
    while(ptr != (buf->data + ((buf->height / 8) * buf->width))){
        *ptr = ~*ptr;
        ptr++;
    }
}

// Public
void gfx_clear(vbuffer_t* buf, uint8_t byte){
    uint8_t *ptr = buf->data;
    while(ptr != (buf->data + ((buf->height / 8) * buf->width))){
        *ptr = byte;
        ptr++;
    }
}