#pragma once

#include <nodetree.h>
#include <string>
#include <map>
#include <matrix.h>


class BoneNode : public NodeTreeItem<BoneNode>
{
public:
	BoneNode();

	void					SetName(const std::string &name);
	const std::string&		GetName() const;

	const Matrix&			GetLocalTransform() const;
	void					SetLocalTransform(const Matrix &transform);

	const Matrix&			GetAnimTransform(int frame);
	void					SetAnimTransform(int frame, const Matrix &transform);
private:

	std::string				mName;
	Matrix					mLocal;

	std::map<int, Matrix>   mAnims;

};

