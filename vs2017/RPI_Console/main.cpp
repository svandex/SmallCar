#include <iostream>
#include <pigpio.h>

int main()
{
	if (gpioInitialise() < 0) {
		std::cout << "GPIO Initialisation Failed." << std::endl;
		return 1;
	}
	else {//Initialisaztion Success
		gpioSetMode(5, PI_INPUT);
		gpioSetMode(6, PI_INPUT);

		gpioWrite(5, PI_HIGH);
	}
	for (int i = 0; i <= 4; i++) {
		gpioWrite(6, PI_HIGH);
		gpioDelay(100000);
		gpioWrite(6, PI_LOW);
		gpioDelay(100000);
	}
	char c;
	std::cout << "Enter a key to end ..." << std::endl;
	std::cin >> c;
	gpioWrite(5, PI_LOW);
	gpioWrite(6, PI_LOW);
	gpioTerminate();
	return 0;
}