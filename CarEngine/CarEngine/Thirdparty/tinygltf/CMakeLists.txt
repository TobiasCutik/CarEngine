cmake_minimum_required(VERSION 3.6)

PROJECT (tinygltf)

SET(CMAKE_CXX_STANDARD 11)

add_library(tinygltf INTERFACE)
target_compile_features(tinygltf INTERFACE cxx_std_17)
target_include_directories(tinygltf INTERFACE include/)

#
# TinuGLTF is a header-only library, so no library build. just install header files.
#
INSTALL ( FILES
  json.hpp
  stb_image.h
  stb_image_write.h
  tiny_gltf.h
  DESTINATION
  include
  )

INSTALL ( FILES
  cmake/TinyGLTFConfig.cmake
  DESTINATION
  cmake
  )