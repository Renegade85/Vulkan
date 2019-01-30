cmake_minimum_required(VERSION 3.12)
project(vulkan_surface)

set(CMAKE_BUILD_TYPE Debug)
set(GLFW_BUILD_DOCS OFF CACHE BOOL "" FORCE)
set(GLFW_BUILD_TESTS OFF CACHE BOOL "" FORCE)
set(GLFW_BUILD_EXAMPLES OFF CACHE BOOL "" FORCE)

add_subdirectory("${CMAKE_SOURCE_DIR}/glfw-3.2.1/")

include_directories("${CMAKE_SOURCE_DIR}/glfw-3.2.1/include" "c:/VulkanSDK/1.1.85.0/Include" "${CMAKE_SOURCE_DIR}/inc")

add_executable(example02 ./src/main.cpp ./src/Core.cpp ./src/EngineCore.cpp)

target_link_libraries(example02 glfw)
