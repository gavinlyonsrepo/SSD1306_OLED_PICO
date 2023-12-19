# Set minimum required version of CMake
cmake_minimum_required(VERSION 3.12)

# Include build functions from Pico SDK
include($ENV{PICO_SDK_PATH}/external/pico_sdk_import.cmake)

# Set name of project (as PROJECT_NAME) and C/C++ standards
project(ssd1306 C CXX ASM)
set(CMAKE_C_STANDARD 11)
set(CMAKE_CXX_STANDARD 20)

# turn on all compiler warnings 
add_compile_options(-Wall)

# Creates a pico-sdk subdirectory in our project for the libraries
pico_sdk_init()

# Tell CMake where to find the executable source file
add_executable(${PROJECT_NAME} 
  examples/SSD1306_HELLO/main.cpp
  #examples/SSD1306_HELLO_128_32/main.cpp
  #examples/SSD1306_TEXT/main.cpp
  #examples/SSD1306_GRAPHICS/main.cpp
  #examples/SSD1306_FUNCTIONS/main.cpp
  #examples/SSD1306_BITMAP/main.cpp
  #examples/SSD1306_CLOCK_DEMO/main.cpp
  #examples/SSD1306_SPEED_TEST/main.cpp
)

# Create map/bin/hex/uf2 files
pico_add_extra_outputs(${PROJECT_NAME})

add_library(pico_ssd1306 INTERFACE)

target_sources(pico_ssd1306 INTERFACE
  ${CMAKE_CURRENT_LIST_DIR}/src/ssd1306/SSD1306_OLED.cpp
  ${CMAKE_CURRENT_LIST_DIR}/src/ssd1306/SSD1306_OLED_graphics.cpp
  ${CMAKE_CURRENT_LIST_DIR}/src/ssd1306/SSD1306_OLED_Print.cpp
  ${CMAKE_CURRENT_LIST_DIR}/src/ssd1306/SSD1306_OLED_font.cpp
)

target_include_directories(pico_ssd1306 INTERFACE ${CMAKE_CURRENT_LIST_DIR}/include)

# Pull in pico libraries that we need
target_link_libraries(${PROJECT_NAME} pico_stdlib hardware_i2c pico_ssd1306 )


# Enable usb output, disable uart output
pico_enable_stdio_usb(${PROJECT_NAME} 1)
pico_enable_stdio_uart(${PROJECT_NAME} 0)

