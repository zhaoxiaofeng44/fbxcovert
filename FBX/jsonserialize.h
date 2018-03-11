#pragma once
#include "serialize.h"
#include "json/json.h"

class JsonSerialize
	: public ISerialize
{
public:
	virtual void Set(Node *node);
	
	virtual void Get(Node *node);

	void Save(std::string name);

	void Read(std::string name);
protected:

	Json::Value mJsonValue;
};

