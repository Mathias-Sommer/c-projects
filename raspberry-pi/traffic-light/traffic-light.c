#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

// MODES
#define OUTPUT (1)

// VALUES
#define LOW    (0)  // OFF
#define HIGH   (1)  // ON

typedef struct {
        int pin;
        char* fn;
} pin_t;


static pin_t open_pin(int pin, int mode);
static void close_pin(pin_t pin);
static void write_pin(pin_t pin, int value);


int main(int argc, char** argv){
	// Base counter for GPIO on current ubuntu server version is 512.
	// To check which are available for other distros:
	// ~$ ls /sys/class/gpio
	// Current is: export  gpiochip512  gpiochip570  unexport

	int base         = 512;

	// PINS
	int gpio_red     = 17 + base;
	int gpio_green   = 27 + base;
	int gpio_yellow  = 22 + base;
	
	// Export pins
	pin_t pin_red    = open_pin(gpio_red,    OUTPUT);
	pin_t pin_green  = open_pin(gpio_green,  OUTPUT);
	pin_t pin_yellow = open_pin(gpio_yellow, OUTPUT);

	// Convert argument to integer from running ./traffic-light n (where n represents number of times)
	int n = atoi(argv[1]);

	// Write pins; run full traffic light cycle n times
	printf("Running traffic light for %d cycles\n", n);
	for(int i = 0; i < n; i++){
		printf("Cycle %d of %d\n", i + 1, n);
		// FULL STOP (RED LIGHT)
		printf("Red\n");
		write_pin(pin_red, HIGH);
		sleep(10);

		// GET READY (RED + YELLOW LIGHT)
		printf("Red + Yellow\n");
		write_pin(pin_yellow, HIGH);
		sleep(2);

		// IT'S GREEN ... PUT THE PEDAL TO THE METAL (GREEN LIGHT)
		write_pin(pin_red, LOW);
		write_pin(pin_yellow, LOW);
		
		printf("Green\n");
		write_pin(pin_green, HIGH);
		sleep(10);

		// Prepare for a full stop (YELLOW LIGHT)
		write_pin(pin_green, LOW);
		
		printf("Yellow\n");
		write_pin(pin_yellow, HIGH);
		sleep(3);
		write_pin(pin_yellow, LOW);
	}
	
	// Close pins and de-allocate memory
	close_pin(pin_red);
	close_pin(pin_green);
	close_pin(pin_yellow);

	return 0;
}


pin_t open_pin(int pin, int mode){
	// Export pin to GPIO modules to "open" the pin. 
	FILE* pin_direction = NULL;
	FILE* gpio_export = fopen("/sys/class/gpio/export", "w");
	fprintf(gpio_export, "%d", pin);
	fclose(gpio_export);

	// Setting the direction to "output" and the value to pin.
	char* pin_value = malloc(1024);
	char  direction[1024];
	snprintf(direction, 1024, "/sys/class/gpio/gpio%d/direction", pin);
	snprintf(pin_value, 1024, "/sys/class/gpio/gpio%d/value",     pin);

	pin_direction = fopen(direction, "w");
	fprintf(pin_direction, "out");
	fclose(pin_direction);
	
	return (pin_t) { pin, pin_value };
}

void close_pin(pin_t pin){
	// Setting the pin in unexport to close the pin.
	FILE* gpio_unexport = fopen("/sys/class/gpio/unexport", "w");
	fprintf(gpio_unexport, "%d", pin.pin);
	fclose(gpio_unexport);
	free(pin.fn);
}

void write_pin(pin_t pin, int value){
	FILE* gpio_write = fopen(pin.fn, "w");

	if (value == LOW) {
		fprintf(gpio_write, "0");
	} else {
		fprintf(gpio_write, "1");
	}
	
	fclose(gpio_write);
}
