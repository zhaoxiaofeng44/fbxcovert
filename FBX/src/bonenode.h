#pragma once

#include <nodetree.h>
#include <string>
#include <matrix.h>


class BoneNode : public NodeTreeItem<BoneNode>
{
public:
	BoneNode();
	~BoneNode();

	void					SetName(const std::string &name);
	const std::string&		GetName() const;

	const Matrix&			GetLocalTransform() const;
	void					SetLocalTransform(const Matrix &transform);
private:

	std::string				mName;
	Matrix				mLocal;
};

