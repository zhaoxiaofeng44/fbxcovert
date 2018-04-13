#pragma once
#include "node.h"
#include "json/json.h"

class JsonSerialize
{
public:
	static Node Get(const std::string &);
	static void Set(const std::string &, Node &);

};

