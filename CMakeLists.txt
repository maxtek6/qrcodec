cmake_minimum_required(VERSION 3.5)

project(qrcodec)

file(
    GLOB 
    VENDOR_SOURCES 
    ${CMAKE_CURRENT_SOURCE_DIR}/vendor/QR-Code-generator/c/qrcodegen.c 
    ${CMAKE_CURRENT_SOURCE_DIR}/vendor/QR-Code-generator/c/qrcodegen.h
    ${CMAKE_CURRENT_SOURCE_DIR}/vendor/quirc/lib/*.c
    ${CMAKE_CURRENT_SOURCE_DIR}/vendor/quirc/lib/*.h)

add_library(qrcodec STATIC qrcodec.c ${VENDOR_SOURCES})

target_include_directories(
    qrcodec 
    PUBLIC ${CMAKE_CURRENT_SOURCE_DIR} 
    PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/vendor/QR-Code-generator/c ${CMAKE_CURRENT_SOURCE_DIR}/vendor/quirc/lib)

add_subdirectory(example)