#pragma once
#include <string>
#include <map>
#include <vector>
#include "config.h"
#include "bonenode.h"
#include "meshnode.h"
#include "animlayer.h"

class Node
{
public:
	Node();

	Node(const Node &);

	/*------------ BONE NODE -------------*/
	BoneNode*			GetBoneNodeRoot();
	BoneNode*			GetBoneNodeByName(const std::string &name);
	void				AddChildBoneNode(BoneNode* parent, BoneNode* node);

	/*------------ MESH NODE -------------*/
	MeshNode*			GetMeshNodeRoot();
	void				AddChildMeshNode(MeshNode* parent, MeshNode* node);
	MeshNode*			GetCurrentMeshNode();

	/*------------ ANIM NODE -------------*/
	AnimLayer&			GetAnimLayer();
	void				SetAnimLayer(AnimLayer& layer);

	void                Clear();

	void                Copy(const Node &other);

private:
	NodeTree<BoneNode>	mBoneNodes;
	NodeTree<MeshNode>	mMeshNodes;
	AnimLayer           mAnimLayer;

};