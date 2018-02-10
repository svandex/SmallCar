#pragma once

#include <pigpio.h>

class SmallCarGPIO
{
public:
	SmallCarGPIO();
	SmallCarGPIO(SmallCarGPIO &) = delete; //cannot copy gpio instance, only one exists
	~SmallCarGPIO();
	void ledFlash(const int &ledPinNum);
private:
	static int numOfInstance;//no more than one instance
};

