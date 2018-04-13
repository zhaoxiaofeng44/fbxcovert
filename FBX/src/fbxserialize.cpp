#include "fbxserialize.h"

#include <config.h>
#include <map>
#include <vector>
#include <meshnode.h>
#include <bonenode.h>
#include "Common.h"

#include <fbxcore.h>
#include <fbxsdk.h>
#include <iostream>  
#include <fstream>  
#include <stdlib.h>   
#include <fbxcast.h>   

#include"vec.h"
#include"matrix.h"
#include"node.h"


void onObtainMesh(MeshNode *meshNode, FbxScene* scene, FbxNode* rootNode, std::map<FbxCluster*, std::string> &fbxClusters) {

	for (MeshNode* node = meshNode; node != NULL; node = node->mNext)
	{
		FbxMesh* lMesh = FbxMesh::Create(scene, "");
		FbxNode* lNode = FbxNode::Create(scene, "");
		lNode->SetNodeAttribute(lMesh);
		lNode->SetShadingMode(FbxNode::eTextureShading);
		rootNode->AddChild(lNode);

		auto lclMatrix = FBXCast::cast(node->GetLocalTransform());
		lNode->LclScaling.Set(lclMatrix.GetS());
		lNode->LclRotation.Set(lclMatrix.GetR());
		lNode->LclTranslation.Set(lclMatrix.GetT());

		auto pNormalElement = lMesh->CreateElementNormal();
		pNormalElement->SetMappingMode(FbxGeometryElement::eByControlPoint);
		pNormalElement->SetReferenceMode(FbxGeometryElement::eDirect);

		auto m_pUVElement = lMesh->CreateElementUV("UVSet1");
		m_pUVElement->SetMappingMode(FbxGeometryElement::eByControlPoint);
		m_pUVElement->SetReferenceMode(FbxGeometryElement::eDirect);

		for (auto &material : node->GetMaterialNodes())
		{
			fbxsdk::FbxSurfacePhong * phong = fbxsdk::FbxSurfacePhong::Create(scene, "");

			auto diffuse = material.GetPropNode("Diffuse");
			auto &color = diffuse.color;
			phong->Diffuse = FbxDouble3(color[0], color[1], color[2]);
			phong->DiffuseFactor = diffuse.opacity;

			for (auto &texture : diffuse.textures)
			{
				FbxFileTexture * ptr = FbxFileTexture::Create(scene, "");
				ptr->SetFileName(texture.filename.c_str());
				ptr->SetTextureUse(FbxTexture::eStandard);
				ptr->SetMappingType(FbxTexture::eUV);
				ptr->SetMaterialUse(FbxFileTexture::eModelMaterial);
				ptr->SetSwapUV(false);
				ptr->SetTranslation(0.0, 0.0);
				ptr->SetScale(1.0, 1.0);
				ptr->SetRotation(0.0, 0.0);
				ptr->UVSet.Set(texture.uvSet.c_str());
				phong->Diffuse.ConnectSrcObject(ptr);
			}
		}

		std::string name = node->GetName();
		FbxSkin* lSkin = FbxSkin::Create(scene, name.c_str());

		auto& faces = node->GetFaces();
		auto& points = node->GetPoints();
		auto& uvs = node->GetUVs()[0];
		auto& normals = node->GetNormals()[0];

		lMesh->InitControlPoints(points.size());
		for (auto faceIndex = 0; faceIndex < faces.size(); ++faceIndex)
		{
			auto &face = faces[faceIndex];

			lMesh->BeginPolygon();
			for (int facePointIndex = 0; facePointIndex < FACE_COMPONENT_NUM; ++facePointIndex)
			{
				auto index = face.GetVertexIndex(facePointIndex);

				auto &point = points[index];

				//face
				lMesh->AddPolygon(index);

				//position
				auto &positon = point.GetPosition();
				lMesh->SetControlPointAt({ positon[0], positon[1], positon[2], 0 }, index);

				//normal
				auto &normal = normals.GetST(index);
				pNormalElement->GetDirectArray().SetAt(index, { normal[0], normal[1], normal[2], 0 });

				//uv
				auto &uv = uvs.GetST(index);
				m_pUVElement->GetDirectArray().SetAt(index, { uv[0], uv[1] });
			}
			lMesh->EndPolygon();
		}

		auto &clusters = node->GetClusterNodes();
		for (auto& cluster : clusters)
		{
			FbxCluster *lCluster = FbxCluster::Create(scene, "");
			lCluster->SetLinkMode(FbxCluster::eTotalOne);
			lCluster->SetTransformMatrix(FBXCast::cast(cluster.GetTransformMatrix()));
			lCluster->SetTransformLinkMatrix(FBXCast::cast(cluster.GetLinkTransformLinkMatrix()));

			auto &weights = cluster.GetWeights();
			for (auto n = 0; n < weights.size(); n++)
			{
				lCluster->AddControlPointIndex(weights[n].GetIndex(), weights[n].GetWeight());
			}
			lSkin->AddCluster(lCluster);
			fbxClusters[lCluster] = cluster.GetLinkName();
		}
		lMesh->AddDeformer(lSkin);
	}
}

void onObtainNode(BoneNode *boneNode, FbxScene* scene, FbxNode* parent, std::map<FbxCluster*, std::string> &fbxClusters) {

	for (BoneNode* node = boneNode; node != NULL; node = node->mNext)
	{
		std::string name = node->GetName();
		FbxSkeleton* lSkeleton = FbxSkeleton::Create(scene, name.c_str());
		lSkeleton->SetSkeletonType(FbxSkeleton::eLimbNode);
		FbxNode* lSkeletonNode = FbxNode::Create(scene, name.c_str());
		lSkeletonNode->SetNodeAttribute(lSkeleton);
		parent->AddChild(lSkeletonNode);

		auto lclMatrix = FBXCast::cast(node->GetLocalTransform());
		lSkeletonNode->LclScaling.Set(lclMatrix.GetS());
		lSkeletonNode->LclRotation.Set(lclMatrix.GetR());
		lSkeletonNode->LclTranslation.Set(lclMatrix.GetT());

		for (auto v : fbxClusters)
		{
			if (NULL != v.first && name == v.second)
			{
				v.first->SetLink(lSkeletonNode);
			}
		}
		onObtainNode(node->mFirstChild, scene, lSkeletonNode, fbxClusters);
	}
}

Node FbxSerialize::Get(const std::string & name)
{
	Node sNode;
	FBXCore core(name, &sNode);
	return sNode;
}

void FbxSerialize::Set(const std::string &name, Node & sNode)
{
	FbxScene* mScene = NULL;
	FbxManager* mSdkManager = NULL;
	InitializeSdkObjects(mSdkManager, mScene);

	FbxNode* lNode = mScene->GetRootNode();;

	std::map<FbxCluster*, std::string> fbxClusters;
	onObtainMesh(sNode.GetMeshNodeRoot(), mScene, lNode, fbxClusters);
	onObtainNode(sNode.GetBoneNodeRoot(), mScene, lNode, fbxClusters);

	SaveScene(mSdkManager, mScene, name.c_str());
	DestroySdkObjects(mSdkManager, true);

}

