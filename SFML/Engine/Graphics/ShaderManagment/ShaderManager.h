#pragma once
#include <vector>
#include "Shader.h"
#include "SFML/Graphics.hpp"

class ShaderManager
{
public:
	static void Init();
	static sf::Shader* GetShader(Shader* shader);
	static void Destroy();
private:

	static void LoadShader(sf::Shader& shader, unsigned ID);

	static std::vector<std::string> _shaders_path;
	static bool _is_aviable;
};
