#include <node.h>


Node::Node()
{

}

Node::~Node()
{
	
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


