#pragma once

#include "BlueToothDevice.h"

Enumeration::DeviceWatcher BlueToothDevice::m_DeviceWatcher = Enumeration::DeviceInformation::CreateWatcher(Bluetooth::Rfcomm::RfcommDeviceService::GetDeviceSelector(Bluetooth::Rfcomm::RfcommServiceId::SerialPort()));

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
		if (m_DeviceWatcher.Status() == Enumeration::DeviceWatcherStatus::Created) {
			m_DeviceWatcher.Start();
		}

		if (m_DeviceWatcher.Status() == Enumeration::DeviceWatcherStatus::Started) {
		}
		else {
			break;
		}
	}

	if (m_DevicesFound > 0) {
		std::cout << "Has found" << m_DevicesFound << " devices." << std::endl;
	}
	else {
		std::cout << "Didnt find any required devices" << std::endl;
	}
}

void BlueToothDevice::connect(hstring DeviceId)
{
	if (!m_RfcommDeviceService.get()) {
		m_RfcommDeviceService = std::make_shared<RfcommDeviceService>(RfcommDeviceService::FromIdAsync(DeviceId).get());
	}
	else {
		std::wcout << "A BlueTooth Has already been connected to. ID: " << m_RfcommDeviceService->ConnectionHostName().DisplayName().c_str() << std::endl;
	}
}

void BlueToothDevice::disconnect()
{
	m_RfcommDeviceService = nullptr;
}

bool BlueToothDevice::send(std::shared_ptr<void> payload)
{
	auto envoyer = DataWriter(m_StreamSocket.OutputStream());
	envoyer.UnicodeEncoding(UnicodeEncoding::Utf8);
	envoyer.ByteOrder(ByteOrder::BigEndian);

	//CAUSTION: if send block, throw logic_error bluetooth device malfunction
	return false;
}

std::ostream & BlueToothDevice::operator<<(std::ostream & os)
{
	auto BtDevice = m_RfcommDeviceService->Device();
	// TODO: insert return statement here
	return os;
}

BlueToothDevice::BlueToothDevice()
{
	if (m_DeviceWatcher.Status() == Enumeration::DeviceWatcherStatus::Created) {
		m_DeviceWatcher.Added([&](auto &&, Enumeration::DeviceInformation temp) {
			std::wcout << "Device Name: " << temp.Name().c_str() << std::endl
				<< "Device ID: " << temp.Id().c_str() << std::endl;

			//TODO: Select specified property
			param::async_iterable<param::hstring> additionalProperties(IIterable<param::hstring>(L"System.InterfaceClassGuid"));
			auto di = Enumeration::DeviceInformation::CreateFromIdAsync(temp.Id(), additionalProperties);
			//

			m_DeviceName = temp.Id();

			std::cout << "Added happened." << std::endl;

		});

		m_DeviceWatcher.Updated([&](auto &&, Enumeration::DeviceInformationUpdate temp) {
			std::cout << "Updated Happened." << std::endl;
			m_DeviceName = temp.Id().c_str();
			std::wcout << "Updated ID: " << m_DeviceName << std::endl;
		});

		m_DeviceWatcher.EnumerationCompleted([=](auto &&, auto &&) {
			std::cout << "Enum Completed" << std::endl;
			m_DeviceWatcher.Stop();
		});
	}
	else {
		throw std::logic_error("Bluetooth Watcher failed to initialize");
	}
}
