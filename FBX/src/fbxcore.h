#pragma once
#include <fbxsdk.h>
#include <vec2f.h>
#include <vec3f.h>
#include <vec4f.h>
#include <matrix4x4.h>
#include <vector>
#include <string>
#include <map>
#include <face.h>
#include <point.h>
#include <node.h>

using namespace fbxsdk;

class FBXCore
{
public:
	FBXCore(const std::string &filename);
	~FBXCore();
	
	std::shared_ptr<Node> mNode;
	FbxImporter* mImporter;

	bool        ProcessBoneNodes(FbxNode* pNode, BoneNode* parentBoneNode);
	bool        ProcessMeshNodes(FbxNode* pNode, MeshNode* parentMeshNode);

	
	BoneNode*	ProcessBoneNode(FbxNode* pNode, BoneNode* parent);
	MeshNode*	ProcessMeshNode(FbxNode* pNode, MeshNode* parent);

	void		BuildMaterial(FbxNode* pNode, MeshNode* meshNode);
	void		BuildMeshNode(FbxMesh* pMesh, MeshNode* meshNode);
	void        BuildMeshSkin(FbxMesh* pMesh, MeshNode* meshNode);

	void		ProcessPoses(FbxNode* pNode, FbxImporter* importer);

};

//// Get the matrix of the given pose
//FbxAMatrix GetPoseMatrix(FbxPose* pPose, int pNodeIndex)
//{
//	FbxAMatrix lPoseMatrix;
//	FbxMatrix lMatrix = pPose->GetMatrix(pNodeIndex);
//
//	memcpy((double*)lPoseMatrix, (double*)lMatrix, sizeof(lMatrix.mData));
//
//	return lPoseMatrix;
//}
//
//// Get the geometry offset to a node. It is never inherited by the children.
//FbxAMatrix GetGeometry(FbxNode* pNode)
//{
//	const FbxVector4 lT = pNode->GetGeometricTranslation(FbxNode::eSourcePivot);
//	const FbxVector4 lR = pNode->GetGeometricRotation(FbxNode::eSourcePivot);
//	const FbxVector4 lS = pNode->GetGeometricScaling(FbxNode::eSourcePivot);
//
//	return FbxAMatrix(lT, lR, lS);
//}

//const FbxArray<FbxPose *> & lPoseArray = gSceneContext->GetPoseArray();
//for (int lPoseIndex = 0; lPoseIndex < lPoseArray.GetCount(); ++lPoseIndex)
//{
//	if (lPoseArray[lPoseIndex]->IsBindPose())
//	{
//		glutAddMenuEntry(lPoseArray[lPoseIndex]->GetName(), lPoseIndex);
//		lBindPoseCount++;
//	}
//}