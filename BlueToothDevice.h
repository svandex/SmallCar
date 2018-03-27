#pragma once

#include <winrt\Windows.Devices.Bluetooth.Rfcomm.h>
#include <winrt\Windows.Devices.Enumeration.h>
#include <winrt\Windows.Foundation.Collections.h>

#include <winrt\Windows.Storage.Streams.h>
#include <winrt\Windows.Networking.Sockets.h>

#include <memory>
#include <iostream>
#include <stdexcept>

using namespace winrt;
using namespace Windows::Devices;
using namespace Windows::Foundation::Collections;
using namespace Windows::Devices::Bluetooth::Rfcomm;
using namespace Windows::Storage::Streams;
using namespace Windows::Networking::Sockets;

class BlueToothDevice {//Singleton Pattern, only one instance exist
public:
	~BlueToothDevice() {};
	static std::shared_ptr<BlueToothDevice> Instance();
	void on_initialise();
	void connect(hstring DeviceId);
	void disconnect();
	bool send(std::shared_ptr<void> payload);
	std::shared_ptr<void> receive();

	std::ostream& operator<< (std::ostream& os);
protected:
	static std::shared_ptr<BlueToothDevice> m_pInstance;
private:
	static Enumeration::DeviceWatcher m_DeviceWatcher;
	std::shared_ptr<RfcommDeviceService> m_RfcommDeviceService = nullptr;
	StreamSocket m_StreamSocket;

	//data member
	hstring m_DeviceName = L"";// Bluetooth Device you want to connect to
	uint32_t m_DevicesFound = 0;

	//private ctor
	BlueToothDevice();
};
