cmake_minimum_required(VERSION 3.20)

project(imageprocex)

find_package(PNG CONFIG REQUIRED)

add_executable(imageprocex 
    main.cpp
    imageread.cpp    
)

target_link_libraries(imageprocex PRIVATE PNG::PNG)