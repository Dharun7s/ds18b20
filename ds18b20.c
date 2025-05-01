#include "ds18b20.h"
#include "hardware/gpio.h"

static uint DATA_PIN;

static void ds18b20_reset() {
    gpio_set_dir(DATA_PIN, GPIO_OUT);
    gpio_put(DATA_PIN, 0);
    sleep_us(480);
    gpio_set_dir(DATA_PIN, GPIO_IN);
    sleep_us(70);
    while (!gpio_get(DATA_PIN));
    sleep_us(410);
}

static void ds18b20_write_bit(bool bit) {
    gpio_set_dir(DATA_PIN, GPIO_OUT);
    gpio_put(DATA_PIN, 0);
    sleep_us(bit ? 5 : 60);
    gpio_put(DATA_PIN, 1);
    sleep_us(bit ? 55 : 5);
}

static bool ds18b20_read_bit() {
    bool bit;
    gpio_set_dir(DATA_PIN, GPIO_OUT);
    gpio_put(DATA_PIN, 0);
    sleep_us(2);
    gpio_set_dir(DATA_PIN, GPIO_IN);
    sleep_us(10);
    bit = gpio_get(DATA_PIN);
    sleep_us(50);
    return bit;
}

static void ds18b20_write_byte(uint8_t data) {
    for (int i = 0; i < 8; i++) {
        ds18b20_write_bit(data & 0x01);
        data >>= 1;
    }
}

static uint8_t ds18b20_read_byte() {
    uint8_t value = 0;
    for (int i = 0; i < 8; i++) {
        value >>= 1;
        if (ds18b20_read_bit()) {
            value |= 0x80;
        }
        sleep_us(50);
    }
    return value;
}

void ds18b20_init(uint gpio_pin) {
    DATA_PIN = gpio_pin;
    gpio_init(DATA_PIN);
    gpio_pull_up(DATA_PIN);
}

float ds18b20_get_temperature() {
    ds18b20_reset();
    ds18b20_write_byte(0xCC);
    ds18b20_write_byte(0x44);
    sleep_ms(750);
    ds18b20_reset();
    ds18b20_write_byte(0xCC);
    ds18b20_write_byte(0xBE);
    uint8_t temp_lsb = ds18b20_read_byte();
    uint8_t temp_msb = ds18b20_read_byte();
    int16_t temp = (temp_msb << 8) | temp_lsb;
    return temp / 16.0f;
}
