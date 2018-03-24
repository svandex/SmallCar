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
	}
	catch (std::exception e) {
		std::cout << e.what() << std::endl;
	}

	system("pause");
	return 1;
}

