#include "meshnode.h"

MeshNode::MeshNode()
{
}

MeshNode::~MeshNode()
{
	
}

void MeshNode::SetName(const std::string &name)
{
	mName = name;
}

const std::string& MeshNode::GetName() const
{
	return mName;
}

void MeshNode::SetLinkName(const std::string &name)
{
	mLinkName = name;
}

const std::string&	MeshNode::GetLinkName() const
{
	return mLinkName;
}


std::vector<Face>& MeshNode::GetFaces(int size)
{
	if (size >= 0) {
		mFaces.resize(size);
	}
	return mFaces;
}

std::vector<Point>& MeshNode::GetPoints(int size)
{
	if (size >= 0) {
		mPoints.resize(size);
	}
	return mPoints;
}

std::vector<ElementVec2f>& MeshNode::GetUVs(int size)
{
	if (size >= 0) {
		mUVs.resize(size);
	}
	return mUVs;
}

std::vector<ElementVec4f>& MeshNode::GetNormals(int size) 
{
	if (size >= 0) {
		mNormals.resize(size);
	}
	return mNormals;
}

const Matrix4x4& MeshNode::GetLocalTransform() const
{
	return mLocal;
}

void MeshNode::SetLocalTransform(const Matrix4x4 &transform)
{
	mLocal = transform;
}

ClusterNode& MeshNode::NewClusterNode()
{
	mClusterNodes.resize(mClusterNodes.size() + 1);
	return mClusterNodes[mClusterNodes.size() - 1];
}

std::vector<ClusterNode>& MeshNode::GetClusterNodes()
{
	return mClusterNodes;
}

MaterialNode& MeshNode::NewMaterialNode()
{
	mMaterialNodes.resize(mMaterialNodes.size() + 1);
	return mMaterialNodes[mMaterialNodes.size() - 1];
}

std::vector<MaterialNode>& MeshNode::GetMaterialNodes()
{
	return mMaterialNodes;
}
