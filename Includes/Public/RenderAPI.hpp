#pragma once
#include "CoreAPI.hpp"
#include <Windows.h>
#include <memory>

namespace mr { //Mira Renderer


class RENDER_API RenderAPI {
public:
	RenderAPI() = default;
	virtual ~RenderAPI() = default;
	virtual void init(HWND handle)									= 0;
	virtual void clear()											= 0;
	virtual void swap()												= 0;
	virtual void drawIndexed(int count)								= 0;
	virtual void setClearColor(float r, float g, float b, float a)	= 0;
	virtual void preRenderSetup()									= 0;
	virtual void setWireFrameMode(bool cond)						= 0;


};


} //Mira Renderer