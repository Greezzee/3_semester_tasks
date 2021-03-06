#include "DrawData.h"
DrawData::DrawData() :
	spriteID(0), position({ 0, 0 }), layer(0), rotation(0), frame(0), color(Color::White()), shader(nullptr) {}
DrawData::DrawData(unsigned id, unsigned lr, Vector2F& pos) :
	spriteID(id), position(pos), layer(lr), rotation(0), frame(0), color(Color::White()), shader(nullptr) {}
DrawData::DrawData(unsigned id, unsigned lr, float x, float y) :
	spriteID(id), position({ x, y }), layer(lr), rotation(0), frame(0), color(Color::White()), shader(nullptr) {}

DrawData::DrawData(unsigned id, unsigned lr, Vector2F& pos, float rot) :
	spriteID(id), position(pos), layer(lr), rotation(rot), frame(0), color(Color::White()), shader(nullptr) {}
DrawData::DrawData(unsigned id, unsigned lr, float x, float y, float rot) :
	spriteID(id), position({ x, y }), layer(lr), rotation(rot), frame(0), color(Color::White()), shader(nullptr) {}

DrawData::DrawData(unsigned id, unsigned lr, Vector2F& pos, unsigned fr) :
	spriteID(id), position(pos), layer(lr), rotation(0), frame(fr), color(Color::White()), shader(nullptr) {}
DrawData::DrawData(unsigned id, unsigned lr, float x, float y, unsigned fr) :
	spriteID(id), position({ x, y }), layer(lr), rotation(0), frame(fr), color(Color::White()), shader(nullptr) {}

DrawData::DrawData(unsigned id, unsigned lr, Vector2F& pos, float rot, unsigned fr) :
	spriteID(id), position(pos), layer(lr), rotation(rot), frame(fr), color(Color::White()), shader(nullptr) {}
DrawData::DrawData(unsigned id, unsigned lr, float x, float y, float rot, unsigned fr) :
	spriteID(id), position({ x, y }), layer(lr), rotation(rot), frame(fr), color(Color::White()), shader(nullptr) {}
