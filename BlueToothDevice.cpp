#pragma once

#include "BlueToothDevice.h"

Enumeration::DeviceWatcher BlueToothDevice::cw = Enumeration::DeviceInformation::CreateWatcher(Bluetooth::Rfcomm::RfcommDeviceService::GetDeviceSelector(Bluetooth::Rfcomm::RfcommServiceId::SerialPort()));

std::shared_ptr<BlueToothDevice> BlueToothDevice::Instance()
{
	if (!m_pInstance.get()) {
		m_pInstance = std::shared_ptr<BlueToothDevice>(new BlueToothDevice());
		return m_pInstance;
	}
	else {
		return m_pInstance;
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

void BlueToothDevice::connect(hstring DeviceId)
{
	if (!tDevice.get()) {
		tDevice = std::make_shared<RfcommDeviceService>(RfcommDeviceService::FromIdAsync(DeviceId).get());
	}
	else {
		std::wcout << "A BlueTooth Has already been connected to. ID: " << tDevice->ConnectionHostName().DisplayName().c_str() << std::endl;
	}
}

void BlueToothDevice::disconnect()
{
	tDevice = nullptr;
}

bool BlueToothDevice::send(std::shared_ptr<void> payload)
{
	auto envoyer = DataWriter(btSS.OutputStream());
	envoyer.UnicodeEncoding(UnicodeEncoding::Utf8);
	envoyer.ByteOrder(ByteOrder::BigEndian);

	//CAUSTION: if send block, throw logic_error bluetooth device malfunction
	return false;
}

BlueToothDevice::BlueToothDevice()
{
	if (cw.Status() == Enumeration::DeviceWatcherStatus::Created) {
		cw.Added([&](auto &&, Enumeration::DeviceInformation temp) {
			std::wcout << "Device Name: " << temp.Name().c_str() << std::endl
				<< "Device ID: " << temp.Id().c_str() << std::endl;

			//TODO: Select specified property
			param::async_iterable<param::hstring> additionalProperties(IIterable<param::hstring>(L"System.InterfaceClassGuid"));
			auto di = Enumeration::DeviceInformation::CreateFromIdAsync(temp.Id(), additionalProperties);
			//

			tdi = temp.Id();

			std::cout << "Added happened." << std::endl;

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
