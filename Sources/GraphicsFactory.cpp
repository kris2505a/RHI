#include <Public/GraphicsFactory.hpp>
#include <Private/Dx11/Dx11GraphicsFactory.hpp>

namespace mr {

std::unique_ptr<GraphicsFactory> GraphicsFactory::create(GraphicsAPI api) {
	if (api == GraphicsAPI::DirectX11) {
		return std::make_unique <Dx11GraphicsFactory>();
	}
	return std::make_unique <Dx11GraphicsFactory>();
}

}