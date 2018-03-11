#pragma once
#include <config.h>
#include <vec2f.h>
#include <vec3f.h>
#include <vec4f.h>

template<typename T>
class Element
{
public:
	void					SetName(const std::string &name);
	const std::string&		GetName() const;

	void					SetST(int vertexIndex, const T& st);
	const T&				GetST(int vertexIndex) const;
	std::vector<T>&			GetSTs();

private:
	std::string				mName;
	std::vector<T>			mSTs;
};

template<typename T>
void Element<T>::SetName(const std::string &name)
{
	mName = name;
}

template<typename T>
const std::string& Element<T>::GetName() const
{
	return mName;
}

template<typename T>
inline void Element<T>::SetST(int vertexIndex, const T& st)
{
	if (vertexIndex >= mSTs.size())
	{
		mSTs.resize(vertexIndex + 1);
	} 
	mSTs[vertexIndex] = st;
}

template<typename T>
inline const T& Element<T>::GetST(int vertexIndex) const
{
	if (vertexIndex < mSTs.size())
	{
		return mSTs[vertexIndex];
	}
	return T::zero();
}

template<typename T>
inline std::vector<T>& Element<T>::GetSTs()
{
	return mSTs;
}

typedef  Element<vec2f> ElementVec2f;
typedef  Element<vec3f> ElementVec3f;
typedef  Element<vec4f> ElementVec4f;
