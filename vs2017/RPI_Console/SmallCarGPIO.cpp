#include "SmallCarGPIO.h"
#include <iostream>

int SmallCarGPIO::numOfInstance = 0;

SmallCarGPIO::SmallCarGPIO()
{
	if (numOfInstance++ > 1) {
		std::cout << "Only One Instance should be exist.";
		return;
	}
	gpioInitialise();
}


SmallCarGPIO::~SmallCarGPIO()
{
	gpioTerminate();
}

void SmallCarGPIO::ledFlash(const int &ledPinNum)
{
	gpioSetMode(ledPinNum, PI_OUTPUT);

	for (int i = 0; i < 3; i++) {
		gpioWrite(ledPinNum, PI_HIGH);
		gpioDelay(1000000);
		gpioWrite(ledPinNum, PI_LOW);
		gpioDelay(1000000);
	}
}
