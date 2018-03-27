#pragma once

#include <winrt\Windows.System.h>
#include <winrt\Windows.Gaming.Input.h>
#include <winrt\Windows.Devices.Power.h>
#include <memory>
#include <iostream>
#include <thread>
#include <chrono>
#include <stdexcept>

using namespace winrt::Windows::Gaming::Input;
using namespace winrt::Windows::Devices::Power;

class XboxController {
public:
	static std::shared_ptr<Gamepad> Instance();
	static void showBattery();

	static bool isConnected = false;
protected:
	static std::shared_ptr<Gamepad> m_XboxController;
private:
	XboxController();
};