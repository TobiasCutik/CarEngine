#=============================================================================
#
# CMake configuration file for all Chrono libraries, demos, and tests.
#
#=============================================================================

#-----------------------------------------------------------------------------
# Search prefixes specified by <PackageName>_ROOT
#-----------------------------------------------------------------------------
if (POLICY CMP0074)
  cmake_policy(SET CMP0074 NEW)
endif()

#-----------------------------------------------------------------------------
# Collect compiler flags required to build the Chrono libraries
#-----------------------------------------------------------------------------

set(CH_C_FLAGS "${CMAKE_C_FLAGS}")
set(CH_CXX_FLAGS "${CMAKE_CXX_FLAGS}")
set(CH_LINKERFLAG_EXE "${CMAKE_EXE_LINKER_FLAGS}")
set(CH_LINKERFLAG_SHARED "")

#-----------------------------------------------------------------------------
# Enable C++11 support.  Check for C++14 support
#-----------------------------------------------------------------------------

# Allow user to enforce strict C++11 standard
option(CH_ENFORCE_CXX11 "Enforce strict C++11 standard" OFF)


if (CH_ENFORCE_CXX11)
	#  set(CH_CXX_FLAGS "${CH_CXX_FLAGS} ${CMAKE_CXX11_STANDARD_COMPILE_OPTION}")
	set(CMAKE_CXX_STANDARD 11)
else()
	#  set(CH_CXX_FLAGS "${CH_CXX_FLAGS} ${CMAKE_CXX14_STANDARD_COMPILE_OPTION}")
	set(CMAKE_CXX_STANDARD 14)
endif()

# This is a hack -- look for a more robust solution
set(CH_CXX14 FALSE)

if(CMAKE_CXX_COMPILER_ID MATCHES "GNU")
  message(STATUS "GCC version:  ${CMAKE_CXX_COMPILER_VERSION}")
  if(NOT CH_ENFORCE_CXX11 AND CMAKE_CXX_COMPILER_VERSION VERSION_GREATER 4.9.9)
    set(CH_CXX14 TRUE)
  endif()
elseif(CMAKE_CXX_COMPILER_ID MATCHES "Clang")
  message(STATUS "Clang version:  ${CMAKE_CXX_COMPILER_VERSION}")
  if(NOT CH_ENFORCE_Cxx11 AND CMAKE_CXX_COMPILER_VERSION VERSION_GREATER_EQUAL 3.4)
    set(CH_CXX14 TRUE)
  endif()
elseif(CMAKE_CXX_COMPILER_ID MATCHES "MSVC")
  message(STATUS "Visual Studio version:  ${MSVC_VERSION}")
  if(NOT CH_ENFORCE_CXX11 AND MSVC_VERSION GREATER 1800)
    set(CH_CXX14 TRUE)
  endif()
endif()
message(STATUS "Compiler supports C++14:  ${CH_CXX14}")

# CMake's standard support flags behave strangely with PGI compilers, don't trust them
if (CMAKE_CXX_COMPILER_ID MATCHES "PGI")
  set(CH_CXX_FLAGS "")
  message(STATUS "PGI version: ${CMAKE_CXX_COMPILER_VERSION}")
  if(NOT CH_ENFORCE_CXX11 AND CMAKE_CXX_COMPILER_VERSION VERSION_GREATER_EQUAL 17.4)
    message(STATUS "CMAKE_CXX14_STANDARD_COMPILE_OPTION can't be trusted for PGI, manually adding C++14 support")
    set(CH_CXX14 TRUE)
    set(CH_CXX_FLAGS "-std=c++14")
  endif()
endif()

# Allow user to enable whole program optimization (MSVC only)
if (MSVC)
  option(CH_WHOLE_PROG_OPT "Enable whole program optimization" OFF)

  if(CH_WHOLE_PROG_OPT)
     set(CH_CXX_FLAGS "${CH_CXX_FLAGS} /GL")
     set(CH_C_FLAGS "${CH_C_FLAGS} /GL")
     set(CH_LINKERFLAG_EXE "${CH_LINKERFLAG_EXE} /LTCG")
     set(CH_LINKERFLAG_SHARED "${CH_LINKERFLAG_SHARED} /LTCG")
     message(STATUS "Enabling whole program optimization...")
     message(STATUS "  Compiler flag: /GL")
     message(STATUS "  Linker flag: /LTGL")
  endif()
endif()

#-----------------------------------------------------------------------------
# Threads and OpenMP support
#-----------------------------------------------------------------------------

message(STATUS "Searching for Threads...")
set(THREADS_PREFER_PTHREAD_FLAG TRUE)
find_package(Threads)

message(STATUS "  Thread library:      ${CMAKE_THREAD_LIBS_INIT}")
message(STATUS "  Using Win32 threads? ${CMAKE_USE_WIN32_THREADS_INIT}")
message(STATUS "  Using pthreads?      ${CMAKE_USE_PTHREADS_INIT}")

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} ${CMAKE_THREAD_LIBS_INIT}")
set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} ${CMAKE_THREAD_LIBS_INIT}")
set(CH_CXX_FLAGS "${CH_CXX_FLAGS} ${CMAKE_THREAD_LIBS_INIT}")
set(CH_C_FLAGS "${CH_C_FLAGS} ${CMAKE_THREAD_LIBS_INIT}")

# We always look for OpenMP (which is required for the PARALLEL module).
# However, we allow the user to optionally disable OpenMP support in the
# main ChronoEngine library, regardless on whether or not OpenMP is found.

message(STATUS "Searching for OpenMP...")
find_package(OpenMP)

# Determine OpenMP version. Prepare substitution variables that can be used in
# generating configuration header files.

if(OPENMP_FOUND)
#  include(CheckOpenMPVersion)
  message(STATUS "  OpenMP version:   ${OMP_VERSION}")
  set(CHRONO_OMP_FOUND "#define CHRONO_OMP_FOUND")
  set(CHRONO_OMP_VERSION "#define CHRONO_OMP_VERSION \"${OMP_VERSION}\"")
  if(OMP_20)
    set(CHRONO_OMP_20 "#define CHRONO_OMP_20")
  else()
    set(CHRONO_OMP_20 "")
  endif()
  if(OMP_30)
    set(CHRONO_OMP_30 "#define CHRONO_OMP_30")
  else()
    set(CHRONO_OMP_30 "")
  endif()
  if(OMP_40)
    set(CHRONO_OMP_40 "#define CHRONO_OMP_40")
  else()
    set(CHRONO_OMP_40 "")
  endif()
else()
  message(STATUS "  OpenMP not found")
endif()

# Provide an option (dependent on OPENMP_FOUND) for the user to disable OpenMP
# support in the Chrono libraries (note that some modules may override this).

cmake_dependent_option(ENABLE_OPENMP "Enable OpenMP support in Chrono::Engine" ON
                       "OPENMP_FOUND" OFF)

set(OPENMP_INCLUDE_DIR "")
set(OPENMP_LIBRARIES "")
if(ENABLE_OPENMP)
  message(STATUS "  OpenMP CXX flags: ${OpenMP_CXX_FLAGS}")
  message(STATUS "  OpenMP C flags:   ${OpenMP_C_FLAGS}")
  message(STATUS "  OpenMP includes:  ${OpenMP_CXX_INCLUDE_DIRS}")
  message(STATUS "  OpenMP library:   ${OpenMP_CXX_LIBRARY}")
  message(STATUS "  OpenMP libraries: ${OpenMP_CXX_LIBRARIES}")
  set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} ${OpenMP_CXX_FLAGS}")
  set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} ${OpenMP_C_FLAGS}")
  set(CH_CXX_FLAGS "${CH_CXX_FLAGS} ${OpenMP_CXX_FLAGS}")
  set(CH_C_FLAGS "${CH_C_FLAGS} ${OpenMP_C_FLAGS}")
  set(OPENMP_INCLUDE_DIR "${OpenMP_CXX_INCLUDE_DIRS}")
  set(OPENMP_LIBRARIES "${OpenMP_CXX_LIBRARIES}")
else()
	# As an alternative, if TBB is located the Multicore module will use it for all thrust calls
	find_package("TBB")
	cmake_dependent_option(ENABLE_TBB "Enable TBB support in Chrono::Engine" ON "TBB_FOUND" OFF)
endif()

#-----------------------------------------------------------------------------
# SSE / AVX / FMA support
#-----------------------------------------------------------------------------

option(USE_SIMD "Enable use of SIMD if supported (SSE, AVX)" ON)

if(false) #USE_SIMD)

   # Figure out SIMD level support
   message(STATUS "Searching for SSE...")
   find_package(SSE)
   
   # Set substitution variables for configuration file
   if(SSE_FOUND)
     set(CHRONO_HAS_SSE "#define CHRONO_HAS_SSE")
     set(CHRONO_SSE_LEVEL "#define CHRONO_SSE_LEVEL \"${SSE_VERSION}\"")
     if(DETECTED_SSE_10)
       set(CHRONO_SSE_1_0 "#define CHRONO_SSE_1_0")
     endif()
     if(DETECTED_SSE_20)
       set(CHRONO_SSE_2_0 "#define CHRONO_SSE_2_0")
     endif()
     if(DETECTED_SSE_30)
       set(CHRONO_SSE_3_0 "#define CHRONO_SSE_3_0")
     endif()
     if(DETECTED_SSE_41)
       set(CHRONO_SSE_4_1 "#define CHRONO_SSE_4_1")
     endif()
     if(DETECTED_SSE_42)
       set(CHRONO_SSE_4_2 "#define CHRONO_SSE_4_2")
     endif()
   endif()
   
   # Figure out AVX level support
   message(STATUS "Searching for AVX...")
   find_package(AVX)
   
   # Set substitution variables for configuration file
   if(AVX_FOUND)
     set(CHRONO_HAS_AVX "#define CHRONO_HAS_AVX")
     set(CHRONO_AVX_LEVEL "#define CHRONO_AVX_LEVEL \"${AVX_VERSION}\"")
     if(DETECTED_AVX_10)
       set(CHRONO_AVX_1_0 "#define CHRONO_AVX_1_0")
     endif()
     if(DETECTED_AVX_20)
       set(CHRONO_AVX_2_0 "#define CHRONO_AVX_2_0")
     endif()
   endif()

   # Figure out FMA level support
   set(CHECK_FMA TRUE)
   if(MSVC)
     if(NOT CH_WHOLE_PROG_OPT)
        message(STATUS "FMA requires enabling whole program optimization. FMA check disabled.")
        set(CHECK_FMA FALSE)
     endif()
     if(NOT DETECTED_AVX_20)
        message(STATU "FMA requires AVX2 support. FMA check disabled.")
        set(CHECK_FMA FALSE)
     endif()
   endif()
     
   if(CHECK_FMA)    
     message(STATUS "Searching for FMA...")
     find_package(FMA)
   
     # Set substitution variables for configuration file
     if(FMA_FOUND)
       set(CHRONO_HAS_FMA "#define CHRONO_HAS_FMA")
     endif()
   endif()
   
   # Add SSE, AVX, and FMA flags to Chrono compiler flags
   # Note that these flags are already added to CMake compiler flags
   set(CH_CXX_FLAGS "${CH_CXX_FLAGS} ${SSE_FLAGS} ${AVX_FLAGS} ${FMA_FLAGS}")
   set(CH_C_FLAGS "${CH_C_FLAGS} ${SSE_FLAGS} ${AVX_FLAGS} ${FMA_FLAGS}")

else()

   message(STATUS "SIMD support disabled")

endif() 

#-----------------------------------------------------------------------------
# Eigen library
#-----------------------------------------------------------------------------

message(STATUS "Searching for Eigen3...")
#find_package(Eigen3 3.3.0)
set(EIGEN3_INCLUDE_DIR "${CMAKE_SOURCE_DIR}/CarEngine/Thirdparty/eigen/")
set(EIGEN3_FOUND true)
if(EIGEN3_FOUND)
  message(STATUS "  Eigen3 version: ${EIGEN3_VERSION}")
  message(STATUS "  Eigen3 include directory: ${EIGEN3_INCLUDE_DIR}")
else()
  mark_as_advanced(CLEAR EIGEN3_INCLUDE_DIR)
  mark_as_advanced(FORCE Eigen3_DIR)
  message(FATAL_ERROR "  Eigen3 not found. Specify EIGEN3_INCLUDE_DIR")
endif()

# Fix for VS 2017 15.8 and newer to handle alignment specification with Eigen.
if(${CMAKE_SYSTEM_NAME} MATCHES "Windows")
	if(MSVC AND ${MSVC_VERSION} GREATER_EQUAL 1915)
	  add_definitions( "-D_ENABLE_EXTENDED_ALIGNED_STORAGE" )
	endif()
endif()

cmake_dependent_option(USE_EIGEN_OPENMP "Compile Chrono with OpenMP support in Eigen" ON "ENABLE_OPENMP" OFF)

if (NOT USE_EIGEN_OPENMP)
  set(CH_CXX_FLAGS "${CH_CXX_FLAGS} -DEIGEN_DONT_PARALLELIZE")
  set(CH_C_FLAGS "${CH_C_FLAGS} -DEIGEN_DONT_PARALLELIZE")
endif()

#-----------------------------------------------------------------------------
# Miscellaneous compiler flags
#-----------------------------------------------------------------------------

if(${CMAKE_SYSTEM_NAME} MATCHES "Windows")
    if(MSVC)
        add_definitions( "-D_CRT_SECURE_NO_DEPRECATE" )  # avoids deprecation warnings
        add_definitions( "-D_SCL_SECURE_NO_DEPRECATE" )  # avoids deprecation warnings
        add_definitions( "-DNOMINMAX" ) # do not use MSVC's min/max macros
        add_definitions( "-MP" ) # perform parallel builds

        set(CH_CXX_FLAGS "${CH_CXX_FLAGS} /Zc:__cplusplus")
    endif(MSVC)

    if(MINGW OR CMAKE_COMPILER_IS_GNUCC OR CMAKE_COMPILER_IS_GNUCXX)
        set(CH_CXX_FLAGS "${CH_CXX_FLAGS} -D_MINGW -D_WINDOWS")
        set(CH_C_FLAGS "${CH_C_FLAGS} -D_MINGW -D_WINDOWS")
        set(CH_LINKERFLAG_EXE "${CH_LINKERFLAG_EXE} -Wl,--enable-runtime-pseudo-reloc")
        set(CH_LINKERFLAG_SHARED "${CH_LINKERFLAG_SHARED} -Wl,--export-all-symbols -Wl,--enable-auto-import -Wl,--enable-runtime-pseudo-reloc")

        if(CMAKE_SIZEOF_VOID_P MATCHES 8)
            set(CH_CXX_FLAGS "${CH_CXX_FLAGS} -m64")
            set(CH_C_FLAGS "${CH_C_FLAGS} -m64")
        endif()
    endif()
endif()


# Compiler warning level and disbaled warnings

if(CMAKE_CXX_COMPILER_ID MATCHES "Clang")
    message(STATUS "[Clang] Warning level set to default")
    #add_compile_options(-Wall)
    add_compile_options(-Wno-unknown-warning-option)
    add_compile_options(-Wno-reorder-ctor)
    add_compile_options(-Wno-pragma-pack)
    add_compile_options(-Wno-unused-local-typedef)
    add_compile_options(-Wno-unused-function)
    add_compile_options(-Wno-unused-parameter)
elseif(MSVC)
    message(STATUS "[MSVC] Warning level set to /W4")
    add_compile_options(/W4)
    add_compile_options(/wd4100)   # unreferenced formal parameter
    add_compile_options(/wd4127)   # conditional expression is constant
    add_compile_options(/wd4201)   # nameless struct/union
    add_compile_options(/wd4244)   # conversion; possible loss of data
    add_compile_options(/wd4251)   # class needs to have dll-interface
    add_compile_options(/wd4324)   # structure padded due to alignment specifier
    add_compile_options(/wd4505)   # unreferenced local function
    add_compile_options(/wd4458)   # declaration hides class member
    add_compile_options(/wd4515)   # namespace uses itself
else()
    message(STATUS "Warning level set to -Wall")
    add_compile_options(-Wall)
    add_compile_options(-Wno-sign-compare)
    add_compile_options(-Wno-reorder)
    add_compile_options(-Wno-unused-function)
endif()

#-----------------------------------------------------------------------------
# Set the base compilation flags
#-----------------------------------------------------------------------------

message(STATUS "Compiler and linker flags:")
message(STATUS "  C++ compiler flags: ${CH_CXX_FLAGS}")
message(STATUS "  C compiler flags:   ${CH_C_FLAGS}")
message(STATUS "  Linker flags:       ${CH_LINKERFLAG_EXE}")

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} ${CH_CXX_FLAGS}")
set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} ${CH_CXX_FLAGS} -D_DEBUG -DDEBUG")
set(CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS_RELEASE} ${CH_CXX_FLAGS} -DNDEBUG")

# For Bullet to use 32 bit math
add_definitions( "-DBP_USE_FIXEDPOINT_INT_32" )

#OPTION(USE_BULLET_DOUBLE "Compile Bullet in double precision" ON)
#if (USE_BULLET_DOUBLE)
#   add_definitions("-DBT_USE_DOUBLE_PRECISION")
#endif()


#-----------------------------------------------------------------------------
# Set the base include directories
#-----------------------------------------------------------------------------

include_directories(${CMAKE_CURRENT_SOURCE_DIR})
include_directories(${PROJECT_BINARY_DIR})
include_directories(${EIGEN3_INCLUDE_DIR})
include_directories(${OPENMP_INCLUDE_DIR})
include_directories(${THRUST_INCLUDE_DIR})

#------------------------------------------------------------
# Build submodules
#------------------------------------------------------------

message(STATUS "")
message(STATUS "==== Chrono Modules ====")
message(STATUS "")

#------------------------------------------------------------
# Propagate the CMake build to other directories
#------------------------------------------------------------

# Add directory for main ChronoEngine library
add_subdirectory(chrono)

# Let all modules have access to the headers of the main library
include_directories(${CH_INCLUDES})

# Add directories to build various modules.

# Add directory for vehicle simulation
add_subdirectory(chrono_vehicle)

# Add directory for vehicle models
add_subdirectory(chrono_models)

#------------------------------------------------------------
# Install headers from chrono_thirdparty folder
#------------------------------------------------------------

install(DIRECTORY ${CMAKE_SOURCE_DIR}/src/chrono_thirdparty/HACD
        DESTINATION include/chrono_thirdparty
        FILES_MATCHING PATTERN "*.h" PATTERN "*.cuh" PATTERN "*.hpp" PATTERN "*.inl")

install(DIRECTORY ${CMAKE_SOURCE_DIR}/src/chrono_thirdparty/HACDv2
        DESTINATION include/chrono_thirdparty
        FILES_MATCHING PATTERN "*.h" PATTERN "*.cuh" PATTERN "*.hpp" PATTERN "*.inl")

install(DIRECTORY ${CMAKE_SOURCE_DIR}/src/chrono_thirdparty/tinyobjloader
        DESTINATION include/chrono_thirdparty
        FILES_MATCHING PATTERN "*.h" PATTERN "*.cuh" PATTERN "*.hpp" PATTERN "*.inl")

install(DIRECTORY ${CMAKE_SOURCE_DIR}/src/chrono_thirdparty/yafel
        DESTINATION include/chrono_thirdparty
        FILES_MATCHING PATTERN "*.h" PATTERN "*.cuh" PATTERN "*.hpp" PATTERN "*.inl")
        
install(DIRECTORY ${CMAKE_SOURCE_DIR}/src/chrono_thirdparty/stb
        DESTINATION include/chrono_thirdparty
        FILES_MATCHING PATTERN "*.h" PATTERN "*.cuh" PATTERN "*.hpp" PATTERN "*.inl")

install(DIRECTORY ${CMAKE_SOURCE_DIR}/src/chrono_thirdparty/variant
        DESTINATION include/chrono_thirdparty
        FILES_MATCHING PATTERN "*.h" PATTERN "*.hpp" PATTERN "*.inl")

#------------------------------------------------------------
# Generate and install the Chrono configuration header
#------------------------------------------------------------

# Prepare substitution variables for the modules that were enabled
# TODO: the following tests should be more precise (we should test
#       that a particular module was successfully built, not just
#       enabled)

if(ENABLE_MODULE_CASCADE)
  set(CHRONO_CASCADE "#define CHRONO_CASCADE")
else()
  set(CHRONO_CASCADE "#undef CHRONO_CASCADE")
endif()

if(ENABLE_MODULE_COSIMULATION)
  set(CHRONO_COSIMULATION "#define CHRONO_COSIMULATION")
else()
  set(CHRONO_COSIMULATION "#undef CHRONO_COSIMULATION")
endif()

if(ENABLE_MODULE_DISTRIBUTED)
  set(CHRONO_DISTRIBUTED "#define CHRONO_DISTRIBUTED")
else()
  set(CHRONO_DISTRIBUTED "#undef CHRONO_DISTRIBUTED")
endif()

if(ENABLE_MODULE_IRRLICHT)
  set(CHRONO_IRRLICHT "#define CHRONO_IRRLICHT")
else()
  set(CHRONO_IRRLICHT "#undef CHRONO_IRRLICHT")
endif()

if(ENABLE_MODULE_MATLAB)
  set(CHRONO_MATLAB "#define CHRONO_MATLAB")
else()
  set(CHRONO_MATLAB "#undef CHRONO_MATLAB")
endif()

if(ENABLE_MODULE_PARDISO_MKL)
  set(CHRONO_PARDISO_MKL "#define CHRONO_PARDISO_MKL")
else()
  set(CHRONO_PARDISO_MKL "#undef CHRONO_PARDISO_MKL")
endif()

if(ENABLE_MODULE_MUMPS)
  set(CHRONO_MUMPS "#define CHRONO_MUMPS")
else()
  set(CHRONO_MUMPS "#undef CHRONO_MUMPS")
endif()

if(ENABLE_MODULE_MULTICORE)
  set(CHRONO_MULTICORE "#define CHRONO_MULTICORE")
else()
  set(CHRONO_MULTICORE "#undef CHRONO_MULTICORE")
endif()

if(ENABLE_MODULE_OPENGL)
  set(CHRONO_OPENGL "#define CHRONO_OPENGL")
else()
  set(CHRONO_OPENGL "#undef CHRONO_OPENGL")
endif()

if(ENABLE_MODULE_POSTPROCESS)
  set(CHRONO_POSTPROCESS "#define CHRONO_POSTPROCESS")
else()
  set(CHRONO_POSTPROCESS "#undef CHRONO_POSTPROCESS")
endif()

if(ENABLE_MODULE_PARDISO_PROJECT)
  set(CHRONO_PARDISOPROJECT "#define CHRONO_PARDISOPROJECT")
else()
  set(CHRONO_PARDISOPROJECT "#undef CHRONO_PARDISOPROJECT")
endif()

if(ENABLE_MODULE_PYTHON)
  set(CHRONO_PYTHON "#define CHRONO_PYTHON")
else()
  set(CHRONO_PYTHON "#undef CHRONO_PYTHON")
endif()

set(CHRONO_VEHICLE "#define CHRONO_VEHICLE")

if(ENABLE_MODULE_FSI)
  set(CHRONO_FSI "#define CHRONO_FSI")
else()
  set(CHRONO_FSI "#undef CHRONO_FSI")
endif()

if(ENABLE_MODULE_GPU)
  set(CHRONO_GPU "#define CHRONO_GPU")
else()
  set(CHRONO_GPU "#undef CHRONO_GPU")
endif()

if(ENABLE_MODULE_SENSOR)
  set(CHRONO_SENSOR "#define CHRONO_SENSOR")
else()
  set(CHRONO_SENSOR "#undef CHRONO_SENSOR")
endif()

if(ENABLE_MODULE_SYNCHRONO)
  set(CHRONO_SYNCHRONO "#define CHRONO_SYNCHRONO")
else()
  set(CHRONO_SYNCHRONO "#undef CHRONO_SYNCHRONO")
endif()

if (USE_SIMD)
   set(CHRONO_SIMD_ENABLED "#define CHRONO_SIMD_ENABLED")
else()
   set(CHRONO_SIMD_ENABLED "#undef CHRONO_SIMD_ENABLED")
endif()

if(ENABLE_OPENMP)
  set(CHRONO_OPENMP_ENABLED "#define CHRONO_OPENMP_ENABLED")
else()
  set(CHRONO_OPENMP_ENABLED "#undef CHRONO_OPENMP_ENABLED")
endif()

if(ENABLE_TBB)
  set(CHRONO_TBB_ENABLED "#define CHRONO_TBB_ENABLED")
else()
  set(CHRONO_TBB_ENABLED "#undef CHRONO_TBB_ENABLED")
endif()

if(CUDA_FOUND)
  set(CHRONO_HAS_CUDA "#define CHRONO_HAS_CUDA")
  set(CHRONO_CUDA_VERSION "#define CHRONO_CUDA_VERSION \"${CUDA_VERSION_STRING}\"")
else()
  set(CHRONO_HAS_CUDA "#undef CHRONO_HAS_CUDA")
  set(CHRONO_CUDA_VERSION "#undef CHRONO_CUDA_VERSION")
endif()

if(THRUST_FOUND)
  set(CHRONO_HAS_THRUST "#define CHRONO_HAS_THRUST")
  set(CHRONO_THRUST_VERSION "#define CHRONO_THRUST_VERSION \"${THRUST_VERSION}\"")
  set(CHRONO_COLLISION "#define CHRONO_COLLISION")
else()
  set(CHRONO_HAS_THRUST "#undef CHRONO_HAS_THRUST")
  set(CHRONO_THRUST_VERSION "#undef CHRONO_THRUST_VERSION")
  set(CHRONO_COLLISION "#undef CHRONO_COLLISION")
endif()

install(FILES "${PROJECT_BINARY_DIR}/chrono/ChConfig.h"
        DESTINATION include/chrono)

#-----------------------------------------------------------------------------
# Install the versioning header file
#-----------------------------------------------------------------------------

install(FILES "${PROJECT_BINARY_DIR}/chrono/ChVersion.h"
        DESTINATION include/chrono)

#------------------------------------------------------------
# Create the Chrono CMake project configuration file
#------------------------------------------------------------

# For the BUILD tree
set(CH_THIRDPARTY_INCLUDE_DIR "${PROJECT_SOURCE_DIR}/src/chrono_thirdparty")
set(CH_INCLUDE_DIRS "${PROJECT_SOURCE_DIR}/src")
set(CH_INCLUDE_DIRS ${CH_INCLUDE_DIRS} "${PROJECT_SOURCE_DIR}/src/chrono")
set(CH_INCLUDE_DIRS ${CH_INCLUDE_DIRS} "${PROJECT_SOURCE_DIR}/src/chrono/collision/bullet")
set(CH_INCLUDE_DIRS ${CH_INCLUDE_DIRS} "${PROJECT_SOURCE_DIR}/src/chrono/collision/gimpact")
set(CH_INCLUDE_DIRS ${CH_INCLUDE_DIRS} "${PROJECT_SOURCE_DIR}/src/chrono_thirdparty/HACD")
set(CH_INCLUDE_DIRS ${CH_INCLUDE_DIRS} "${PROJECT_SOURCE_DIR}/src/chrono_thirdparty/HACDv2")
set(CH_INCLUDE_DIRS ${CH_INCLUDE_DIRS} "${PROJECT_BINARY_DIR}")
set(CH_INCLUDE_DIRS ${CH_INCLUDE_DIRS} "${EIGEN3_INCLUDE_DIR}")
set(CH_INCLUDE_DIRS ${CH_INCLUDE_DIRS} "${OPENMP_INCLUDE_DIR}")
set(CH_INCLUDE_DIRS ${CH_INCLUDE_DIRS} "${THRUST_INCLUDE_DIR}")

set(CH_BINARY_DIR "${PROJECT_BINARY_DIR}")
set(CH_DATA_DIR "${PROJECT_SOURCE_DIR}/data/")

configure_file("${PROJECT_SOURCE_DIR}/cmake/ChronoConfig.cmake.in"
               "${PROJECT_BINARY_DIR}/cmake/ChronoConfig.cmake"
               @ONLY)

# For the INSTALL tree
set(CH_THIRDPARTY_INCLUDE_DIR "${CMAKE_INSTALL_PREFIX}/include/chrono_thirdparty")
set(CH_INCLUDE_DIRS "${CMAKE_INSTALL_PREFIX}/include")
set(CH_INCLUDE_DIRS ${CH_INCLUDE_DIRS} "${CMAKE_INSTALL_PREFIX}/include/chrono")
set(CH_INCLUDE_DIRS ${CH_INCLUDE_DIRS} "${CMAKE_INSTALL_PREFIX}/include/chrono/collision/bullet")
set(CH_INCLUDE_DIRS ${CH_INCLUDE_DIRS} "${CMAKE_INSTALL_PREFIX}/include/chrono/collision/gimpact")
set(CH_INCLUDE_DIRS ${CH_INCLUDE_DIRS} "${CMAKE_INSTALL_PREFIX}/include/chrono_thirdparty/HACD")
set(CH_INCLUDE_DIRS ${CH_INCLUDE_DIRS} "${CMAKE_INSTALL_PREFIX}/include/chrono_thirdparty/HACDv2")
set(CH_INCLUDE_DIRS ${CH_INCLUDE_DIRS} "${EIGEN3_INCLUDE_DIR}")
set(CH_INCLUDE_DIRS ${CH_INCLUDE_DIRS} "${OPENMP_INCLUDE_DIR}")
set(CH_INCLUDE_DIRS ${CH_INCLUDE_DIRS} "${THRUST_INCLUDE_DIR}")

set(CH_BINARY_DIR ${CMAKE_INSTALL_PREFIX})
set(CH_DATA_DIR "${CMAKE_INSTALL_PREFIX}/${CH_INSTALL_DATA}/")

configure_file("${PROJECT_SOURCE_DIR}/cmake/ChronoConfig.cmake.in"
               "${PROJECT_BINARY_DIR}/cmake/ChronoConfig.cmake.install"
               @ONLY)

if(${CMAKE_SYSTEM_NAME} MATCHES "Windows")
    set(CH_CONFIG_INSTALL_PATH "cmake")
else()
    set(CH_CONFIG_INSTALL_PATH "lib/cmake")
endif()

install(FILES "${PROJECT_BINARY_DIR}/cmake/ChronoConfig.cmake.install"
        DESTINATION ${CH_CONFIG_INSTALL_PATH}
        RENAME ChronoConfig.cmake)
