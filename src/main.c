/*
 * Author: Omi Shrestha (work.omishrestha@gmail.com)
*/

#include <zephyr/device.h>
#include <zephyr/devicetree.h>
#include <errno.h>
#include <zephyr/drivers/led.h>
#include <zephyr/sys/util.h>
#include <zephyr/kernel.h>
#include <zephyr/logging/log.h>

LOG_MODULE_REGISTER(main, CONFIG_LOG_DEFAULT_LEVEL);

// Macro to get the PWM LED node from the device tree
#define LED_PWM_NODE_ID	 DT_COMPAT_GET_ANY_STATUS_OKAY(pwm_leds)

#define MAX_BRIGHTNESS	100
#define FADE_DELAY_MS	10
#define FADE_DELAY	K_MSEC(FADE_DELAY_MS) // Convert milliseconds to kernel time delay

// Array of LED labels fetched from the device tree
const char *led_label[] = {
	DT_FOREACH_CHILD_SEP_VARGS(LED_PWM_NODE_ID, DT_PROP_OR, (,), label, NULL)
};

const int num_leds = ARRAY_SIZE(led_label);

/**
 * @brief Run tests on a single LED using the LED API syscalls.
 *
 * This function gradually increases the brightness of the LED to the maximum level,
 * pauses for a moment, and then gradually decreases the brightness back to zero.
 *
 * @param led_pwm LED PWM device.
 * @param led Number of the LED to test.
 */
static void run_led_test(const struct device *led_pwm, uint8_t led)
{
	int err;
	uint16_t level;

	LOG_INF("Testing LED %d - %s", led, led_label[led] ? : "no label");

	// Gradually increase LED brightness from 0 to MAX_BRIGHTNESS
	LOG_INF("  Increasing brightness gradually");
	for (level = 0; level <= MAX_BRIGHTNESS; level++) {
		err = led_set_brightness(led_pwm, led, level);
		if (err < 0) {
			LOG_ERR("err=%d brightness=%d\n", err, level);
			return;
		}
		k_sleep(FADE_DELAY);
	}
	
	k_sleep(K_MSEC(1000)); // Wait for 1 second

	// Gradually decrease LED brightness from MAX_BRIGHTNESS to 0
	LOG_INF("  Decreasing brightness gradually");
	for (int level = MAX_BRIGHTNESS; level >= 0; level--) {
    	err = led_set_brightness(led_pwm, led, (uint16_t)level);
    	if (err < 0) {
        	LOG_ERR("err=%d brightness=%d\n", err, level);
        	return;
   		}
    	k_sleep(FADE_DELAY); 
	}
	k_sleep(K_MSEC(1000)); // Wait for 1 second
}

/**
 * @brief Main function of the LED test application.
 *
 * This function initializes the LED PWM device, checks its readiness, and iterates
 * through all available LEDs to run the brightness test indefinitely.
 */
int main(void)
{
	const struct device *led_pwm;
	uint8_t led;

	led_pwm = DEVICE_DT_GET(LED_PWM_NODE_ID);
	if (!device_is_ready(led_pwm)) {
		LOG_ERR("Device %s is not ready", led_pwm->name);
		return 0;
	}

	// Check if any LEDs are available
	if (!num_leds) {
		LOG_ERR("No LEDs found for %s", led_pwm->name);
		return 0;
	}

	// Iterates through all available LEDs
	do {
		for (led = 0; led < num_leds; led++) {
			run_led_test(led_pwm, led);
		}
	} while (true);

	return 0; // This line will never be reached
}
