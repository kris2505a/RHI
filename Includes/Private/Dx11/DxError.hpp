#pragma once

#include <Windows.h>
#include <d3d11.h>
#include <dxgidebug.h>

#include <vector>
#include <string>
#include <iostream>
#include <sstream>
#include <stdexcept>

namespace mr {

inline std::string HResultToString(HRESULT hr)
{
    LPVOID msgBuffer = nullptr;

    DWORD result = FormatMessageA(
        FORMAT_MESSAGE_ALLOCATE_BUFFER |
        FORMAT_MESSAGE_FROM_SYSTEM |
        FORMAT_MESSAGE_IGNORE_INSERTS,
        nullptr,
        hr,
        0,
        (LPSTR)&msgBuffer,
        0,
        nullptr
    );

    std::string errorStr;

    if (result && msgBuffer) {
        errorStr = static_cast<LPCSTR>(msgBuffer);
        LocalFree(msgBuffer);
    }
    else {
        char buf[64];
        snprintf(buf, sizeof(buf), "Unknown error 0x%08X", (unsigned int)hr);
        errorStr = buf;
    }

    return errorStr;
}

#ifdef _DEBUG
#define RUN(dx_call, device_ptr) \
    do { \
        dx_call; \
        ID3D11InfoQueue* infoQueue = nullptr; \
        if (SUCCEEDED((device_ptr)->QueryInterface(__uuidof(ID3D11InfoQueue), (void**)&infoQueue)) && infoQueue) { \
            SIZE_T numMessages = infoQueue->GetNumStoredMessages(); \
            std::string out; \
            for (SIZE_T i = 0; i < numMessages; ++i) { \
                SIZE_T messageLength = 0; \
                infoQueue->GetMessage(i, nullptr, &messageLength); \
                std::vector<char> bytes(messageLength); \
                auto* message = reinterpret_cast<D3D11_MESSAGE*>(bytes.data()); \
                if (SUCCEEDED(infoQueue->GetMessage(i, message, &messageLength))) { \
                    out.append(message->pDescription); \
                    out.append("\n"); \
                } \
            } \
            infoQueue->ClearStoredMessages(); \
            infoQueue->Release(); \
            if (!out.empty()) { \
                std::ostringstream oss; \
                oss << "DirectX debug error(s)!\n\n"; \
                oss << "File: " << __FILE__ << "\n"; \
                oss << "Line: " << __LINE__ << "\n\n"; \
                oss << "Call: " << #dx_call << "\n\n"; \
                oss << out; \
                throw std::runtime_error(oss.str()); \
            } \
        } \
    } while (0)
#else
#define RUN(dx_call, device_ptr) dx_call
#endif

#ifdef _DEBUG
#define HRUN(hr_call) \
    do { \
        HRESULT hr__ = (hr_call); \
        if (FAILED(hr__)) { \
            std::ostringstream oss; \
            oss << "DirectX call failed!\n\n"; \
            oss << "File: " << __FILE__ << "\n"; \
            oss << "Line: " << __LINE__ << "\n\n"; \
            oss << "Call: " << #hr_call << "\n"; \
            oss << "HRESULT: 0x" << std::hex << (unsigned int)hr__ << std::dec << "\n"; \
            oss << "Message: " << mr::HResultToString(hr__); \
            throw std::runtime_error(oss.str()); \
        } \
    } while (0)
#else
#define HRUN(hr_call) hr_call
#endif

}