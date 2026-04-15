# Mira Renderer

A modular and extensible rendering library built in **C++20**, currently featuring a **DirectX 11 backend**, with architecture designed for future **multi-backend support** such as Vulkan.

The project focuses on clean abstraction layers between the public rendering API and backend-specific implementations.

---

## Features

* **C++20**
* **DirectX 11 backend**
* Backend-agnostic renderer interfaces
* Modular resource abstractions
* Designed for future Vulkan / OpenGL backends
* Separate public and private API structure
* Easy integration into game engines and tooling

---

## Project Structure

```text
Renderer/
│   CMakeLists.txt
│   README.md
│
├── Includes
│   ├── Private
│   │   ├── stb_image.h
│   │   └── Dx11
│   │       ├── Dx11ConstantBuffer.hpp
│   │       ├── Dx11GraphicsFactory.hpp
│   │       ├── Dx11IndexBuffer.hpp
│   │       ├── Dx11RenderAPI.hpp
│   │       ├── Dx11Shader.hpp
│   │       ├── Dx11Texture.hpp
│   │       ├── Dx11VertexBuffer.hpp
│   │       └── DxError.hpp
│   │
│   └── Public
│       ├── ConstantBuffer.hpp
│       ├── CoreAPI.hpp
│       ├── GraphicsAPI.hpp
│       ├── GraphicsFactory.hpp
│       ├── IndexBuffer.hpp
│       ├── InputLayout.hpp
│       ├── RenderAPI.hpp
│       ├── Shader.hpp
│       ├── ShaderType.hpp
│       ├── Texture.hpp
│       └── VertexBuffer.hpp
│
└── Sources
    ├── GraphicsFactory.cpp
    ├── stb_image.cpp
    └── Dx11
        ├── Dx11ConstantBuffer.cpp
        ├── Dx11GraphicsFactory.cpp
        ├── Dx11IndexBuffer.cpp
        ├── Dx11RenderAPI.cpp
        ├── Dx11Shader.cpp
        ├── Dx11Texture.cpp
        └── Dx11VertexBuffer.cpp
```

---

## Architecture

The renderer is split into two layers:

### Public API

Contains backend-independent interfaces used by the engine or game.

Examples:

* `RenderAPI`
* `GraphicsFactory`
* `Shader`
* `Texture`
* `VertexBuffer`

These interfaces should remain free of backend-specific types.

---

### Backend Implementations

Contains concrete implementations for a graphics API.

Current:

* **DirectX 11**

Planned:

* **Vulkan**

Example:

```cpp
class Dx11RenderAPI : public RenderAPI
```

This structure allows the renderer to support multiple backends without affecting higher-level systems.

---

## Current Backend

### DirectX 11

Implemented under:

```text
Includes/Private/Dx11
Sources/Dx11
```

Uses:

* `d3d11`
* `dxgi`
* `d3dcompiler`

---

## Build

### Requirements

* CMake 3.20+
* Visual Studio 2022
* Windows SDK
* DirectX 11 libraries

### Build Steps

```bash
cmake -B build
cmake --build build --config Debug
```

---

## Future Goals

* Vulkan backend
* Shader compilation pipeline
* Material system
* Render passes
* Resource caching
* Pipeline abstraction

---

## Philosophy

This project aims to keep rendering concepts abstract and backend-independent wherever possible.

The long-term goal is:

```text
Game -> Engine -> Renderer -> Backend
```

Where the backend can be swapped between DirectX 11 and Vulkan with minimal changes to the engine layer.

---

## License

MIT
