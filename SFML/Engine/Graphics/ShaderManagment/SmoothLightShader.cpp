#include "SmoothLightShader.h"
#include "../../AllEngine.h"
sf::Shader SmoothLightShader::my_shader;

SmoothLightShader::SmoothLightShader()
{
	my_shader.setUniform("texture", sf::Shader::CurrentTexture);
	my_shader.setUniform("global_color", sf::Glsl::Vec4(1.f, 1.f, 1.f, 1.f));
	_type = tge::ShaderType::SmoothLightShader;
}
SmoothLightShader::~SmoothLightShader()
{

}
