cmake_minimum_required(VERSION 3.17)

project(GLAD C)

set(HEADER_FILES include/glad/glad.h
                 include/KHR/khrplatform.h)

set(SOURCE_FILES src/glad.c)

add_library(glad STATIC ${HEADER_FILES} ${SOURCE_FILES})

target_compile_features(glad PRIVATE cxx_std_17)

target_include_directories(glad PUBLIC include/)