#include "jsonserialize.h"

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


Json::Value serialize(const Vec2f &v)
{
	Json::Value value;
	value.append(v[0]);
	value.append(v[1]);
	return value;
}

void serialize(Vec2f &v, const Json::Value &value)
{
	v[0] = value[0].asFloat();
	v[1] = value[1].asFloat();
}

Json::Value serialize(const Vec3f &v)
{
	Json::Value value;
	value.append(v[0]);
	value.append(v[1]);
	value.append(v[2]);
	return value;
}

void serialize(Vec3f &v, const Json::Value &value)
{
	v[0] = value[0].asFloat();
	v[1] = value[1].asFloat();
	v[2] = value[2].asFloat();
}

Json::Value serialize(const Vec4f &v)
{
	Json::Value value;
	value.append(v[0]);
	value.append(v[1]);
	value.append(v[2]);
	value.append(v[3]);
	return value;
}

void serialize(Vec4f &v, const Json::Value &value)
{
	v[0] = value[0].asFloat();
	v[1] = value[1].asFloat();
	v[2] = value[2].asFloat();
	v[3] = value[3].asFloat();
}


Json::Value serialize(const Matrix &m)
{
	Json::Value value;
	value["translation"] = serialize(Vec4f(m.getT()));
	value["quaternion"] = serialize(Vec4f(m.getQ()));
	value["scale"] = serialize(Vec4f(m.getS()));
	return value;
}

void serialize(Matrix &matrix, const Json::Value &value)
{

	Quat4f quaternion;
	serialize(quaternion, value["quaternion"]);

	Vec4f translation;
	serialize(translation, value["translation"]);

	Vec4f scale;
	serialize(scale, value["scale"]);

	matrix.setTRS(translation, quaternion, scale);
}

Json::Value serialize(const Point &point)
{
	return serialize(point.GetPosition());
}

void serialize(Point &point, const Json::Value &value)
{
	Vec3f position;
	serialize(position, value);
	point.SetPosition(position);
}

Json::Value serialize(const Face &face)
{
	Json::Value value;
	auto pointSize = face.GetPointSize();
	for (auto pointIndex = 0; pointIndex < pointSize; ++pointIndex)
	{
		value.append(face.GetVertexIndex(pointIndex));
	}
	return value;
}

void serialize(Face &face, const Json::Value &value)
{
	auto pointSize = value.size();
	face.SetPointSize(pointSize);
	for (auto pointIndex = 0; pointIndex < pointSize; ++pointIndex)
	{
		face.SetVertexIndex(pointIndex, value[pointIndex].asInt());
	}
}


Json::Value serialize(const Weight &v)
{
	Json::Value value;
	value.append(v.GetIndex());
	value.append(v.GetWeight());
	return value;
}

void serialize(Weight &v, const Json::Value &value)
{
	v.SetIndex(value[0].asInt());
	v.SetWeight(value[1].asFloat());
}

Json::Value serialize(ElementVec2f &element2f)
{
	Json::Value value;
	value["name"] = element2f.GetName();
	auto sts = element2f.GetSTs();
	for (auto index = 0; index < sts.size(); ++index)
	{
		value["array"].append(serialize(element2f.GetST(index)));
	}
	return value;
}

void serialize(ElementVec2f &element2f, const Json::Value &value)
{
	element2f.SetName(value["name"].asString());

	auto& arr = value["array"];
	for (auto index = 0; index < arr.size(); ++index)
	{
		Vec2f v;
		serialize(v, arr[index]);
		element2f.SetST(index, v);
	}
}


Json::Value serialize(ElementVec4f &element4f)
{
	Json::Value value;
	value["name"] = element4f.GetName();
	auto sts = element4f.GetSTs();
	for (auto index = 0; index < sts.size(); ++index)
	{
		value["array"].append(serialize(element4f.GetST(index)));
	}
	return value;
}

void serialize(ElementVec4f &element4f, const Json::Value &value)
{
	element4f.SetName(value["name"].asString());

	auto& arr = value["array"];
	for (auto index = 0; index < arr.size(); ++index)
	{
		Vec4f v;
		serialize(v, arr[index]);
		element4f.SetST(index, v);
	}
}

Json::Value serialize(const MaterialPropNode &materialProp)
{
	Json::Value value;
	value["type"] = materialProp.type;
	value["opacity"] = materialProp.opacity;
	value["color"] = serialize(materialProp.color);
	for (auto &texture : materialProp.textures)
	{
		Json::Value object;
		object["name"] = texture.name;
		object["filename"] = texture.filename;
		object["uvSet"] = texture.uvSet;
		object["uvScale"] = serialize(texture.uvScale);
		object["uvTranslation"] = serialize(texture.uvTranslation);
		value["textures"].append(object);
	}
	return value;
}

void serialize(MaterialPropNode &prop, const Json::Value &value)
{
	prop.type = value["type"].asString();
	serialize(prop.color, value["color"]);
	prop.opacity = value["opacity"].asFloat();

	for (auto &v : value["textures"])
	{
		TextureNode texture;
		texture.name = v["name"].asString();
		texture.filename = v["filename"].asString();
		texture.uvSet = v["uvSet"].asString();
		serialize(texture.uvScale, v["uvScale"]);
		serialize(texture.uvTranslation, v["uvTranslation"]);
		prop.textures.push_back(texture);
	}
}

Json::Value serialize(MaterialNode &material)
{
	Json::Value value;
	value["name"] = material.GetName();
	for (auto &prop : material.GetPropNodes()) {
		value["props"].append(serialize(prop));
	}
	return value;
}

void serialize(MaterialNode &material, const Json::Value &value)
{
	material.SetName(value["name"].asString());
	for (auto &value : value["props"])
	{
		MaterialPropNode prop;
		serialize(prop,value);
		material.SetPropNode(prop);
	}
}

Json::Value serialize(BoneNode* bone)
{
	Json::Value value;
	value["name"] = bone->GetName();
	value["transform"] = serialize(bone->GetLocalTransform());

	for (BoneNode* child = bone->mFirstChild; NULL != child; child = child->mNext)
	{
		value["children"].append(serialize(child));
	}
	return value;
}

void serialize(Node *node, BoneNode* parent,const Json::Value &value)
{
	BoneNode* bone = new BoneNode();
	bone->SetName(value["name"].asString());

	Matrix matrix;
	serialize(matrix, value["transform"]);
	bone->SetLocalTransform(matrix);

	for (auto &jChild : value["children"])
	{
		serialize(node, bone, jChild);
	}
	node->AddChildBoneNode(parent, bone);
}


Node JsonSerialize::Get(const std::string & name)
{
	Node sNode;
	Json::Reader reader;
	Json::Value mJsonValue;
	std::ifstream cin(name);
	if (cin.is_open())
	{
		if (!reader.parse(cin, mJsonValue, false))
		{
			mJsonValue.clear();
			return sNode;
		}
		cin.close();
	}

	for (auto &jsonMesh : mJsonValue["meshes"]) {

		MeshNode* mesh = new MeshNode;
		mesh->SetName(jsonMesh["name"].asString());

		Matrix matrix;
		serialize(matrix, jsonMesh["transform"]);
		mesh->SetLocalTransform(matrix);

		for (auto &jsonMaterial : jsonMesh["materials"])
		{
			auto &material = mesh->NewMaterialNode();
			material.SetName(jsonMaterial["name"].asString());

			for (auto &jsonProp : jsonMaterial["props"]) {
				MaterialPropNode prop;
				serialize(prop, jsonProp);
				material.SetPropNode(prop);
			}
		}


		auto& jsonPoints = jsonMesh["points"];
		auto& points = mesh->GetPoints(jsonPoints.size());
		for (auto index = 0; index < jsonPoints.size(); ++index)
		{
			serialize(points[index], jsonPoints[index]);
		}

		auto& jsonFaces = jsonMesh["faces"];
		auto& faces = mesh->GetFaces(jsonFaces.size());
		for (auto index = 0; index < jsonFaces.size(); ++index)
		{
			serialize(faces[index], jsonFaces[index]);
		}

		auto& jsonUvs = jsonMesh["uvs"];
		auto& uvs = mesh->GetUVs(jsonUvs.size());
		for (auto index = 0; index < jsonUvs.size(); ++index)
		{
			serialize(uvs[index], jsonUvs[index]);
		}

		auto& jsonNormals = jsonMesh["normals"];
		auto& normals = mesh->GetNormals(jsonNormals.size());
		for (auto index = 0; index < jsonNormals.size(); ++index)
		{
			serialize(normals[index], jsonNormals[index]);
		}

		for (auto &jsonCluster : jsonMesh["clusters"]) {

			auto& cluster = mesh->NewClusterNode();
			cluster.SetName(jsonCluster["name"].asString());
			cluster.SetLinkName(jsonCluster["linkName"].asString());

			Matrix transform;
			serialize(transform, jsonCluster["transform"]);
			cluster.SetTransformMatrix(transform);

			Matrix linkTransform;
			serialize(linkTransform, jsonCluster["linkTransform"]);
			cluster.SetLinkTransformLinkMatrix(linkTransform);

			auto& jsonWeights = jsonCluster["weights"];
			for (auto n = 0; n < jsonWeights.size(); n++)
			{
				Weight w;
				serialize(w, jsonWeights[n]);
				cluster.AddWeight(w.GetIndex(), w.GetWeight());
			}
		}
		sNode.AddChildMeshNode(NULL, mesh);
	}

	for (auto &jsonBone : mJsonValue["bones"])
	{
		serialize(&sNode, NULL, jsonBone);
	}
	return sNode;
}

void JsonSerialize::Set(const std::string &name, Node &sNode)
{
	Json::Value root;
	for (MeshNode* mesh = sNode.GetMeshNodeRoot(); mesh != NULL; mesh = mesh->mNext)
	{
		Json::Value jsonMesh;
		jsonMesh["name"] = mesh->GetName();
		jsonMesh["transform"] = serialize(mesh->GetLocalTransform());

		for (auto &material : mesh->GetMaterialNodes())
		{
			Json::Value jsonMaterial;
			jsonMaterial["name"] = material.GetName();
			for (auto &prop : material.GetPropNodes()) {
				jsonMaterial["props"].append(serialize(prop));
			}
			jsonMesh["materials"].append(jsonMaterial);
		}

		auto& points = mesh->GetPoints();
		for (auto pointIndex = 0; pointIndex < points.size(); ++pointIndex)
		{
			auto &point = points[pointIndex];
			jsonMesh["points"].append(serialize(point));
		}

		auto& faces = mesh->GetFaces();
		for (auto faceIndex = 0; faceIndex < faces.size(); ++faceIndex)
		{
			auto &face = faces[faceIndex];
			jsonMesh["faces"].append(serialize(face));
		}

		auto& uvs = mesh->GetUVs();
		for (auto index = 0; index < uvs.size(); ++index)
		{
			auto &uv = uvs[index];
			jsonMesh["uvs"].append(serialize(uv));
		}

		auto& normals = mesh->GetNormals();
		for (auto index = 0; index < normals.size(); ++index)
		{
			auto &normal = normals[index];
			jsonMesh["normals"].append(serialize(normal));
		}

		for (auto& cluster : mesh->GetClusterNodes())
		{
			Json::Value jsonCluster;
			jsonCluster["name"] = cluster.GetName();
			jsonCluster["linkName"] = cluster.GetLinkName();
			jsonCluster["transform"] = serialize(cluster.GetTransformMatrix());
			jsonCluster["linkTransform"] = serialize(cluster.GetLinkTransformLinkMatrix());

			auto &weights = cluster.GetWeights();
			for (auto n = 0; n < weights.size(); n++)
			{
				jsonCluster["weights"].append(serialize(weights[n]));
			}
			jsonMesh["clusters"].append(jsonCluster);
		}

		root["meshes"].append(jsonMesh);
	}

	for (BoneNode* bone = sNode.GetBoneNodeRoot(); bone != NULL; bone = bone->mNext)
	{
		root["bones"].append(serialize(bone));
	}

	if (!root.empty()) 
	{
		std::ofstream out(name);
		if (out.is_open())
		{
			out << root.toStyledString();
			out.close();
		}
	}
}
