cmake_minimum_required(VERSION 3.7)

project(facecli VERSION 1.0)

set(CMAKE_AUTOMOC ON)

find_package(Qt5 COMPONENTS Core REQUIRED)

add_executable(${PROJECT_NAME}
               facecli.cpp
               facecli.h
               main.cpp)

target_link_libraries(${PROJECT_NAME} facelib)
target_link_libraries(${PROJECT_NAME} Qt5::Core)

INSTALL(TARGETS ${PROJECT_NAME} DESTINATION /usr/bin)

set(CPACK_GENERATOR "DEB")
set(CPACK_PACKAGE_VERSION_MAJOR "1")
set(CPACK_PACKAGE_VERSION_MINOR "0")
set(CPACK_PACKAGE_VERSION_PATCH "0")
set(CPACK_DEBIAN_PACKAGE_DEPENDS "openssl")
set(CPACK_PACKAGE_DESCRIPTION "facecli")
set(CPACK_PACKAGE_CONTACT "Unknown maintainer")

include(CPack)
