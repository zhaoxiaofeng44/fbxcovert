#pragma once
#include <fbxsdk.h>
#include <vec2f.h>
#include <vec3f.h>
#include <vec4f.h>
#include <matrix4x4.h>
#include <vector>
#include <string>
#include <map>
#include <face.h>
#include <point.h>
#include <node.h>

using namespace fbxsdk;

class FBXCast
{
public:
	
	
	static inline vec2f cast(FbxDouble2 v)
	{
		return vec2f(v[0],v[1]);
	}

	static inline vec3f cast(FbxDouble3 v)
	{
		return vec3f(v[0], v[1], v[2]);
	}

	static inline vec4f cast(FbxDouble4 v)
	{
		return vec4f(v[0], v[1], v[2], v[3]);
	}

	static inline FbxDouble4 cast(vec4f v)
	{
		return FbxDouble4(v.x, v.y, v.z, v.w);
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
