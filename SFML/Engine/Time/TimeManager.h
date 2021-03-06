#pragma once
#include <SFML/System.hpp>

/*!
* Менеджер времени.
* Своего рода обычный таймер. Таймер единственный во всей игре!
* Создание дополнительных таймеров запрещено!!!
* 
* Зависит от SFML
* СИСТЕМНОЕ
*/
class TimeManager
{
public:

	//! Автоматическая инициализация
	static void Init();

	//! Автоматическое обновление каждый кадр
	static void Update();

	//! Позволяет сбросить все времена на таймере, обнулить его
	static void Restart();

	//! Возвращает время с последнего рестарта или инициализации В МИКРОСЕКУНДАХ
	static unsigned long long GetTimeFromLastRestart();

	//! Vozvraschaet kollichestvo vremeny s momenta poslednego obnovleniya v SECUNDAH!
	static float GetDeltaTimeF();

	//! Возвращает время, прошедшее с предыдущего кадра (время на данный кадр) В МИКРОСЕКУНДАХ
	static unsigned long long GetDeltaTime();
private:
	static sf::Clock clock; //! SFML'ые часы
	static unsigned long long TimeFromLastRestart; //! Внутренняя переменная, время с последнего рестарта в микросекундах
	static unsigned long long DeltaTime; //! Внутренняя переменная, время данного кадра в микросекундах
};

