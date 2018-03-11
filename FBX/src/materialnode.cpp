#include "materialnode.h"

MaterialNode::MaterialNode()
{
}

MaterialNode::~MaterialNode()
{
}

void MaterialNode::SetName(const std::string &name)
{
	mName = name;
}

const std::string& MaterialNode::GetName() const
{
	return mName;
}

MaterialPropNode& MaterialNode::GetPropNode(std::string name)
{
	for (auto &v : mProps) {
		if (v.type == name) {
			return v;
		}
	}
	return mProps[0];
}

void MaterialNode::SetPropNode(const MaterialPropNode &v)
{
	mProps.push_back(v);
}

const std::vector<MaterialPropNode>& MaterialNode::GetPropNodes()
{
	return mProps;
}