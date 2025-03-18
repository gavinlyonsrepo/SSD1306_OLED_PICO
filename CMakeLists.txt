# Set minimum required version of CMake
cmake_minimum_required(VERSION 3.20)

# Include build functions from Pico SDK
include($ENV{PICO_SDK_PATH}/external/pico_sdk_import.cmake)

# Set name of project (as PROJECT_NAME) and C/C++ standards
project(displaylib C CXX ASM)
set(CMAKE_C_STANDARD 11)
set(CMAKE_CXX_STANDARD 20)

# Creates a pico-sdk subdirectory in our project for the libraries
pico_sdk_init()

# turn on all compiler warnings 
add_compile_options(-Wall -Wextra)

# Tell CMake where to find the executable source file
add_executable(${PROJECT_NAME} 
  examples/ssd1306/hello/main.cpp
  #examples/ssd1306/hello_128_32/main.cpp
  #examples/ssd1306/text_graphics_functions/main.cpp
  #examples/ssd1306/bitmap/main.cpp
  #examples/ssd1306/clock_demo/main.cpp
  #examples/ssd1306/FPS_test/main.cpp
  #examples/ssd1306/I2C_test/main.cpp

  #examples/sh1106/hello/main.cpp
  #examples/sh1107/hello/main.cpp

  #examples/ch1115/hello/main.cpp
  #examples/ch1115/bitmap/main.cpp
  #examples/ch1115/FPS_Function/main.cpp
  #examples/ch1115/text_graphics/main.cpp

  #examples/erm19264/hello/main.cpp
  #examples/erm19264/text_graphics/main.cpp
  #examples/erm19264/FPS_functions/main.cpp
  #examples/erm19264/bitmap/main.cpp

  #examples/nokia5110/hello/main.cpp
  #examples/nokia5110/bitmap/main.cpp
  #examples/nokia5110/framerate_test/main.cpp
  #examples/nokia5110/text_graphics_functions/main.cpp

)

# Create map/bin/hex/uf2 files
pico_add_extra_outputs(${PROJECT_NAME})

add_library(pico_displaylib INTERFACE)


target_sources(pico_displaylib INTERFACE
  ${CMAKE_CURRENT_LIST_DIR}/src/displaylib/nokia5110.cpp
  ${CMAKE_CURRENT_LIST_DIR}/src/displaylib/sh110x.cpp
  ${CMAKE_CURRENT_LIST_DIR}/src/displaylib/ssd1306.cpp
  ${CMAKE_CURRENT_LIST_DIR}/src/displaylib/ch1115.cpp
  ${CMAKE_CURRENT_LIST_DIR}/src/displaylib/erm19264.cpp
  ${CMAKE_CURRENT_LIST_DIR}/src/displaylib/display_graphics.cpp
  ${CMAKE_CURRENT_LIST_DIR}/src/displaylib/display_print.cpp
  ${CMAKE_CURRENT_LIST_DIR}/src/displaylib/display_fonts.cpp
)

target_include_directories(pico_displaylib INTERFACE ${CMAKE_CURRENT_LIST_DIR}/include)

# Pull in pico libraries that we need
target_link_libraries(${PROJECT_NAME} pico_stdlib hardware_spi hardware_i2c pico_displaylib)


# Enable usb output, disable uart output
pico_enable_stdio_usb(${PROJECT_NAME} 1)
pico_enable_stdio_uart(${PROJECT_NAME} 0)

