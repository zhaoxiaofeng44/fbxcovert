#pragma once

#include <config.h>
#include <vec2f.h>
#include <vec3f.h>

class Face
{
public:

	void			SetPointSize(const int&  faceSize);

	const int&      GetPointSize() const;

	void			SetVertexIndex(const int&  facePointIndex, const int&  vertexIndex);

	const int& 		GetVertexIndex(const int&  facePointIndex) const;

private:

	int	mPointSize;
	std::vector<int> mVerticeIndex;
};

inline void Face::SetPointSize(const int&  faceSize)
{
	mPointSize = faceSize;
	mVerticeIndex.resize(faceSize);
}

inline const int&  Face::GetPointSize() const
{
	return mPointSize;
}

inline void Face::SetVertexIndex(const int&  facePointIndex, const int&  vertexIndex)
{
	mVerticeIndex[facePointIndex] = vertexIndex;
}

inline const int&  Face::GetVertexIndex(const int&  facePointIndex) const
{
	return mVerticeIndex[facePointIndex];
}
