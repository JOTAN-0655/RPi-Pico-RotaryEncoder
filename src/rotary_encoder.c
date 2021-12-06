#include <math.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include "pico/stdlib.h"
//PIN DEFINE

bool led = false;
uint32_t pre_time = 0;

const double radius = 1.2; //cm
const int rotary_encoder_div = 4;
const double arc_len = (M_PI * 2 * radius) * 1 / (double)rotary_encoder_div;
double speed = 0.0;

void gpio_callback(){
	led = !led;
	gpio_put(25,led);

	uint32_t current = time_us_32();
	uint32_t time = current - pre_time;

	speed = arc_len / (double)time * 10000; // m/s

	pre_time = current;
}

int time_100sec = 0;
bool timer_callback( repeating_timer_t *rt )
{
    printf("%d,%lf\r\n",time_100sec,speed);
	time_100sec++;
}

int main()
{
	stdio_init_all();

	gpio_init(25);
	gpio_init(15);
	gpio_init(14);

	gpio_set_dir(14,false);
	gpio_set_dir(15,false);
	gpio_set_dir(25,true);

	pre_time = time_us_32();
	gpio_set_irq_enabled_with_callback(15, GPIO_IRQ_EDGE_FALL, true, &gpio_callback);

	static repeating_timer_t timer;

    while (true) {
		
		cancel_repeating_timer	(	&timer	);
		time_100sec = 0;
		speed = 0;
		sleep_ms(200);
		while (gpio_get(14) != 1);
		while (gpio_get(14) != 0);

		add_repeating_timer_ms( -100, &timer_callback, NULL, &timer );
		sleep_ms(200);
		while (gpio_get(14) != 1);
		while (gpio_get(14) != 0);

		
		
    }
    return 0;
}
