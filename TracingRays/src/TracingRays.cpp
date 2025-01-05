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

#include "Time/Stopwatch.h"
#include "Scene/Geometry/VoxelizedShape.h"

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

	/// <summary>
	/// /// TEST
	/// </summary>
	/// <returns></returns>
	bool bTestOutput = false;
	bool bTestInput = false;
	if (bTestOutput)
	{

		std::ofstream testOut(
			"C:\\Users\\Sheri\\Documents\\ContinuousFractals\\Output\\TracingRays\\Vox.txt",
			std::ios::out);

		std::vector<std::vector<std::vector<bool>>> VoxelData;
		VoxelData.push_back(std::vector<std::vector<bool>>());
		VoxelData.push_back(std::vector<std::vector<bool>>());
		VoxelData.push_back(std::vector<std::vector<bool>>());

		VoxelData[0].push_back(std::vector<bool>());
		VoxelData[0].push_back(std::vector<bool>());
		VoxelData[0].push_back(std::vector<bool>());
		VoxelData[1].push_back(std::vector<bool>());
		VoxelData[1].push_back(std::vector<bool>());
		VoxelData[1].push_back(std::vector<bool>());
		VoxelData[2].push_back(std::vector<bool>());
		VoxelData[2].push_back(std::vector<bool>());
		VoxelData[2].push_back(std::vector<bool>());

		VoxelData[0][0].push_back(false);
		VoxelData[0][0].push_back(false);
		VoxelData[1][0].push_back(false);
		VoxelData[1][0].push_back(false);
		VoxelData[2][0].push_back(false);
		VoxelData[2][0].push_back(false);

		VoxelData[0][1].push_back(false);
		VoxelData[0][1].push_back(false);
		VoxelData[1][1].push_back(false);
		VoxelData[1][1].push_back(false);
		VoxelData[2][1].push_back(false);
		VoxelData[2][1].push_back(false);

		VoxelData[0][2].push_back(false);
		VoxelData[0][2].push_back(false);
		VoxelData[1][2].push_back(false);
		VoxelData[1][2].push_back(false);
		VoxelData[2][2].push_back(false);
		VoxelData[2][2].push_back(false);

		VoxelData[1][1][0] = true;
		VoxelData[1][0][1] = true;
		VoxelData[2][2][0] = true;

		VoxelizedShape* V = new VoxelizedShape(Vector3f(1), VoxelData);
		V->Serialize(testOut);
		testOut.close();
		delete V;
		return 0;
	}

	if (bTestInput)
	{
		std::ifstream testIn(
			"C:\\Users\\Sheri\\Documents\\ContinuousFractals\\Output\\TracingRays\\Vox.txt",
			std::ios::in);
		VoxelizedShape* V = new VoxelizedShape();
		V->Deserialize(testIn);


		delete V;
		return 0;
	}

	
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

	Time::Stopwatch stopwatch;
	stopwatch.Start();
	
	for( size_t i = 0; i < 30; ++i )
	{
		Vector3f position = camera.GetPosition();
		position.x += 0.001f;
		position.y = std::sinf( position.x  * 2);
		camera.SetPosition( position );

		std::stringstream filename;
		filename << "rayTrace_" << i << ".png";
		Image::Image image( fixedImageSize, fixedImageSize, directory + filename.str() );
		
		camera.Render( image );

		giraffe.AddFrame( image );
		image.Save();

		std::cout << " - " << stopwatch.Lap() << " s / " << stopwatch.TotalTime() << " s\n";
	}
	giraffe.Save();

	return 0;
}

