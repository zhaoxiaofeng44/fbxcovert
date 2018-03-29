#pragma once

#include <nodetree.h>
#include <face.h>
#include <point.h>
#include <weight.h>
#include <vector>
#include <matrix.h>

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

	const Matrix&			GetTransformMatrix() const;
	void					SetTransformMatrix(const Matrix &transform);

	const Matrix&			GetLinkTransformLinkMatrix() const;
	void					SetLinkTransformLinkMatrix(const Matrix &transform);

private:

	std::string				mName;
	std::string				mLinkName;
	std::vector<Weight>		mWeights;

	Matrix					mMatrix;
	Matrix					mLinkMatrix;
};
