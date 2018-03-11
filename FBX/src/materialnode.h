#pragma once

#include <nodetree.h>
#include <face.h>
#include <point.h>
#include <vector>
#include <matrix4x4.h>
#include <clusternode.h>

typedef struct _TextureNode
{
	std::string name;
	std::string filename;
	std::string uvSet;
	vec2f uvScale;
	vec2f uvTranslation;
} TextureNode;

class MaterialPropNode
{
public:
	std::string type;
	vec3f	color;
	float	opacity;
	std::vector<TextureNode> textures;

	static MaterialPropNode ZERO;
};

class MaterialNode
{
public:
	MaterialNode();
	~MaterialNode();

	void					SetName(const std::string &name);
	const std::string&		GetName() const;

	MaterialPropNode&				GetPropNode(std::string name);
	void    						SetPropNode(const MaterialPropNode &v);

	const std::vector<MaterialPropNode>&  GetPropNodes();

private:

	std::string							mName;
	std::vector<MaterialPropNode>       mProps;
};
