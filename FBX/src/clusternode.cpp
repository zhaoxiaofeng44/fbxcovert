#include "clusternode.h"

ClusterNode::ClusterNode()
{
}


void ClusterNode::SetName(const std::string &name)
{
	mName = name;
}

const std::string& ClusterNode::GetName() const
{
	return mName;
}

void ClusterNode::SetLinkName(const std::string &name)
{
	mLinkName = name;
}

const std::string&	ClusterNode::GetLinkName() const
{
	return mLinkName;
}

void ClusterNode::AddWeight(const int& index, const float& weight)
{
	mWeights.push_back({ index,weight });
}

std::vector<Weight>& ClusterNode::GetWeights()
{
	return mWeights;
}
		
const Matrix& ClusterNode::GetTransformMatrix() const
{
	return mMatrix;
}

void ClusterNode::SetTransformMatrix(const Matrix &transform)
{
	mMatrix = transform;
}

const Matrix& ClusterNode::GetLinkTransformLinkMatrix() const
{
	return mLinkMatrix;
}

void ClusterNode::SetLinkTransformLinkMatrix(const Matrix &transform)
{
	mLinkMatrix = transform;
}
				