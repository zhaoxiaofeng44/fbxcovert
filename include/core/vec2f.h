#pragma once
#ifdef TEST
#include <algorithm>
#include <iostream>

template<typename T>
class vec2
{
public:
	vec2() : x(0), y(0) {}
	vec2(T _x) : x(_x), y(_x){}
	vec2(T _x, T _y) : x(_x), y(_y){}

	vec2 operator + (const vec2 &v) const{
		return vec2(x + v.x, y + v.y);
	}
	vec2 operator - (const vec2 &v) const{
		return vec2(x - v.x, y - v.y);
	}
	vec2 operator / (const vec2 &v) const{
		return vec2(x / v.x, y / v.y);
	}
	vec2 operator * (const vec2 &v) const{
		return vec2(x * v.x, y * v.y);
	}
	vec2& operator *= (const vec2 &v){
		x *= v.x;
		y *= v.y;
		return *this;
	}
	vec2& operator /= (const vec2 &v){
		x /= v.x;
		y /= v.y;
		return *this;
	}
	vec2 operator *(const float &f)
	{
		return vec2(x * f, y * f);
	}
	friend vec2 operator *(const float &f, const vec2 &v)
	{
		return vec2(f * v.x, f * v.y);
	}

	bool operator==(const vec2 &other) const
	{
		return x == other.x && y == other.y;
	}
	
	float length() const;

	T x, y;

	friend std::ostream& operator<<(std::ostream &o, const vec2<T> &v)
	{
		if(typeid(T) == typeid(int))
			o << "vec2i( " << v.x << ", " << v.y << ')';
		else
			o << "vec2f( " << v.x << ", " << v.y << ')';
		return o;
	}
};

template<typename T>
inline float vec2<T>::length() const
{
	return sqrtf(x * x + y * y);
}

class vec2i : public vec2<int> {
public:

	static vec2i & zero() {
		return *pZero;
	}
	static vec2i *pZero;
};

class vec2f : public vec2<float> {
public:

	vec2f()
	{
		x = 0;
		y = 0;
	}

	vec2f(double _x, double _y)
	{
		x = static_cast<float>(_x);
		y = static_cast<float>(_y);
	}

	static const vec2f ZERO;

	static vec2f & zero() {
		return *pZero;
	}
	static vec2f *pZero;
};
#endif // TEST