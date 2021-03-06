#pragma once
#include "../Utility/Coordinate.h"
#include "../Graphics/GraphicManager.h"
#include "../Graphics/ShaderManagment/ShaderManager.h"
#include "../Graphics/ShaderManagment/Shader.h"
#include "../Colliders/Collider.h"

class Debugger
{
public:
	//! ������ ����� ���������� ����� � ������� � pos, ����� ������� size
	static bool DrawPoint(Vector2F pos, float size, unsigned view_id = 0, Color color = Color::White(), unsigned layer = 10, Shader* shader = nullptr);

	//! ������ ����� ����� ����� ������� start � end, ������� ����� thick
	static bool DrawLine(Vector2F start, Vector2F end, float thick, unsigned view_id = 0, Color color = Color::White(), unsigned layer = 10);

	//! ������ ���������� ��������� � �������� ������� ����� points_size � �������� ����������� �� ����� line_thick
	static bool DrawCollider(const SquareCollider& col, float points_size, float line_thick, unsigned view_id = 0, Color color = Color::White(), unsigned layer = 10);

	static bool DrawCollider(const PolygonCollider& col, float points_size, float line_thick, unsigned view_id = 0, Color color = Color::White(), unsigned layer = 10);

	static bool DrawCollider(const CircleCollider& col, unsigned view_id = 0, Color color = Color::White(), unsigned layer = 10);

	static bool DrawCollider(const EllipseCollider& col, unsigned view_id = 0, Color color = Color::White(), unsigned layer = 10);
};


