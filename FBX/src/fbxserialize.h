#pragma once
#include "node.h"
class FbxSerialize
{
public:
	static Node Get(const std::string &);
	static void Set(const std::string &, Node &);
};

