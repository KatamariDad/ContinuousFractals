#include "pch.h"
#include "Camera.h"

#include <RayTrace/RayTracer.h>

#include <thread>

#define PI 3.14159265358979323846
#define NUM_THREADS 16

void Camera::Render( Image::Image& image )
{
	const size_t fixedImageSize = image.GetHeight();
	const size_t h = fixedImageSize;
	const size_t w = fixedImageSize;
	const float r = h / w;
	const float d = 1.f;
	const float fovy_radian = m_fovy * (PI / 180.f);

	const float windowH = 2 * (d / std::tan( fovy_radian ));
	const float windowW = r * windowH;
	const float deltaX = windowW / (float)w;
	const float deltaY = windowH / (float)h;

	const float projection_distance = d;
	const Vector3f forward = m_view.ComputeNormal();
	const Vector3f projection_centerpoint = m_eye + projection_distance * forward;
	const Vector3f left = CrossProduct( forward, m_up.ComputeNormal() );

	const Vector3f topLeft =
		projection_centerpoint + ((double)windowW / 2.f) * left + ((double)windowH / 2.f) * m_up;

	std::vector<RayTracer::TraceParameters> params;
	std::thread threads[NUM_THREADS];

	size_t width_x = fixedImageSize; // (float)fixedImageSize / (float)NUM_THREADS;
	float width_y = (float)fixedImageSize / (float)NUM_THREADS;
	for (size_t i = 0; i < NUM_THREADS; ++i)
	{
		RayTracer::TraceParameters param(
			topLeft, projection_distance, deltaX, deltaY,
			m_root, m_eye, m_view, m_up, m_ambient, m_lights, i
		);

		param.SetIndices(
			0, fixedImageSize,
			width_y * i, (width_y * (i + 1)) );

		params.push_back( param );
	}

	for (size_t i = 0; i < NUM_THREADS; ++i)
	{
		threads[i] = std::thread( RayTracer::Trace, &params[i], &image );
	}

	for (size_t i = 0; i < NUM_THREADS; ++i)
	{
		threads[i].join();
	}
}

Camera& Camera::Pan( const Vector3f& offset )
{
	m_root->m_relativePosition + offset;
	return *this;
}