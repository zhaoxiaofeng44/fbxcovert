#pragma once

#include <vml.h>
#include <assert.h>
#include <vec.h>

class Matrix : public Vec<Number, 16>
{
public:
	Matrix()
		: Matrix((Number)0)
	{
		mArr[0] = 1.f; mArr[5] = 1.f; mArr[10] = 1.f; mArr[15] = 1.f;
	}

	Matrix(const Number &v)
		: Vec<Number, 16>(v)
	{}

	void setIdentity()
	{
		*this = Matrix();
	}

	void setQ(const Quat4f &v)
	{
		Number x2 = 2.0f * v[0], y2 = 2.0f * v[1], z2 = 2.0f * v[2];

		Number xx2 = x2 * v[0], xy2 = x2 * v[1];
		Number xz2 = x2 * v[2], xw2 = x2 * v[3];

		Number yy2 = y2 * v[1], yz2 = y2 * v[2], yw2 = y2 * v[3];
		Number zz2 = z2 * v[2], zw2 = z2 * v[3];

		mArr[0] = 1.f - yy2 - zz2;	mArr[1] = xy2 - zw2;		mArr[2] = xz2 + yw2;
		mArr[4] = xy2 + zw2;		mArr[5] = 1.f - xx2 - zz2;	mArr[6] = yz2 - xw2;
		mArr[8] = xz2 - yw2;		mArr[9] = yz2 + xw2;		mArr[10] = 1.f - xx2 - yy2;
	}

	inline Quat4f getQ() const
	{
		Quat4f quat;
		float trace = mArr[0] + mArr[5] + mArr[10];
		if (trace > 0.0f)
		{
			float s = sqrtf(trace + 1.0f);
			quat[3] = s / 2.0f;
			s = 0.5f / s;
			quat[0] = (mArr[9] - mArr[6]) * s;
			quat[1] = (mArr[2] - mArr[8]) * s;
			quat[2] = (mArr[4] - mArr[1]) * s;
		}
		else
		{
			const int nxt[3] = { 1, 2, 0 };
			float q[3];
			int i = 0;
			if (mArr[5] > mArr[0]) i = 1;
			if (mArr[10] > mArr[i * 4 + i]) i = 2;
			int j = nxt[i];
			int k = nxt[j];
			float s = sqrtf((mArr[i * 4 + i] - (mArr[j * 4 + j] + mArr[k * 4 + k])) + 1.0);
			q[i] = s * 0.5f;
			if (s != 0.f) s = 0.5f / s;
			quat[3] = (mArr[k * 4 + j] - mArr[j * 4 + k]) * s;
			q[j] = (mArr[j * 4 + i] + mArr[i * 4 + j]) * s;
			q[k] = (mArr[k * 4 + i] + mArr[i * 4 + k]) * s;
			quat[0] = q[0];
			quat[1] = q[1];
			quat[2] = q[2];
		}
		return quat;
	}

	inline void setT(Vec3f t)
	{
		mArr[3] = t[0];
		mArr[7] = t[1];
		mArr[11] = t[2];
	}

	inline Vec3f getT() const
	{
		Vec3f t;
		t[0] = mArr[3];
		t[1] = mArr[7];
		t[2] = mArr[11];
		return t;
	}

	inline void setS(const Vec3f &s)
	{
		mArr[0] = s[0];
		mArr[5] = s[1];
		mArr[10] = s[2];
	}

	inline Vec3f getS() const
	{
		Vec3f s;
		s[0] = Vec3f(mArr[0], mArr[4], mArr[8]).length();
		s[1] = Vec3f(mArr[1], mArr[5], mArr[9]).length();
		s[2] = Vec3f(mArr[2], mArr[6], mArr[10]).length();
		return s;
	}

	inline void setTRS(const Vec3f& transform, const Quat4f& orientation,const Vec3f& scale)
	{
		Matrix rot4x4;
		rot4x4.setQ(orientation);

		// Set up final matrix with scale, rotation and translation
		mArr[0] = scale[0] * rot4x4[0]; mArr[1] = scale[1] * rot4x4[1]; mArr[2] = scale[2] * rot4x4[2];  mArr[3] = transform[0];
		mArr[4] = scale[0] * rot4x4[4]; mArr[5] = scale[1] * rot4x4[5]; mArr[6] = scale[2] * rot4x4[6];  mArr[7] = transform[1];
		mArr[8] = scale[0] * rot4x4[8]; mArr[9] = scale[1] * rot4x4[9]; mArr[10] = scale[2] * rot4x4[10]; mArr[11] = transform[2];
		// No projection term
		mArr[12] = 0; mArr[13] = 0; mArr[14] = 0; mArr[15] = 1;
	}


};


