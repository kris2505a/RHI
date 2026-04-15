#pragma once
#include "CoreAPI.hpp"


namespace mr {

class RENDER_API VertexBuffer {
public:
	VertexBuffer() = default;
	virtual ~VertexBuffer() = default;
	virtual void bind() const = 0;
};

}