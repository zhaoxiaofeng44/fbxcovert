#pragma once
#include <vec3f.h>
#include <config.h>

class Point
{
public:
	const vec3f&	GetPosition() const;
	void			SetPosition(const vec3f &pos);
private:
	vec3f			mPosition;
};

inline void Point::SetPosition(const vec3f &pos)
{
	mPosition= pos;
}

inline const vec3f& Point::GetPosition() const
{
	return mPosition;
}
