#pragma once

class  Color
{
public:

	Color() : r(0.0f), g(0.0f), b(0.0f), a(1.0f) {}
	Color(float r, float g, float b, float a = 1.0f) : r(r), g(g), b(b), a(a) {}

	void Set(float r, float g, float b, float a = 1.0f);
	void Reset();

	float* operator & ()
	{
		return (float*)this;
	}

	Color& operator +=(const Color& rhs)
	{
		r += rhs.r;
		g += rhs.g;
		b += rhs.b;

		return *this;
	}

	Color operator *(float scalar) const
	{
		return Color(r * scalar, g * scalar, b * scalar, a);
	}

	static Color black;
	static Color blue;
	static Color clear;
	static Color cyan;
	static Color gray;
	static Color green;
	static Color grey;
	static Color magenta;
	static Color red;
	static Color white;
	static Color yellow;
	static Color orange;
	static Color purple;

public:
	float r, g, b, a;
};