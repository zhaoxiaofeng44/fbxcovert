#pragma once

#include <algorithm>
#include <iostream>

typedef double Number;

template<typename T,int tSize >
class Vec
{
public:

	Vec()
	{
		set(0);
	}

	template<typename... T2>
	Vec(T v, T2 ...arg)
	{
		set(0, v, arg...);
	}

	template<int vSize>
	Vec(const Vec<T, vSize> &other)
	{
		for (int i = 0; i < tSize; i++)
		{
			if (i >= vSize) 
			{
				mArr[i] = (T)0;
			}
			else 
			{
				mArr[i] = other[i];
			}
		}
	}

	inline void set(T v)
	{
		for (int i = 0; i < tSize; i++)
		{
			mArr[i] = v;
		}
	}

	inline void set(int i, T v)
	{
		if (i < tSize) {
			mArr[i] = v;
		}
	}

	template<typename... T2>
	inline void set(int i, T v, T2 ...arg)
	{
		if (i < tSize) {
			mArr[i] = v;
			set(i + 1, arg ...);
		}
	}

	inline T* get()
	{
		return mArr;
	}

	inline T& operator [](int i)
	{
		assert(i >= 0 && i < tSize);
		return mArr[i];
	}

	inline T operator [](int i) const
	{
		assert(i >= 0 && i < tSize);
		return mArr[i];
	}

	template<int vSize>
	inline bool operator ==(const Vec<T, vSize> &other) const
	{
		for (int i = 0; i < tSize && i < vSize ; i++)
		{
			if (mArr[i] != other.mArr[i])
			{
				return false;
			}
		}
		return true;
	}

	inline Vec & operator =(const Vec &other) const
	{
		return *this = other;
	}

	inline Number length2() const
	{
		Number sum = 0;
		for (int i = 0; i < tSize; i++)
		{
			sum += mArr[i] * mArr[i];
		}
		return sum;
	}

	inline Number length() const
	{
		return std::sqrt(length2());
	}

protected :
	T mArr[tSize];
};

typedef Vec<Number, 2> Vec2f;
typedef Vec<Number, 3> Vec3f;
typedef Vec<Number, 4> Vec4f;
typedef Vec<Number, 4> Quat4f;
