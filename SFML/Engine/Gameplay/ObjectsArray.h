#pragma once
#include <vector>
#include "GameObject.h"
namespace tge {
	//! Структура, хранящая все удалённые в данный кадр объекты (их ID)
	struct KilledArray
	{
		std::vector<int> obj_id; //! Массив ID удалённых объектов. -1 означает отсутствия объекта
		unsigned top; //! Кол-во удалённых объектов
	};

	//! Хранит все объекты на данном слое
	struct Layer {
		std::vector<int> obj_id; //! Массив ID объектов на данном слое. -1 означает отсутствие объекта
		unsigned top; //! Кол-во объектов на слое
	};

	//! Данные об одном конкретном объекте
	struct ObjectData
	{
		GameObject* obj; //! Указатель на объект
		/*!
		* Указывает индекс объекта в соотвествуеющем слое.
		* -1, если объект отсутствует в соответствующем слое
		*/
		std::vector<int> layer_pos;
	};

	//! Данные обо всех объектах на игровом поле, о всех слоях и об объектах, подлежащих удалению
	struct ObjectsArray
	{
		ObjectsArray();

		void SetMaxObjects(unsigned new_size);
		void SetLayersCount(unsigned count);

		std::vector<ObjectData> obj; //! массив данных ВСЕХ объектов. Динамический ID объекта - позиция его данных в этом массиве
		std::vector<Layer> layers; //! массив данных обо всех слоях
		KilledArray to_kill; //! объекты, подлежащие удалению по окончанию кадра
		unsigned top; //! кол-во всех объектов на игровом поле

		unsigned MAX_OBJECTS; //! Максимальное количество объектов на одном игровом поле
		unsigned LAYERS_COUNT; //! Количество слоёв объектов на одном игровом поле
	};
}

//! Данные, которые необходимо предоставить игровому полю, чтобы добавить в него объект
struct GameobjectSpawnData
{
	GameObject* obj; //! Указатель на уже созданный объект, которому выделена память и который инициализирован
	std::vector<bool> is_on_layer; //! Информация о том, на каких слоях он присутствует
};
