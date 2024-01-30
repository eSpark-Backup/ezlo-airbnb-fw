/*
  DFRobotHCHOSensor.h - DFRobotHCHOSensor library
  Developed by Jason - 2016/12/15

  This library is free software; you can redistribute it and/or
  modify it under the terms of the GNU Lesser General Public
  License as published by the Free Software Foundation; either
  version 2.1 of the License, or (at your option) any later version.

  This library is distributed in the hope that it will be useful,
  but WITHOUT ANY WARRANTY; without even the implied warranty of
  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
  Lesser General Public License for more details.

Gravity: HCHO Sensor info found at
http://www.dfrobot.com/

Version 1.0: 15 Dec 2016 by Jason<jason.ling@dfrobot.com@dfrobot.com>
- Updated to build against Arduino 1.0.6
- Made accessors inline in the header so they can be optimized away
*/


#ifndef __DFRobotHCHOSensor_H_
#define __DFRobotHCHOSensor_H_

#include <stddef.h>
#include "driver/gpio.h"
#include "ezlopi_uart.h"

#define MAXLENGTH  9
#define CH2O_RX_PIN      GPIO_NUM_45
#define CH2O_TX_PIN      GPIO_NUM_48
#define CH2O_UART_BUAD_RATE 9600

typedef struct ch2o_sensor_data
{
  bool available;
  float ppm;
} ch2o_sensor_data_t;


bool ch2o_sensor_init(ch2o_sensor_data_t* data);

#endif