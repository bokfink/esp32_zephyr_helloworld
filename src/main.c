#include <zephyr/device.h>
#include <zephyr/drivers/pwm.h>

#define NUM_STEPS 100U
#define SLEEP_DELTA_MSEC 20U

static const struct pwm_dt_spec fading_led =
  PWM_DT_SPEC_GET(DT_NODELABEL(fading_led));

static uint32_t pulse_width_nsec = 0U;
static uint32_t pulse_width_delta_nsec = 0U;
static uint32_t steps_taken = 0U;
static bool increasing_intensity = true;
int ret;

void led_delta_timer_handler(struct k_timer *timer_info)
{
  if (increasing_intensity) {
    if (steps_taken < NUM_STEPS) {
      ret = pwm_set_pulse_dt(&fading_led, pulse_width_nsec);
      steps_taken++;
      pulse_width_nsec += pulse_width_delta_nsec;
    } else {
      increasing_intensity = false;
      steps_taken--;
      pulse_width_nsec -= pulse_width_delta_nsec;
    }
  } else {
    if (steps_taken > 0) {
      ret = pwm_set_pulse_dt(&fading_led, pulse_width_nsec);
      steps_taken--;
      pulse_width_nsec -= pulse_width_delta_nsec;
    } else {
      increasing_intensity = true;
      steps_taken++;
      pulse_width_nsec += pulse_width_delta_nsec;
    }
  }
}

K_TIMER_DEFINE(led_delta_timer, led_delta_timer_handler, NULL);

void main(void)
{
  if (!device_is_ready(fading_led.dev)) {
    printk("Error: PWM device %s is not ready\n",
	   fading_led.dev->name);
    return;
  }

  pulse_width_delta_nsec = fading_led.period / NUM_STEPS;
  
  k_timer_start(&led_delta_timer, K_MSEC(SLEEP_DELTA_MSEC),
		K_MSEC(SLEEP_DELTA_MSEC));
}
