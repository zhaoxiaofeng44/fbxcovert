#include <node.h>


Node::Node()
{

}

Node::Node(const Node &other)
{
	Copy(other);
}

/*------------ BONE NODE -------------*/

BoneNode *Node::GetBoneNodeRoot()
{
	return mBoneNodes.mRoot;
}

void Node::AddChildBoneNode(BoneNode* parent, BoneNode* node)
{
	mBoneNodes.AddChild(parent, node);
}

BoneNode* Node::GetBoneNodeByName(const std::string &name)
{
	for (BoneNode* node = mBoneNodes.mRoot;
		node != NULL;
		node = mBoneNodes.GetNextChildFirst(node))
	{
		if (node->GetName() == name) {
			return node;
		}
	}
	//empty
	return NULL;
}

/*------------ MESH NODE -------------*/

MeshNode* Node::GetMeshNodeRoot()
{
	return mMeshNodes.mRoot;
}

void Node::AddChildMeshNode(MeshNode* parent, MeshNode *node)
{
	mMeshNodes.AddChild(parent, node);
}

MeshNode* Node::GetCurrentMeshNode()
{
	for (auto* node = mMeshNodes.mRoot;
		node != NULL;
		node = mMeshNodes.GetNextChildFirst(node))
	{
		if (node) {
			return node;
		}
	}
	return NULL;
}

AnimLayer& Node::GetAnimLayer()
{
	return mAnimLayer;
}

void Node::SetAnimLayer(AnimLayer& layer)
{
	mAnimLayer = layer;
}

void  Node::Clear()
{
	mBoneNodes.Clear();
	mMeshNodes.Clear();
	mAnimLayer.Clear();
}

void Node::Copy(const Node &other)
{
	mBoneNodes = other.mBoneNodes;
	mMeshNodes = other.mMeshNodes;
	mAnimLayer = other.mAnimLayer;
}