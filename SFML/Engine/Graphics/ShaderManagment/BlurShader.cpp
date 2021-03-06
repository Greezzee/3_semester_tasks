#include "BlurShader.h"

float BlurShader::_radius;
sf::Shader BlurShader::my_shader;

BlurShader::BlurShader()
{
	_type = tge::ShaderType::BlurShader;
	my_shader.setUniform("texture", sf::Shader::CurrentTexture);
}
BlurShader::~BlurShader()
{

}
void BlurShader::SetBlurRadius(float r)
{
	_radius = r;
	my_shader.setUniform("blur_radius", _radius);
}
float BlurShader::GetBlurRadius()
{
	return _radius;
}
