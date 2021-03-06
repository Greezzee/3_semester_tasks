#include "PixelLightShader.h"

sf::Shader PixelLightShader::my_shader;

PixelLightShader::PixelLightShader()
{
	_type = tge::ShaderType::PixelLightShader;
	my_shader.setUniform("texture", sf::Shader::CurrentTexture);
	my_shader.setUniform("global_color", sf::Glsl::Vec4(1.f, 1.f, 1.f, 1.f));
}
PixelLightShader::~PixelLightShader()
{

}
