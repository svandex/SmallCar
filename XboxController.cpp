#include"XboxController.h"

juncheng::XboxController() {
	isConnected = false;

	Gamepad::GamepadAdded([=](auto &&, Gamepad newAdded) {
		// add lock to prevent routines write to std::cout simutaneously
		//std::cout << "isWireless :" << newAdded.IsWireless() << std::endl;
		std::cout << "Gamepad Added!" << std::endl;
	});

	Gamepad::GamepadRemoved([=, &isConnected](auto &&, auto &&) {
		std::cout << "Gamepad Removed!" << std::endl;
		isConnected = false;
	});

	auto listGamepads = Gamepad::Gamepads();
	while (listGamepads.Size() == 0) {
		listGamepads = Gamepad::Gamepads();
		if (listGamepads.Size() > 0) {
			std::cout << listGamepads.Size() << " gamepad has been found!" << std::endl;

			isConnected = true;
			break;
		}
	}

	mygamepad = listGamepads
}