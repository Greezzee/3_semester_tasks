#pragma once

#include "SFML/Graphics.hpp"
#include "../../GraphicManager.h"
#include "../../../Utility/Coordinate.h"
/*
uniform int source_count;
uniform vec2 pos[128];
uniform vec4 light_color[128];
uniform float full_light_dist[128];
uniform float any_light_dist[128];
uniform float softness[128];
*/

struct LightData
{
	Vector2F pos; //! Позиция источника в игровых координатах
	Color color; //! Цвет источника
	float full_dist; //! Радиус, в котором источник будет освещать на полную мощность, в игровых единицах
	float any_dist; //! Радиус за пределами full_dist, в пределах которого есть какое-либо освещение
	float softness; //! Задаёт "мягкость" освещение. Число от 0 до 5
};
const unsigned source_max_count = 128;
class LightManager
{
public:
	//! Обязательная инициализация. Происходит автоматически
	static void Init();

	//! Добавляет новый источник света
	//! Вернёт его ID в системе
	//! Вернёт -1, если добавить новый источник света не удалось
	static int AddLightSource(const LightData& data);

	//! Позволяет изменить уже добавленный источник света по его ID
	//! Вернёт false, если изменить источник не удалось, иначе true
	static bool SetLightSource(unsigned ID, const LightData& data);

	//! Удаляет все источники света
	static void ClearLightSource();

	//! Задаёт размер пикселя для пиксельного освещения В ИГРОВЫХ ЕДИНИЦАХ (в активном view)
	static void SetPixelSize(float ps);

	//! Устанавливает активный view. Автоматически очищает все источники света
	static void SetView(unsigned id);

	//! Устанавливает цвет глобального освещения
	static void SetGlobalLight(const Color& glc);
private:

	static void ApplyLight(sf::Shader* shader);
	static void ApplyPixelLight(sf::Shader* shader);

	static unsigned view_id;

	//! Информация для шейдеров
	static unsigned source_count;
	static sf::Glsl::Vec2 pos[source_max_count];
	static sf::Glsl::Vec4 light_color[source_max_count];
	static float full_light_dist[source_max_count];
	static float any_light_dist[source_max_count];
	static float softness[source_max_count];

	static float pixel_size;
	static sf::Glsl::Vec4 global_light_color;

	friend class ShaderManager;

	static bool is_change;
};