#include <zephyr/device.h>
#include <zephyr/drivers/gpio.h>
#include <zephyr/zephyr.h>

static const struct device *gpio_ct_dev =
  DEVICE_DT_GET(DT_NODELABEL(gpio0));

void main(void)
{
  if (!device_is_ready(gpio_ct_dev)) {
    return;
  }

  int ret;
  ret = gpio_pin_configure(gpio_ct_dev,
			   25, GPIO_OUTPUT_ACTIVE);
  if (ret != 0) {
    return;
  }
  
  while (true) {
    ret = gpio_pin_set_raw(gpio_ct_dev, 25, 1);
    if (ret != 0) {
      return;
    }
    k_msleep(1000);

    ret = gpio_pin_set_raw(gpio_ct_dev, 25, 0);
    if (ret != 0) {
      return;
    }
    k_msleep(1000);
  }
}
