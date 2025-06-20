#include "gfx.h"
#include "font.h"

// Forward declarations
void gfx_draw_pixel_repeated_left_right(vbuffer_t* buf, uint8_t y, uint8_t x, direction_t direction, uint8_t repeat);
void gfx_draw_pixel_repeated_up_down(vbuffer_t* buf, uint8_t y, uint8_t x, direction_t direction, uint8_t repeat);

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

void gfx_draw_line(vbuffer_t* buf, uint8_t y, uint8_t x){
    // Bresenham's Line Algorithm

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
void gfx_draw_rect(vbuffer_t* buf, uint8_t y, uint8_t x, uint8_t width, uint8_t height, uint8_t border){
    uint8_t* ptr = buf->data + ((y/8)*buf->width) + x;
    for(register uint16_t i = 0; i < (width*height); i++){
        if(i % width == 0) ptr += buf->width; // Increase row
        *ptr |= (1 << 7);
    }
}


// Public
inline void gfx_draw_pixel(vbuffer_t* buf, uint8_t y, uint8_t x){
    *(buf->data + ((y/8)*buf->width) + x) |= (1 << y % 8);
}

// Public
// Wrapper function because we dont want one big complex function
void gfx_draw_pixel_repeated(vbuffer_t* buf, uint8_t y, uint8_t x, direction_t direction, uint8_t repeat){
    if(direction == LEFT || direction == RIGHT) gfx_draw_pixel_repeated_left_right(buf, y, x, direction, repeat);
    else gfx_draw_pixel_repeated_up_down(buf, y, x, direction, repeat);
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

// Private
void gfx_draw_pixel_repeated_left_right(vbuffer_t* buf, uint8_t y, uint8_t x, direction_t direction, uint8_t repeat){
    uint8_t* ptr = buf->data + ((y/8)*buf->width) + x;
    for(register uint8_t i = 0; i < repeat; ++i){
        *ptr |= (1 << y % 8);

        // Increase or decrease ptr
        if(direction == RIGHT) ptr++;
        else ptr--;
    }
}

void gfx_draw_pixel_repeated_up_down(vbuffer_t* buf, uint8_t y, uint8_t x, direction_t direction, uint8_t repeat){
    uint8_t* ptr = buf->data + ((y/8)*buf->width) + x;

    uint16_t i = repeat;
    if(y%8){
        if(y%8 > repeat){
            // Fill remaining byte partially
            *ptr |= direction ? (((1 << repeat)-1) << y%8) : (((1 << repeat)-1) << (y%8) - repeat);
            return;
        } else{
            // Fill remaning byte fully
            *ptr |= direction ? ~((1 << y%8)-1) : ((1 << y%8)-1); // down up
            i -= y%8;
            ptr += direction ? buf->width : -buf->width;
        }
    }

    while(i>=8){
        *ptr = 0xFF;
        i -= 8;
        ptr += direction ? buf->width : -buf->width;
    }

    if(i){
        // Fill remaining byte partially
        *ptr |= direction ? ((1 << i)-1) : (0xFF << (8 - i));
    }
}