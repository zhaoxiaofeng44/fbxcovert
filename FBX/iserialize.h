#pragma once
#include "node.h"

class ISerialize
{
public:

	virtual void Set(const std::string &) = 0;
	virtual Node * Get(const std::string &) = 0;

};

