#=============================================================================
# CMake configuration file for Chrono Vehicle Models
#
# Cannot be used stand-alone (it is loaded by parent CMake configuration file)
#=============================================================================

message(STATUS "==== Chrono models ====")

install(FILES ChApiModels.h DESTINATION include/chrono_models)

# ----------------------------------------------------------------------------
# Set CH_INCLUDES
# ----------------------------------------------------------------------------

set(CH_INCLUDES "${CMAKE_SOURCE_DIR}/CarEngine/Thirdparty/chrono/src")

# ----------------------------------------------------------------------------
# List the files in the Chrono_vehicle_models library
# ----------------------------------------------------------------------------

set(CVM_COMMON_FILES
    ChApiModels.h
    ChVehicleModelDefs.h
)
source_group("" FILES ${CVM_COMMON_FILES})

set(CVM_HMMWV_HEADERS
        hmmwv/HMMWV.h
        hmmwv/HMMWV_BrakeSimple.h
        hmmwv/HMMWV_BrakeShafts.h
        hmmwv/HMMWV_Chassis.h
        hmmwv/HMMWV_DoubleWishbone.h
        hmmwv/HMMWV_DoubleWishboneReduced.h
        hmmwv/HMMWV_Driveline2WD.h
        hmmwv/HMMWV_Driveline4WD.h
        hmmwv/HMMWV_RigidTire.h
        hmmwv/HMMWV_FialaTire.h
        hmmwv/HMMWV_LugreTire.h
        hmmwv/HMMWV_PacejkaTire.h
        hmmwv/HMMWV_Pac89Tire.h
        hmmwv/HMMWV_Pac02Tire.h
        hmmwv/HMMWV_TMeasyTire.h
        hmmwv/HMMWV_PitmanArm.h
        hmmwv/HMMWV_PitmanArmShafts.h
        hmmwv/HMMWV_Powertrain.h
        hmmwv/HMMWV_RackPinion.h
        hmmwv/HMMWV_SimpleDriveline.h
        hmmwv/HMMWV_SimplePowertrain.h
        hmmwv/HMMWV_SimpleCVTPowertrain.h
        hmmwv/HMMWV_SimpleMapPowertrain.h
        hmmwv/HMMWV_Vehicle.h
        hmmwv/HMMWV_VehicleFull.h
        hmmwv/HMMWV_VehicleReduced.h
        hmmwv/HMMWV_Wheel.h
        )
set(CVM_HMMWV_SOURCE
        hmmwv/HMMWV.cpp
        hmmwv/HMMWV_BrakeSimple.cpp
        hmmwv/HMMWV_BrakeShafts.cpp
        hmmwv/HMMWV_Chassis.cpp
        hmmwv/HMMWV_DoubleWishbone.cpp
        hmmwv/HMMWV_DoubleWishboneReduced.cpp
        hmmwv/HMMWV_Driveline2WD.cpp
        hmmwv/HMMWV_Driveline4WD.cpp
        hmmwv/HMMWV_RigidTire.cpp
        hmmwv/HMMWV_FialaTire.cpp
        hmmwv/HMMWV_LugreTire.cpp
        hmmwv/HMMWV_PacejkaTire.cpp
        hmmwv/HMMWV_Pac89Tire.cpp
        hmmwv/HMMWV_Pac02Tire.cpp
        hmmwv/HMMWV_TMeasyTire.cpp
        hmmwv/HMMWV_PitmanArm.cpp
        hmmwv/HMMWV_PitmanArmShafts.cpp
        hmmwv/HMMWV_Powertrain.cpp
        hmmwv/HMMWV_RackPinion.cpp
        hmmwv/HMMWV_SimpleDriveline.cpp
        hmmwv/HMMWV_SimplePowertrain.cpp
        hmmwv/HMMWV_SimpleCVTPowertrain.cpp
        hmmwv/HMMWV_SimpleMapPowertrain.cpp
        hmmwv/HMMWV_VehicleFull.cpp
        hmmwv/HMMWV_VehicleReduced.cpp
        hmmwv/HMMWV_Wheel.cpp
        )
source_group("hmmwv" FILES ${CVM_HMMWV_HEADERS} ${CVM_HMMWV_SOURCE})

set(CVM_SEDAN_HEADERS
        sedan/Sedan.h
        sedan/Sedan_BrakeSimple.h
        sedan/Sedan_BrakeShafts.h
        sedan/Sedan_Chassis.h
        sedan/Sedan_DoubleWishbone.h
        sedan/Sedan_Driveline2WD.h
        sedan/Sedan_MultiLink.h
        sedan/Sedan_RigidTire.h
        sedan/Sedan_TMeasyTire.h
        sedan/Sedan_Pac02Tire.h
        sedan/Sedan_RackPinion.h
        sedan/Sedan_SimpleMapPowertrain.h
        sedan/Sedan_Vehicle.h
        sedan/Sedan_Wheel.h
        )
set(CVM_SEDAN_SOURCE
        sedan/Sedan.cpp
        sedan/Sedan_BrakeSimple.cpp
        sedan/Sedan_BrakeShafts.cpp
        sedan/Sedan_Chassis.cpp
        sedan/Sedan_DoubleWishbone.cpp
        sedan/Sedan_Driveline2WD.cpp
        sedan/Sedan_MultiLink.cpp
        sedan/Sedan_RigidTire.cpp
        sedan/Sedan_TMeasyTire.cpp
        sedan/Sedan_Pac02Tire.cpp
        sedan/Sedan_RackPinion.cpp
        sedan/Sedan_SimpleMapPowertrain.cpp
        sedan/Sedan_Vehicle.cpp
        sedan/Sedan_Wheel.cpp
        )
source_group("sedan" FILES ${CVM_SEDAN_HEADERS} ${CVM_SEDAN_SOURCE})

set(CVM_GATOR_HEADERS
        gator/Gator.h
        gator/Gator_BrakeSimple.h
        gator/Gator_BrakeShafts.h
        gator/Gator_Chassis.h
        gator/Gator_SingleWishbone.h
        gator/Gator_SimpleDriveline.h
        gator/Gator_Driveline2WD.h
        gator/Gator_RigidSuspension.h
        gator/Gator_RigidTire.h
        gator/Gator_TMeasyTire.h
        gator/Gator_RackPinion.h
        gator/Gator_SimplePowertrain.h
        gator/Gator_SimpleMapPowertrain.h
        gator/Gator_Vehicle.h
        gator/Gator_Wheel.h
        )
set(CVM_GATOR_SOURCE
        gator/Gator.cpp
        gator/Gator_BrakeSimple.cpp
        gator/Gator_BrakeShafts.cpp
        gator/Gator_Chassis.cpp
        gator/Gator_SingleWishbone.cpp
        gator/Gator_SimpleDriveline.cpp
        gator/Gator_Driveline2WD.cpp
        gator/Gator_RigidSuspension.cpp
        gator/Gator_RigidTire.cpp
        gator/Gator_TMeasyTire.cpp
        gator/Gator_RackPinion.cpp
        gator/Gator_SimplePowertrain.cpp
        gator/Gator_SimpleMapPowertrain.cpp
        gator/Gator_Vehicle.cpp
        gator/Gator_Wheel.cpp
        )
source_group("gator" FILES ${CVM_GATOR_HEADERS} ${CVM_GATOR_SOURCE})

set(CVM_RCCAR_HEADERS
        rccar/RCCar_BrakeSimple.h
        rccar/RCCar_Chassis.h
        rccar/RCCar_DoubleWishbone.h
        rccar/RCCar_Driveline4WD.h
        rccar/RCCar_PitmanArm.h
        rccar/RCCar_RigidTire.h
        rccar/RCCar_SimpleMapPowertrain.h
        rccar/RCCar_Vehicle.h
        rccar/RCCar_Wheel.h
        rccar/RCCar.h
        rccar/RCCar_TMeasyTire.h
        )
set(CVM_RCCAR_SOURCE
        rccar/RCCar_BrakeSimple.cpp
        rccar/RCCar_Chassis.cpp
        rccar/RCCar_DoubleWishbone.cpp
        rccar/RCCar_Driveline4WD.cpp
        rccar/RCCar_PitmanArm.cpp
        rccar/RCCar_RigidTire.cpp
        rccar/RCCar_SimpleMapPowertrain.cpp
        rccar/RCCar_Vehicle.cpp
        rccar/RCCar_Wheel.cpp
        rccar/RCCar.cpp
        rccar/RCCar_TMeasyTire.cpp
        )
source_group("rccar" FILES ${CVM_RCCAR_HEADERS} ${CVM_RCCAR_SOURCE})

# ----------------------------------------------------------------------------
# Add the Chrono_vehicle_models library
# ----------------------------------------------------------------------------

set(CXX_FLAGS ${CH_CXX_FLAGS})
set(LIBRARIES ChronoEngine_vehicle)

add_library(ChronoModels_vehicle STATIC
    ${CVM_COMMON_FILES}
    ${CVM_HMMWV_HEADERS}
    ${CVM_HMMWV_SOURCE}
    ${CVM_SEDAN_HEADERS}
    ${CVM_SEDAN_SOURCE}
    ${CVM_GATOR_HEADERS}
    ${CVM_GATOR_SOURCE}
    ${CVM_RCCAR_HEADERS}
    ${CVM_RCCAR_SOURCE}
 )

if(MSVC)
    set_property(TARGET ChronoModels_vehicle PROPERTY MSVC_RUNTIME_LIBRARY "MultiThreadedDebugDLL")
endif()

set_target_properties(ChronoModels_vehicle PROPERTIES
                      COMPILE_FLAGS "${CXX_FLAGS}"
                      LINK_FLAGS "${CH_LINKERFLAG_SHARED}")

target_compile_definitions(ChronoModels_vehicle PRIVATE "CH_API_COMPILE_MODELS")
target_compile_definitions(ChronoModels_vehicle PRIVATE "CH_IGNORE_DEPRECATED")

target_link_libraries(ChronoModels_vehicle ${LIBRARIES})

target_include_directories(ChronoModels_vehicle PUBLIC
        ${CH_INCLUDES}
        )

install(TARGETS ChronoModels_vehicle
        RUNTIME DESTINATION bin
        LIBRARY DESTINATION lib
        ARCHIVE DESTINATION lib)

install(DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/
        DESTINATION include/chrono_models/vehicle
        FILES_MATCHING PATTERN "*.h")