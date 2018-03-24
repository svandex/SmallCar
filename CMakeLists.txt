cmake_minimum_required(VERSION 3.0)
project(mylittlecar)

set(CMAKE_SYSTEM_NAME WindowsStore)
set(CMAKE_SYSTEM_VERSION 10.0)
set(CMAKE_TOOLCHAIN_FILE "C:/Users/svandex/Documents/GitHub/vcpkg/scripts/buildsystems/vcpkg.cmake")
set(CMAKE_CXX_FLAGS "/EHsc /std:c++latest /await")

include_directories("C:/Users/svandex/Documents/GitHub/vcpkg/installed/x64-windows/include/")
#set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

#add the executable
add_executable(mlc main.cpp BlueToothDevice.cpp XboxController.cpp)
target_link_libraries(mlc WindowsApp.lib)

