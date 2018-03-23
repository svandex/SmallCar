#pragma once

#include "stdafx.h"

#include <memory>

class BlueToothDevice {//Singleton Pattern, only one instance exist
public:
	~BlueToothDevice() {};
	static BlueToothDevice* Instance();

	//data member
	static std::shared_ptr<BlueToothDevice> m_pInstance;
protected:
	BlueToothDevice();

	//Rfcomm device selector
	static auto rfselector = Bluetooth::Rfcomm::RfcommDeviceService::GetDeviceSelector(Bluetooth::Rfcomm::RfcommServiceId::SerialPort());
	static auto cw = Enumeration::DeviceInformation::CreateWatcher(rfselector);
};
