#pragma once

#include "device.h"

namespace juncheng {
	class XboxController {
	public:
		XboxController();
		Gamepad mygamepad;
	private:
		bool isConnected;
	};
}