#pragma once
#include <string>

#include "../Utility/Coordinate.h"

class ObjectContainer;

/*!
* Абстрактный класс, от которого должны наследоваться все игровые объекты (персонаж, враг, элемент окружения, комната, дверь и тд)
* При создании объекта необходимо вручную вызывать функцию Init, если она необходима
* Функции Update и Draw вызываются автоматически каждый кадр
* Destroy вызывается автоматически при удалении объекта
*
* СИСТЕМНОЕ
*/
class GameObject
{
public:

	/*!
	* Инициализация объекта
	*/
	virtual void Init() {};

	/*!
	* Инициализация объекта, вызывается вручную у дочерних классов.
	* Передаётся указатель на родительский объект owner, но НЕ ХРАНИТСЯ после инициализации!!!
	* Сделано так по причине отсутствия сборщика мусора, ведь игровой объект-родитель может исчезнуть,
	* а его дочерние объекты ничего об этом не узнают. Потому owner доступен только в теле Init.
	* Можете хранить owner в переменных дочерних объектов, если будете помнить о такой опасности исчезновения.
	*/
	virtual void Init(GameObject* owner) {};

	//! Обновление объекта. Вызывается каждый кадр (на данный момент. В будущем появится пауза и данная функция вызываться в ней не будет)
	virtual void Update() = 0;

	//! Рисование объекта. Переопределяйте вручную! Вызывается вообще каждый кадр (даже на паузе)
	virtual void Draw() = 0;

	//! Уничтожение объекта. Вызывается 1 раз, когда игровое поле уничтожает объект
	virtual void Destroy() = 0;

	//! Создание абсолютно нового объекта этого типа
	virtual GameObject* Clone() const = 0;

	Vector2F GetPos();
	void SetPos(Vector2F pos);
	Vector2F GetDirection();
	void SetDirection(Vector2F dir);
	float GetRotation();
	void SetRotation(float angle);

	void SetTag(const std::string& new_tag);
	const std::string& GetTag();

	virtual ~GameObject() {};
protected:
	std::string _tag; //! Строка, в которой может быть записано всё, что угодно. Никак не используется внешним кодом

	Vector2F _pos; //! Позиция игрового объекта в пространстве
	Vector2F _basic_direction; //! WIP, всегда устанавливать значение по умолчанию! (1, 0)
	float _angle = 0; //! Угол поворота объекта в радианах против часовой стрелки

	ObjectContainer* gamefield = nullptr; //! Указатель на игровое поле, в котором живёт данный игровой объект

private:

	//! Убедительная просьба НЕ ТРОГАТЬ ЭТУ ФУНКЦИЮ ВООБЩЕ НИКОГДА
	void SetID(unsigned new_id);
	unsigned GetID() const;

	//! Убедительная просьба НЕ ТРОГАТЬ ЭТУ ФУНКЦИЮ ВООБЩЕ НИКОГДА
	void SetGamefield(ObjectContainer* field);
	ObjectContainer* GetGamefield();

	unsigned _ID; //! Динамический номер объекта в массиве всех объектов. НЕ ТРОГАТЬ ЕГО И НЕ МЕНЯТЬ ЕГО

	friend class Collider;
	friend class ObjectContainer;
};

