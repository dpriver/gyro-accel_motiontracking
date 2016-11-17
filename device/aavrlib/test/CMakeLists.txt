# uc library files
#cmake_minimum_required(VERSION 2.8)

set(tests 
    test_pwm test_analog test_systick test_softPWM_long test_softPWM_short 
    test_HC-SR04 test_spi_block test_spi_burst test_infrared test_lcd_1602a
    test_voltage_ctrl test_mpu6050_datagather test_mpu6050_calib test_avrcar)


SET(CMAKE_C_FLAGS ${CFLAGS} CACHE STRING "" FORCE)



link_directories(${CMAKE_ARCHIVE_OUTPUT_DIRECTORY})

foreach(TEST ${tests})
	add_executable(${TEST} ${TEST}.c)
	#add_executable(${TEST}.hex ${TEST})
	target_link_libraries(${TEST} ${AVRLIBC_FILES} aavr)
endforeach()
