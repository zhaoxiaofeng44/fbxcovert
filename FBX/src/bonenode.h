#pragma once

#include <nodetree.h>
#include <string>
#include <matrix4x4.h>


class BoneNode : public NodeTreeItem<BoneNode>
{
public:
	BoneNode();
	~BoneNode();

	void					SetName(const std::string &name);
	const std::string&		GetName() const;

	const Matrix4x4&		GetLocalTransform() const;
	void					SetLocalTransform(const Matrix4x4 &transform);
private:

	std::string				mName;
	Matrix4x4				mLocal;
};

