cmake_minimum_required(VERSION 3.5)

set(SOURCE_FILES
        Source/Main.cpp
        Source/Game.cpp
        Source/Game.h
        Source/SampleLevel/PlayerCar.cpp
        Source/SampleLevel/PlayerCar.h
        Source/SampleLevel/DirectionalLight.cpp
        Source/SampleLevel/DirectionalLight.h
        Source/SampleLevel/SampleLevel.cpp
        Source/SampleLevel/SampleLevel.h
        Source/SampleLevel/SampleLevelGeometry.cpp
        Source/SampleLevel/SampleLevelGeometry.h
)

set(EXE_FILE SampleProject)

add_executable(${EXE_FILE} ${SOURCE_FILES})

target_compile_features(${EXE_FILE} PRIVATE cxx_std_17)

if(MSVC)
    set_property(TARGET SampleProject PROPERTY MSVC_RUNTIME_LIBRARY "MultiThreadedDebugDLL")
endif()

target_link_libraries(${EXE_FILE} PRIVATE CarEngine)

target_compile_definitions(CarEngine PUBLIC GAME_ASSET_ROOT=${PROJECT_SOURCE_DIR}/SampleProject/Assets)
# For shipped games:
target_compile_definitions(CarEngine PUBLIC RELATIVE_GAME_ASSET_ROOT=SampleProject/Assets)