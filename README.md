# MAX7219 SPI Driver for Nordic nRF52 (SDK 12.3.0)

An high-level library to program with a 8x8 LEDs matrix driven by the
[MAX7219 integrated circuit](http://www.dx.com/it/p/diy-max7219-8-8-dot-matrix-module-for-arduino-448286#.WWOzasaB0o8).

## Requirements

  * Nordic nRF52 Development Kit ([link](https://www.nordicsemi.com/eng/Products/Bluetooth-low-energy/nRF52-DK))
  * Nordic SDK (version 12.3.0) ([link](https://www.nordicsemi.com/eng/nordic/Products/nRF52-DK/nRF5-SDK-v12-zip/54283))

## Specifications

The library is currently using the following Nordic SPI instance: `NRF_DRV_SPI_INSTANCE(0)`.
You need to enable the SPI driver from the `sdk_config.h` file of the Nordic SDK.

## Quick Start

  1. Connect the 8x8 LEDs matrix to your Development Kit:
      * MOSI pin
      * Clock pin
      * Circuit select pin
  1. Copy the `max7219.c` and `max7219.h` files in your `src` folder.
  1. Build
  1. Init the max7219 driver by calling the `MAX7219_Init()` function.
  1. Enjoy the demo by calling the `MAX7219_Test()` function.

## Contributors

  * Stefano Martinallo (nallo) - s.martinallo@gmail.com

Feel free to reach me out with advices and *nice to have* features, use pull requests
for proposing changes in the codebase.

*Nallo*
