#pragma once

#include <bonenode.h>
#include <meshnode.h>
#include <config.h>
#include <string>
#include <map>
#include <vector>

class AnimNode;
class FlimBoxNode;
class Node
{
public:
	Node();
	~Node();

	/*------------ BONE NODE -------------*/
	BoneNode*			GetBoneNodeRoot();
	BoneNode*			GetBoneNodeByName(const std::string &name);
	void				AddChildBoneNode(BoneNode* parent, BoneNode* node);

	/*------------ MESH NODE -------------*/
	MeshNode*			GetMeshNodeRoot();
	void				AddChildMeshNode(MeshNode* parent, MeshNode* node);
	MeshNode*			GetCurrentMeshNode();

private:
	NodeTree<BoneNode>	mBoneNodes;
	NodeTree<MeshNode>	mMeshNodes;

};