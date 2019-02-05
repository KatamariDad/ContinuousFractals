#include "pch.h"
#include <ctime>
#include <iostream>
#include <thread>
#include <vector>
#include <string>

#include <Scene/SceneNode.h>
#include <Scene/Geometry/Sphere/Sphere.h>
#include <Scene/Material/Material.h>
#include <Scene/Material/PhongMaterial.h>
#include <Scene/Light/Light.h>
#include <Image/Image.h>
#include <RayTrace/RayTracer.h>


#define PI 3.14159265358979323846
#define FIXED_IMAGE_SIZE 256
#define NUM_THREADS 16


void RenderScene(
		// What to render
		SceneNode* root,
		// Image to write to, set to a given width and height
		Image::Image& image,
		// Viewing parameters
		const Vector3f& eye,
		const Vector3f& view,
		const Vector3f& up,
		double fovy,
		// Lighting parameters
		const Vector3f& ambient,
		const std::vector<const Light *>& lights
) {

    // Render the Scene
    constexpr size_t fixedImageSize = FIXED_IMAGE_SIZE;
	const size_t h = fixedImageSize; //image.height();
	const size_t w = fixedImageSize; //image.width();
    const float r = h / w;
    const float d = 100.f;
    const float fovy_radian = fovy * (PI / 180.f);

    const float windowH = 2 * d * std::tan( fovy_radian );
    const float windowW = r * windowH;
    const float deltaX = windowW / (float)w;
    const float deltaY = windowH / (float)h;

    const float projection_distance = d; 
    const Vector3f forward = view.ComputeNormal();
    const Vector3f projection_centerpoint = eye + projection_distance * forward;
    const Vector3f left = CrossProduct(forward, up.ComputeNormal());

    const Vector3f topLeft = 
        projection_centerpoint - ((double)windowW / 2.f) * left + ((double)windowH / 2.f) * up;

    RayTracer::TraceParameters params[NUM_THREADS];
    std::thread threads[NUM_THREADS];
    
    size_t width_x = fixedImageSize; // (float)fixedImageSize / (float)NUM_THREADS;
    float width_y = (float)fixedImageSize / (float)NUM_THREADS;
    for (size_t i = 0; i < NUM_THREADS; ++i)
    {
        params[i] = RayTracer::TraceParameters(
            topLeft, projection_distance, deltaX, deltaY,
            root, eye, view, up, ambient, lights, i
            );
        params[i].SetIndices(
            0, fixedImageSize,
            width_y * i, (width_y * (i + 1)));

        threads[i] =  std::thread(RayTracer::Trace, &params[i], &image);
    }

    for (size_t i = 0; i < NUM_THREADS; ++i)
    {
        threads[i].join();
    }

}


int main()
{
	// lights
	Light topLight( 
		Vector3f( 0.f, 5.f, 0.f ), 
		Vector3f( 200.2, 0, 0 ) );

	std::vector<const Light*> lights;
	lights.push_back( &topLight );

	PhongMaterial phongMaterial;
	phongMaterial.m_diffuse = Vector3f( 0.f, 200.f, 0.f );
	phongMaterial.m_specular = Vector3f( 0.f, 200.f, 0.f );
	phongMaterial.m_shininess = 2;

	Sphere sphere( 2.f );
	SceneNode root(sphere, phongMaterial);
	const std::string directory( "D:\\Projects\\ContinuousFractals\\out\\" );
	const std::string filename = "rayTrace.png";
	Image::Image image( FIXED_IMAGE_SIZE, FIXED_IMAGE_SIZE, directory + filename );
	// z forward
	const Vector3f eyePosition( 0.f, 0.f, -5.f );
	const Vector3f view( 0.f, 0.f, 1.f );
	const Vector3f up( 0.f, 1.f, 0.f );
	const Vector3f ambientLight( 0.f ); // TODO: as PixelColour
	double fovy = 15.f;

	RenderScene( &root,
		image, eyePosition,
		view, up, fovy, 
		ambientLight, lights );


	image.Save();
	return 0;
}


