#pragma once

#include <nodetree.h>
#include <face.h>
#include <point.h>
#include <weight.h>
#include <vector>
#include <matrix4x4.h>

class ClusterNode
{
public:
	ClusterNode();
	~ClusterNode();

	void					SetName(const std::string &name);
	const std::string&		GetName() const;

	void					SetLinkName(const std::string &name);
	const std::string&		GetLinkName() const;

	std::vector<Weight>&	GetWeights();
	void                    AddWeight(const int& index, const float& weight);

	const Matrix4x4&		GetTransformMatrix() const;
	void					SetTransformMatrix(const Matrix4x4 &transform);

	const Matrix4x4&		GetLinkTransformLinkMatrix() const;
	void					SetLinkTransformLinkMatrix(const Matrix4x4 &transform);

private:

	std::string						mName;
	std::string						mLinkName;

	std::vector<Weight>				mWeights;

	Matrix4x4						mMatrix;
	Matrix4x4						mLinkMatrix;
};
