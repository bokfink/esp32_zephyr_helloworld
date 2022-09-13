#include <zephyr/device.h>
#include <zephyr/drivers/gpio.h>
#include <zephyr/zephyr.h>

static const struct gpio_dt_spec led =
  GPIO_DT_SPEC_GET(DT_NODELABEL(blinking_led), gpios);

void main(void)
{
  if (!device_is_ready(led.port)) {
    return;
  }

  int ret;
  ret = gpio_pin_configure_dt(&led, GPIO_OUTPUT_ACTIVE);
  if (ret != 0) {
    return;
  }
  
  while (true) {
    ret = gpio_pin_toggle_dt(&led);
    if (ret != 0) {
      return;
    }
    k_msleep(1000);

  }
}
