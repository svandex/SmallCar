#pragma once

#include<winrt/windows.devices.Enumeration.h>
#include<winrt/Windows.Devices.Bluetooth.Rfcomm.h>
#include<winrt/Windows.Devices.SerialCommunication.h>
#include<winrt/Windows.Devices.Power.h>

#include<winrt/Windows.Storage.Streams.h>
#include<winrt/Windows.Networking.Sockets.h>

//keyboard event
#include<winrt/Windows.UI.Core.h>
#include<winrt/Windows.System.h>

#include<winrt/Windows.Gaming.Input.h>
//Foundation Namespace
#include<winrt/Windows.Foundation.h>
#include<winrt/Windows.Foundation.Collections.h>

//Arduino
/*
#include "UwpFirmata.h"
#include "RemoteDevice.h"
#include "TwoWire.h"
*/

//other
#include<experimental/resumable>
#include<pplawait.h>
#include<iostream>
#include<vector>

//openFrameworks
//#include<ofArduino.h>

//cpprest
#include<cpprest/http_client.h>
#include<cpprest/filestream.h>

using namespace winrt;
using namespace winrt::Windows::Devices;
using namespace winrt::Windows::Devices::SerialCommunication;
using namespace winrt::Windows::Devices::Bluetooth::Rfcomm;
using namespace winrt::Windows::Devices::Power;

using namespace winrt::Windows::UI::Core;

using namespace winrt::Windows::Storage::Streams;
using namespace winrt::Windows::Networking::Sockets;

using namespace winrt::Windows::Foundation;
using namespace winrt::Windows::Foundation::Collections;
using namespace winrt::Windows::Gaming::Input;
using namespace std::experimental;

//cpprest
using namespace utility;
using namespace web;
using namespace web::http::client;
using namespace web::http;
using namespace concurrency::streams;

namespace juncheng {
	IAsyncAction bySerialPort();
	IAsyncAction byBlueTooth();
	IAsyncAction uGamepad();
	IAsyncAction uRESTtest();
	IAsyncOperation<bool> menuInitialize();

	static bool bluetoothIsConnected = false;
	static bool xboxcontrollerIsConnected = false;
}