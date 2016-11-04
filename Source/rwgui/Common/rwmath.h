#ifndef RWMATH_H
#define RWMATH_H

#include <d2d1.h>

#pragma warning (disable : 4251)

class Vector2D
{
public:
	float x;
	float y;

	Vector2D() : x(0.0f), y(0.0f) {};
	Vector2D(float in) : x(in), y(in) {};
	Vector2D(float inX, float inY) : x(inX), y(inY) {};
	Vector2D operator+(Vector2D b) { return Vector2D(x + b.x, y + b.y); };
	Vector2D operator-(Vector2D b) { return Vector2D(x - b.x, y - b.y); };
	Vector2D operator*(Vector2D b) { return Vector2D(x * b.x, y * b.y); };
	Vector2D operator/(Vector2D b) { return Vector2D(x / b.x, y / b.y); };
	Vector2D operator+(float val) { return Vector2D(x + val, y + val); };
	Vector2D operator-(float val) { return Vector2D(x - val, y - val); };
	Vector2D operator/(float val) { return Vector2D(x / val, y / val); };
	Vector2D operator*(float val) { return Vector2D(x * val, y * val); };
	Vector2D& operator+=(Vector2D b) { x += b.x; y+=b.y; return *this; };
	Vector2D& operator-=(Vector2D b) { x -= b.x; y -= b.y; return *this; }
};

class Bounds
{
public:
	Vector2D Pos;
	Vector2D Size;

	Bounds() : Pos(Vector2D(0,0)),Size(Vector2D(0,0)) {};
	Bounds(Vector2D inPos, Vector2D inSize) : Pos(inPos),Size(inSize) {};
	Bounds(float posx, float posy, float sizex, float sizey) : Pos(Vector2D(posx,posy)),Size(Vector2D(sizex,sizey)) {};
	Bounds operator+(Bounds b) { return Bounds(Pos+b.Pos,Size+b.Size); };
	Bounds operator-(Bounds b) { return Bounds(Pos-b.Pos,Size-b.Size); };
	Bounds& operator+=(Bounds b) { Pos = Pos + b.Pos; return *this; };
	Bounds operator/(float val) { return Bounds(Pos.x/val,Pos.y / val,Size.x /val,Size.y / val); };
	bool IsInBound(Vector2D b) {
		return (
			b.x >= Pos.x
			&& b.y >= Pos.y
			&& b.x <= Pos.x+Size.x
			&& b.y <= Pos.y+Size.y
			); };
	D2D_RECT_F ToD2DRect() {
		D2D_RECT_F outRect;
		outRect.left = Pos.x;
		outRect.right = Pos.x + Size.x;
		outRect.bottom = Pos.y + Size.y;
		outRect.top = Pos.y;
		return outRect;
	};
};

class Color
{
public:
	float r;
	float g;
	float b;
	float a;
	Color() : r(0.0f),g(0.0f),b(0.0f),a(1.0f) {};
	Color(float inr, float ing, float inb, float ina = 1.0f) : r(inr), g(ing), b(inb), a(ina) {};
	D2D1_COLOR_F ToD2D1ColorF() { return D2D1::ColorF(r,g,b,a); };
	bool operator==(Color bc) { return (r == bc.r && g == bc.g && b == bc.b && a == bc.a); };
	Color operator*(float val) { return Color(r*val, g*val, b*val, a*val); };
};

#endif