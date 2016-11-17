# uc library files
#cmake_minimum_required(VERSION 2.8)

set(uc_source_files interrupt.c analog.c spi.c twi.c system.c timers.c usart.c)

add_library(uc_objects OBJECT ${uc_source_files})
