#pragma once
#include "CoreAPI.hpp"

namespace mr {

class RENDER_API IndexBuffer {
public:
	IndexBuffer()							= default;
	virtual ~IndexBuffer()					= default;
	virtual void bind()		const			= 0;
	virtual unsigned int getCount() const	= 0;
};

}