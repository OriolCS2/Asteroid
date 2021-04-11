#include "Color.h"

Color Color::black = Color(0, 0, 0, 1);
Color Color::blue = Color(0, 0, 1, 1);
Color Color::clear = Color(0, 0, 0, 0);
Color Color::cyan = Color(0, 1, 1, 1);
Color Color::gray = Color(0.5F, 0.5F, 0.5F, 1);
Color Color::green = Color(0, 1, 0, 1);
Color Color::grey = Color(0.5F, 0.5F, 0.5F, 1);
Color Color::magenta = Color(1, 0, 1, 1);
Color Color::red = Color(1, 0, 0, 1);
Color Color::white = Color(1, 1, 1, 1);
Color Color::yellow = Color(1, 0.92F, 0.016F, 1);
Color Color::orange = Color(1, 0.5F, 0, 1);
Color Color::purple = Color(0.5F, 0, 0.5F, 1);

void Color::Set(float r, float g, float b, float a)
{
	this->r = r;
	this->g = g;
	this->b = b;
	this->a = a;
}

void Color::Reset()
{
	r = g = b = a = 1.0F;
}
