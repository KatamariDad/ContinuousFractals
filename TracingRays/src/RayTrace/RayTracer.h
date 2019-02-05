#pragma once

#include <vector>
#include <Math/Vector3.h>

class SceneNode;
namespace Image
{
	class Image;
}

class Light;
struct PixelColour;

class RayTracer
{
public:

	struct TraceParameters;

	static void Trace( 
		const TraceParameters* params, 
		Image::Image* outImage );

	struct TraceParameters
	{
	public:
		TraceParameters(){}
		TraceParameters( const TraceParameters& ) = default;
		TraceParameters(
			const Vector3f& in_topLeft,
			const float in_projection_distance,
			const float in_deltaX,
			const float in_deltaY,
			const SceneNode* in_root,
			const Vector3f& in_eye,
			const Vector3f& in_view,
			const Vector3f& in_up,
			const Vector3f& in_ambient,
			const std::vector<const Light*>& in_lights,
			size_t in_threadId
		) :
			m_topLeft( in_topLeft ),
			m_projectionDistance( in_projection_distance ),
			m_deltaX( in_deltaX ),
			m_deltaY( in_deltaY ),
			m_sceneRoot( in_root ),
			m_eye( in_eye ),
			m_view( in_view ),
			m_up( in_up ),
			m_ambient( in_ambient ),
			m_lights( in_lights ),
			m_threadId( in_threadId )
		{}

		void SetIndices(
			const size_t in_startX,
			const size_t in_endX,
			const size_t in_startY,
			const size_t in_endY)
		{
			m_startX = in_startX;
			m_endX = in_endX;
			m_startY = in_startY;
			m_endY = in_endY;
		}

		size_t m_startX;
		size_t m_endX;
		size_t m_startY;
		size_t m_endY;
		Vector3f m_topLeft;
		float m_projectionDistance;
		float m_deltaX;
		float m_deltaY;
		const SceneNode* m_sceneRoot;
		Vector3f m_eye;
		Vector3f m_view;
		Vector3f m_up;
		Vector3f m_ambient;
		std::vector<const Light*> m_lights;
		size_t m_threadId;
	};

private:
	struct PreLightingParams
	{
		PreLightingParams(
			const SceneNode* root,
			const Vector3f& rayOrigin,
			const Vector3f& rayDir,
			const Vector3f& ambient,
			const std::vector<const Light*>& lights )
			: m_sceneRoot( root )
			, m_rayOrigin( rayOrigin )
			, m_rayDir( rayDir )
			, m_ambient( ambient )
			, m_lights( lights )
		{}

		// What to render
		const SceneNode* m_sceneRoot;
		Vector3f m_rayOrigin;
		Vector3f m_rayDir;
		Vector3f m_ambient;
		const std::vector<const Light*>& m_lights;
	};

	static bool TraceInternal( 
		const PreLightingParams& params, 
		PixelColour& outPixelColour );

};
