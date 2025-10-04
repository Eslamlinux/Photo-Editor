cmake_minimum_required(VERSION 3.12)
project(PhotoEditor VERSION 1.0.0 LANGUAGES CXX)

# تعيين معيار C++
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

# تعيين خيارات البناء
if(NOT CMAKE_BUILD_TYPE)
    set(CMAKE_BUILD_TYPE "Release" CACHE STRING "Choose the type of build" FORCE)
    set_property(CACHE CMAKE_BUILD_TYPE PROPERTY STRINGS "Debug" "Release" "MinSizeRel" "RelWithDebInfo")
endif()

# استخدام wx-config للحصول على علامات التجميع الصحيحة
find_program(WX_CONFIG_EXECUTABLE wx-config)
if(NOT WX_CONFIG_EXECUTABLE)
    message(FATAL_ERROR "wx-config not found. Please install wxWidgets development files.")
endif()

# الحصول على علامات التجميع من wx-config
execute_process(
    COMMAND ${WX_CONFIG_EXECUTABLE} --cxxflags
    OUTPUT_VARIABLE wxWidgets_CXX_FLAGS
    OUTPUT_STRIP_TRAILING_WHITESPACE
)
execute_process(
    COMMAND ${WX_CONFIG_EXECUTABLE} --libs
    OUTPUT_VARIABLE wxWidgets_LIBRARIES
    OUTPUT_STRIP_TRAILING_WHITESPACE
)

# إضافة علامات التجميع إلى المشروع
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} ${wxWidgets_CXX_FLAGS}")

# تحسين الأداء في وضع الإصدار
if(CMAKE_BUILD_TYPE STREQUAL "Release")
    set(CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS_RELEASE} -O3")
    if(UNIX AND NOT APPLE)
        set(CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS_RELEASE} -march=native")
    endif()
endif()

# البحث عن OpenCV
find_package(OpenCV REQUIRED)

# تضمين الدلائل
include_directories(
    ${OpenCV_INCLUDE_DIRS}
    ${CMAKE_SOURCE_DIR}/src
)

# إضافة ملفات المصدر
file(GLOB_RECURSE SOURCES 
    "src/*.cpp"
)

# استبعاد ملفات الذكاء الاصطناعي وقاعدة البيانات
list(FILTER SOURCES EXCLUDE REGEX ".*ai/.*\\.cpp$")
list(FILTER SOURCES EXCLUDE REGEX ".*database/.*\\.cpp$")
list(FILTER SOURCES EXCLUDE REGEX ".*tests/.*\\.cpp$")
list(FILTER SOURCES EXCLUDE REGEX ".*main_window\\.cpp$")

# إضافة ملفات الرأس
file(GLOB_RECURSE HEADERS 
    "src/*.h"
)

# استبعاد ملفات الذكاء الاصطناعي وقاعدة البيانات
list(FILTER HEADERS EXCLUDE REGEX ".*ai/.*\\.h$")
list(FILTER HEADERS EXCLUDE REGEX ".*database/.*\\.h$")
list(FILTER HEADERS EXCLUDE REGEX ".*tests/.*\\.h$")

# إنشاء الملف التنفيذي
if(WIN32)
    add_executable(PhotoEditor WIN32 ${SOURCES} ${HEADERS})
    
    # إضافة ملف الموارد لنظام Windows
    if(EXISTS "${CMAKE_SOURCE_DIR}/resources/windows/app.rc.in")
        configure_file(
            ${CMAKE_SOURCE_DIR}/resources/windows/app.rc.in
            ${CMAKE_BINARY_DIR}/app.rc
            @ONLY
        )
        target_sources(PhotoEditor PRIVATE ${CMAKE_BINARY_DIR}/app.rc)
    endif()
else()
    add_executable(PhotoEditor ${SOURCES} ${HEADERS})
endif()

# ربط المكتبات
target_link_libraries(PhotoEditor 
    ${wxWidgets_LIBRARIES}
    ${OpenCV_LIBS}
)

# تعيين دليل الإخراج
set_target_properties(PhotoEditor PROPERTIES
    RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin
)

# تثبيت الهدف
install(TARGETS PhotoEditor DESTINATION bin)

# تثبيت الموارد
if(EXISTS ${CMAKE_SOURCE_DIR}/resources)
    install(DIRECTORY ${CMAKE_SOURCE_DIR}/resources DESTINATION bin)
endif()

# تثبيت الترجمات
if(EXISTS ${CMAKE_SOURCE_DIR}/translations)
    install(DIRECTORY ${CMAKE_SOURCE_DIR}/translations DESTINATION bin)
endif()

# طباعة ملخص التكوين
message(STATUS "Configuration Summary:")
message(STATUS "  wxWidgets flags: ${wxWidgets_CXX_FLAGS}")
message(STATUS "  wxWidgets libs: ${wxWidgets_LIBRARIES}")
message(STATUS "  OpenCV version: ${OpenCV_VERSION}")
message(STATUS "  C++ standard: ${CMAKE_CXX_STANDARD}")
message(STATUS "  Build type: ${CMAKE_BUILD_TYPE}")
