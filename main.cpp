#include "device.h"

using namespace winrt;
using namespace std::experimental;

int main() {
	init_apartment();
	//bySerialPort().get();
	byBlueTooth().get(); 
//	uGamepad().get();
	//uRESTtest().get();

	system("pause");
	return 1;
}

