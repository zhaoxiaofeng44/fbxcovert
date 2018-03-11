#pragma once
#pragma once

#include <stdlib.h>
#include <vector>

template<class T>
class NodeSet
{
public:

	int Size()
	{
		return mSet.size();
	}

	T& operator[](unsigned int i)
	{
		return mSet[i];
	}

	T operator[](unsigned int i) const
	{
		return mSet[i];
	}
private:

	std::vector<T> mSet;
};
