#include <zephyr/device.h>
#include <zephyr/drivers/gpio.h>
#include <zephyr/kernel.h>

static struct gpio_callback button_cb_data;

static const struct gpio_dt_spec led =
  GPIO_DT_SPEC_GET(DT_NODELABEL(blinking_led), gpios);
static const struct gpio_dt_spec button =
  GPIO_DT_SPEC_GET(DT_NODELABEL(button), gpios);

void button_pressed(const struct device *dev,
		    struct gpio_callback *cb,
		    uint32_t pins)
{
  int ret;
  ret = gpio_pin_toggle_dt(&led);
  if (ret != 0) {
    printk("Could not toggle LED\n");
  }
}

void main(void)
{
  if (!device_is_ready(led.port)) {
    return;
  }

  if (!device_is_ready(button.port)) {
    return;
  }
  
  int ret;
  ret = gpio_pin_configure_dt(&led, GPIO_OUTPUT_ACTIVE);
  if (ret != 0) {
    return;
  }

  ret = gpio_pin_configure_dt(&button, GPIO_INPUT);
  if (ret != 0) {
    return;
  }

  ret = gpio_pin_interrupt_configure_dt(&button,
					GPIO_INT_EDGE_TO_ACTIVE);
  if (ret != 0) {
    return;
  }

  gpio_init_callback(&button_cb_data, button_pressed,
		     BIT(button.pin));
  gpio_add_callback(button.port, &button_cb_data);
}
