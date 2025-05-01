# LED PWM Control 

This project demonstrates the control of PWM LEDs using the Zephyr LED API. It is designed to test the functionality of the `led-pwm` driver and provides a simple yet effective way to interact with LEDs connected to a PWM device.

## Overview

The sample application identifies the first `pwm-leds` compatible device instance defined in the device tree (DT). For each LED attached to this device (defined as child nodes), a series of test patterns are executed to validate and demonstrate LED control using the LED API.

## Test Patterns

For each PWM LED, the following sequence of operations is performed:

1. **Turning On**: The LED is turned on at full brightness.
2. **Turning Off**: The LED is turned off completely.
3. **Gradual Brightness Increase**: The LED brightness is gradually increased from 0% to 100%.
4. **Blinking (Fast)**: The LED blinks with a pattern of 0.1 seconds on and 0.1 seconds off.
5. **Blinking (Slow)**: The LED blinks with a pattern of 1 second on and 1 second off.
6. **Turning Off**: The LED is turned off again to complete the cycle.

## Building and Running

This sample can be built and executed on any board with PWM LEDs connected. Ensure that the LEDs are properly described in the device tree source (DTS) file:

- The `compatible` property of the device node must match `pwm-leds`.
- Each LED must be defined as a child node with the appropriate PWM configuration provided through a `pwms` phandle.

### Steps to Build and Run:

1. Configure your Zephyr environment and select a compatible board.
2. Build the sample application using the following command:
   ```
   west build -b <board_name> .
   ```
3. Flash the application to your board:
   ```
   west flash
   ```
4. Observe the LED behavior as the test patterns are executed.

## Notes

- This is compatible with all boards that support PWM LEDs.
- Ensure that the hardware connections and device tree configurations are correct before running the application.

## Relevant API

This project uses the following Zephyr API:

- [LED Interface](https://docs.zephyrproject.org/latest/reference/peripherals/led.html)
