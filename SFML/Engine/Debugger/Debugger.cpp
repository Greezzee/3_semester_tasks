#include "Debugger.h"

#include "../Graphics/GraphicManager.h"
#include "../Colliders/Collider.h"
#include <cmath>
bool Debugger::DrawPoint(Vector2F pos, float size, unsigned view_id, Color color, unsigned layer, Shader* shader)
{
	DrawData point;
	point.origin = { 0.5, 0.5 };
	point.spriteID = GraphicManager::_basic_shapes[tge::BasicShapes::Square];
	point.size = { size, size };
	point.position = pos;
	point.layer = layer;
	point.rotation = 0;
	point.frame = 0;
	point.color = color;
	point.shader = shader;
	return GraphicManager::Draw(point, view_id);
}
bool Debugger::DrawLine(Vector2F start, Vector2F end, float thick, unsigned view_id, Color color, unsigned layer)
{
	DrawData line;
	line.spriteID = GraphicManager::_basic_shapes[tge::BasicShapes::Square];
	line.origin = { 0.5, 0.5 };
	line.size = { thick, Vector2F(start - end).Magnitude() };
	line.position = (start + end) / 2;
	line.layer = layer;
	line.frame = 0;
	line.rotation = -atan2f(end.x - start.x, end.y - start.y) / 3.1415926f * 180;
	Vector2F unit = GraphicManager::GetView(view_id)->unit_vector;
	line.rotation = unit.y * unit.x * line.rotation;
	line.color = color;
	return GraphicManager::Draw(line, view_id);
}

bool Debugger::DrawCollider(const SquareCollider& col, float points_size, float line_thick, unsigned view_id, Color color, unsigned layer)
{
	for (int i = 0; i < 4; i++) {
		DrawPoint(col.GetPoint(i), points_size, view_id, color, layer);
		DrawLine(col.GetPoint(i), col.GetPoint((i + 1) % 4), line_thick, view_id, color, layer);
	}
	return true;
}

bool Debugger::DrawCollider(const PolygonCollider& col, float points_size, float line_thick, unsigned view_id, Color color, unsigned layer)
{
	unsigned col_s = col.GetPointsCount();
	for (unsigned i = 0; i < col_s; i++) {
		DrawPoint(col.GetPoint(i), points_size, view_id, color, layer);
		DrawLine(col.GetPoint(i), col.GetPoint((i + 1) % col_s), line_thick, view_id, color, layer);
	}
	return true;
}

bool Debugger::DrawCollider(const CircleCollider& col, unsigned view_id, Color color, unsigned layer)
{
	DrawData circle;
	circle.origin = { 0.5, 0.5 };
	circle.spriteID = GraphicManager::_basic_shapes[tge::BasicShapes::Circle];
	circle.size = { col.GetRadius() * 2, col.GetRadius() * 2 };
	circle.position = col.GetPos();
	circle.layer = layer;
	circle.rotation = 0;
	circle.frame = 0;
	circle.color = color;
	return GraphicManager::Draw(circle, view_id);
}

bool Debugger::DrawCollider(const EllipseCollider& col, unsigned view_id, Color color, unsigned layer)
{
	DrawData circle;
	circle.origin = { 0.5, 0.5 };
	circle.spriteID = GraphicManager::_basic_shapes[tge::BasicShapes::Circle];
	Vector2F s = col.GetSize();
	circle.size = { s.x * 2, s.y * 2 };
	circle.position = col.GetPos();
	circle.layer = layer;
	circle.rotation = col.GetAngle() / 3.1415926f * 180;
	circle.frame = 0;
	circle.color = color;
	return GraphicManager::Draw(circle, view_id);
}