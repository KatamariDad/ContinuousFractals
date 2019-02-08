#include "pch.h"
#include <ctime>
#include <iostream>
#include <sstream>
#include <vector>
#include <string>

#include <Scene/SceneNode.h>
#include <Scene/Geometry/Sphere/Sphere.h>
#include <Scene/Material/Material.h>
#include <Scene/Material/PhongMaterial.h>
#include <Scene/Light/Light.h>
#include <Image/GIF/GifUtil.h>
#include <Image/Image.h>

#include "Camera.h"

#define FIXED_IMAGE_SIZE 256 


int main()
{
	// lights
	Light topRightLight( 
		Vector3f( 1.f, 3.f, -2.5f ), 
		Vector3f( 20, 0, 0 ) );
	Light bottomLight( 
		Vector3f( 0.f, -3.f, -2.5f ), 
		Vector3f( 200, 200, 200 ) );

	std::vector<const Light*> lights;
	lights.push_back( &bottomLight );
	lights.push_back( &topRightLight );

	PhongMaterial phongMaterial(
		Vector3f( 5.f, 1.f, 0.f ),
		Vector3f( 1.f, 0.f, 0.f ),
		Vector3f( 0.f, 0.f, 30.f ),
		100, 100 );

	Sphere sphere( 2.f );
	// look down the Z positive axis
	const Vector3f eyePosition( 0.f, 0.f, -3.f );
	const Vector3f view( 0.f, 0.f, 1.f );
	const Vector3f up( 0.f, 1.f, 0.f );
	const Vector3f ambientLight( 0.f, 0.f, 0.f ); // TODO: as PixelColour
	double fovy = 15.f;

	const std::string directory( "D:\\Projects\\ContinuousFractals\\out\\" );
	const std::string gifName = directory + "rayTrace.gif";

	Image::Gif giraffe( gifName.c_str(), FIXED_IMAGE_SIZE, FIXED_IMAGE_SIZE );
	Vector3f sceneLocation( -3.f, 0.f, 0.f );
	SceneNode root( sceneLocation, sphere, phongMaterial );

	Camera camera( &root, eyePosition, view, up, fovy, ambientLight, lights );
	
	for( size_t i = 0; i < 70; ++i )
	{
		Vector3f position = camera.GetPosition();
		position.x += 0.1f;
		position.y = std::sinf( position.x  * 2);
		camera.SetPosition( position );

		//if( i % 2 == 0 )
		{
			//phongMaterial.m_diffuse.z = i * 0.1f;
			//phongMaterial.m_diffuse.x = 5.f - ( i * 0.1f );
		}
		//else
		//{
		//	phongMaterial.m_diffuse.z = 0.f;
		//	phongMaterial.m_diffuse.x = 0.f;
		//}

		
		std::stringstream filename;
		filename << "rayTrace_" << i << ".png";
		Image::Image image( FIXED_IMAGE_SIZE, FIXED_IMAGE_SIZE, directory + filename.str() );
		
		camera.Render( image );

		giraffe.AddFrame( image );
		image.Save();
	}
	giraffe.Save();

	return 0;
}


