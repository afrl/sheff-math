add_executable(simplemath_dev source/main.cpp)
target_link_libraries(simplemath_dev PRIVATE SimpleMath::SimpleMath)

include(${CMAKE_SOURCE_DIR}/cmake/Utilities.cmake)
assign_source_group("simple_math" ${SIMPLEMATH_HEADER_LIST})
assign_source_group("simple_math" ${SIMPLEMATH_SOURCE_LIST})
