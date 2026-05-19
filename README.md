# STM32F429 GPIO Driver Example

This repository contains a simple GPIO driver and example code for the STM32F429 microcontroller.

## Structure
- `header/`: Header files for GPIO driver and MCU definitions
- `src/`: Source files for driver implementation and main application
- `uVisionProject/`: Keil uVision project files and build outputs

## How to Use
1. Open the project in Keil uVision (open `STM32F429_DriverDev.uvprojx` in `uVisionProject/`).
2. Build and flash to an STM32F429 board.
3. The example toggles LEDs and reads a push button using the custom GPIO driver.

## Files
- `stm32f429xx_gpio_driver.h/c`: GPIO driver interface and implementation
- `stm32f429xx.h`: MCU register and peripheral definitions
- `main.c`: Example usage of the GPIO driver

## License
This project is for educational purposes.
