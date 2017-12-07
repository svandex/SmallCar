#include "device.h"

const wchar_t *BSP_TARGETNAME{ L"BT-RIG01" };
//const wchar_t *BSP_TARGETNAME{ L"HC-05" };
const wchar_t *BBT_TARGETNAME{ L"Dev B" };

void coWriteAndReadWithOneByte(uint16_t x, const DataWriter & dw, const DataReader & dr) {
	dw.WriteInt16(x);
	//Do not exist number of 8-bit, take care local buffer.
	//Arduino only take 8-bit from buffer while a 16-bit int has been pushed into buffer.

	dw.StoreAsync().get();

	dr.LoadAsync(1).get();

	dr.ReadByte();
	std::cout << "Unconsumed Buffer Length: " << dr.UnconsumedBufferLength() << ", x is " << x << std::endl;
	if (dr.UnconsumedBufferLength() > 0) {
		std::cout << "readFrom: " << (int)dr.ReadByte() << std::endl;
	}
}

IAsyncAction bySerialPort() {
	auto selector = SerialDevice::GetDeviceSelector(L"COM5");//AQS string

	auto result = co_await Enumeration::DeviceInformation::FindAllAsync(selector);

	//see how many devices has been found
	auto mdl = result.Size();
	std::cout << "Has found " << mdl << " devices" << std::endl;

	if (mdl == 0) {
		std::cout << "Didn't find any required devices." << std::endl;
		co_return;
	}

	uint8_t isPairedDevices = 0;
	uint32_t tIndex = -1;
	for (uint32_t i = 0; i < mdl; i++) {
		auto temp = result.GetAt(i);
		std::wcout << "Device " << i << " : " << temp.Id().c_str() << std::endl
			<< temp.Name().c_str()
			<< std::endl;
		if (temp.Pairing().IsPaired() == true) {
			std::wcout << temp.Name().c_str() << " is paired." << std::endl;
			isPairedDevices++;
			if (wcscmp(temp.Name().c_str(), BSP_TARGETNAME) == 0) {
				tIndex = i;
			}
		}
	}

	if (isPairedDevices == 0) { std::cout << "No Device Has Ever Paired Before !" << std::endl; co_return; }
	if (tIndex == -1) { std::cout << "No TARGETNAME device" << std::endl; co_return; }
	//connect to the bluetooth device
	std::cout << "connecting......" << std::endl << std::endl;
	auto targetDevice = co_await SerialDevice::FromIdAsync(result.GetAt(tIndex).Id());

	//Check if target device has been connected.
	auto connectStatus = Enumeration::DeviceAccessInformation::CreateFromId(result.GetAt(tIndex).Id());
	if (connectStatus.CurrentStatus() == Enumeration::DeviceAccessStatus::Allowed) {
		std::cout << "connected!" << std::endl;
	}
	else {
		std::cout << "device is not connected!" << std::endl;
		co_return;
	}


	std::wcout << "PortName : " << targetDevice.PortName().c_str() << std::endl;
	std::cout << "UsbProductID : " << targetDevice.UsbProductId() << std::endl;

	//serial device setting
	targetDevice.BaudRate(115200);
	targetDevice.DataBits(8);
	targetDevice.StopBits(SerialCommunication::SerialStopBitCount::One);
	targetDevice.Parity(SerialCommunication::SerialParity::None);
	targetDevice.Handshake(SerialCommunication::SerialHandshake::None);

	//Device OutputStream
	auto outs = targetDevice.OutputStream();

	std::cout << "ReadTImeout Count: " << targetDevice.WriteTimeout().count() << std::endl;
	std::cout << "ClearToSendState: " << targetDevice.ClearToSendState() << std::endl;

	//DataWriter
	auto outdw = DataWriter(outs);

	outdw.UnicodeEncoding(UnicodeEncoding::Utf8);
	outdw.ByteOrder(ByteOrder::BigEndian);

	int16_t x;
	while (std::cin >> x) {
		if (x == -1) {
			/*outdw.WriteByte('0');
			outdw.WriteByte('0');
			co_await outdw.StoreAsync();
			*/break;
		}
		//.WriteByte('0');
		//outdw.WriteByte();
		outdw.WriteInt16(x);
		co_await outdw.StoreAsync();
	}
}

IAsyncAction byBlueTooth() {
	//Bluetooth device selector
	auto bdselector = Bluetooth::BluetoothDevice::GetDeviceSelector();
	//Rfcomm device selector
	auto rfselector = Bluetooth::Rfcomm::RfcommDeviceService::GetDeviceSelector(
		Bluetooth::Rfcomm::RfcommServiceId::SerialPort());

	//DeviceWatcher to add each device
	auto cw = Enumeration::DeviceInformation::CreateWatcher(rfselector);

	uint32_t devicesFound = 0;
	std::shared_ptr<uint32_t> deviceAdded = std::make_shared<uint32_t>(devicesFound);
	std::wstring tdi;
	while (true) {
		//before start() called, only go through this branch
		if (cw.Status() == Enumeration::DeviceWatcherStatus::Created) {
			cw.Added([=, &tdi](auto &&, Enumeration::DeviceInformation temp) {
				//std::wcout << "Added Device: " << temp.Name().c_str() << ", " << temp.Id().c_str() << std::endl;
				std::cout << "Added happened." << std::endl;
				
				if (wcscmp(temp.Name().c_str(), BBT_TARGETNAME) == 0) {// find device by NAME!! Optimized !
					tdi = temp.Id().c_str();
					(*deviceAdded)++;
				}
			});

			cw.Updated([=, &tdi](auto &&, Enumeration::DeviceInformationUpdate temp) {
				std::cout << "Updated Happened." << std::endl;
				tdi = temp.Id().c_str();
				std::wcout << "Updated ID: " << tdi << std::endl;
			});

			cw.EnumerationCompleted([=](auto &&, auto &&) {
				std::cout << "Enum Completed" << std::endl;
				cw.Stop();
			});

			cw.Start();
		}

		if (cw.Status() == Enumeration::DeviceWatcherStatus::Started) {

		}
		else {
			break;
		}
	}

	//see how many devices has been found
	std::cout << "Has found " << *deviceAdded << " devices" << std::endl;

	if (*deviceAdded == 0) {
		std::cout << "Didn't find any required devices." << std::endl;
		co_return;
	}

	//RfcommDeviceService aqusition by useing bluetooth device instanc
	//auto targetDevice = co_await Bluetooth::BluetoothDevice::FromIdAsync(tdi);
	auto target = co_await Bluetooth::Rfcomm::RfcommDeviceService::FromIdAsync(tdi);

	//std::cout << std::endl << "Connected Device Information: " << std::endl;
	//StreamSocket and DataWriter to read/write data
	StreamSocket ss2bt;
	//ss2bt.Control().KeepAlive(true);

	ss2bt.ConnectAsync(target.ConnectionHostName(), target.ConnectionServiceName()).get();

	//Stream Setting
	auto outs = ss2bt.OutputStream();
	auto ins = ss2bt.InputStream();

	auto outdw = DataWriter(outs);
	auto indr = DataReader(ins);

	outdw.UnicodeEncoding(UnicodeEncoding::Utf8);
	outdw.ByteOrder(ByteOrder::BigEndian);

	indr.UnicodeEncoding(UnicodeEncoding::Utf8);
	indr.ByteOrder(ByteOrder::BigEndian);

	//Read/Write
	int16_t x = 1;
	uint32_t y = 2;
	//char x;
	//byte x;
	while (std::cin >> x) {
		if (x == 512) {
			break;
		}

		coWriteAndReadWithOneByte(x, outdw, indr);
	}

	co_return;
}

IAsyncAction uGamepad() {
	bool isConnected = false;
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

	auto myGamepad = listGamepads.GetAt(0);
	GamepadReading oldreading = myGamepad.GetCurrentReading();
	GamepadReading newreading = myGamepad.GetCurrentReading();

	auto percentageBattery = (double)myGamepad.TryGetBatteryReport().RemainingCapacityInMilliwattHours().Value() /
		(double)myGamepad.TryGetBatteryReport().FullChargeCapacityInMilliwattHours().Value();
	std::cout << "Battery Report, remaining " << percentageBattery * 100 << " % ." << std::endl
		<< "isWireless: " << myGamepad.IsWireless() << std::endl;
	std::wcout << "User: " << myGamepad.User().NonRoamableId().c_str() << std::endl;

	while (isConnected) {
		oldreading = newreading;
		newreading = myGamepad.GetCurrentReading();

		if (newreading.LeftTrigger > 0) { std::cout << "Left Trigger: " << newreading.LeftTrigger << std::endl; }

		//std::cout << "Right Trigger: " << newreading.RightTrigger << std::endl;
		/*
		std::cout << "Left Thumb Sticker X: " << newreading.LeftThumbstickX << std::endl;
		std::cout << "Left Thumb Sticker X: " << newreading.LeftThumbstickX << std::endl;
		*/
		if (GamepadButtons::A == (newreading.Buttons & GamepadButtons::A) &&
			GamepadButtons::None == (oldreading.Buttons & GamepadButtons::A)) {
			std::cout << "A is pressed." << std::endl;
		}

		if (GamepadButtons::None == (newreading.Buttons & GamepadButtons::A) &&
			GamepadButtons::A == (oldreading.Buttons & GamepadButtons::A)) {
			std::cout << "A is released." << std::endl;
		}

		/*
		if (GamepadButtons::A == (newreading.Buttons & GamepadButtons::A) &&
			GamepadButtons::A == (oldreading.Buttons & GamepadButtons::A)) {
			std::cout << "A is consitently pressed." << std::endl;
		}
		*/
	}

	co_return;
}

IAsyncAction uRESTtest() {
	auto fileStream = std::make_shared<ostream>();

	// Open stream to output file.
	pplx::task<void> requestTask = fstream::open_ostream(U("C:/Users/svandex/Desktop/results.html")).then([=](ostream outFile)
	{
		*fileStream = outFile;

		// Create http_client to send the request.
		http_client client(U("http://www.bing.com/"));

		// Build request URI and start the request.
		uri_builder builder(U("/search"));
		builder.append_query(U("q"), U("cpprestsdk github"));
		return client.request(methods::GET, builder.to_string());
	})

		// Handle response headers arriving.
		.then([=](http_response response)
	{
		printf("Received response status code:%u\n", response.status_code());

		// Write response body into the file.
		return response.body().read_to_end(fileStream->streambuf());
	})

		// Close the file stream.
		.then([=](size_t)
	{
		return fileStream->close();
	});

	// Wait for all the outstanding I/O to complete and handle any exceptions
	try
	{
		requestTask.wait();
	}
	catch (const std::exception &e)
	{
		printf("Error exception:%s\n", e.what());
	}

	return 0;

}
