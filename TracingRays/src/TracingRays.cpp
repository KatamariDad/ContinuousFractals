#include "pch.h"
#include <ctime>
#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <string>

#include <Scene/Scenefactory.h>
#include <Scene/Light/Light.h>
#include <Image/GIF/GifUtil.h>
#include <Image/Image.h>
#include <IOManip/FileSystem.h>
#include <JSON/JSONUtil.h>

#include "Camera.h"

#include <Paint/Primitive2D.h>
#include <Paint/Rasterizer.h>
#include <Paint/Layer.h>

int main()
{

	/*
	Line line( Vector2f( -0.5f, -0.5f ), Vector2f( 1.0f, 1.0f ) );
	Layer2D l;
	l.m_primitives.push_back( &line );

	Image::Image image( 256, 256, "C:\\Users\\Sheri\\Documents\\ContinuousFractals\\Output\\TracingRays.png" );
	LayerClippingWindow window( Vector2f( -1.f, -1.f ), Vector2f( 1.f, 1.f ), image );

	Rasterizer::RasterizeLayerToImage( l, window, image );

	image.Save();

	return 0;
	*/

	using json = nlohmann::json;
	std::ifstream i( "config.json" );
	json config;
	i >> config;

	std::string directory = config["globals"]["OUTPUT_DIR"];
	size_t fixedImageSize = config["globals"]["IMAGE_SIZE"];
	
	// Setup lights
	Vector3f ambientLight;
	JSON::Make(config["lights"]["ambient"], ambientLight);	
	std::vector<LightSharedPtr> lights;
	for (json light : config["lights"]["directional"])
	{
		LightSharedPtr l(new Light(0,0));
		JSON::Make( light, *l );
		lights.push_back( l );
	}
	
	// Setup camera params
	Vector3f eyePosition;
	JSON::Make( config["camera"]["eyePosition"], eyePosition );
	Vector3f view;
	JSON::Make( config["camera"]["view"], view);
	Vector3f up;
	JSON::Make( config["camera"]["up"], up);
	
	double fovy = config["camera"]["fovy"];
	
	// Setup scene
	Factory<SceneNode> sceneFactory;
	SceneNodePtr root = sceneFactory.Build( "", config["scene"] );

	if (!root->IsValid())
	{
		std::cout << "Invalid config" << std::endl;
		return 1;
	}
	
	// Action!
	Camera camera( root.get(), eyePosition, view, up, fovy, ambientLight, lights );
	
	FileSystem::MakeDirectory( directory );
	const std::string gifName = directory + "rayTrace.gif";
	Image::Gif giraffe( gifName.c_str(), fixedImageSize, fixedImageSize );
	
	for( size_t i = 0; i < 100; ++i )
	{
		Vector3f position = camera.GetPosition();
		position.x += 0.1f;
		position.y = std::sinf( position.x  * 2);
		camera.SetPosition( position );

		std::stringstream filename;
		filename << "rayTrace_" << i << ".png";
		Image::Image image( fixedImageSize, fixedImageSize, directory + filename.str() );
		
		camera.Render( image );

		giraffe.AddFrame( image );
		image.Save();
	}
	giraffe.Save();

	return 0;
}

