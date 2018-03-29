#pragma once
#include "iserialize.h"
#include "json/json.h"

class JsonSerialize
	: public ISerialize
{
public:

	virtual void Set(const std::string &);
	virtual Node * Get(const std::string &);
protected:

	Node mNode;
};

