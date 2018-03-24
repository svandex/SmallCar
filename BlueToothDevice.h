#pragma once

#include <winrt\Windows.Devices.Bluetooth.Rfcomm.h>
#include <winrt\Windows.Devices.Enumeration.h>
#include <memory>
#include <iostream>
#include <stdexcept>

using namespace winrt;
using namespace Windows::Devices;

class BlueToothDevice {//Singleton Pattern, only one instance exist
public:
	~BlueToothDevice() {};
	static BlueToothDevice* Instance();
	void on_initialise();
protected:
	static std::shared_ptr<BlueToothDevice> m_pInstance;
private:
	//create watcher
	static Enumeration::DeviceWatcher cw;

	//data member
	std::wstring tdi=L"";// Bluetooth Device you want to connect to
	uint32_t devicesFound = 0;

	//private ctor
	BlueToothDevice();
};
