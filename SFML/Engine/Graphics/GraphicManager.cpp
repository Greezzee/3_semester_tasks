#include "GraphicManager.h"
#include "../Time/TimeManager.h"
#include "../AllEngine.h"
#include <cmath>
using namespace tge;
Vector2F GraphicManager::ConvertRealToView(Vector2F pos, unsigned view_id)
{
	View& view = views[view_id];
	pos -= view.real_position - view.real_size * view.real_origin;
	pos -= view.real_size * (Vector2F(1, 1) - view.unit_vector) / 2;
	pos = pos * view.unit_vector;
	pos = pos / view.real_size * view.virtual_size;

	pos += view.virtual_position;
	pos -= view.virtual_size * view.virtual_origin;

	return pos;
}

Vector2F GraphicManager::ConvertRealPosToView(Vector2F pos, unsigned view_id)
{
	View& view = views[view_id];
	pos -= view.real_position - view.real_size * view.real_origin;
	pos -= view.real_size * (Vector2F(1, 1) - view.unit_vector) / 2;
	pos = pos * view.unit_vector;
	pos = pos / view.real_size * view.virtual_size;

	pos += view.virtual_position;
	pos -= view.virtual_size * view.virtual_origin;

	return pos;
}

Vector2F GraphicManager::ConvertViewPosToReal(Vector2F pos, unsigned view_id)
{
	View& view = views[view_id];
	pos += view.virtual_size * view.virtual_origin;
	pos -= view.virtual_position;
	pos = pos * view.real_size / view.virtual_size;
	pos = pos / view.unit_vector;
	pos += view.real_size * (Vector2F(1, 1) - view.unit_vector) / 2;
	pos += view.real_position - view.real_size * view.real_origin;

	return pos;
}

float GraphicManager::ConvertViewSizeToReal(float len, unsigned view_id)
{
	View& view = views[view_id];
	len = len * view.real_size.x / view.virtual_size.x;
	return len;
}

sf::RenderWindow GraphicManager::window;
std::vector<TextureVertex> GraphicManager::textures;
std::vector<Sprite> GraphicManager::sprites;
std::vector<tge::GraphicLayer> GraphicManager::layers_to_draw;
std::vector<int> GraphicManager::_basic_shapes;
unsigned GraphicManager::_engine_textures_count;
unsigned GraphicManager::_engine_sprites_count;
tge::FPSCounter GraphicManager::_fps_counter;

std::vector<View> GraphicManager::views;
unsigned GraphicManager::LAYER_COUNT;

void GraphicManager::Init()
{
	window.create(sf::VideoMode(1280, 720), "Test", sf::Style::Titlebar | sf::Style::Close);
	//window.setFramerateLimit(65);

	textures.resize(_engine_textures_count);
	SetLayersCount(20);
	_engine_textures_count = 2;
	_engine_sprites_count = 2;
	_basic_shapes.resize(2);

	_basic_shapes[BasicShapes::Square] = LoadSprite(GraphicPrefabData("./Engine/Debugger/DebugResourses/Square.png", { 32, 32 }, 1));
	_basic_shapes[BasicShapes::Circle] = LoadSprite(GraphicPrefabData("./Engine/Debugger/DebugResourses/Circle.png", { 128, 128 }, 1));

	views.resize(1);
	views[0] = { {0, 0}, {1280, 720}, {0, 0}, {0, 0}, {1280, 720}, {0, 0}, {1, 1} };

	ShaderManager::Init();

}

bool GraphicManager::Update()
{
	sf::Event event;
	while (window.pollEvent(event))
	{
		if (event.type == sf::Event::Closed)
			return true;
	}

	window.clear();
	
	for (unsigned i = 0; i < LAYER_COUNT; i++) {
		layers_to_draw[i].buffer->clear(sf::Color(0, 0, 0, 0));

		for (unsigned j = 0; j < GetTexturesCount(); j++) {
			layers_to_draw[i].buffer->draw(textures[j].layer_vertex[i], &textures[j].texture);
			textures[j].layer_vertex[i].resize(0);
		}
		layers_to_draw[i].buffer->display();
		sf::Sprite layer;
		layer.setTexture(layers_to_draw[i].buffer->getTexture());
		layers_to_draw[i].layer_shader ? window.draw(layer, ShaderManager::GetShader(layers_to_draw[i].layer_shader)) : window.draw(layer);
	}
	
	window.display();

	_fps_counter.Update();

	return false;
}

void GraphicManager::Exit()
{
	for (unsigned i = 0; i < LAYER_COUNT; i++) {
		delete layers_to_draw[i].buffer;
	}
	layers_to_draw.resize(0);
	_basic_shapes.resize(0);
	textures.resize(0);
	views.resize(0);
	window.close();
	ShaderManager::Destroy();
}

bool GraphicManager::Draw(DrawData data, unsigned view_id)
{
	if (data.spriteID >= sprites.size() || view_id >= views.size())
		return false;
	Sprite& spr = sprites[data.spriteID];
	if (spr.texture_id >= textures.size())
		return false;
	TextureVertex& text_vert = textures[spr.texture_id];
	SetView(data, view_id);

	if (data.spriteID == -1)
		return false;

	if (data.layer >= LAYER_COUNT)
		return false;

	
	unsigned a = text_vert.layer_vertex[data.layer].getVertexCount();
	text_vert.layer_vertex[data.layer].resize(a + 4);

	float ca = cosf(data.rotation / 180.f * PI);
	float sa = sinf(data.rotation / 180.f * PI);
	float x, old_x, y;
	old_x = x = -data.size.x * data.origin.x;
	y = -data.size.y * data.origin.y;
	text_vert.layer_vertex[data.layer][a].position = sf::Vector2f(data.position.x + ca * x + sa * y, data.position.y - sa * x + ca * y);

	x = data.size.x * (1 - data.origin.x);
	text_vert.layer_vertex[data.layer][a + 1].position = sf::Vector2f(data.position.x + ca * x + sa * y, data.position.y - sa * x + ca * y);

	y = data.size.y * (1 - data.origin.y);
	text_vert.layer_vertex[data.layer][a + 2].position = sf::Vector2f(data.position.x + ca * x + sa * y, data.position.y - sa * x + ca * y);

	x = old_x;
	text_vert.layer_vertex[data.layer][a + 3].position = sf::Vector2f(data.position.x + ca * x + sa * y, data.position.y - sa * x + ca * y);



	float frame_offset = spr.size.x * (data.frame % spr.frames_count);

	text_vert.layer_vertex[data.layer][a].texCoords = sf::Vector2f(spr.sprite_pos.x + frame_offset, spr.sprite_pos.y);
	text_vert.layer_vertex[data.layer][a + 1].texCoords = sf::Vector2f(spr.sprite_pos.x + frame_offset + spr.size.x, spr.sprite_pos.y);
	text_vert.layer_vertex[data.layer][a + 2].texCoords = sf::Vector2f(spr.sprite_pos.x + frame_offset + spr.size.x, spr.sprite_pos.y + spr.size.y);
	text_vert.layer_vertex[data.layer][a + 3].texCoords = sf::Vector2f(spr.sprite_pos.x + frame_offset, spr.sprite_pos.y + spr.size.y);

	sf::Color color = sf::Color(data.color.r, data.color.g, data.color.b, data.color.a);
	text_vert.layer_vertex[data.layer][a].color = color;
	text_vert.layer_vertex[data.layer][a + 1].color = color;
	text_vert.layer_vertex[data.layer][a + 2].color = color;
	text_vert.layer_vertex[data.layer][a + 3].color = color;

	return true;
}

void GraphicManager::SetView(DrawData& data, unsigned view_id)
{
	View& view = views[view_id];
	Vector2F obj_pos = data.position - view.virtual_position + view.virtual_size * view.virtual_origin;
	data.position = (obj_pos * view.real_size / view.virtual_size) * view.unit_vector;
	data.position -= view.real_size * (view.unit_vector - Vector2F(1, 1)) / 2.f;
	data.position += view.real_position - view.real_size * view.real_origin;
	data.size = data.size * view.real_size / view.virtual_size;
	data.rotation *= -1;
	
	auto view_pos = view.real_position - view.real_size * view.real_origin;

	float delta = fabsf(data.size.x) + fabsf(data.size.y);
	if (data.position.x + delta < view_pos.x ||
		data.position.y + delta < view_pos.y ||
		data.position.x - delta > view_pos.x + view.real_size.x ||
		data.position.y - delta > view_pos.y + view.real_size.y)
		data.spriteID = -1;
	
}

View* GraphicManager::GetView(unsigned view_id)
{
	if (view_id >= views.size())
		return nullptr;
	return &(views[view_id]);
}

void GraphicManager::ClearSprites()
{
	sprites.resize(_engine_sprites_count);
}

unsigned GraphicManager::GetSpritesCount()
{
	return sprites.size();
}

int GraphicManager::LoadSprite(const GraphicPrefabData& data)
{
	int text_id = data.texture_id;
	if (text_id == -1)
		text_id = LoadTexture(data.file);
	if (text_id == -1)
		return -1;

	Sprite new_sprite;

	new_sprite.frames_count = data.frames_count;
	new_sprite.size = data.size;
	new_sprite.sprite_pos = data.sprite_pos;
	new_sprite.texture_id = text_id;

	sprites.push_back(new_sprite);
	return GetSpritesCount() - 1;
}

unsigned GraphicManager::GetTexturesCount()
{
	return textures.size();
}

void GraphicManager::ClearTextures()
{
	textures.resize(_engine_textures_count);
}

int GraphicManager::LoadTexture(const std::string& path)
{
	TextureVertex new_vertex;
	bool is_loaded = new_vertex.texture.loadFromFile(path);
	if (!is_loaded)
		return -1;
	new_vertex.layer_vertex.resize(LAYER_COUNT);
	for (unsigned i = 0; i < LAYER_COUNT; i++) {
		new_vertex.layer_vertex[i].setPrimitiveType(sf::Quads);
		new_vertex.layer_vertex[i].resize(0);
	}
	textures.push_back(new_vertex);
	return GetTexturesCount() - 1;
}

sf::RenderWindow* GraphicManager::GetWindow()
{
	return &window;
}

int GraphicManager::AddView(View view)
{
	if (fabsf(view.unit_vector.x) != 1 || fabsf(view.unit_vector.y) != 1)
		return -1;
	views.push_back(view);
	return views.size() - 1;
}

void GraphicManager::ResetViews()
{
	views.resize(1);
}

void GraphicManager::SetResolution(Vector2U new_size)
{
	window.create(sf::VideoMode(new_size.x, new_size.y), "Test", sf::Style::Titlebar | sf::Style::Close);
	views[0].real_size = Vector2F((float)new_size.x, (float)new_size.y);
	views[0].virtual_size = views[0].real_size;

	for (unsigned i = 0; i < LAYER_COUNT; i++) {
		layers_to_draw[i].buffer->clear(sf::Color(0, 0, 0, 0));
		layers_to_draw[i].buffer->create(GetResolution().x, GetResolution().y);
	}
}

Vector2U GraphicManager::GetResolution()
{
	auto size = window.getSize();
	return { size.x, size.y };
}

void GraphicManager::ShowFPS(bool is_active)
{
	_fps_counter.SetActive(is_active);
}

void GraphicManager::SetLayerShader(unsigned layer, Shader* shader)
{
	if (layer >= LAYER_COUNT)
		return;
	layers_to_draw[layer].layer_shader = shader;
}

void GraphicManager::SetLayersCount(unsigned count)
{
	if (count > LAYER_COUNT) {
		layers_to_draw.resize(count);
		for (unsigned i = LAYER_COUNT; i < count; i++) {
			layers_to_draw[i].buffer = new sf::RenderTexture;
			layers_to_draw[i].buffer->create(GetResolution().x, GetResolution().y);
			layers_to_draw[i].layer_shader = nullptr;
		}
		LAYER_COUNT = count;
	}
	else if (count < LAYER_COUNT) {
		for (unsigned i = count; i < LAYER_COUNT; i++) {
			delete layers_to_draw[i].buffer;
		}
		LAYER_COUNT = count;
		layers_to_draw.resize(LAYER_COUNT);
	}
	for (unsigned i = 0; i < GetTexturesCount(); i++)
		textures[i].layer_vertex.resize(LAYER_COUNT);
}