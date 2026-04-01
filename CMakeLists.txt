cmake_minimum_required(VERSION 3.20)

project(ImageProcex VERSION 0.1 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# --------------------------------
# Find dependencies
# --------------------------------
find_package(PNG CONFIG REQUIRED)

# --------------------------------
# Library
# --------------------------------
add_library(imageprocex
    src/Image.cpp
    src/imageread.cpp
    src/intensity.cpp
    src/imagewrite.cpp
    src/transforms.cpp
    src/exposure.cpp
)

target_include_directories(imageprocex
    PUBLIC
        $<BUILD_INTERFACE:${PROJECT_SOURCE_DIR}/src>
)

target_link_libraries(imageprocex
    PUBLIC
        PNG::PNG
)

# --------------------------------
# Examples
# --------------------------------
add_executable(main_example
    examples/main_example.cpp
)

target_link_libraries(main_example
    PRIVATE
        imageprocex
)

if(EXISTS "${CMAKE_CURRENT_SOURCE_DIR}/examples/playground.cpp")
    add_executable(playground 
        examples/playground.cpp
    )
    target_link_libraries(playground
        PRIVATE
            imageprocex
    )
endif()

# --------------------------------
# Working directory
# --------------------------------
set_target_properties(main_example PROPERTIES
    VS_DEBUGGER_WORKING_DIRECTORY ${PROJECT_SOURCE_DIR}
)