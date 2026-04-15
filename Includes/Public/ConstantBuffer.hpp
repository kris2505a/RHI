#pragma once

namespace mr {

class ConstantBuffer {
public:
	ConstantBuffer()								= default;
	virtual ~ConstantBuffer()						= default;
	virtual void bind()	const						= 0;
	virtual void update(const void* data, int size) = 0;
};

}