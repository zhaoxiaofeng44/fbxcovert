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

	inline void set(T* v, int size)
	{
		for (int i = 0; i < tSize; i++)
		{
			set(i, i >= size ? (T)0 : v[i]);
		}
	}

	template<typename... T2>
	inline void set(int i, T v, T2 ...arg)
	{
		if (i < tSize) {
			set(i, v);
			set(i + 1, arg ...);
		}
	}

	template<int vSize >
	inline void set(const Vec<T,vSize> &v)
	{
		set(v.get(), vSize);
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

	template<int vSize>
	inline Vec & operator =(const Vec<T, vSize> &other) const
	{
		set(other);
		return *this;
	}

	inline T length2() const
	{
		T sum = 0;
		for (int i = 0; i < tSize; i++)
		{
			sum += mArr[i] * mArr[i];
		}
		return sum;
	}

	inline T length() const
	{
		return std::sqrt(length2());
	}

	static Vec<T, 0> zero() {
		
		static Vec<T, 0> _zero;
		return _zero;
	};

protected :
	T mArr[tSize];
};


typedef Vec<Number, 2> Vec2f;
typedef Vec<Number, 3> Vec3f;
typedef Vec<Number, 4> Vec4f;
typedef Vec<Number, 4> Quat4f;
