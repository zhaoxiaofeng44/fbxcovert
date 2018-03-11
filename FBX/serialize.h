#pragma once
#include "node.h"

class ISerialize
{
public:
	virtual void Set(Node *) = 0;
	virtual void Get(Node *) = 0;
};

