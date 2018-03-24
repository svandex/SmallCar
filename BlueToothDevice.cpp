#pragma once

#include "BlueToothDevice.h"

std::shared_ptr<BlueToothDevice> BlueToothDevice::m_pInstance;

Enumeration::DeviceWatcher BlueToothDevice::cw = Enumeration::DeviceInformation::CreateWatcher(Bluetooth::Rfcomm::RfcommDeviceService::GetDeviceSelector(Bluetooth::Rfcomm::RfcommServiceId::SerialPort()));

BlueToothDevice * BlueToothDevice::Instance()
{
	if (!m_pInstance.get()) {
		m_pInstance = std::shared_ptr<BlueToothDevice>(new BlueToothDevice());
		return m_pInstance.get();
	}
	else {
		return BlueToothDevice::m_pInstance.get();
	}
}

void BlueToothDevice::on_initialise()
{
	while (true) {
		if (cw.Status() == Enumeration::DeviceWatcherStatus::Created) {
			cw.Start();
		}

		if (cw.Status() == Enumeration::DeviceWatcherStatus::Started) {
		}
		else {
			break;
		}
	}

	if (devicesFound > 0) {
		std::cout << "Has found" << devicesFound << " devices." << std::endl;
	}
	else {
		std::cout << "Didnt find any required devices" << std::endl;
	}
}

BlueToothDevice::BlueToothDevice()
{
	if (cw.Status() == Enumeration::DeviceWatcherStatus::Created) {
		cw.Added([&](auto &&, Enumeration::DeviceInformation temp) {
			std::wcout << "Device Name: " << temp.Name().c_str() << std::endl
				<< "Device ID: " << temp.Id().c_str() << std::endl;

			//<< "isPaired: " << temp.Pairing().IsPaired() << std::endl;
		//isPaired function doesnt' work.
			std::cout << "Added happened." << std::endl;

			//	if (wcscmp(temp.Name().c_str(), BBT_TARGETNAME) == 0) {// find device by NAME!! Optimized !
			/*
			std::wcout << "Result: " << std::wcsstr(temp.Id().c_str(), bthaddr_1) << std::endl;
			if (std::wcsstr(temp.Id().c_str(), bthaddr_1)) {
				devicesFound++;
				tdi = temp.Id().c_str();
			}*/
		});

		cw.Updated([&](auto &&, Enumeration::DeviceInformationUpdate temp) {
			std::cout << "Updated Happened." << std::endl;
			tdi = temp.Id().c_str();
			std::wcout << "Updated ID: " << tdi << std::endl;
		});

		cw.EnumerationCompleted([=](auto &&, auto &&) {
			std::cout << "Enum Completed" << std::endl;
			cw.Stop();
		});
	}
	else {
		throw std::logic_error("Bluetooth Watcher failed to initialize");
	}
}
