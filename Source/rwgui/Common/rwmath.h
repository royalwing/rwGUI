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
	bool operator==(const Vector2D& Other) const { return  x == Other.x || y == Other.y; };
	bool operator!=(const Vector2D& Other) const { return !(*this==Other); };
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

class Transform2D
{
public:
	Vector2D Position = Vector2D(0.0f, 0.0f);
	float Rotation = 0.0f;
	Vector2D Scale = Vector2D(1.0f, 1.0f);

	void SetPosition(Vector2D InPosition) { Position = InPosition; };
	Vector2D GetPosition() const { return Position; };

	Transform2D() {};
	Transform2D(Vector2D inTranslation, float inRotation = 0.0f, Vector2D inScale = Vector2D(1.0f, 1.0f))
		: Position(inTranslation), Rotation(inRotation), Scale(inScale)
	{		
	}

	D2D1_MATRIX_3X2_F ToD2D1Matrix() const
	{
		D2D1_MATRIX_3X2_F pos = D2D1::Matrix3x2F::Translation(Position.x, Position.y);
		D2D1_MATRIX_3X2_F rot = D2D1::Matrix3x2F::Rotation(Rotation);
		D2D1_MATRIX_3X2_F scale = D2D1::Matrix3x2F::Scale(Scale.x, Scale.y);
		return rot*pos*scale;
	}

	Transform2D Inverse() const
	{
		Transform2D Result;
		Result.Position = Vector2D(-Position.x, -Position.y);
		Result.Rotation = -Rotation;
		Result.Scale = Vector2D(Scale.x != 0 ? 1/Scale.x : 0.0f, Scale.y != 0 ? 1/Scale.y : 0.0f);
		return Result;
	}

	Transform2D operator*(const Transform2D& Other) const
	{
		D2D1_MATRIX_3X2_F A = ToD2D1Matrix();
		D2D1_MATRIX_3X2_F B = Other.ToD2D1Matrix();
		D2D1_MATRIX_3X2_F result = A * B;
		return Transform2D(Vector2D(result._31, result._32), 0, Vector2D(result._11, result._22));
	}
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

	Color(const float& F)
		: r(F), g(F), b(F), a(1.0f)
	{
		
	}

	Color operator+(const Color& Other) const
	{
		Color result;
		result.r = r + Other.r;
		result.b = b + Other.b;
		result.g = g + Other.g;
		return result;
	};

	Color operator-(const Color& Other) const
	{
		Color result;
		result.r = r - Other.r;
		result.b = b - Other.b;
		result.g = g - Other.g;
		return result;
	};

	Color operator*(const Color& Other) const
	{
		Color result;
		result.r = r * Other.r;
		result.b = b * Other.b;
		result.g = g * Other.g;
		return result;
	};

	Color operator/(const Color& Other) const
	{
		Color result;
		result.r = r / Other.r;
		result.b = b / Other.b;
		result.g = g / Other.g;
		return result;
	};

	Color operator+(const float& Other) const
	{
		Color result;
		result.r = r + Other;
		result.g = g + Other;
		result.b = b + Other;
		return result;
	}

	Color operator-(const float& Other) const
	{
		Color result;
		result.r = r - Other;
		result.g = g - Other;
		result.b = b - Other;
		return result;
	}

	Color operator*(const float& Other) const
	{
		Color result;
		result.r = r * Other;
		result.g = g * Other;
		result.b = b * Other;
		return result;
	}

	Color operator/(const float& Other) const
	{
		Color result;
		result.r = r / Other;
		result.g = g / Other;
		result.b = b / Other;
		return result;
	}
};

namespace Colors
{
	const Color Red = Color(1.0f, 0.0f, 0.0f);
	const Color Green = Color(0.0f, 1.0f, 0.0f);
	const Color Blue = Color(0.0f, 0.0f, 1.0f);
	const Color Yellow = Color(1.0f, 1.0f, 0.0f);
}

#endif