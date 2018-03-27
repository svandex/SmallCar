#include "BlueToothDevice.h"
#include "XboxController.h"

using namespace winrt;
using namespace std::experimental;

int main() {
	init_apartment();

	try {
		//get connected controller
		auto mygamepad = XboxController::Instance();
		XboxController::showBattery();

		//get bluetooth device
		auto mybluetoothdevice = BlueToothDevice::Instance();
		mybluetoothdevice->on_initialise();

		//initialise gamepad reading
		GamepadReading oldReading = mygamepad->GetCurrentReading();
		GamepadReading newReading = mygamepad->GetCurrentReading();

		//loop
		while (XboxController::isConnected) {
			oldReading = newReading;
			newReading = mygamepad->GetCurrentReading();
			//if newReading != oldReading, start a new thread to do response
			//make sure controller recognise your continous input
			//make sure loop in main thread not block
		}
	}
	//TWO SITUATIONS:
	//1. xbox controller disconnected
	//2. bluetooth device disconnected or communiation blocking
	catch (std::exception e) {
		std::cout << e.what() << std::endl;
	}

	system("pause");
	return 1;
}

