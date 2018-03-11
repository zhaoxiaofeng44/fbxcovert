#pragma once
#include <vector>
#include <nodetree.h>
#include <face.h>
#include <point.h>
#include <element.h>
#include <matrix4x4.h>
#include <clusternode.h>
#include <materialnode.h>

class MeshNode : public NodeTreeItem<MeshNode>
{
public:
	MeshNode();
	~MeshNode();

	void							SetName(const std::string &name);
	const std::string&				GetName() const;

	void							SetLinkName(const std::string &name);
	const std::string&				GetLinkName() const;

	std::vector<Face>&				GetFaces(int size = -1);
	std::vector<Point>&				GetPoints(int size = -1);
	std::vector<ElementVec2f>&		GetUVs(int size = -1);
	std::vector<ElementVec4f>&		GetNormals(int size = -1);

	const Matrix4x4&				GetLocalTransform() const;
	void							SetLocalTransform(const Matrix4x4 &transform);

	ClusterNode&					NewClusterNode();
	std::vector<ClusterNode>&		GetClusterNodes();

	MaterialNode&					NewMaterialNode();
	std::vector<MaterialNode>&		GetMaterialNodes();
private:

	std::string				  mName;
	std::string				  mLinkName;

	std::vector<Face>		  mFaces;
	std::vector<Point>		  mPoints;
	std::vector<ElementVec2f> mUVs;
	std::vector<ElementVec4f> mNormals;

	Matrix4x4				  mGlobal;
	Matrix4x4				  mLocal;

	std::vector<ClusterNode>  mClusterNodes;
	std::vector<MaterialNode> mMaterialNodes;
};
