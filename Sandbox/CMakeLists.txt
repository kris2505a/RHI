cmake_minimum_required(VERSION 3.20)

project(Sandbox LANGUAGES CXX)

find_package(glfw3 REQUIRED)
find_package(glm REQUIRED)

# Find glslc (comes with the Vulkan SDK or shaderc package)
find_program(GLSLC glslc REQUIRED)

file(GLOB_RECURSE SANDBOX_SOURCES
    CONFIGURE_DEPENDS
    src/*.cpp
)

file(GLOB_RECURSE SANDBOX_HEADERS
    CONFIGURE_DEPENDS
    include/*.h
)

file(GLOB_RECURSE SHADER_SOURCES
    CONFIGURE_DEPENDS
    shaders/*
)

set(SHADER_OUTPUT_DIR
    ${CMAKE_SOURCE_DIR}/Binaries/Shaders
)

set(COMPILED_SHADERS)

foreach(SHADER ${SHADER_SOURCES})

    get_filename_component(FILE_NAME ${SHADER} NAME)

    set(OUTPUT_FILE
        ${SHADER_OUTPUT_DIR}/${FILE_NAME}.spv
    )

    add_custom_command(
        OUTPUT ${OUTPUT_FILE}

        COMMAND ${CMAKE_COMMAND} -E make_directory
                ${SHADER_OUTPUT_DIR}

        COMMAND ${GLSLC}
                ${SHADER}
                -o
                ${OUTPUT_FILE}

        DEPENDS ${SHADER}

        COMMENT "Compiling shader ${FILE_NAME}"
        VERBATIM
    )

    list(APPEND COMPILED_SHADERS ${OUTPUT_FILE})

endforeach()

add_custom_target(Shaders
    ALL
    DEPENDS ${COMPILED_SHADERS}
)

add_executable(Sandbox
    ${SANDBOX_SOURCES}
    ${SANDBOX_HEADERS}
)

add_dependencies(Sandbox Shaders)

target_include_directories(Sandbox
    PRIVATE
        ${CMAKE_CURRENT_SOURCE_DIR}/include
)

target_link_libraries(Sandbox
    PRIVATE
        Engine
        glfw
        glm::glm
)
