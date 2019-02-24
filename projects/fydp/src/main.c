// Example program for STM32F072 Controller board or Discovery Board.
// Blinks the LEDs sequentially.
#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>
#include <stdio.h>

#include "ads1252.h"
#include "cd74hct4067.h"
#include "common_misc.h"  // Various helper functions/macros.
#include "delay.h"
#include "gpio.h"  // General Purpose I/O control.
#include "interrupt.h"
#include "soft_timer.h"

// Depending on which board you are working with you will need to (un)comment
// the relevant block of GPIO pins. Generally these would be in a configuration
// file somewhere and we normally write configurations only for the controller
// board.

// Controller board LEDs
static const GpioAddress leds[] = {
  { .port = GPIO_PORT_A, .pin = 0 },  //
  { .port = GPIO_PORT_A, .pin = 1 },  //
  { .port = GPIO_PORT_A, .pin = 2 },  //
  { .port = GPIO_PORT_A, .pin = 3 },  //
};

int main(void) {
  // Enable various peripherals
  printf("Hello\n");
  gpio_init();
  // interrupt_init();
  // soft_timer_init();

  // GpioSettings led_settings = {
  //   .direction = GPIO_DIR_OUT,        // The pin needs to output.
  //   .state = GPIO_STATE_HIGH,         // Start in the "on" state.
  //   .alt_function = GPIO_ALTFN_NONE,  // No connections to peripherals.
  //   .resistor = GPIO_RES_NONE,        // No need of a resistor to modify floating logic levels.
  // };

  // // Init all of the LED pins
  // for (size_t i = 0; i < SIZEOF_ARRAY(leds); i++) {
  //   gpio_init_pin(&leds[i], &led_settings);
  // }

  // // Keep toggling the state of the pins from on to off with a 50 ms delay between.
  // while (true) {
  //   for (size_t i = 0; i < SIZEOF_ARRAY(leds); i++) {
  //     gpio_toggle_state(&leds[i]);
  //     for (volatile uint32_t i = 0; i < 1000000; ++i) {
  //     }
  //   }
  // }

  // LED Test code
  // CD74HCT4067Config config = { .s = { { .port = GPIO_PORT_C, .pin = 4 },
  //                                     { .port = GPIO_PORT_C, .pin = 5 },
  //                                     { .port = GPIO_PORT_C, .pin = 6 },
  //                                     { .port = GPIO_PORT_C, .pin = 7 } } };
  // cd74hct4067_init(&config);
  // while (true) {
  //   for (size_t i = 0; i < 16; ++i) {
  //     cd74hct4067_set_output(&config, i);
  //     for (volatile uint32_t i = 0; i < 1000000; ++i) {
  //     }
  //   }
  // }

  // ADC Test Code
  ADS1252Config cfg = { .data = { .port = GPIO_PORT_A, .pin = 11 },
                        .sclk = { .port = GPIO_PORT_A, .pin = 10 } };
  ads1252_init(&cfg);
  volatile uint32_t res = 0;
  while (true) {
    ads1252_read(&cfg, &res);
    printf("%d\n", res);
  }

  return 0;
}
