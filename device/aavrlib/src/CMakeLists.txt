# cmake
#cmake_minimum_required(VERSION 2.8)

add_subdirectory(uc)
add_subdirectory(peripherals)


set(util_source_files time.c systick.c softPWM_short.c softPWM_long.c)
add_library(util_objects OBJECT ${util_source_files})
