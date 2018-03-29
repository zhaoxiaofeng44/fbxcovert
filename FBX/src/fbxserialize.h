#pragma once
#include "iserialize.h"

class FbxSerialize
	: public ISerialize
{
public:
	virtual void Set(const std::string &);
	virtual Node * Get(const std::string &);
protected:

	Node mNode;
};

