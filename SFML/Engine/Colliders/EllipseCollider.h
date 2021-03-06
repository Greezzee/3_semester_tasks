#pragma once
#include "../Gameplay/GameObject.h"
#include "../Utility/Coordinate.h"
#include "UniversalCollider.h"
/*!
* Коллайдер в виде эллипса
* Хранить можно либо как свойство Gameobject, либо просто как отдельный объект
* Задаёт эллипс в пространстве.
* 
* 
* 
* IsCollide() НЕ ТРОГАТЬ
* Остальное СИСТЕМНОЕ
*/
class EllipseCollider : public UniversalCollider
{
public:

	EllipseCollider();
	virtual ~EllipseCollider() {};

	/*!
	* Инициализация нулевого эллипса (точки с координатами (0, 0))
	*/
	void Init();

	/*!
	* Инициализация эллипса
	* pos - позиция центра эллипса
	* angle - угол поворота эллипса в РАДИАНАХ относительно центра (0 означает канонический вид, поворот против часовой)
	* size - длины полуосей
	*/
	void Init(Vector2F pos, float angle, Vector2F size);

	Vector2F GetPos() const;
	Vector2F GetSize() const;
	float GetAngle() const;

	void SetPos(const Vector2F& pos);
	void SetSize(const Vector2F& size);
	void SetAngle(float angle);

protected:
	Vector2F _pos, _size;//! Позиция центра эллипса и длина его полуосей
	float _angle;        //! Угол поворота эллипса относительно центра против часовой стрелки в радианах

	const static unsigned COLLIDE_ITERATIONS; //! Определяет точность коллизий. Больше значение - точнее, но дольше. Не использовать числа меньше 4

	friend class Collider;
};

