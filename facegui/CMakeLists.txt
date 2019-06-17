cmake_minimum_required(VERSION 3.7)

project(facegui VERSION 1.0)

set(CMAKE_AUTOMOC ON)

find_package(Qt5 COMPONENTS Widgets REQUIRED)

add_executable(${PROJECT_NAME}
               facegui.cpp
               facegui.h
               filedialog.cpp
               filedialog.h
               graphicsview.cpp
               graphicsview.h
               main.cpp)

target_link_libraries(${PROJECT_NAME} facelib)
target_link_libraries(${PROJECT_NAME} Qt5::Widgets)

INSTALL(TARGETS ${PROJECT_NAME} DESTINATION /usr/bin)

set(CPACK_GENERATOR "DEB")
set(CPACK_PACKAGE_VERSION_MAJOR "1")
set(CPACK_PACKAGE_VERSION_MINOR "0")
set(CPACK_PACKAGE_VERSION_PATCH "0")
set(CPACK_DEBIAN_PACKAGE_DEPENDS "qt5-default,openssl")
set(CPACK_PACKAGE_DESCRIPTION "facegui")
set(CPACK_PACKAGE_CONTACT "Unknown maintainer")

include(CPack)
