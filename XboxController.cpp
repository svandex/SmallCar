#include"XboxController.h"

std::shared_ptr<Gamepad> XboxController::m_XboxController;

std::shared_ptr<Gamepad> XboxController::Instance()
{
	if (!m_XboxController.get()) {
		auto temp = std::shared_ptr<XboxController>(new XboxController());
		m_XboxController = std::make_shared<Gamepad>(Gamepad::Gamepads().GetAt(0));// default to get the first controller
		return m_XboxController;
	}
	else {
		return m_XboxController;
	}
	return nullptr;
}

void XboxController::showBattery()
{
	if (m_XboxController.get()) {
		auto myGamepad = *m_XboxController;
		auto percentageBattery = (double)myGamepad.TryGetBatteryReport().RemainingCapacityInMilliwattHours().Value() /
			(double)myGamepad.TryGetBatteryReport().FullChargeCapacityInMilliwattHours().Value();
		std::cout << "Battery Report, remaining " << percentageBattery * 100 << " % ." << std::endl
			<< "isWireless: " << myGamepad.IsWireless() << std::endl;
		std::wcout << "User: " << myGamepad.User().NonRoamableId().c_str() << std::endl;
	}
	else {
		std::cout << "no controller connected, no battery report" << std::endl;
	}
}

XboxController::XboxController() {
	Gamepad::GamepadAdded([=](auto &&, Gamepad newAdded) {
		// add lock to prevent routines write to std::cout simutaneously
		//std::cout << "isWireless :" << newAdded.IsWireless() << std::endl;
		std::cout << "Gamepad Added!" << std::endl;
	});

	Gamepad::GamepadRemoved([&](auto &&, auto &&) {
		std::cout << "Gamepad Removed!" << std::endl;
		isConnected = false;
		throw std::logic_error("Controller Disconnected.");
	});

	auto listGamepads = Gamepad::Gamepads();
	uint8_t count = 0;
	while (listGamepads.Size() == 0) {
		listGamepads = Gamepad::Gamepads();
		if (listGamepads.Size() > 0) {
			std::cout << std::endl;
			std::cout << listGamepads.Size() << " gamepad has been found!" << std::endl;
			break;
		}
		//set 10 seconds for connection, or throw excpetion
		std::this_thread::sleep_for(std::chrono::seconds(1));
		std::cout << "-";
		if (count++ > 10) {
			std::cout << std::endl;
			throw std::logic_error("Didnt find any controller");
			break;
		}
	}
}