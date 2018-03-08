cmake_minimum_required(VERSION 3.0)
project(mylittlecar)
include_directories(
		"C:\\Users\\svandex\\Documents\\GitHub\\cppwinrt\\10.0.16299.0\\"
		"C:\\Program Files (x86)\\Windows Kits\\10\\Include\\10.0.16299.0\\"
		"C:\\Program Files (x86)\\Microsoft Visual Studio\\2017\\BuildTools\\VC\\Tools\\MSVC\\14.12.25827r\\include\\"
		"C:\\Users\\svandex\\Documents\\GitHub\\remote-wiring\\source\\Firmata\\"
		"C:\\Users\\svandex\\Documents\\GitHub\\vcpkg\\installed\\x64-windows\\include\\"
		)

link_directories("C:/Program Files (x86)/Windows Kits/10/Lib/10.0.16299.0/ucrt/x64/"
		"C:/Program Files (x86)/Windows Kits/10/Lib/10.0.16299.0/um/x64/"
		"C:/Users/svandex/Documents/GitHub/vcpkg/installed/x64-windows/lib/"
		)

set(CMAKE_CXX_FLAGS "/EHsc /std:c++latest /await")
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

#add the executable
add_executable(mlc main.cpp device.cpp)
target_link_libraries(mlc WindowsApp.lib)# cpprest_2_10.lib)

