cmake_minimum_required(VERSION 3.0)
project(bc)
include_directories(
		"C:/Users/svandex/Documents/GitHub/cppwinrt/10.0.16299.0/"
		"C:/Program Files (x86)/Windows Kits/10/Include/10.0.16299.0/"
		"C:/Users/svandex/Documents/GitHub/remote-wiring/source/Firmata/"
		"C:/Users/svandex/Documents/GitHub/vcpkg/installed/x64-windows/include/"
		)

link_directories("C:/Program Files (x86)/Windows Kits/10/Lib/10.0.16299.0/ucrt/x64/"
		"C:/Program Files (x86)/Windows Kits/10/Lib/10.0.16299.0/um/x64/"
		"C:/Users/svandex/Documents/GitHub/vcpkg/installed/x64-windows/lib"
		)

set(CMAKE_CXX_FLAGS "/EHsc /std:c++latest /await")

#add the executable
add_executable(bc main.cpp device.cpp)
target_link_libraries(bc WindowsApp.lib)# cpprest_2_10.lib)

