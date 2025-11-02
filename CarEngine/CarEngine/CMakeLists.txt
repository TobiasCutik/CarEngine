add_subdirectory(Thirdparty/glad)
add_subdirectory(Thirdparty/glfw)
add_subdirectory(Thirdparty/glm)
add_subdirectory(Thirdparty/soloud)
add_subdirectory(Thirdparty/tinygltf)
add_subdirectory(Thirdparty/eigen)
add_subdirectory(Thirdparty/freetype)
add_subdirectory(Thirdparty/freetype-gl)

# Search for chrono libraries that are built in release mode
set(chrono_DIR ${CMAKE_CURRENT_SOURCE_DIR}/Thirdparty/chrono/cmake)
find_package(chrono REQUIRED)

if (CHRONO_LIBRARIES MATCHES "ChronoEngine.lib" AND CHRONO_LIBRARIES MATCHES "ChronoEngine_vehicle.lib" AND CHRONO_LIBRARIES MATCHES "ChronoModels_vehicle.lib")
    set(RELEASE_CHRONO_LIBRARIES_ARE_BUILT "TRUE")
    message(STATUS "Release chrono libraries are built!")
else()
    set(RELEASE_CHRONO_LIBRARIES_ARE_BUILT "FALSE")
    message(STATUS "Release chrono libraries are not built yet!")
endif()

set(ALLOW_CHRONO_DEBUG_BUILD "FALSE")

# Add chrono to project whenever we build chrono and CarEngine with the same build type
if (CMAKE_BUILD_TYPE MATCHES Release OR ALLOW_CHRONO_DEBUG_BUILD)
    add_subdirectory(Thirdparty/chrono)
endif()

add_subdirectory(Source)

target_compile_definitions(CarEngine PRIVATE ENGINE_ASSET_ROOT=${PROJECT_SOURCE_DIR}/CarEngine)
# For shipped games:
target_compile_definitions(CarEngine PRIVATE RELATIVE_ENGINE_ASSET_ROOT=CarEngine)