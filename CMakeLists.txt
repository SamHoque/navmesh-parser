cmake_minimum_required(VERSION 3.12)
project(navmesh-parser VERSION 0.0.1 LANGUAGES CXX)

option(RUN_FORMAT "Run clang-format after CMake configuration" OFF)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_SOURCE_DIR}/bin)

include_directories(${CMAKE_SOURCE_DIR}/src)

file(GLOB_RECURSE SOURCES "${CMAKE_SOURCE_DIR}/src/*.cpp")
file(GLOB_RECURSE HEADERS "${CMAKE_SOURCE_DIR}/src/*.h")

add_executable(${PROJECT_NAME} ${SOURCES} ${HEADERS})

target_compile_features(${PROJECT_NAME} PRIVATE cxx_std_20)

if(RUN_FORMAT)
    find_program(CLANG_FORMAT "clang-format")
    if(CLANG_FORMAT)
        file(GLOB_RECURSE SOURCES_TO_FORMAT "${CMAKE_SOURCE_DIR}/src/*.cpp"
             "${CMAKE_SOURCE_DIR}/src/*.h")
        execute_process(
            COMMAND ${CLANG_FORMAT} -i ${SOURCES_TO_FORMAT}
            WORKING_DIRECTORY ${CMAKE_SOURCE_DIR}
            RESULT_VARIABLE format_result
        )
        if(format_result EQUAL 0)
            message(STATUS "Successfully formatted source files")
        else()
            message(WARNING "clang-format returned error: ${format_result}")
        endif()
    else()
        message(WARNING "clang-format not found, skipping formatting")
    endif()
endif()
