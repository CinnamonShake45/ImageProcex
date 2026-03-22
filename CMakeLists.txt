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
)

target_include_directories(imageprocex
    PUBLIC
        ${PROJECT_SOURCE_DIR}/src
)

target_link_libraries(imageprocex
    PRIVATE
        PNG::PNG
)

# --------------------------------
# Examples
# --------------------------------
add_executable(main_example
    "examples/main_example.cpp"
)

target_link_libraries(main_example
    PRIVATE
        imageprocex
)

# Only build the playground if the file exists -- used for testing random bits and pieces of code
if(EXISTS "${CMAKE_CURRENT_SOURCE_DIR}/examples/playground.cpp")
    add_executable(playground 
        "examples/playground.cpp"
    )
    target_link_libraries(main_example
        PRIVATE
            imageprocex
)
endif()

# --------------------------------
# Optional: set working directory
# so examples can find test_images
# --------------------------------
set_target_properties(main_example PROPERTIES
    VS_DEBUGGER_WORKING_DIRECTORY ${PROJECT_SOURCE_DIR}
)