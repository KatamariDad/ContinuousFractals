#pragma once
#include "SceneNode.h"

#include "Geometry/Geometry.h"
#include "Geometry/Cube/AABB.h"
#include "Geometry/Sphere/Sphere.h"
#include "Geometry/VoxelizedShape.h"
#include "Material/Material.h"
#include "Material/PhongMaterial.h"

#include <Factory/Factory.h>
#include <JSON/JSONUtil.h>


template<>
class Factory<Geometry>
{
public:

	static GeometryPtr Build(const nlohmann::json& params )
	{
		if (params["type"] == Sphere::GetJsonTypeName())
		{
			return GeometryPtr( new Sphere( params["radius"] ) );
		}
		else if (params["type"] == AABB::GetJsonTypeName())
		{
			Vector3f extent;
			JSON::Make(params["extent"], extent);
			return GeometryPtr( new AABB( extent ));
		}
		else if (params["type"] == VoxelizedShape::GetJsonTypeName())
		{
			return GeometryPtr( new VoxelizedShape(params["voxFile"]));
		}
		return nullptr;
	}
};

template<>
class Factory<Material>
{
public:

	static MaterialPtr Build(const nlohmann::json& params ) 
	{
		if (params["type"] == PhongMaterial::GetJsonTypeName())
		{
			Vector3f diffuse;
			Vector3f specular;
			Vector3f ambient;

			JSON::Make( params["diffuse"], diffuse );
			JSON::Make( params["specular"], specular );
			JSON::Make( params["ambient"], ambient );

			const float shininess = params["shininess"];
			const float diffusePower = params["diffusePower"];
			return MaterialPtr( new PhongMaterial( diffuse, specular, ambient, shininess, diffusePower ) );
		}
		return nullptr;
	}
};


template<>
class Factory<SceneNode>
{
public:
	SceneNodePtr Build(const std::string& key, const nlohmann::json& params )
	{
		const nlohmann::json& geometryList = params["geometryList"];
		const nlohmann::json& materialList = params["materialList"];

		SceneNodePtr rootNode = nullptr;
		for (const nlohmann::json& geometry : geometryList)
		{
			Vector3f position;
			JSON::Make(geometry["position"], position );

			std::string material = geometry["material"];
			
			SceneNodePtr NewNode = SceneNodePtr( new SceneNode(
				position, 
				Factory<Geometry>::Build(geometry),
				Factory<Material>::Build(materialList[material])));

			if (NewNode != nullptr)
			{
				if (rootNode == nullptr)
				{
					rootNode.reset(NewNode.release());
				}
				else
				{
					rootNode->AddChild(std::move(NewNode));
				}
			}
		}

		return rootNode;
	}

private:

};
