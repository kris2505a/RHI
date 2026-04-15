#pragma once

#ifdef MIRE_RENDERER_BUILD
	#define RENDER_API __declspec(dllexport)
#else
	#define RENDER_API __declspec(dllimport)
#endif // BUILD_DLL
