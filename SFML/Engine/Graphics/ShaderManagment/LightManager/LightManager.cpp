#include "LightManager.h"
#include "../../GraphicManager.h"
unsigned LightManager::source_count;
sf::Glsl::Vec2 LightManager::pos[source_max_count];
sf::Glsl::Vec4 LightManager::light_color[source_max_count];
float LightManager::full_light_dist[source_max_count];
float LightManager::any_light_dist[source_max_count];
float LightManager::softness[source_max_count];
float LightManager::pixel_size;
unsigned LightManager::view_id;
sf::Glsl::Vec4 LightManager::global_light_color;
bool LightManager::is_change;
void LightManager::Init()
{
	source_count = 0;
	view_id = 0;
	pixel_size = 1;
	global_light_color = { 0, 0, 0, 0 };
	is_change = true;
}

int LightManager::AddLightSource(const LightData& data)
{
	is_change = true;
	if (source_count + 1 >= source_max_count)
		return -1;

	Vector2F p = GraphicManager::ConvertViewPosToReal(data.pos, view_id);
	p.y = GraphicManager::GetResolution().y - p.y;
	pos[source_count] = { p.x, p.y };
	light_color[source_count] = { (float)data.color.r / 255.f, (float)data.color.g / 255.f, (float)data.color.b / 255.f, (float)data.color.a / 255.f };
	full_light_dist[source_count] = GraphicManager::ConvertViewSizeToReal(data.full_dist, view_id);
	any_light_dist[source_count] = GraphicManager::ConvertViewSizeToReal(data.any_dist, view_id);
	softness[source_count] = data.softness;

	source_count++;
	return source_count - 1;
}
bool LightManager::SetLightSource(unsigned ID, const LightData& data)
{
	is_change = true;
	if (ID >= source_count)
		return false;

	Vector2F p = GraphicManager::ConvertViewPosToReal(data.pos, view_id);
	p.y = GraphicManager::GetResolution().y - p.y;
	pos[ID] = { p.x, p.y };
	light_color[ID] = { (float)data.color.r / 255.f, (float)data.color.g / 255.f, (float)data.color.b / 255.f, (float)data.color.a / 255.f };
	full_light_dist[ID] = GraphicManager::ConvertViewSizeToReal(data.full_dist, view_id);
	any_light_dist[ID] = GraphicManager::ConvertViewSizeToReal(data.any_dist, view_id);
	softness[ID] = data.softness;

	return true;
}

void LightManager::ClearLightSource()
{
	is_change = true;
	source_count = 0;
}

void LightManager::SetPixelSize(float ps)
{
	is_change = true;
	pixel_size = ps;
}

void LightManager::SetView(unsigned id)
{
	ClearLightSource();
	view_id = id;
}

void LightManager::ApplyLight(sf::Shader* shader)
{
	if (is_change) {
		shader->setUniform("global_light_color", global_light_color);
		shader->setUniform("source_count", (int)source_count);
		shader->setUniformArray("pos", pos, source_max_count);
		shader->setUniformArray("light_color", light_color, source_max_count);
		shader->setUniformArray("full_light_dist", full_light_dist, source_max_count);
		shader->setUniformArray("any_light_dist", any_light_dist, source_max_count);
		shader->setUniformArray("softness", softness, source_max_count);
		shader->setUniform("time", (float)TimeManager::GetTimeFromLastRestart() / 1000000.f);
	}
	is_change = false;
}

void LightManager::ApplyPixelLight(sf::Shader* shader)
{
	if (is_change) {
		ApplyLight(shader);
		shader->setUniform("pixel_size", GraphicManager::ConvertViewSizeToReal(pixel_size, view_id));
	}
	is_change = false;
}

void LightManager::SetGlobalLight(const Color& glc)
{
	is_change = true;
	global_light_color.x = glc.r / 255.f;
	global_light_color.y = glc.g / 255.f;
	global_light_color.z = glc.b / 255.f;
	global_light_color.w = glc.a / 255.f;
}