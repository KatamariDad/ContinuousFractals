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
			return std::unique_ptr<Geometry>( new Sphere( params["radius"] ));
		}
		return nullptr;
	}

	std::unique_ptr<Geometry> Build( nlohmann::json params )
	{
		if (params["type"] == "sphere")
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

	std::unique_ptr<Material> Build( nlohmann::json params )
	{
		if (params["type"] == "phong")
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
		// Soon, precious
		//nlohmann::json geometryList = params["geometry"];
		//std::vector< std::unique_ptr<Geometry> > sceneGeometry;
		//for (nlohmann::json geometry : geometryList)
		//{
		//	std::unique_ptr<Geometry> geo = geometryFactory.Build( geometry["type"], geoParams );
		//	if (geo != nullptr)
		//	{
		//		sceneGeometry.push_back( geo );
		//	}
		//}


		Factory<Geometry> geoFactory;
		Factory<Material> materialFactory;
		
		Vector3f position;
		JSON::Make( params["position"], position );
		
		return std::unique_ptr<SceneNode>( new SceneNode( 
			position, 
			geoFactory.Build( params["geometry"] ),
			materialFactory.Build( params["material"] )
		));
	}

private:

};
