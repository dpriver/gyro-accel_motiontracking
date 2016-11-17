# uc library files
#cmake_minimum_required(VERSION 2.8)

set(peripherals_source_files HCSR04.c infrared.c 1602A.c MPU60X0.c)

add_library(peripherals_objects OBJECT ${peripherals_source_files})
