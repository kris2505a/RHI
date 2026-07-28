cmake_minimum_required(VERSION 3.20)

project(Engine LANGUAGES CXX)

find_package(Vulkan REQUIRED)
find_package(glfw3 REQUIRED)
find_package(glm REQUIRED)

file(GLOB_RECURSE ENGINE_SOURCES
    CONFIGURE_DEPENDS
    src/*.cpp
)

file(GLOB_RECURSE ENGINE_HEADERS
    CONFIGURE_DEPENDS
    include/*.h
)

add_library(Engine SHARED
    ${ENGINE_SOURCES}
    ${ENGINE_HEADERS}
)

target_include_directories(Engine
    PUBLIC
        ${CMAKE_CURRENT_SOURCE_DIR}/src
)

target_link_libraries(Engine
    PUBLIC
        Vulkan::Vulkan
        glfw
        glm::glm
)

target_compile_features(Engine PUBLIC cxx_std_23)

target_compile_definitions(Engine
    PUBLIC
        VULKAN_HPP_NO_STRUCT_CONSTRUCTORS
        $<$<CONFIG:Debug>:MI_DEBUG>
        RHI_VK
)
