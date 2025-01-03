cmake_minimum_required(VERSION 3.10)
project(FrodoKEM)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED True)

find_package(OpenSSL REQUIRED)

add_executable(FrodoKEM
        main.cpp
        FrodoSupport.cpp
        FrodoRandom.cpp
        SHAKE.cpp
        AES.cpp
        FrodoPke.cpp
        FrodoPke.h
)

target_link_libraries(FrodoKEM PRIVATE OpenSSL::SSL OpenSSL::Crypto)

if(OPENSSL_FOUND)
  message(STATUS "Found OpenSSL: ${OPENSSL_VERSION_TEXT}")
else()
  message(FATAL_ERROR "OpenSSL not found!")
endif()
