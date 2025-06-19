## Overview
This repository contains demo code for fast I2C communication on the CH552 microcontroller. A versatile controller with hardware USB support that can be bought for $0.20. The controller is not widely in use yet but some people have done alot of heavy lifting, to add to those examples this project was started.

[![CH552 animation demo](Demo video)](https://youtube.com/shorts/glV4BcsJGZQ?feature=shared)

## Hardware
The hardware used is a CH552 board from aliexpress and a monochromic 128*64 oled display from the same source. The oled display uses a SD1306 internally and is controlled by the CH552 through a heavy overclocked I2C signal.

## How to use
Most likely the code runs out of the box, however the I2C timing is done with NOPs to get the absolute fastest timing posible. I have tested multiple displays and some can handle higher clock rates then others. If you get no output on the display after uploading the code go to i2c.c and add some NOPs to the DLY_us define. To troubleshoot connections remove the _I2C_HIGH_SPEED inside the i2c.c file. This slows the I2C communication dramaticly but provides a stable signal that should be accepted by all displays.

The amount of balls on the display can be changed by editing the NUMBER_OF_CIRCLES define inside main.c. Beware that too high of a number will fail to compile because the circles data is stored in limited IRAM. Also included in the code are the gfx.h and gfx.c files that support simple drawing functions. Out-of-the-box text, pixels, and circles are supported.

Most likely more performance can be gained by writing faster logic for the bouncing off the balls.



