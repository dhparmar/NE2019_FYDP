// ads1252.c
// Copyright (C) 2019 ckitagawa <ckitagawa@edu.uwaterloo.ca>
// Distributed under terms of the MIT license.

#include "ads1252.h"

#include "delay.h"

#define NUM_ADS1252_BITS 24

void ads1252_init(const ADS1252Config *cfg) {
  GpioSettings settings = {
    .direction = GPIO_DIR_OUT,
    .state = GPIO_STATE_HIGH,
    .resistor = GPIO_RES_NONE,
    .alt_function = GPIO_ALTFN_NONE,
  };
  gpio_init_pin(&cfg->sclk, &settings);
  settings.state = GPIO_STATE_LOW;
  settings.direction = GPIO_DIR_IN;
  gpio_init_pin(&cfg->data, &settings);
}

void ads1252_enable(const ADS1252Config *cfg) {
  gpio_set_state(&cfg->sclk, GPIO_STATE_LOW);
}

void ads1252_reset(const ADS1252Config *cfg) {
  gpio_set_state(&cfg->sclk, GPIO_STATE_HIGH);
  delay_us(720);
}

void ads1252_read(const ADS1252Config *cfg, int32_t *buf) {
  // NOTE: This is super sketchy and should probably use SPI. However, this
  // implementation should emulate the behavior closely enough.
  ads1252_enable(cfg);
  // ads1252_reset(cfg);
  GpioState bit = GPIO_STATE_LOW;
  *buf = 0;
  volatile GpioState rdy = GPIO_STATE_LOW;
  do {
    gpio_get_state(&cfg->data, &rdy);
  } while (rdy != GPIO_STATE_LOW);
  do {
    gpio_get_state(&cfg->data, &rdy);
  } while (rdy != GPIO_STATE_HIGH);
  delay_us(15);

  for (uint16_t i = 0; i < NUM_ADS1252_BITS; ++i) {
    gpio_set_state(&cfg->sclk, GPIO_STATE_HIGH);
    delay_us(1);
    gpio_get_state(&cfg->data, &bit);
    *buf <<= 0x01;
    *buf |= bit;
    gpio_set_state(&cfg->sclk, GPIO_STATE_LOW);
    delay_us(1);
  }
  *buf <<= 8;
  *buf >>= 8;
}
