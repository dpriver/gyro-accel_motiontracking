# AAVRLIB
![](https://travis-ci.org/Kuronogard/aavrlib.svg?branch=master)

AVR microcontrollers code library.

## About
This library, whose original name comes from "Arduino AVR Library", was aimed to program the Arduino included microcontrollers in pure C. Specifically, the atmega328p packaged in the Arduino UNO.

By now, the objective is to create a general AVR library, with functionality to handle whatever variety of boards, in particular the Arduino ones. 


### Suported μcontrollers
* atmega328p

### Suported boards
* Arduino UNO

### Characteristics
 * Macros for "easy" pin usage
 * Run-time configurable ISRs

### Functionality
* Digital and analog I/O
* Timers (ctc, pwm,...)
* SPI
* TWI/I2C (**Not tested**)
* USART
* System tick (uptime, delay, timeout,...)
* Long pulse software PWM (~0-100% duty at 200Hz)
* Short pulse software PWM (~0-10% duty at 50Hz)

### Peripherals
* HC-SR04 ultrasonic sensor
* Infrared receive (NEC protocol)
* 1602a LCD display
* MPU-60X0 gyroscope, accelerometer and temperature-meter board (**Not tested**)

## Dependencies
Note that this library is based on Atmel's avr library, and aimed to avr microcontrollers, so cross compile is needed.
Make sure you have installed avr-libc and avr build tools.

If that is not the case, the following commands can be used in a debian based system to install them.
```bash
sudo apt-get update
sudo apt-get install gcc-avr avr-libc
```
Also note that link-time optimizations and the linker-plugin are used, so make sure that your compiler does support this feature. If not, you can change the compile options in cmake/toolchain/Toolchain-avr.cmake. 

NOTE: Since lto is not supported by many compilers, it is disabled by now.


## Compilation
To compile the project, cmake is used, so the following commands should be executed in the project directory.
```bash
mkdir build
cd build

cmake ..
make
```

## Generate Documentation
To install the documentation dependencies the following command can be executed
```bash
sudo apt-get install doxyfile texlive texlive-latex-extra graphviz
```

In order to create the documentation, the following can be executed
```bash
doxygen Doxyfile
```
Once doxygen is executed, doc/latex and doc/html directories should have been created. Open doc/latex/html/index.html to view the html version. To create the pdf from latex, execute
```bash
cd doc/latex
make
```
The pdf documentation file should have been created in doc/latex.

## Installation
To install the library, the following command should be used in the project's build directory after compilation
```bash
make install
```

## Usage
Once installed, to use this library, the flag -laavr has to be included in compiler invocation, as any other library. To take advantage of link-time optimizations, the flags -flto -fuse-linker-plugin should also be included.
