#include "ShaderManager.h"
#include "SmoothLightShader.h"
#include "PixelLightShader.h"
#include "BlurShader.h"
#include "LightManager/LightManager.h"
#include <iostream>
using namespace tge;

std::vector<std::string> ShaderManager::_shaders_path = {
	"Engine/Graphics/Shaders/SmoothLight.frag",
	"Engine/Graphics/Shaders/PixelLight.frag",
	"Engine/Graphics/Shaders/Blur.frag"
};

bool ShaderManager::_is_aviable;

void ShaderManager::Init()
{
	if (!sf::Shader::isAvailable()) {
		printf("Shaders not available on this system\nShaders disabled\n");
		_is_aviable = false;
		return;
	}
	_is_aviable = true;
	
	LoadShader(SmoothLightShader::my_shader, 0);
	LoadShader(PixelLightShader::my_shader, 1);
	LoadShader(BlurShader::my_shader, 2);
}

void ShaderManager::Destroy() {}

sf::Shader* ShaderManager::GetShader(Shader* shader)
{
	switch (shader->_type)
	{
	case ShaderType::BlurShader:
		return &BlurShader::my_shader;
	case ShaderType::PixelLightShader:
		LightManager::ApplyPixelLight(&PixelLightShader::my_shader);
		return &PixelLightShader::my_shader;
	case ShaderType::SmoothLightShader:
		LightManager::ApplyLight(&SmoothLightShader::my_shader);
		return &SmoothLightShader::my_shader;
	default:
		return nullptr;
		break;
	}
}

void ShaderManager::LoadShader(sf::Shader& shader, unsigned ID)
{
	bool x;
	x = shader.loadFromFile(_shaders_path[ID], sf::Shader::Fragment);
	if (!x) {
		_is_aviable = false;
		std::cout << "Shader: [" << _shaders_path[ID] << "] not aviable\nShaders disabled\n";
	}
}

