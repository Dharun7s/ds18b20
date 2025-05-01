#ifndef DS18B20_H
#define DS18B20_H

#include "pico/stdlib.h"

void ds18b20_init(uint gpio_pin);
float ds18b20_get_temperature();

#endif // DS18B20_H
