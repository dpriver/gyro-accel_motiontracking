#cmake try...

cmake_minimum_required(VERSION 2.8)

set(CMAKE_TOOLCHAIN_FILE cmake/toolchain/Toolchain-avr.cmake)

set(CMAKE_PREFIX_PATH /usr)
set(CMAKE_FIND_LIBRARY_PREFIXES lib)
set(CMAKE_FIND_LIBRARY_SUFFIXES .a)
set(CMAKE_CROSSCOMPILING TRUE)

set(LINKER_SCRIPT "${CMAKE_CURRENT_SOURCE_DIR}/ldscript.ld")

set(HEADERS_INSTALL_PREFIX "${CMAKE_INSTALL_PREFIX}/include/libaavr")

project(aavrlib C ASM)

set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/bin/lib")
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/bin/lin")
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/bin/test")

SET(AVRLIBC_FILES c m)

foreach(FILE ${AVRLIBC_FILES})
	find_library(LIBTEMP ${FILE} HINTS /usr/lib/avr/lib /usr/local/cellar/avr-libc/1.8.1/avr/lib REQUIRED)
	if(NOT LIBTEMP)
		message("-- ERROR: No se ha encontrado la librería avrlibc ${FILE}")
		return()
	endif()
	set(AVRLIBC "${AVRLIBC} ${LIBTEMP}")
endforeach()
message("-- AVR-libc library found")


include_directories( /usr/lib/avr/include include)

# compile library
add_subdirectory( src )
add_library(aavr STATIC $<TARGET_OBJECTS:uc_objects> $<TARGET_OBJECTS:peripherals_objects>
			$<TARGET_OBJECTS:util_objects>)

# compile tests
add_subdirectory( test )


install(TARGETS aavr ARCHIVE DESTINATION ${CMAKE_INSTALL_PREFIX}/lib)
install(DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/include/
		DESTINATION ${HEADERS_INSTALL_PREFIX}
		PATTERN "${CMAKE_CURRENT_SOURCE_DIR}/include/*")

# uninstall target
configure_file(
    "${CMAKE_CURRENT_SOURCE_DIR}/cmake/uninstall.cmake.in"
    "${CMAKE_CURRENT_BINARY_DIR}/cmake/uninstall.cmake"
    IMMEDIATE @ONLY)

add_custom_target(uninstall
    COMMAND ${CMAKE_COMMAND} -P ${CMAKE_CURRENT_BINARY_DIR}/cmake/uninstall.cmake)
