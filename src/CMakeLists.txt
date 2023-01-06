cmake_minimum_required(VERSION 2.6)
project(gpio_test)
add_executable(gpio_test gpio_test.c)
target_link_libraries(gpio_test wiringPi)
