# Find GTest
find_package(GTest QUIET)

if(NOT GTest_FOUND)
    message(STATUS "GTest not found. Tests will not be built.")
    return()
endif()

# Include directories
include_directories(${GTEST_INCLUDE_DIRS})
include_directories(${CMAKE_SOURCE_DIR}/src)

# Define test source files
set(TEST_SOURCES
    test_image_processor.cpp
)

# Add test executable
add_executable(PhotoEditorTests ${TEST_SOURCES})

# Link libraries
target_link_libraries(PhotoEditorTests ${GTEST_BOTH_LIBRARIES})
target_link_libraries(PhotoEditorTests ${wxWidgets_LIBRARIES})
target_link_libraries(PhotoEditorTests ${OpenCV_LIBS})
target_link_libraries(PhotoEditorTests pthread)

# Add source files to test executable
target_sources(PhotoEditorTests PRIVATE
    ${CMAKE_SOURCE_DIR}/src/core/image_processor.cpp
)

# Add test
add_test(NAME PhotoEditorTests COMMAND PhotoEditorTests)
