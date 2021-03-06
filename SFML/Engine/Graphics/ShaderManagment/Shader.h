#pragma once
#include "SFML/Graphics.hpp"
namespace tge {
	enum class ShaderType
	{
		EmptyShader = -1,
		SmoothLightShader,
		PixelLightShader,
		BlurShader
	};
}

class Shader
{
public:
	Shader() :
		_type(tge::ShaderType::EmptyShader) {}
	virtual ~Shader() {}

protected:
	tge::ShaderType _type;
	friend class ShaderManager;
};
