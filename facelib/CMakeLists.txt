cmake_minimum_required(VERSION 3.7)

project(facelib VERSION 1.0 LANGUAGES CXX)

set(CMAKE_AUTOMOC ON)

find_package(Qt5 COMPONENTS Core Network Gui REQUIRED)

add_library(${PROJECT_NAME} SHARED
            include/facelib/facexport.h
            include/facelib/faceobject.h
            include/facelib/nodedata.h
            include/facelib/nodeitem.h
            include/facelib/nodemodel.h
            include/facelib/worker.h
            faceobject.cpp
            nodedata.cpp
            nodeitem.cpp
            nodemodel.cpp
            worker.cpp)

if(WIN32)
    target_compile_definitions(${PROJECT_NAME} PRIVATE FACELIB)
endif()

target_include_directories(${PROJECT_NAME} PUBLIC ./include PRIVATE ./)
target_link_libraries(${PROJECT_NAME} PUBLIC Qt5::Core Qt5::Network Qt5::Gui)

INSTALL(TARGETS ${PROJECT_NAME} DESTINATION /usr/lib)

set(CPACK_GENERATOR "DEB")
set(CPACK_PACKAGE_VERSION_MAJOR "1")
set(CPACK_PACKAGE_VERSION_MINOR "0")
set(CPACK_PACKAGE_VERSION_PATCH "0")
set(CPACK_DEBIAN_PACKAGE_DEPENDS "openssl")
set(CPACK_PACKAGE_DESCRIPTION "facelib")
set(CPACK_PACKAGE_CONTACT "Unknown maintainer")

include(CPack)
