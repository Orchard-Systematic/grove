if(NOT GROVE_BUILD_TESTS)
  return()
endif()

include(GoogleTest)

add_executable(grove_tests
  test_black76.cpp
  test_cost_of_carry.cpp
  test_calendar_spread.cpp
  test_cubic_spline.cpp
  test_bootstrap.cpp
  test_moving_average.cpp
  test_zscore.cpp
  test_kalman_filter.cpp
)

target_link_libraries(grove_tests PRIVATE grove GTest::gtest_main)
gtest_discover_tests(grove_tests)
