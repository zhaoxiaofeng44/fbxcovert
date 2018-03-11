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

vec2i* vec2i::pZero = new vec2i();
vec2f* vec2f::pZero = new vec2f();
vec3f* vec3f::pZero = new vec3f();
vec4f* vec4f::pZero = new vec4f();
Json::Value serialize(const vec2f &v)
{
	Json::Value value;
	value.append(v.x);
	value.append(v.y);
	return value;
}

void serialize(vec2f &v, const Json::Value &value)
{
	v.x = value[0].asFloat();
	v.y = value[1].asFloat();
}

Json::Value serialize(const vec3f &v)
{
	Json::Value value;
	value.append(v[0]);
	value.append(v[1]);
	value.append(v[2]);
	return value;
}

void serialize(vec3f &v, const Json::Value &value)
{
	v.x = value[0].asFloat();
	v.y = value[1].asFloat();
	v.z = value[2].asFloat();
}

Json::Value serialize(const vec4f &v)
{
	Json::Value value;
	value.append(v.x);
	value.append(v.y);
	value.append(v.z);
	value.append(v.w);
	return value;
}

void serialize(vec4f &v, const Json::Value &value)
{
	v.x = value[0].asFloat();
	v.y = value[1].asFloat();
	v.z = value[2].asFloat();
	v.w = value[3].asFloat();
}

Json::Value serialize(const Quaternion &v)
{
	Json::Value value;
	value.append(v.v[0]);
	value.append(v.v[1]);
	value.append(v.v[2]);
	value.append(v.w);
	return value;
}

void serialize(Quaternion &v, const Json::Value &value)
{
	v.v[0] = value[0].asFloat();
	v.v[1] = value[1].asFloat();
	v.v[2] = value[2].asFloat();
	v.w	= value[3].asFloat();
}

Json::Value serialize(const Matrix4x4 &matrix)
{
	fbxsdk::FbxAMatrix m;
	matrix.copy(m);

	Json::Value value;
	/*value["translation"] = serialize(matrix.getTransform());
	value["quaternion"] = serialize(matrix.getQuaternion());
	value["scale"] = serialize(matrix.getScale());
	for (int index = 0; index < 16; index++)
	{
		//value.append(matrix.mData[index]);
	}*/

	value["translation"] = serialize(FBXCast::cast(m.GetT()));
	value["quaternion"] = serialize(FBXCast::cast(m.GetR()));
	value["scale"] = serialize(FBXCast::cast(m.GetS()));
	return value;
}

void serialize(Matrix4x4 &matrix, const Json::Value &value)
{
	/*Quaternion q;
	serialize(q, value["quaternion"]);

	vec4f t;
	serialize(t, value["translation"]);

	vec4f s;
	serialize(s, value["scale"]);*/

	/*Matrix m;
	m.setTRS(Vec3f(t.x, t.y, t.z), Quat4f(q.v.x, q.v.y, q.v.z, q.w), Vec3f(s.x, s.y, s.z));
	matrix.copy(m.get());*/


	Quaternion quaternion;
	serialize(quaternion, value["quaternion"]);

	vec4f translation;
	serialize(translation, value["translation"]);

	vec4f scale;
	serialize(scale, value["scale"]);

	fbxsdk::FbxAMatrix m;
	m.SetT(FbxVector4(translation.x, translation.y, translation.z, translation.w));
	m.SetR(FbxVector4(quaternion.v.x, quaternion.v.y, quaternion.v.z, quaternion.w));
	m.SetS(FbxVector4(scale.x, scale.y, scale.z, scale.w));


	Matrix cc;

	cc.setTRS(Vec3f(), Quat4f(), Vec3f());
	Matrix4x4 m44;
	for (int index = 0; index < 16; index++)
	{
		m44.mData[index] = m[index/4][index %4];
		//value.append(matrix.mData[index]);
	}
	matrix = m44;
}

Json::Value serialize(const Point &point)
{
	return serialize(point.GetPosition());
}

void serialize(Point &point, const Json::Value &value)
{
	vec3f position;
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
		vec2f v;
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
		vec4f v;
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

	Matrix4x4 matrix4x4;
	serialize(matrix4x4, value["transform"]);
	bone->SetLocalTransform(matrix4x4);

	for (auto &jChild : value["children"])
	{
		serialize(node, bone, jChild);
	}
	node->AddChildBoneNode(parent, bone);
}


void JsonSerialize::Set(Node *Src)
{
	Json::Value root;
	for (MeshNode* mesh = Src->GetMeshNodeRoot(); mesh != NULL; mesh = mesh->mNext)
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

	for (BoneNode* bone = Src->GetBoneNodeRoot(); bone != NULL; bone = bone->mNext)
	{
		root["bones"].append(serialize(bone));
	}

	mJsonValue = root;
}

void JsonSerialize::Get(Node *Src)
{
	for (auto &jsonMesh : mJsonValue["meshes"]) {

		MeshNode* mesh = new MeshNode;
		mesh->SetName(jsonMesh["name"].asString());

		Matrix4x4 matrix4x4;
		serialize(matrix4x4, jsonMesh["transform"]);
		mesh->SetLocalTransform(matrix4x4);

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

			Matrix4x4 transform;
			serialize(transform, jsonCluster["transform"]);
			cluster.SetTransformMatrix(transform);

			Matrix4x4 linkTransform;
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
		Src->AddChildMeshNode(NULL, mesh);
	}

	for (auto &jsonBone : mJsonValue["bones"]) 
	{
		serialize(Src, NULL,jsonBone);
	}
}


void JsonSerialize::Save(std::string name)
{
	std::ofstream out(name);
	if (out.is_open())
	{
		out << mJsonValue.toStyledString();
		out.close();
	}
}

void JsonSerialize::Read(std::string name)
{
	Json::Reader reader;
	std::string strJson;
	std::ifstream cin(name);
	if (cin.is_open())
	{
		if (!reader.parse(cin, mJsonValue, false))
		{
			mJsonValue.clear();
		}
		cin.close();
	}
}