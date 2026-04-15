cmake_minimum_required(VERSION 3.20)

project(Renderer LANGUAGES CXX)

set(RENDERER_SOURCES
    Sources/GraphicsFactory.cpp
    Sources/stb_image.cpp

    Sources/Dx11/Dx11ConstantBuffer.cpp
    Sources/Dx11/Dx11GraphicsFactory.cpp
    Sources/Dx11/Dx11IndexBuffer.cpp
    Sources/Dx11/Dx11RenderAPI.cpp
    Sources/Dx11/Dx11Shader.cpp
    Sources/Dx11/Dx11Texture.cpp
    Sources/Dx11/Dx11VertexBuffer.cpp
)

set(RENDERER_HEADERS
    Includes/Public/ConstantBuffer.hpp
    Includes/Public/CoreAPI.hpp
    Includes/Public/GraphicsAPI.hpp
    Includes/Public/GraphicsFactory.hpp
    Includes/Public/IndexBuffer.hpp
    Includes/Public/InputLayout.hpp
    Includes/Public/RenderAPI.hpp
    Includes/Public/Shader.hpp
    Includes/Public/ShaderType.hpp
    Includes/Public/Texture.hpp
    Includes/Public/VertexBuffer.hpp

    Includes/Private/stb_image.h

    Includes/Private/Dx11/Dx11ConstantBuffer.hpp
    Includes/Private/Dx11/Dx11GraphicsFactory.hpp
    Includes/Private/Dx11/Dx11IndexBuffer.hpp
    Includes/Private/Dx11/Dx11RenderAPI.hpp
    Includes/Private/Dx11/Dx11Shader.hpp
    Includes/Private/Dx11/Dx11Texture.hpp
    Includes/Private/Dx11/Dx11VertexBuffer.hpp
    Includes/Private/Dx11/DxError.hpp
)

add_library(Renderer SHARED
    ${RENDERER_SOURCES}
    ${RENDERER_HEADERS}
)

target_compile_features(Renderer PUBLIC cxx_std_20)

target_include_directories(Renderer PUBLIC
    ${CMAKE_CURRENT_SOURCE_DIR}/Includes
    ${CMAKE_CURRENT_SOURCE_DIR}/Includes/Public
    ${CMAKE_CURRENT_SOURCE_DIR}/Includes/Private
)

target_link_libraries(Renderer PRIVATE
    d3d11
    dxgi
    d3dcompiler
)

target_compile_definitions(Renderer PRIVATE
    MIRE_RENDERER_BUILD
)

set_target_properties(Renderer PROPERTIES
    RUNTIME_OUTPUT_DIRECTORY_DEBUG   ${CMAKE_SOURCE_DIR}/Binaries/Debug/Renderer
    RUNTIME_OUTPUT_DIRECTORY_RELEASE ${CMAKE_SOURCE_DIR}/Binaries/Release/Renderer
    LIBRARY_OUTPUT_DIRECTORY_DEBUG   ${CMAKE_SOURCE_DIR}/Binaries/Debug/Renderer
    LIBRARY_OUTPUT_DIRECTORY_RELEASE ${CMAKE_SOURCE_DIR}/Binaries/Release/Renderer
)

add_custom_command(TARGET Renderer POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy_if_different
        $<TARGET_FILE:Renderer>
        ${CMAKE_SOURCE_DIR}/Binaries/$<CONFIG>/Game
)