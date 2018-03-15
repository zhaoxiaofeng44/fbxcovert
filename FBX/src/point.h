#pragma once
#include <vec.h>
#include <config.h>

class Point
{
public:
	const Vec3f&	GetPosition() const;
	void			SetPosition(const Vec3f &pos);
private:
	Vec3f			mPosition;
};

inline void Point::SetPosition(const Vec3f &pos)
{
	mPosition= pos;
}

inline const Vec3f& Point::GetPosition() const
{
	return mPosition;
}
