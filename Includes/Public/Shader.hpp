#pragma once
#include "CoreAPI.hpp"

namespace mr {

class RENDER_API Shader {
public:
	Shader() = default;
	~Shader() = default;
	virtual void bind() const = 0;
	
};

}