#=============================================================================
# CMake configuration file for Chrono Vehicle module
#
# Cannot be used stand-alone (it is loaded by parent CMake configuration file)
#=============================================================================

message(STATUS "==== Chrono Vehicle module ====")

# ----------------------------------------------------------------------------
# Generate and install configuration file
# ----------------------------------------------------------------------------

install(FILES "${PROJECT_BINARY_DIR}/chrono_vehicle/ChConfigVehicle.h"
        DESTINATION include/chrono_vehicle)

# ----------------------------------------------------------------------------
# Set CH_INCLUDES
# ----------------------------------------------------------------------------

set(CH_INCLUDES "${CMAKE_SOURCE_DIR}/CarEngine/Thirdparty/chrono/src")

# ----------------------------------------------------------------------------
# List the source files in the ChronoEngine_vehicle library
# ----------------------------------------------------------------------------

# --------------- COMMON SOURCE FILES

set(CV_BASE_SOURCE
        ChChassis.cpp
        ChDriver.cpp
        ChPart.cpp
        ChDriveline.cpp
        ChPowertrain.cpp
        ChSubsysDefs.cpp
        ChTerrain.cpp
        ChVehicle.cpp
        ChVehicleJoint.cpp
        ChVehicleModelData.cpp
        ChWorldFrame.cpp
        )
source_group("base" FILES ${CV_BASE_SOURCE})

set(CV_CHASSIS_SOURCE
        chassis/ChRigidChassis.cpp
        chassis/ChChassisConnectorArticulated.cpp
        chassis/ChChassisConnectorHitch.cpp
        chassis/ChChassisConnectorTorsion.cpp
        )
source_group("chassis" FILES ${CV_CHASSIS_SOURCE})

set(CV_POVERTRAIN_SOURCE
        powertrain/ChSimplePowertrain.cpp
        powertrain/ChShaftsPowertrain.cpp
        powertrain/ChSimpleMapPowertrain.cpp
        powertrain/ChSimpleCVTPowertrain.cpp
        )
source_group("powertrain" FILES ${CV_POVERTRAIN_SOURCE})

set(CV_TERRAIN_SOURCE
        terrain/ObsModTerrain.cpp
        terrain/FlatTerrain.cpp
        terrain/RigidTerrain.cpp
        terrain/RandomSurfaceTerrain.cpp
        )
source_group("terrain" FILES ${CV_TERRAIN_SOURCE})

# --------------- WHEELED VEHICLE SOURCE FILES

set(CV_WV_BASE_SOURCE
        wheeled_vehicle/ChAxle.cpp
        wheeled_vehicle/ChAntirollBar.cpp
        wheeled_vehicle/ChBrake.cpp
        wheeled_vehicle/ChDrivelineWV.cpp
        wheeled_vehicle/ChSuspension.cpp
        wheeled_vehicle/ChSteering.cpp
        wheeled_vehicle/ChSubchassis.cpp
        wheeled_vehicle/ChWheeledTrailer.cpp
        wheeled_vehicle/ChWheeledVehicle.cpp
        wheeled_vehicle/ChWheel.cpp
        wheeled_vehicle/ChTire.cpp
        )
source_group("wheeled_vehicle\\base" FILES ${CV_WV_BASE_SOURCE})

set(CV_WV_ANTIROLL_SOURCE
        wheeled_vehicle/antirollbar/ChAntirollBarRSD.cpp
        )
source_group("wheeled_vehicle\\antirollbar" FILES ${CV_WV_ANTIROLL_SOURCE})

set(CV_WV_BRAKE_SOURCE
        wheeled_vehicle/brake/ChBrakeSimple.cpp
        wheeled_vehicle/brake/ChBrakeShafts.cpp
        )
source_group("wheeled_vehicle\\brake" FILES ${CV_WV_BRAKE_SOURCE})

set(CV_WV_DRIVELINE_SOURCE
        wheeled_vehicle/driveline/ChShaftsDriveline2WD.cpp
        wheeled_vehicle/driveline/ChShaftsDriveline4WD.cpp
        wheeled_vehicle/driveline/ChShaftsDriveline6WD.cpp
        wheeled_vehicle/driveline/ChShaftsDriveline8WD.cpp
        wheeled_vehicle/driveline/ChSimpleDrivelineXWD.cpp
        wheeled_vehicle/driveline/ChSimpleDriveline.cpp
        )
source_group("wheeled_vehicle\\driveline" FILES ${CV_WV_DRIVELINE_SOURCE})

set(CV_WV_STEERING_SOURCE
        wheeled_vehicle/steering/ChPitmanArm.cpp
        wheeled_vehicle/steering/ChPitmanArmShafts.cpp
        wheeled_vehicle/steering/ChRackPinion.cpp
        wheeled_vehicle/steering/ChRotaryArm.cpp
        )
source_group("wheeled_vehicle\\steering" FILES ${CV_WV_STEERING_SOURCE})

set(CV_WV_SUBCHASSIS_SOURCE
        wheeled_vehicle/subchassis/ChBalancer.cpp
        )
source_group("wheeled_vehicle\\subchassis" FILES ${CV_WV_SUBCHASSIS_SOURCE})

set(CV_WV_SUSPENSION_SOURCE
        wheeled_vehicle/suspension/ChDoubleWishbone.cpp
        wheeled_vehicle/suspension/ChDoubleWishboneReduced.cpp
        wheeled_vehicle/suspension/ChSingleWishbone.cpp
        wheeled_vehicle/suspension/ChSolidAxle.cpp
        wheeled_vehicle/suspension/ChMultiLink.cpp
        wheeled_vehicle/suspension/ChHendricksonPRIMAXX.cpp
        wheeled_vehicle/suspension/ChMacPhersonStrut.cpp
        wheeled_vehicle/suspension/ChSemiTrailingArm.cpp
        wheeled_vehicle/suspension/ChThreeLinkIRS.cpp
        wheeled_vehicle/suspension/ChRigidSuspension.cpp
        wheeled_vehicle/suspension/ChRigidPinnedAxle.cpp
        wheeled_vehicle/suspension/ChLeafspringAxle.cpp
        wheeled_vehicle/suspension/ChSAELeafspringAxle.cpp
        wheeled_vehicle/suspension/ChSolidBellcrankThreeLinkAxle.cpp
        wheeled_vehicle/suspension/ChSolidThreeLinkAxle.cpp
        wheeled_vehicle/suspension/ChToeBarLeafspringAxle.cpp
        wheeled_vehicle/suspension/ChSAEToeBarLeafspringAxle.cpp
        )
source_group("wheeled_vehicle\\suspension" FILES ${CV_WV_SUSPENSION_SOURCE})

set(CV_WV_TIRE_SOURCE
        wheeled_vehicle/tire/ChRigidTire.cpp
        wheeled_vehicle/tire/ChPacejkaTire.cpp
        wheeled_vehicle/tire/ChPac02Tire.cpp
        wheeled_vehicle/tire/ChPac89Tire.cpp
        wheeled_vehicle/tire/ChLugreTire.cpp
        wheeled_vehicle/tire/ChFialaTire.cpp
        wheeled_vehicle/tire/ChTMeasyTire.cpp
        )
source_group("wheeled_vehicle\\tire" FILES ${CV_WV_TIRE_SOURCE})

# --------------- THIRD-PARTY SOURCE FILES

set(CV_STB_SOURCE
        ../chrono_thirdparty/stb/stb_image.cpp
        ../chrono_thirdparty/stb/stb_image_write.cpp
        )
source_group("utils\\stb" FILES ${CV_STB_SOURCE})

# ----------------------------------------------------------------------------
# List the header files in the ChronoEngine_vehicle library
# ----------------------------------------------------------------------------

# --------------- COMMON HEADERS

set(CV_BASE_HEADERS
        ChApiVehicle.h
        ChChassis.h
        ChDriver.h
        ChPart.h
        ChDriveline.h
        ChPowertrain.h
        ChSubsysDefs.h
        ChTerrain.h
        ChVehicle.h
        ChVehicleJoint.h
        ChVehicleModelData.h
        ChVehicleOutput.h
        ChWorldFrame.h
        )
source_group("base" FILES ${CV_BASE_HEADERS})

set(CV_CHASSIS_HEADERS
        chassis/ChRigidChassis.h
        chassis/ChChassisConnectorArticulated.h
        chassis/ChChassisConnectorHitch.h
        chassis/ChChassisConnectorTorsion.h
        )
source_group("chassis" FILES ${CV_CHASSIS_HEADERS})

set(CV_POVERTRAIN_HEADERS
        powertrain/ChSimplePowertrain.h
        powertrain/ChShaftsPowertrain.h
        powertrain/ChSimpleMapPowertrain.h
        powertrain/ChSimpleCVTPowertrain.h
        )
source_group("powertrain" FILES ${CV_POVERTRAIN_HEADERS})

set(CV_TERRAIN_HEADERS
        terrain/ObsModTerrain.h
        terrain/FlatTerrain.h
        terrain/RigidTerrain.h
        terrain/RandomSurfaceTerrain.h
        )
source_group("terrain" FILES ${CV_TERRAIN_HEADERS})

# --------------- WHEELED VEHICLE HEADERS

set(CV_WV_BASE_HEADERS
        wheeled_vehicle/ChAxle.h
        wheeled_vehicle/ChAntirollBar.h
        wheeled_vehicle/ChBrake.h
        wheeled_vehicle/ChDrivelineWV.h
        wheeled_vehicle/ChSuspension.h
        wheeled_vehicle/ChSteering.h
        wheeled_vehicle/ChSubchassis.h
        wheeled_vehicle/ChWheeledTrailer.h
        wheeled_vehicle/ChWheeledVehicle.h
        wheeled_vehicle/ChWheel.h
        wheeled_vehicle/ChTire.h
        )
source_group("wheeled_vehicle\\base" FILES ${CV_WV_BASE_HEADERS})

set(CV_WV_ANTIROLL_HEADERS
        wheeled_vehicle/antirollbar/ChAntirollBarRSD.h
        )
source_group("wheeled_vehicle\\antirollbar" FILES ${CV_WV_ANTIROLL_HEADERS})

set(CV_WV_BRAKE_HEADERS
        wheeled_vehicle/brake/ChBrakeSimple.h
        wheeled_vehicle/brake/ChBrakeShafts.h
        )
source_group("wheeled_vehicle\\brake" FILES ${CV_WV_BRAKE_HEADERS})

set(CV_WV_DRIVELINE_HEADERS
        wheeled_vehicle/driveline/ChShaftsDriveline2WD.h
        wheeled_vehicle/driveline/ChShaftsDriveline4WD.h
        wheeled_vehicle/driveline/ChShaftsDriveline6WD.h
        wheeled_vehicle/driveline/ChShaftsDriveline8WD.h
        wheeled_vehicle/driveline/ChSimpleDrivelineXWD.h
        wheeled_vehicle/driveline/ChSimpleDriveline.h
        )
source_group("wheeled_vehicle\\driveline" FILES ${CV_WV_DRIVELINE_HEADERS})

set(CV_WV_STEERING_HEADERS
        wheeled_vehicle/steering/ChPitmanArm.h
        wheeled_vehicle/steering/ChPitmanArmShafts.h
        wheeled_vehicle/steering/ChRackPinion.h
        wheeled_vehicle/steering/ChRotaryArm.h
        )
source_group("wheeled_vehicle\\steering" FILES ${CV_WV_STEERING_HEADERS})

set(CV_WV_SUBCHASSIS_HEADERS
        wheeled_vehicle/subchassis/ChBalancer.h
        )
source_group("wheeled_vehicle\\subchassis" FILES ${CV_WV_SUBCHASSIS_HEADERS})

set(CV_WV_SUSPENSION_HEADERS
        wheeled_vehicle/suspension/ChDoubleWishbone.h
        wheeled_vehicle/suspension/ChDoubleWishboneReduced.h
        wheeled_vehicle/suspension/ChSingleWishbone.h
        wheeled_vehicle/suspension/ChSolidAxle.h
        wheeled_vehicle/suspension/ChMultiLink.h
        wheeled_vehicle/suspension/ChHendricksonPRIMAXX.h
        wheeled_vehicle/suspension/ChMacPhersonStrut.h
        wheeled_vehicle/suspension/ChSemiTrailingArm.h
        wheeled_vehicle/suspension/ChThreeLinkIRS.h
        wheeled_vehicle/suspension/ChRigidSuspension.h
        wheeled_vehicle/suspension/ChRigidPinnedAxle.h
        wheeled_vehicle/suspension/ChLeafspringAxle.h
        wheeled_vehicle/suspension/ChSAELeafspringAxle.h
        wheeled_vehicle/suspension/ChSolidBellcrankThreeLinkAxle.h
        wheeled_vehicle/suspension/ChSolidThreeLinkAxle.h
        wheeled_vehicle/suspension/ChToeBarLeafspringAxle.h
        wheeled_vehicle/suspension/ChSAEToeBarLeafspringAxle.h
        )
source_group("wheeled_vehicle\\suspension" FILES ${CV_WV_SUSPENSION_HEADERS})

set(CV_WV_TIRE_HEADERS
        wheeled_vehicle/tire/ChRigidTire.h
        wheeled_vehicle/tire/ChPacejkaTire.h
        wheeled_vehicle/tire/ChPac02Tire.h
        wheeled_vehicle/tire/ChPac89Tire.h
        wheeled_vehicle/tire/ChPac2002_data.h
        wheeled_vehicle/tire/ChLugreTire.h
        wheeled_vehicle/tire/ChFialaTire.h
        wheeled_vehicle/tire/ChTMeasyTire.h
        )
source_group("wheeled_vehicle\\tire" FILES ${CV_WV_TIRE_HEADERS})

# --------------- THIRD-PARTY HEADERS

set(CV_STB_HEADERS
        ../chrono_thirdparty/stb/stb.h
        ../chrono_thirdparty/stb/stb_image.h
        ../chrono_thirdparty/stb/stb_image_write.h
        )
source_group("utils\\stb" FILES ${CV_STB_HEADERS})

# ----------------------------------------------------------------------------
# Add the ChronoEngine_vehicle library
# ----------------------------------------------------------------------------

set(CXX_FLAGS ${CH_CXX_FLAGS})
set(COMPILE_DEFS "CH_API_COMPILE_VEHICLE")
set(LINK_FLAGS ${CH_LINKERFLAG_SHARED})
set(LIBRARIES "ChronoEngine")

add_library(ChronoEngine_vehicle STATIC
        #
        ${CV_BASE_SOURCE}
        ${CV_CHASSIS_SOURCE}
        ${CV_POVERTRAIN_SOURCE}
        ${CV_TERRAIN_SOURCE}
        #
        ${CV_WV_BASE_SOURCE}
        ${CV_WV_ANTIROLL_SOURCE}
        ${CV_WV_BRAKE_SOURCE}
        ${CV_WV_DRIVELINE_SOURCE}
        ${CV_WV_STEERING_SOURCE}
        ${CV_WV_SUBCHASSIS_SOURCE}
        ${CV_WV_SUSPENSION_SOURCE}
        ${CV_WV_TIRE_SOURCE}
        #
        ${CV_STB_SOURCE}
        #
        ${CV_BASE_HEADERS}
        ${CV_CHASSIS_HEADERS}
        ${CV_POVERTRAIN_HEADERS}
        ${CV_TERRAIN_HEADERS}
        #
        ${CV_WV_BASE_HEADERS}
        ${CV_WV_ANTIROLL_HEADERS}
        ${CV_WV_BRAKE_HEADERS}
        ${CV_WV_DRIVELINE_HEADERS}
        ${CV_WV_STEERING_HEADERS}
        ${CV_WV_SUBCHASSIS_HEADERS}
        ${CV_WV_SUSPENSION_HEADERS}
        ${CV_WV_TIRE_HEADERS}
        #
        ${CV_STB_HEADERS}
        )

if(MSVC)
    set_property(TARGET ChronoEngine_vehicle PROPERTY MSVC_RUNTIME_LIBRARY "MultiThreadedDebugDLL")
endif()

set_target_properties(ChronoEngine_vehicle PROPERTIES
                      COMPILE_FLAGS "${CXX_FLAGS}"
                      LINK_FLAGS "${CH_LINKERFLAG_SHARED}")

target_compile_definitions(ChronoEngine_vehicle PRIVATE "CH_API_COMPILE_VEHICLE")
target_compile_definitions(ChronoEngine_vehicle PRIVATE "CH_IGNORE_DEPRECATED")

target_link_libraries(ChronoEngine_vehicle ${LIBRARIES})

target_include_directories(ChronoEngine_vehicle PUBLIC
        ${CH_INCLUDES}
        )

install(TARGETS ChronoEngine_vehicle
        RUNTIME DESTINATION bin
        LIBRARY DESTINATION lib
        ARCHIVE DESTINATION lib)

#-------------------------------------------------------------------------------
# Install Chrono::Vehicle headers
#-------------------------------------------------------------------------------

install(DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/
        DESTINATION include/chrono_vehicle
        FILES_MATCHING PATTERN "*.h")

# Install required chrono_thirdparty headers
install(DIRECTORY ${CMAKE_SOURCE_DIR}/src/chrono_thirdparty/stb
        DESTINATION include/chrono_thirdparty
        FILES_MATCHING PATTERN "*.h" PATTERN "*.cuh" PATTERN "*.hpp" PATTERN "*.inl")