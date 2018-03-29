#include "BoneNode.h"
#include <log.h>


BoneNode::BoneNode()
	: mName("null")
{

}

BoneNode::~BoneNode()
{

}

void BoneNode::SetName(const std::string &name)
{
	mName = name;
}

const std::string& BoneNode::GetName() const
{
	return mName;
}

const Matrix& BoneNode::GetLocalTransform() const
{
	return mLocal;
}

void BoneNode::SetLocalTransform(const Matrix &transform)
{
	mLocal = transform;
}

const Matrix& BoneNode::GetAnimTransform(int frame)
{
	return mAnims[frame];
}

void BoneNode::SetAnimTransform(int frame, const Matrix &transform)
{ 
	mAnims[frame] = transform;
}