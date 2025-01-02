#pragma once
#include "SceneNode.h"

#include "Geometry/Geometry.h"
#include "Geometry/Cube/AABB.h"
#include "Geometry/Sphere/Sphere.h"
#include "Material/Material.h"
#include "Material/PhongMaterial.h"

#include <Factory/Factory.h>
#include <JSON/JSONUtil.h>


template<>
class Factory<Geometry>
{
public:
	GeometryPtr Build( std::string key, nlohmann::json params )
	{
		if (key == "sphere")
		{
			return GeometryPtr( new Sphere( params["radius"] ));
		}
		else if (key == "AABB")
		{
			Vector3f extent;
			JSON::Make(params["extent"], extent);
			return GeometryPtr( new AABB( extent ));
		}
		return nullptr;
	}

	GeometryPtr Build( nlohmann::json params )
	{
		if (params["type"] == "sphere")
		{
			return GeometryPtr( new Sphere( params["radius"] ) );
		}
		else if (params["type"] == "AABB")
		{
			Vector3f extent;
			JSON::Make(params["extent"], extent);
			return GeometryPtr( new AABB( extent ));
		}
		return nullptr;
	}
};

template<>
class Factory<Material>
{
public:
	MaterialPtr Build( std::string key, nlohmann::json params )
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
			return MaterialPtr( new PhongMaterial( diffuse, specular, ambient, shininess, diffusePower ) );
		}
		return nullptr;
	}

	MaterialPtr Build( nlohmann::json params )
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
			return MaterialPtr( new PhongMaterial( diffuse, specular, ambient, shininess, diffusePower ) );
		}
		return nullptr;
	}
};


template<>
class Factory<SceneNode>
{
public:
	SceneNodePtr Build( std::string key, nlohmann::json params )
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
		
		return SceneNodePtr( new SceneNode( 
			position, 
			geoFactory.Build( params["geometry"] ),
			materialFactory.Build( params["material"] )
		));
	}

private:

};
