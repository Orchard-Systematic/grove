cmake_minimum_required(VERSION 3.22)
project(grove VERSION 0.1.0 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)
set(CMAKE_POSITION_INDEPENDENT_CODE ON)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

# ─── Options ─────────────────────────────────────────────────────────
option(GROVE_BUILD_TESTS "Build unit tests" ON)
option(GROVE_BUILD_BENCHMARKS "Build benchmarks" OFF)
option(GROVE_ENABLE_ONNX "Enable ONNX Runtime for ML inference" OFF)

# ─── Dependencies ────────────────────────────────────────────────────
include(FetchContent)

if(GROVE_BUILD_TESTS)
  FetchContent_Declare(
    googletest
    GIT_REPOSITORY https://github.com/google/googletest.git
    GIT_TAG v1.15.2
  )
  FetchContent_MakeAvailable(googletest)
  enable_testing()
endif()

if(GROVE_ENABLE_ONNX)
  find_package(onnxruntime QUIET)
  if(NOT onnxruntime_FOUND)
    message(STATUS "ONNX Runtime not found — ML inference module will be stubbed")
    set(GROVE_ENABLE_ONNX OFF)
  endif()
endif()

# ─── Library ─────────────────────────────────────────────────────────
add_library(grove SHARED
  # Pricing
  src/pricing/black76.cpp
  src/pricing/cost_of_carry.cpp
  src/pricing/calendar_spread.cpp

  # Curves
  src/curves/bootstrap.cpp
  src/curves/cubic_spline.cpp
  src/curves/seasonal_decomposition.cpp

  # Signal math
  src/signals/moving_average.cpp
  src/signals/zscore.cpp
  src/signals/kalman_filter.cpp

  # ML inference
  src/ml/onnx_inference.cpp
)

target_include_directories(grove
  PUBLIC
    $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
    $<INSTALL_INTERFACE:include>
)

if(GROVE_ENABLE_ONNX)
  target_link_libraries(grove PRIVATE onnxruntime::onnxruntime)
  target_compile_definitions(grove PRIVATE GROVE_HAS_ONNX=1)
endif()

set_target_properties(grove PROPERTIES
  VERSION ${PROJECT_VERSION}
  SOVERSION 0
  OUTPUT_NAME grove
)

# ─── Install ─────────────────────────────────────────────────────────
include(GNUInstallDirs)
install(TARGETS grove
  EXPORT groveTargets
  LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
  ARCHIVE DESTINATION ${CMAKE_INSTALL_LIBDIR}
)
install(DIRECTORY include/grove/ DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}/grove)
install(EXPORT groveTargets
  FILE groveTargets.cmake
  NAMESPACE grove::
  DESTINATION ${CMAKE_INSTALL_LIBDIR}/cmake/grove
)

# ─── Tests ───────────────────────────────────────────────────────────
if(GROVE_BUILD_TESTS)
  add_subdirectory(tests)
endif()
