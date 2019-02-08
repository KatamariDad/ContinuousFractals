#pragma once
#include "SceneNode.h"

#include "Geometry/Geometry.h"
#include "Geometry/Sphere/Sphere.h"
#include "Material/Material.h"
#include "Material/PhongMaterial.h"

#include <Factory/Factory.h>
#include <JSON/JSONUtil.h>


template<>
class Factory<Geometry>
{
public:
	std::unique_ptr<Geometry> Build( std::string key, nlohmann::json params )
	{
		if (key == "sphere")
		{
			return std::unique_ptr<Geometry>( new Sphere( params["radius"] ) );
		}

		return nullptr;
	}
};

template<>
class Factory<Material>
{
public:
	std::unique_ptr<Material> Build( std::string key, nlohmann::json params )
	{
		if (key == "phong")
		{
			Vector3f diffuse;
			Vector3f specular;
			Vector3f ambient;

			JSON::Make( params["diffuse"], diffuse );
			JSON::Make( params["specular"], specular );
			JSON::Make( params["ambient"], ambient );

			float shininess = params["shininess"];
			float diffusePower = params["diffusePower"];

			return std::unique_ptr<Material>( new PhongMaterial( diffuse, specular, ambient, shininess, diffusePower ) );
		}

		return nullptr;
	}
};

template<>
class Factory<SceneNode>
{
public:
	std::unique_ptr<SceneNode> Build( std::string key, nlohmann::json params )
	{
		Factory<Geometry> geometryFactory;
		nlohmann::json geoParams = params["geometry"];
		std::unique_ptr<Geometry> geo = geometryFactory.Build( geoParams["type"], geoParams );
		Material material;
		return std::unique_ptr<SceneNode>( new SceneNode( *geo, material ) );
	}

private:
	void RenderScene(
		SceneNode* root,
		const std::vector<Light *>& lights,
		nlohmann::json params );

};
