#pragma once

namespace mr {

class Texture {
public:
	Texture()								= default;
	virtual ~Texture()						= default;
	virtual void bind(int slot = 0) const	= 0;
};

}