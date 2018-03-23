cmake_minimum_required(VERSION 3.0)
project(mylittlecar)
include_directories(
		"C:\\Users\\svandex\\Documents\\GitHub\\remote-wiring\\source\\Firmata\\"
		"C:\\Users\\svandex\\Documents\\GitHub\\vcpkg\\installed\\x64-windows\\include\\"
		)

set(CMAKE_CXX_FLAGS "/EHsc /std:c++latest /await")
#set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

#add the executable
add_executable(mlc main.cpp device.cpp)
target_link_libraries(mlc WindowsApp.lib)# cpprest_2_10.lib)

