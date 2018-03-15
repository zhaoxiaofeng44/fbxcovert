#include <config.h>
#include <map>
#include <vector>
#include <meshnode.h>
#include <bonenode.h>
#include "Common.h"

#include <fbxcast.h>
#include <fbxcore.h>
#include <fbxsdk.h>

#include <jsonserialize.h>


void onObtainMesh(MeshNode *meshNode, FbxScene* scene, FbxNode* rootNode, std::map<FbxCluster*,std::string> &fbxClusters) {

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
			fbxsdk::FbxSurfacePhong * phong = fbxsdk::FbxSurfacePhong::Create(scene,"");

			auto diffuse = material.GetPropNode("Diffuse");
			auto &color = diffuse.color;
			phong->Diffuse = FbxDouble3(color[0], color[1],color[2]);
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

void onObtainBoneNodes(Node* cnode) {

	JsonSerialize cc;
	cc.Set(cnode);
	cc.Save("aaa.txt");


	Node *node = new Node();
	cc.Get(node);
	FbxScene* mScene = NULL;
	FbxManager* mSdkManager = NULL;
	InitializeSdkObjects(mSdkManager, mScene);

	FbxNode* lNode = mScene->GetRootNode();
	mScene->GetRootNode()->AddChild(lNode);

	std::map<int, FbxNode*> skeletons;
	std::map<FbxCluster*, std::string> fbxClusters;
	onObtainMesh(node->GetMeshNodeRoot(), mScene, lNode, fbxClusters);
	onObtainNode(node->GetBoneNodeRoot(), mScene, lNode, fbxClusters);
	
	SaveScene(mSdkManager, mScene, "xxxx.fbx");
	DestroySdkObjects(mSdkManager, true);
}

#include"vec.h"
#include"matrix.h"
int main(int argc, char* argv[])
{
	Vec2f aa = 2.0;
	Vec3f aaa = aa;
	Quat4f c = aaa;

	fbxsdk::FbxAMatrix m(FbxVector4(33, 22, 0, 0), FbxVector4(1, 1, 0, 0.3), FbxVector4(1, 1, 2, 1));
	//m.SetT(FbxVector4(4, 6,2, 0));
	//m.SetQ(FbxQuaternion(1.0, 0, 0, 0));
	//m.SetS(FbxVector4(1.0, 1.0, 1.0, 1.0));
	
	float tt[] = { 1.0f, 2.f, 3.f };

	auto t = m.GetT();
	auto q = m.GetQ();
	auto s = m.GetS();
	auto r = m.GetR();

	fbxsdk::FbxAMatrix g;
	g.SetQ(q);
	g.SetT(t);
	

	/*Matrix4x4 tsst(Quaternion(q[0], q[1], q[2], q[3]));
	
	auto k = g.GetR();
	auto cr = g.GetR();
	Matrix4x4 cc;
	cc.setToIdentity();
	cc.setTRS(vec3f(0,0,0),Quaternion(q[0], q[1], q[2], q[3]),vec3f(1.0,1.0,1.0) );
	Matrix ttt(Quat4f(q[0], q[1], q[2],q[3]));


	Matrix eeee();*/


	Matrix ee;
	ee.setT(Vec3f(t[0], t[1], t[2]));
	ee.setQ(Quat4f(q[0], q[1], q[2], q[3]));
	ee.setS(Vec3f(1, 1, 2));

	Vec3f ec = ee.getT();
	Vec3f eb = ee.getS();
	Quat4f ed = ee.getQ();
	/*matrix = Matrix4x4();
	matrix.setToIdentity((),);

	auto t = m.GetT();
	auto q = m.GetQ();
	auto s = m.GetS();
	auto r = m.GetR();
	matrix.translate(vec3f(t[0], t[1], t[2]));
	matrix.scale(vec3f(s[0], s[1], s[2]));*/




	fbxsdk::FbxAMatrix ccf;
	ccf.SetTQS(FbxVector4(22, 0, 0, 0), FbxQuaternion(1, 0, 0, 0.3), FbxVector4(1, 1, 1, 1));

	auto cct = ccf.GetT();
	auto ccq = ccf.GetQ();
	auto ccs = ccf.GetS();
	auto ccr = ccf.GetR();


	Matrix ddm;
	ddm.setTRS(Vec3f(22, 0, 0), Quat4f(1, 0, 0, 0.3), Vec3f(1, 1, 1));

	auto ddt = ddm.getT();
	auto ddq = ddm.getQ();
	auto dds = ddm.getS();


	

	FBXCore core("Guard.FBX");
	core.mNode;

	onObtainBoneNodes(core.mNode.get());

	system("pause");
	
	return 0;
}

