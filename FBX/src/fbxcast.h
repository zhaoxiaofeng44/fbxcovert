#pragma once
#include <fbxsdk.h>
#include <Vec2f.h>
#include <vec3f.h>
#include <vec4f.h>
#include <matrix4x4.h>
#include <vector>
#include <string>
#include <map>
#include <face.h>
#include <point.h>
#include <node.h>
#include <vec.h>

using namespace fbxsdk;

class FBXCast
{
public:
	
	static inline Vec2f cast(FbxDouble2 v)
	{
		return Vec2f(v[0], v[1]);
	}

	static inline Vec3f cast(FbxDouble3 v)
	{
		return Vec3f(v[0], v[1], v[2]);
	}

	static inline Vec4f cast(FbxDouble4 v)
	{
		return Vec4f(v[0], v[1], v[2], v[3]);
	}

	static inline Quat4f cast(fbxsdk::FbxQuaternion v)
	{
		return Quat4f(v[0], v[1], v[2], v[3]);
	}

	static inline FbxDouble4 cast(Vec4f v)
	{
		return FbxDouble4(v[0], v[1], v[2], v[3]);
	}

	static inline Matrix cast(fbxsdk::FbxAMatrix v)
	{
		Matrix m;
		m.setTRS(cast(v.GetT()),cast(v.GetQ()),cast(v.GetS()));
		return m;
	}

	static inline fbxsdk::FbxAMatrix cast(Matrix v)
	{
		auto t = v.getT();
		auto q = v.getQ();
		auto s = v.getS();

		fbxsdk::FbxAMatrix m;
		m.SetTQS(
			fbxsdk::FbxVector4(t[0],t[1],t[2],0),
			fbxsdk::FbxQuaternion(q[0], q[1], q[2], q[3]),
			fbxsdk::FbxVector4(s[0], s[1], s[2], 0)
		);
		return m;
	}


	template<typename T, typename V>
	static V ConvertFromElement(T *element, int vertexIndex)
	{
		switch (element->GetReferenceMode())
		{
		case FbxGeometryElement::eDirect:
		{
			return cast(element->GetDirectArray().GetAt(vertexIndex));
		}
		break;
		case FbxGeometryElement::eIndexToDirect:
		{
			int id = element->GetIndexArray().GetAt(vertexIndex);
			return cast(element->GetDirectArray().GetAt(id));
		}
		break;
		default:
			LOG_ERROR("unknown element");
			break;
		}
	}
};
