cmake_minimum_required(VERSION 3.17)

set(CMAKE_CXX_STANDARD 17)

message("CMAKE_BUILD_TYPE = ${CMAKE_BUILD_TYPE}")

project(CarEngine CXX)

set(HEADER_FILES
        Rendering/Shader/Shader.h
        ActorComponents/Entity.h
        Rendering/Mesh/Mesh.h
        Core/Actor.h
        Core/Transformable.h
        Rendering/Renderer.h
        Input/Input.h
        Input/KeyCodes.h
        CarSimulation/Cars/Car.h
        CarSimulation/Terrain/Terrain.h
        CarSimulation/Terrain/TerrainPlane.h
        CarSimulation/Terrain/TerrainMesh.h
        CarSimulation/Terrain/TerrainMaterial/TerrainRigidMaterial.h
        CarSimulation/Terrain/TerrainMaterial/TerrainSoftMaterial.h
        CarSimulation/Terrain/TerrainMaterial/TerrainMaterial.h
        ActorComponents/Collision/CollisionBox.h
        ActorComponents/Camera.h
        CarSimulation/Collision/Collision.h
        CarSimulation/CarSimulationHandler.h
        ActorComponents/Collision/CollisionEntity.h
        Utils/Math.h
        CarSimulation/CarSimulationTransformManager.h
        ActorComponents/Terrain/TerrainPlaneEntity.h
        CarSimulation/Terrain/TerrainPatch.h
        Utils/GeneralTransformManager.h
        Rendering/Mesh/MeshNode.h
        ActorComponents/LightEntity.h
        Rendering/Material/Material.h
        Utils/Materials.h
        Rendering/Shader/CameraShader.h
        Rendering/Shader/DepthShader.h
        Rendering/Shader/SkyboxShader.h
        Rendering/Material/SkyboxMaterial.h
        Rendering/Material/TextureMaterial.h
        Rendering/Material/ColorMaterial.h
        Core/ActorContainer.h
        Core/Component.h
        ActorComponents/SkyboxEntity.h
        Utils/Loader.h
        Core/World.h
        Core/Context.h
        Core/AbstractGame.h
        Core/Engine.h
        Input/Car/AutomaticCarInput.h
        CarSimulation/Cars/HmmwvCar.h
        CarSimulation/Cars/HmmwvCarActor.h
        CarSimulation/Cars/CarEnums.h
        Input/Car/CarInputHandler.h
        CarSimulation/Cars/SedanCar.h
        CarSimulation/Cars/SedanCarActor.h
        CarSimulation/Cars/GatorCar.h
        CarSimulation/Cars/GatorCarActor.h
        CarSimulation/Cars/RcCar.h
        CarSimulation/Cars/RcCarActor.h
        Input/Car/AutomaticRcCarInput.h
        ActorComponents/Terrain/TerrainPatchEntity.h
        Core/Level.h
        Core/LevelManager.h
        Rendering/Skybox.h
        Rendering/Light.h
        ActorComponents/EntityCollection.h
        CarSimulation/ContactMethod.h
        Core/Event.h
        UI/UIComponent.h
        ActorComponents/ActorComponent.h
        UI/UIPanel.h
        UI/UIContainer.h
        UI/UIImage.h
        Rendering/Shader/UIShader.h
        UI/UITextField.h
        Rendering/Mesh/TextQuads.h
        Rendering/Shader/TextShader.h
        Rendering/Shader/TilingShader.h
        Utils/AssetManager.h
        Utils/Timer.h
        Input/MouseButtonCodes.h
        UI/UIButton.h
        UI/UIButtonHandler.h
        Audio/AudioPlayer.h
        Audio/AudioClip.h
        Utils/SaveGameManager.h
        Utils/SaveGame.h
        UI/UITextInputPanel.h
        Input/ControllerCodes.h)

set(SOURCE_FILES
        Rendering/Shader/Shader.cpp
        ActorComponents/Entity.cpp
        Rendering/Mesh/Mesh.cpp
        Core/Actor.cpp
        Core/Transformable.cpp
        Rendering/Renderer.cpp
        Input/Input.cpp
        CarSimulation/Cars/Car.cpp
        CarSimulation/Terrain/Terrain.cpp
        CarSimulation/Terrain/TerrainPlane.cpp
        CarSimulation/Terrain/TerrainMesh.cpp
        CarSimulation/Terrain/TerrainMaterial/TerrainRigidMaterial.cpp
        CarSimulation/Terrain/TerrainMaterial/TerrainSoftMaterial.cpp
        CarSimulation/Terrain/TerrainMaterial/TerrainMaterial.cpp
        ActorComponents/Collision/CollisionBox.cpp
        ActorComponents/Camera.cpp
        CarSimulation/Collision/Collision.cpp
        CarSimulation/CarSimulationHandler.cpp
        ActorComponents/Collision/CollisionEntity.cpp
        Utils/Math.cpp
        CarSimulation/CarSimulationTransformManager.cpp
        ActorComponents/Terrain/TerrainPlaneEntity.cpp
        CarSimulation/Terrain/TerrainPatch.cpp
        Utils/GeneralTransformManager.cpp
        Rendering/Mesh/MeshNode.cpp
        ActorComponents/LightEntity.cpp
        Rendering/Material/Material.cpp
        Utils/Materials.cpp
        Rendering/Shader/CameraShader.cpp
        Rendering/Shader/DepthShader.cpp
        Rendering/Shader/SkyboxShader.cpp
        Rendering/Material/SkyboxMaterial.cpp
        Rendering/Material/TextureMaterial.cpp
        Rendering/Material/ColorMaterial.cpp
        Core/ActorContainer.cpp
        Core/Component.cpp
        ActorComponents/SkyboxEntity.cpp
        Utils/Loader.cpp
        Core/World.cpp
        Core/Context.cpp
        Core/AbstractGame.cpp
        Core/Engine.cpp
        Input/Car/AutomaticCarInput.cpp
        CarSimulation/Cars/HmmwvCar.cpp
        CarSimulation/Cars/HmmwvCarActor.cpp
        Input/Car/CarInputHandler.cpp
        CarSimulation/Cars/SedanCar.cpp
        CarSimulation/Cars/SedanCarActor.cpp
        CarSimulation/Cars/GatorCar.cpp
        CarSimulation/Cars/GatorCarActor.cpp
        CarSimulation/Cars/RcCar.cpp
        CarSimulation/Cars/RcCarActor.cpp
        Input/Car/AutomaticRcCarInput.cpp
        ActorComponents/Terrain/TerrainPatchEntity.cpp
        Core/Level.cpp
        Core/LevelManager.cpp
        Rendering/Skybox.cpp
        Rendering/Light.cpp
        ActorComponents/EntityCollection.cpp
        UI/UIComponent.cpp
        ActorComponents/ActorComponent.cpp
        UI/UIPanel.cpp
        UI/UIContainer.cpp
        UI/UIImage.cpp
        Rendering/Shader/UIShader.cpp
        UI/UITextField.cpp
        Rendering/Mesh/TextQuads.cpp
        Rendering/Shader/TextShader.cpp
        Rendering/Shader/TilingShader.cpp
        Utils/AssetManager.cpp
        Utils/Timer.cpp
        UI/UIButton.cpp
        UI/UIButtonHandler.cpp
        Audio/AudioPlayer.cpp
        Audio/AudioClip.cpp
        Utils/SaveGameManager.cpp
        Utils/SaveGame.cpp
        UI/UITextInputPanel.cpp)

add_library(CarEngine STATIC ${HEADER_FILES} ${SOURCE_FILES})

if(MSVC)
    set_property(TARGET CarEngine PROPERTY MSVC_RUNTIME_LIBRARY "MultiThreadedDebugDLL")
endif()

# Link chrono libraries to CarEngine whenever both are built with the same build type
if(CMAKE_BUILD_TYPE MATCHES Release OR ALLOW_CHRONO_DEBUG_BUILD)
    message(STATUS "Chrono libraries are linked to project as a subdirectory")
    target_link_libraries(CarEngine PUBLIC ChronoEngine ChronoEngine_vehicle ChronoModels_vehicle)
endif()

if(CMAKE_BUILD_TYPE MATCHES Debug AND ALLOW_CHRONO_DEBUG_BUILD)
    message(WARNING "Chrono libraries will be built in Debug Mode as ALLOW_CHRONO_DEBUG_BUILD is set to true! Using them in Debug Mode heavily reduces performance! It is recommended to build chrono libraries in Release Mode! \nYou can set ALLOW_CHRONO_DEBUG_BUILD in ../CarEngine/CMakeLists.txt")
endif()

# Build CarEngine in Debug Mode and link release chrono libraries as an external project
if(CMAKE_BUILD_TYPE MATCHES Debug AND NOT ALLOW_CHRONO_DEBUG_BUILD)
    if (RELEASE_CHRONO_LIBRARIES_ARE_BUILT)
        message(STATUS "Chrono libraries are linked as an external project in Release Mode")
        target_include_directories(CarEngine PUBLIC "${CMAKE_SOURCE_DIR}/CarEngine/Thirdparty/chrono/src/")
        target_link_libraries(CarEngine PUBLIC ${CHRONO_LIBRARIES})
    else()
        message(SEND_ERROR "Chrono libraries are set to be built in Debug Mode but ALLOW_CHRONO_DEBUG_BUILD is set to false! \nYou can set ALLOW_CHRONO_DEBUG_BUILD in ../CarEngine/CMakeLists.txt")
        message(WARNING "Using chrono libraries in Debug Mode heavily reduces performance! It is recommended to build chrono libraries in Release Mode!")
        message(WARNING "To use CarEngine in Debug Mode (and Chrono libraries in Release Mode), build your project in Release Mode first and afterwards in Debug Mode.")
        return()
    endif()
endif()

target_link_libraries(CarEngine PUBLIC glad glfw glm soloud tinygltf eigen freetype freetype-gl)

target_compile_features(CarEngine PRIVATE cxx_std_17)

target_include_directories(CarEngine PUBLIC ${CMAKE_SOURCE_DIR}/CarEngine/Source)