#ifndef VECTOR2_H
#define VECTOR2_H

#include <math.h>

#define PI 3.14159265

class Vector2
{
public:
	Vector2() : x(0), y(0) {}
	Vector2(float _x, float _y) : x(_x), y(_y) {}
	float x, y;

	Vector2 operator-(const Vector2& v)
	{
		Vector2 vec;
		vec.x = this->x - v.x; 
		vec.y = this->y - v.y; 
		return vec;
	}

	Vector2 operator/(float value)
	{
		Vector2 vec;
		vec.x = this->x / value; 
		vec.y = this->y / value; 
		return vec;
	}

	Vector2 operator*(float value)
	{
		Vector2 vec;
		vec.x = this->x * value; 
		vec.y = this->y * value; 
		return vec;
	}

	void operator+=(const Vector2& v)
	{
		this->x += v.x;
		this->y += v.y;
	}

	float Magnitude() { return sqrtf((x * x) + (y * y)); }

	void Normalize() 
	{ 
		float mag = Magnitude();
		x /= mag;
		y /= mag;
	}

};

#endif