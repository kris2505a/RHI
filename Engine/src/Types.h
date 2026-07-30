#pragma once
#include <memory>
#include <cstdint>

#if defined(_WIN32)

    #if defined(ENGINE_BUILD)
        #define ENGINE_API __declspec(dllexport)
    #else
        #define ENGINE_API __declspec(dllimport)
    #endif

#else

    #if __GNUC__ >= 4
        #define ENGINE_API __attribute__((visibility("default")))
    #else
        #define ENGINE_API
    #endif

#endif

namespace MRe
{



//UINT
using u8 = uint8_t;
using u16 = uint16_t;
using u32 = uint32_t;
using u64 = uint64_t;

//INT
using i8 = int8_t;
using i16 = int16_t;
using i32 = int32_t;
using i64 = int64_t;

//FLOAT
using f32 = float;
using f64 = double;


template<typename T>
using Scope = std::unique_ptr<T>;

template<typename T>
using Ref = std::shared_ptr<T>;

template<typename T, typename... Args>
constexpr auto CreateScope(Args&&... args) -> Scope<T> 
{
    return std::make_unique<T>(std::forward<Args>(args)...);
}

template<typename T, typename... Args>
constexpr auto CreateRef(Args&&... args) -> Ref<T> 
{
    return std::make_shared<T>(std::forward<Args>(args)...);
}


}
