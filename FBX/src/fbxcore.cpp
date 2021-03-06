
#include <log.h>
#include <meshnode.h>
#include <fbxcast.h>
#include <fbxdevice.h>
#include <fbxcore.h>

#include <fbxsdk.h>
using namespace fbxsdk;

FBXCore::FBXCore(const std::string &filename ,Node* pNode)
{
	//create fbxscene importer manager animation layers
	FBXDevice Device(filename);
	auto FbxRoot = Device.GetRootNode();
	auto Importer = Device.GetImporter();
	
	mNode = pNode ? pNode : new Node();
	ProcessBoneNodes(FbxRoot, mNode->GetBoneNodeRoot());
	ProcessMeshNodes(FbxRoot, mNode->GetMeshNodeRoot());
	ProcessPoses(FbxRoot, Importer);
}

bool FBXCore::ProcessBoneNodes(FbxNode* pNode, BoneNode* parentBoneNode)
{
	BoneNode* newBoneNode = NULL;

	const FbxNodeAttribute* pNodeAttr = pNode->GetNodeAttribute();
	if (pNodeAttr)
	{
		const auto type = pNodeAttr->GetAttributeType();
		switch (type)
		{
		case fbxsdk::FbxNodeAttribute::eSkeleton:
			newBoneNode = ProcessBoneNode(pNode, parentBoneNode);
			break;

		default:
			break;
		}
	}
	for (int childIndex = 0; childIndex < pNode->GetChildCount(); ++childIndex)
	{
		FbxNode* pChildNode = pNode->GetChild(childIndex);
		ProcessBoneNodes(pChildNode,newBoneNode != NULL ? newBoneNode : parentBoneNode);
	}

	return true;
}


bool FBXCore::ProcessMeshNodes(FbxNode* pNode, MeshNode* parentMeshNode)
{
	MeshNode* newMeshNode = NULL;

	const FbxNodeAttribute* pNodeAttr = pNode->GetNodeAttribute();
	if (pNodeAttr)
	{
		const auto type = pNodeAttr->GetAttributeType();
		switch (type)
		{
		case fbxsdk::FbxNodeAttribute::eMesh:
			newMeshNode = ProcessMeshNode(pNode, parentMeshNode);
			break;
		default:
			break;
		}
	}
	for (int childIndex = 0; childIndex < pNode->GetChildCount(); ++childIndex)
	{
		FbxNode* pChildNode = pNode->GetChild(childIndex);
		ProcessMeshNodes(pChildNode, newMeshNode != NULL ? newMeshNode : parentMeshNode);
	}

	return true;
}

BoneNode* FBXCore::ProcessBoneNode(FbxNode* pNode, BoneNode* parent)
{
	const FbxSkeleton* const pSkeleton = static_cast<const FbxSkeleton*>(pNode->GetNodeAttribute());

	BoneNode* boneNode = NULL;
	if (!pSkeleton) {
		LOG_ERROR("failed to convert skeleton node");
		return  NULL;
	}

	boneNode = new BoneNode();

	mNode->AddChildBoneNode(parent, boneNode);

	//set name
	boneNode->SetName(pNode->GetName());

	FbxAMatrix localTRS;
	FbxVector4 T = pNode->LclTranslation.Get();
	FbxVector4 R = pNode->LclRotation.Get();
	FbxVector4 S = pNode->LclScaling.Get();
	localTRS.SetTRS(T, R, S);
	boneNode->SetLocalTransform(FBXCast::cast(localTRS));

	return boneNode;
}


TextureNode createTexture(FbxFileTexture *texture) {

	assert(NULL != texture, "createTexture using a invaild params");

	auto &uvScale = texture->GetUVScaling();
	auto &uvTranslation = texture->GetUVTranslation();
	std::string uvSet = texture->UVSet.Get();
	return{
		texture->GetName(),
		texture->GetFileName(),
		uvSet,
		Vec2f(uvScale[0], uvScale[1]),
		Vec2f(uvTranslation[0], uvTranslation[1])
	};
}

void addTextures(std::vector<TextureNode> &textures, const FbxProperty &prop) {

	const unsigned int ftCount = prop.GetSrcObjectCount<FbxFileTexture>();
	for (unsigned int ft = 0; ft < ftCount; ++ft)
	{
		auto srcTexture = prop.GetSrcObject<FbxFileTexture>(ft);
		if (NULL != srcTexture) {
			textures.push_back(createTexture(srcTexture));
		}
	}

	const unsigned int ltCount = prop.GetSrcObjectCount<FbxLayeredTexture>();
	for (unsigned int lt = 0; lt < ltCount; ++lt) {
		FbxLayeredTexture * const &layeredTexture = prop.GetSrcObject<FbxLayeredTexture>(lt);
		const unsigned int ftCount = layeredTexture->GetSrcObjectCount<FbxFileTexture>();
		for (unsigned int ft = 0; ft < ftCount; ++ft)
		{
			auto srcTexture = prop.GetSrcObject<FbxFileTexture>(ft);
			if (NULL != srcTexture) {
				textures.push_back(createTexture(srcTexture));
			}
		}
	}
}

void addMaterialProp(MaterialNode &material,std::string type,float opacity, const Vec3f &color, const FbxProperty &prop) {

	if (prop.IsValid()) {
		std::vector<TextureNode> textures;
		addTextures(textures, prop);
		if (0 < textures.size()) {
			material.SetPropNode({
				type,
				color,
				opacity,
				textures
			});
		}
	}
}

MeshNode* FBXCore::ProcessMeshNode(FbxNode *pNode, MeshNode *parent)
{
	FbxMesh* pMesh = pNode->GetMesh();
	MeshNode* meshNode = NULL;

	if (!pMesh) {
		LOG << "this node is not supported a mesh" << ENDN;
		return meshNode;
	}

	meshNode = new MeshNode;
	meshNode->SetName(pNode->GetName());
	mNode->AddChildMeshNode(parent, meshNode);

	FbxAMatrix localTRS;
	FbxVector4 T = pNode->LclTranslation.Get();
	FbxVector4 R = pNode->LclRotation.Get();
	FbxVector4 S = pNode->LclScaling.Get();
	localTRS.SetTRS(T, R, S);

	meshNode->SetLocalTransform(FBXCast::cast(localTRS));

	BuildMaterial(pNode, meshNode);
	BuildMeshNode(pMesh, meshNode);
	BuildMeshSkin(pMesh, meshNode);

	return meshNode;
}

void FBXCore::BuildMaterial(FbxNode* pNode, MeshNode* meshNode)
{
	auto materialCount = pNode->GetMaterialCount();
	for (int materialIndex = 0; materialIndex < materialCount; ++materialIndex)
	{
		auto material = pNode->GetMaterial(materialIndex);
		if (material->GetClassId() == fbxsdk::FbxSurfacePhong::ClassId)
		{
			auto surfacePhong = (fbxsdk::FbxSurfacePhong*)material;
			auto &materialNode = meshNode->NewMaterialNode();
			materialNode.SetName(surfacePhong->GetName());
			addMaterialProp(
				materialNode,
				"Ambient",
				surfacePhong->AmbientFactor.Get(),
				FBXCast::cast(surfacePhong->Ambient.Get()),
				surfacePhong->Ambient
			);

			addMaterialProp(
				materialNode,
				"Diffuse",
				surfacePhong->DiffuseFactor.Get(),
				FBXCast::cast(surfacePhong->Diffuse.Get()),
				surfacePhong->Diffuse
			);

			addMaterialProp(
				materialNode,
				"Specular",
				surfacePhong->SpecularFactor.Get(),
				FBXCast::cast(surfacePhong->Specular.Get()),
				surfacePhong->Specular
			);
			addMaterialProp(
				materialNode,
				"Emissive",
				surfacePhong->EmissiveFactor.Get(),
				FBXCast::cast(surfacePhong->Emissive.Get()),
				surfacePhong->Emissive
			);
			addMaterialProp(
				materialNode,
				"Shininess",
				surfacePhong->Shininess.Get(),
				FBXCast::cast(FbxDouble3(0.0f, 0.0f, 0.0f)),
				surfacePhong->Shininess
			);
			addMaterialProp(
				materialNode,
				"Reflection",
				surfacePhong->ReflectionFactor.Get(),
				FBXCast::cast(surfacePhong->Reflection.Get()),
				surfacePhong->Reflection
			);
		}
	}
}

void FBXCore::BuildMeshNode(FbxMesh* pMesh, MeshNode* meshNode)
{
	auto faceNums = pMesh->GetPolygonCount();
	auto vertexNum = pMesh->GetControlPointsCount();
	
	auto& points = meshNode->GetPoints(vertexNum);
	auto contolPoints = pMesh->GetControlPoints();
	for (unsigned int vertexIndex = 0; vertexIndex < vertexNum; ++vertexIndex)
	{
		auto &point = contolPoints[vertexIndex];
		points[vertexIndex].SetPosition(Vec3f(point[0], point[1], point[2]));
	}

	auto& faces = meshNode->GetFaces(faceNums);

	//uv count
	auto uvCount = pMesh->GetElementUVCount();
	auto &sUvs = meshNode->GetUVs(uvCount);

	//normal count
	auto nmCount = pMesh->GetElementNormalCount();
	auto &sNormals = meshNode->GetNormals(nmCount);

	//face size
	for (int faceIndex = 0; faceIndex < faceNums; ++faceIndex)
	{
		Face &face = faces[faceIndex];
		auto facePointSize = pMesh->GetPolygonSize(faceIndex);
		face.SetPointSize(facePointSize);
		for (int facePointIndex = 0; facePointIndex < facePointSize; ++facePointIndex)
		{
			auto vertexIndex = pMesh->GetPolygonVertex(faceIndex, facePointIndex);

			//face
			face.SetVertexIndex(facePointIndex, vertexIndex);
			//normal
			for (auto index = 0; index < nmCount; ++index) {
				auto eST = pMesh->GetElementNormal(index);
				sNormals[index].SetName(eST->GetName());
				sNormals[index].SetST(vertexIndex, FBXCast::ConvertFromElement<FbxLayerElementTemplate<FbxVector4>, Vec4f>(eST, vertexIndex));
			}
			//uv
			for (auto index = 0; index < uvCount; ++index) {
				auto eST = pMesh->GetElementUV(index);
				sUvs[index].SetName(eST->GetName());
				sUvs[index].SetST(vertexIndex, FBXCast::ConvertFromElement<FbxLayerElementTemplate<FbxVector2>, Vec2f>(eST, vertexIndex));
			}	
		}
	}
}


void FBXCore::BuildMeshSkin(FbxMesh* pMesh, MeshNode* meshNode)
{
	const int VertexNum = pMesh->GetControlPointsCount();
	const int SkinNum = pMesh->GetDeformerCount(FbxDeformer::eSkin);
	for (int skinIndex = 0; skinIndex < SkinNum; ++skinIndex)
	{
		FbxDeformer* pDeformer = pMesh->GetDeformer(skinIndex, FbxDeformer::eSkin);
		FbxSkin* pSkin = static_cast<FbxSkin*>(pDeformer);
		const FbxSkin::EType skinType = pSkin->GetSkinningType();
		switch (skinType)
		{
		case FbxSkin::eLinear:
		case FbxSkin::eRigid:
		case FbxSkin::eDualQuaternion:
			break;
		default:
			continue;
		}

		auto clusterNum = pSkin->GetClusterCount();
		for (int clusterIndex = 0; clusterIndex < clusterNum; ++clusterIndex)
		{
			FbxCluster* pCluster = pSkin->GetCluster(clusterIndex);
			const auto clusterMode = pCluster->GetLinkMode();
			switch (clusterMode)
			{
			case FbxCluster::eNormalize:
			case FbxCluster::eTotalOne:
				break;
			default:
				LOG_ASSERT("failed to find link bone node");
				continue;
			}

			FbxNode* const pLinkBoneNode = pCluster->GetLink();
			if (!pLinkBoneNode) {
				LOG << "failed to find linked bone node" << ENDN;
				continue;
			}

			std::string BoneName(pLinkBoneNode->GetName());
			BoneNode* boneNode = mNode->GetBoneNodeByName(BoneName);
			assert(boneNode);

			auto &bCluster = meshNode->NewClusterNode();
			
			//name
			bCluster.SetLinkName(BoneName);

			//transform
			FbxAMatrix tMat;
			pCluster->GetTransformMatrix(tMat);
			bCluster.SetTransformMatrix(FBXCast::cast(tMat));

			//link
			FbxAMatrix tLinkMat;
			pCluster->GetTransformLinkMatrix(tLinkMat);
			bCluster.SetLinkTransformLinkMatrix(FBXCast::cast(tLinkMat));

			//weigtht
			const int ClusterIndicesNum = pCluster->GetControlPointIndicesCount();
			const int *ControlPointIndices = pCluster->GetControlPointIndices();
			const double *ControlPointWeights = pCluster->GetControlPointWeights();
			for (int clusterIndex = 0; clusterIndex < ClusterIndicesNum; ++clusterIndex)
			{
				int controlIndex = ControlPointIndices[clusterIndex];
				assert(controlIndex < VertexNum);

				float controlWeight = (float)ControlPointWeights[clusterIndex];
				if (controlWeight < 0.0000000001f)
					continue;

				bCluster.AddWeight(controlIndex, controlWeight);
			}
		}
	}
}

void FBXCore::ProcessPoses(FbxNode *pNode, FbxImporter *importer)
{
	AnimLayer animLayer(importer);
	for (auto index = 0; index < animLayer.GetNumSamples(); index++) {
		ProcessPose(pNode, animLayer.GetSample(index));
	}
	mNode->SetAnimLayer(animLayer);
}

void FBXCore::ProcessPose(FbxNode * pNode, AnimSample* animSample)
{
	const FbxNodeAttribute* pNodeAttr = pNode->GetNodeAttribute();
	if (pNodeAttr)
	{
		const auto type = pNodeAttr->GetAttributeType();
		switch (type)
		{
		case fbxsdk::FbxNodeAttribute::eSkeleton: {
			auto boneNode = mNode->GetBoneNodeByName(pNode->GetName());
			if (boneNode) {
				boneNode->SetAnimTransform(
					animSample->mStart,
					FBXCast::cast(pNode->EvaluateLocalTransform(FbxTime(animSample->mStart), FbxNode::eDestinationPivot))
				);
				boneNode->SetAnimTransform(
					animSample->mEnd,
					FBXCast::cast(pNode->EvaluateLocalTransform(FbxTime(animSample->mEnd), FbxNode::eDestinationPivot))
				);
			}
			break;
		}
		default:
			break;
		}
	}
	for (int childIndex = 0; childIndex < pNode->GetChildCount(); ++childIndex)
	{
		ProcessPose(pNode->GetChild(childIndex), animSample);
	}
}

