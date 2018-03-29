#pragma once

#include <vector>
#include <string>
#include <node.h>
#include <fbxsdk.h>
using namespace fbxsdk;

class FBXCore
{
public:

	FBXCore(const std::string &filename, Node* pNode = NULL);

private:
	bool        ProcessBoneNodes(FbxNode* pNode, BoneNode* parentBoneNode);
	bool        ProcessMeshNodes(FbxNode* pNode, MeshNode* parentMeshNode);

	BoneNode*	ProcessBoneNode(FbxNode* pNode, BoneNode* parent);
	MeshNode*	ProcessMeshNode(FbxNode* pNode, MeshNode* parent);

	void		BuildMaterial(FbxNode* pNode, MeshNode* meshNode);
	void		BuildMeshNode(FbxMesh* pMesh, MeshNode* meshNode);
	void        BuildMeshSkin(FbxMesh* pMesh, MeshNode* meshNode);

	void		ProcessPoses(FbxNode * pNode, FbxImporter* importer);
	void		ProcessPose(FbxNode * pNode, AnimSample* animSample);

	Node* mNode;
	
};
