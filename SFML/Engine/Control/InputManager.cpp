#include "InputManager.h"
#include "../Graphics/GraphicManager.h"
int max(int a, int b) {
	return a > b ? a : b;
}

MouseData InputManager::_mouse_info;
sf::Keyboard::Key InputManager::_keys[KeysCount]; //Связь KeybordKey с SFML
KeyboardKey InputManager::_virtual_keys[255]; // Связь кодов с KeyboardKey
KeyData InputManager::_key_data[KeysCount]; //! Информация о клавишах

void InputManager::Init()
{
	for (int i = 0; i < KeysCount; i++) {
		_key_data[i] = { false, false, false };
	}
	for (int i = 0; i < 255; i++)
		_virtual_keys[i] = KeyboardKey::NOTHING;

	_mouse_info = { {0, 0}, {false, false, false}, {false, false, false} };

	_keys[KeyboardKey::NOTHING] = sf::Keyboard::Key::Unknown;
	_keys[KeyboardKey::A] = sf::Keyboard::Key::A;
	_keys[KeyboardKey::APOSTROPHE] = sf::Keyboard::Key::Quote;
	_keys[KeyboardKey::B] = sf::Keyboard::Key::B;
	_keys[KeyboardKey::BACK] = sf::Keyboard::Key::BackSpace;
	_keys[KeyboardKey::C] = sf::Keyboard::Key::C;
	_keys[KeyboardKey::COMMA] = sf::Keyboard::Key::Comma;
	_keys[KeyboardKey::D] = sf::Keyboard::Key::D;
	_keys[KeyboardKey::DOT] = sf::Keyboard::Key::Period;
	_keys[KeyboardKey::E] = sf::Keyboard::Key::E;
	_keys[KeyboardKey::ENTER] = sf::Keyboard::Key::Return;
	_keys[KeyboardKey::EQUAL] = sf::Keyboard::Key::Equal;
	_keys[KeyboardKey::ESC] = sf::Keyboard::Key::Escape;
	_keys[KeyboardKey::F] = sf::Keyboard::Key::F;
	_keys[KeyboardKey::F1] = sf::Keyboard::Key::F1;
	_keys[KeyboardKey::F2] = sf::Keyboard::Key::F2;
	_keys[KeyboardKey::F3] = sf::Keyboard::Key::F3;
	_keys[KeyboardKey::F4] = sf::Keyboard::Key::F4;
	_keys[KeyboardKey::F5] = sf::Keyboard::Key::F5;
	_keys[KeyboardKey::F6] = sf::Keyboard::Key::F6;
	_keys[KeyboardKey::F7] = sf::Keyboard::Key::F7;
	_keys[KeyboardKey::F8] = sf::Keyboard::Key::F8;
	_keys[KeyboardKey::F9] = sf::Keyboard::Key::F9;
	_keys[KeyboardKey::F10] = sf::Keyboard::Key::F10;
	_keys[KeyboardKey::F11] = sf::Keyboard::Key::F11;
	_keys[KeyboardKey::F12] = sf::Keyboard::Key::F12;
	_keys[KeyboardKey::G] = sf::Keyboard::Key::G;
	_keys[KeyboardKey::H] = sf::Keyboard::Key::H;
	_keys[KeyboardKey::I] = sf::Keyboard::Key::I;
	_keys[KeyboardKey::J] = sf::Keyboard::Key::J;
	_keys[KeyboardKey::K] = sf::Keyboard::Key::K;
	_keys[KeyboardKey::L] = sf::Keyboard::Key::L;
	_keys[KeyboardKey::LALT] = sf::Keyboard::Key::LAlt;
	_keys[KeyboardKey::LCTRL] = sf::Keyboard::Key::LControl;
	_keys[KeyboardKey::LSHIFT] = sf::Keyboard::Key::LShift;
	_keys[KeyboardKey::M] = sf::Keyboard::Key::M;
	_keys[KeyboardKey::MINUS] = sf::Keyboard::Key::Dash;
	_keys[KeyboardKey::N] = sf::Keyboard::Key::N;
	_keys[KeyboardKey::N0] = sf::Keyboard::Key::Num0;
	_keys[KeyboardKey::N1] = sf::Keyboard::Key::Num1;
	_keys[KeyboardKey::N2] = sf::Keyboard::Key::Num2;
	_keys[KeyboardKey::N3] = sf::Keyboard::Key::Num3;
	_keys[KeyboardKey::N4] = sf::Keyboard::Key::Num4;
	_keys[KeyboardKey::N5] = sf::Keyboard::Key::Num5;
	_keys[KeyboardKey::N6] = sf::Keyboard::Key::Num6;
	_keys[KeyboardKey::N7] = sf::Keyboard::Key::Num7;
	_keys[KeyboardKey::N8] = sf::Keyboard::Key::Num8;
	_keys[KeyboardKey::N9] = sf::Keyboard::Key::Num9;
	_keys[KeyboardKey::NUM0] = sf::Keyboard::Key::Numpad0;
	_keys[KeyboardKey::NUM1] = sf::Keyboard::Key::Numpad1;
	_keys[KeyboardKey::NUM2] = sf::Keyboard::Key::Numpad2;
	_keys[KeyboardKey::NUM3] = sf::Keyboard::Key::Numpad3;
	_keys[KeyboardKey::NUM4] = sf::Keyboard::Key::Numpad4;
	_keys[KeyboardKey::NUM5] = sf::Keyboard::Key::Numpad5;
	_keys[KeyboardKey::NUM6] = sf::Keyboard::Key::Numpad6;
	_keys[KeyboardKey::NUM7] = sf::Keyboard::Key::Numpad7;
	_keys[KeyboardKey::NUM8] = sf::Keyboard::Key::Numpad8;
	_keys[KeyboardKey::NUM9] = sf::Keyboard::Key::Numpad9;
	_keys[KeyboardKey::O] = sf::Keyboard::Key::O;
	_keys[KeyboardKey::P] = sf::Keyboard::Key::P;
	_keys[KeyboardKey::Q] = sf::Keyboard::Key::Q;
	_keys[KeyboardKey::R] = sf::Keyboard::Key::R;
	_keys[KeyboardKey::RALT] = sf::Keyboard::Key::RAlt;
	_keys[KeyboardKey::RCTRL] = sf::Keyboard::Key::RControl;
	_keys[KeyboardKey::RSHIFT] = sf::Keyboard::Key::RShift;
	_keys[KeyboardKey::BSLACH] = sf::Keyboard::Key::BackSlash;
	_keys[KeyboardKey::S] = sf::Keyboard::Key::S;
	_keys[KeyboardKey::SEMICOLON] = sf::Keyboard::Key::SemiColon;
	_keys[KeyboardKey::SLASH] = sf::Keyboard::Key::Slash;
	_keys[KeyboardKey::SPACE] = sf::Keyboard::Key::Space;
	_keys[KeyboardKey::SQBRCL] = sf::Keyboard::Key::RBracket;
	_keys[KeyboardKey::SQBROP] = sf::Keyboard::Key::LBracket;
	_keys[KeyboardKey::T] = sf::Keyboard::Key::T;
	_keys[KeyboardKey::TAB] = sf::Keyboard::Key::Tab;
	_keys[KeyboardKey::TILDE] = sf::Keyboard::Key::Tilde;
	_keys[KeyboardKey::U] = sf::Keyboard::Key::U;
	_keys[KeyboardKey::V] = sf::Keyboard::Key::V;
	_keys[KeyboardKey::W] = sf::Keyboard::Key::W;
	_keys[KeyboardKey::X] = sf::Keyboard::Key::X;
	_keys[KeyboardKey::Y] = sf::Keyboard::Key::Y;
	_keys[KeyboardKey::Z] = sf::Keyboard::Key::Z;
	_keys[KeyboardKey::UP] = sf::Keyboard::Key::Up;
	_keys[KeyboardKey::DOWN] = sf::Keyboard::Key::Down;
	_keys[KeyboardKey::LEFT] = sf::Keyboard::Key::Left;
	_keys[KeyboardKey::RIGHT] = sf::Keyboard::Key::Right;
}

void InputManager::Update()
{
	for (int i = 0; i < KeysCount; i++) {
		if (_key_data[i].is_realesed)
			_key_data[i].is_realesed = false;
		if (_key_data[i].is_pressed)
			_key_data[i].is_pressed = false;
		if (sf::Keyboard::isKeyPressed(_keys[i])) {
			if (!_key_data[i].is_down)
				_key_data[i].is_pressed = true;
			_key_data[i].is_down = true;
		}
		else {
			if (_key_data[i].is_down)
				_key_data[i].is_realesed = true;
			_key_data[i].is_down = false;
		}
	}

	_mouse_info.pos = { (float)sf::Mouse::getPosition(*GraphicManager::GetWindow()).x, (float)sf::Mouse::getPosition(*GraphicManager::GetWindow()).y };

	if (_mouse_info.l_button.is_pressed)
		_mouse_info.l_button.is_pressed = false;
	if (_mouse_info.l_button.is_realesed)
		_mouse_info.l_button.is_realesed = false;
	if (sf::Mouse::isButtonPressed(sf::Mouse::Button::Left)) {
		if (!_mouse_info.l_button.is_down)
			_mouse_info.l_button.is_pressed = true;
		_mouse_info.l_button.is_down = true;
	}
	else {
		if (_mouse_info.l_button.is_down)
			_mouse_info.l_button.is_realesed = true;
		_mouse_info.l_button.is_down = false;
	}

	if (_mouse_info.r_button.is_pressed)
		_mouse_info.r_button.is_pressed = false;
	if (_mouse_info.r_button.is_realesed)
		_mouse_info.r_button.is_realesed = false;
	if (sf::Mouse::isButtonPressed(sf::Mouse::Button::Right)) {
		if (!_mouse_info.r_button.is_down)
			_mouse_info.r_button.is_pressed = true;
		_mouse_info.r_button.is_down = true;
	}
	else {
		if (_mouse_info.r_button.is_down)
			_mouse_info.r_button.is_realesed = true;
		_mouse_info.r_button.is_down = false;
	}

}

bool InputManager::IsDown(unsigned char key)
{
	return _key_data[_virtual_keys[key]].is_down;
}

bool InputManager::IsUp(unsigned char key)
{
	return !_key_data[_virtual_keys[key]].is_down;
}

bool InputManager::IsPressed(unsigned char key)
{
	return _key_data[_virtual_keys[key]].is_pressed;
}

bool InputManager::IsRealesed(unsigned char key)
{
	return _key_data[_virtual_keys[key]].is_realesed;
}

bool InputManager::IsDown(KeyboardKey key)
{
	return _key_data[key].is_down;
}

bool InputManager::IsUp(KeyboardKey key)
{
	return !_key_data[key].is_down;
}

bool InputManager::IsPressed(KeyboardKey key)
{
	return _key_data[key].is_pressed;
}

bool InputManager::IsRealesed(KeyboardKey key)
{
	return _key_data[key].is_realesed;
}

Vector2F InputManager::GetMousePos()
{
	return _mouse_info.pos;
}


bool InputManager::IsPressed(MouseKey key)
{
	switch (key)
	{
	case Mouse_Left:
		return _mouse_info.l_button.is_pressed;
		break;
	case Mouse_Right:
		return _mouse_info.r_button.is_pressed;
		break;
	default:
		return false;
		break;
	}
}

bool InputManager::IsRealesed(MouseKey key)
{
	switch (key)
	{
	case Mouse_Left:
		return _mouse_info.l_button.is_realesed;
		break;
	case Mouse_Right:
		return _mouse_info.r_button.is_realesed;
		break;
	default:
		return false;
		break;
	}
}

bool InputManager::IsUp(MouseKey key)
{
	return !IsDown(key);
}

bool InputManager::IsDown(MouseKey key)
{
	switch (key)
	{
	case Mouse_Left:
		return _mouse_info.l_button.is_down;
		break;
	case Mouse_Right:
		return _mouse_info.r_button.is_down;
		break;
	default:
		return false;
		break;
	}
}

void InputManager::LinkToCode(KeyboardKey key, unsigned char code)
{
	_virtual_keys[code] = key;
}

KeyboardKey InputManager::GetPressedKey()
{
	for (unsigned i = 1; i < KeysCount; i++) {
		if (IsPressed((KeyboardKey)i))
			return (KeyboardKey)i;
	}
	return KeyboardKey::NOTHING;
}
