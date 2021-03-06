#pragma once
#include "Shader.h"


//! Задаёт шейдер пиксельного освещения. 
//! Источники света контролирует LightManager
class PixelLightShader : public Shader
{
public:
	PixelLightShader();
	~PixelLightShader();

private:
	static sf::Shader my_shader;
	friend class ShaderManager;
};
