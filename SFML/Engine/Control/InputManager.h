#pragma once
#include <vector>
#include <SFML/Window/Keyboard.hpp>
#include <SFML/Window/Mouse.hpp>
#include "../Utility/Coordinate.h"

//! Содержит названия виртуальных клавиш.
const unsigned KeysCount = 85;
enum KeyboardKey {
	NOTHING,
	ESC, N1, N2, N3, N4, N5, N6, N7, N8, N9, N0, MINUS, EQUAL, BACK,
	F1, F2, F3, F4, F5, F6, F7, F8, F9, F10, F11, F12,
	TAB, LSHIFT, LCTRL, LALT, RALT, RCTRL, RSHIFT,
	ENTER, BSLACH, //обратный слэш (около энтера)
	NUM1, NUM2, NUM3, NUM4, NUM5, NUM6, NUM7, NUM8, NUM9, NUM0, //Числа на нампаде
	TILDE,
	Q, W, E, R, T, Y, U, I, O, P,
	SQBROP, SQBRCL, //Открытие и закрытие квадратных скобок
	A, S, D, F, G, H, J, K, L,
	SEMICOLON, APOSTROPHE,
	Z, X, C, V, B, N, M,
	COMMA, DOT, SLASH,
	SPACE,
	UP, DOWN, LEFT, RIGHT
};

const unsigned MouseKeysCount = 2;
enum MouseKey {
	Mouse_Left, Mouse_Right
};

//! Хранит информацию о состоянии виртуальной клавиши
struct KeyData {
	bool is_pressed;
	bool is_realesed;
	bool is_down;
};

struct MouseData {
	Vector2F pos;
	KeyData l_button, r_button;
};

/*!
* Менеджер, который перехватывает данные с клавиатуры (в будущем и с мыши) и предоставляет их другим объектам в более удобном виде
*
* Позволяет создавать настройку управления, так как хранит связь между реальными клавишами клавиатуры и виртуальными клавишами (L_Left, L_Right и тд)
* В процессе создания игры, нужно руководствоваться только виртуальными клавишами.
* Если будут появляться настройки управления, одной реальной клавише могут соответствовать разные виртуальные (но не наоборот)
*
* Зависит от SFML
*
* СИСТЕМНОЕ. Лезть только при добавлении новых клавиш управления
*/

struct InputManager
{
	//! НЕ ТРОГАТЬ, автоматически вызывается при инициализации игры
	static void Init();

	//! НЕ ТРОГАТЬ, автоматически вызывается каждый кадр в главном цикле
	static void Update();

	//! Вернёт true в кадр, в который виртуальная клавиша code нажимается, иначе false
	static bool IsPressed(unsigned char code);

	//! Вернёт true в кадр, в который виртуальная клавиша code отжимается, иначе false
	static bool IsRealesed(unsigned char code);

	//! Вернёт true если code нажата, иначе false
	static bool IsUp(unsigned char code);

	//! Вернёт true если code не нажата, иначе false
	static bool IsDown(unsigned char code);

	//! Вернёт true в кадр, в который реальная клавиша key нажимается, иначе false
	static bool IsPressed(KeyboardKey key);

	//! Вернёт true в кадр, в который реальная клавиша key отжимается, иначе false
	static bool IsRealesed(KeyboardKey key);

	//! Вернёт true если key нажата, иначе false
	static bool IsUp(KeyboardKey key);

	//! Вернёт true если key не нажата, иначе false
	static bool IsDown(KeyboardKey key);

	//! Вернёт true в кадр, в который клавиша мыши key нажимается, иначе false
	static bool IsPressed(MouseKey key);

	//! Вернёт true в кадр, в который виртуальная клавиша key отжимается, иначе false
	static bool IsRealesed(MouseKey key);

	//! Вернёт true если key нажата, иначе false
	static bool IsUp(MouseKey key);

	//! Вернёт true если key не нажата, иначе false
	static bool IsDown(MouseKey key);

	//! Вернёт координаты курсора в системе окна SFML
	static Vector2F GetMousePos();

	//! Связывает клавишу с кодом. При отправлении функциям IsUp, IsDown, ... кода, получете информацию о связанной клавише
	//! code не больше 255
	static void LinkToCode(KeyboardKey key, unsigned char code);

	//! Вернёт первую попавшуюся KeyboradKey, которая нажимается (IsPressed = true)
	//! Если ниодна клавиша не нажата, вернёт KeyboardKey::NOTHING
	static KeyboardKey GetPressedKey();
private:
	static MouseData _mouse_info; //! Информация о положении мыши

	static sf::Keyboard::Key _keys[KeysCount]; //Связь KeybordKey с SFML
	static KeyboardKey _virtual_keys[255]; // Связь кодов с KeyboardKey
	static KeyData _key_data[KeysCount]; //! Информация о клавишах
};

