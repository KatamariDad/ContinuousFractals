#pragma once

template<typename T>
T clamp( T f, T min, T max )
{
	f = f < min ? min : f;
	f = f > max ? max : f;
	return f;
}

template<typename T>
T max( T a, T b )
{
	return a > b ? a : b;
}

template<typename T>
T min( T a, T b )
{
	return a < b ? a : b;
}

template<typename V>
bool RayTriangleIntersect(
	const V& rayOrigin, 
	const V& rayDir, 
	const V& V1,
	const V& V2, 
	const V& V3,
    V& outHitLocation,
    V& outNormal)
{
    // impl from https://en.wikipedia.org/wiki/M%C3%B6ller%E2%80%93Trumbore_intersection_algorithm
    constexpr float epsilon = std::numeric_limits<float>::epsilon();

    const V edge1 = V2 - V1;
    const V edge2 = V3 - V1;
    const V ray_cross_e2 = CrossProduct(rayDir, edge2);
    const double det = DotProduct(edge1, ray_cross_e2);

    if (det > -epsilon && det < epsilon)
    {
        // This ray is parallel to this triangle.
        return false;
    }

    double inv_det = 1.0 / det;
    const V s = rayOrigin - V1;
    double u = inv_det * DotProduct(s, ray_cross_e2);

    if ((u < 0 && abs(u) > epsilon) || (u > 1 && abs(u - 1) > epsilon))
    {
        return false;
    }

    const V s_cross_e1 = CrossProduct(s, edge1);
    const double v = inv_det * DotProduct(rayDir, s_cross_e1);

    if ((v < 0 && abs(v) > epsilon) || (u + v > 1 && abs(u + v - 1) > epsilon))
    {
        return false;
    }

    // At this stage we can compute t to find out where the intersection point is on the line.
    const double t = inv_det * DotProduct(edge2, s_cross_e1);

    if (t > epsilon) // ray intersection
    {
        outHitLocation = rayOrigin + t * rayDir;
        outNormal = CrossProduct(edge1, edge2).ComputeNormal();
        return true;
    }
    else // This means that there is a line intersection but not a ray intersection.
    {
        return false;
    }

	return false;
}

template<typename V>
bool AABBIntersectRay(
	const V& boxOrigin,
	const V& boxHalfExtent,
	const V& rayOrigin,
	const V& rayDir,
	V& hitLocation,
	V& hitNormal) 
{
	const V Min = boxOrigin - boxHalfExtent;
	const V Max = boxOrigin + boxHalfExtent;

	// from https://gamedev.stackexchange.com/questions/18436/most-efficient-aabb-vs-ray-collision-algorithms
	V rayDirInv(0.f);
	constexpr float epsilon = std::numeric_limits<float>::epsilon();
	rayDirInv.x = 1.0f / ((abs(rayDir.x) > 0.f) ? rayDir.x : epsilon);
	rayDirInv.y = 1.0f / ((abs(rayDir.y) > 0.f) ? rayDir.y : epsilon);
	rayDirInv.z = 1.0f / ((abs(rayDir.z) > 0.f) ? rayDir.z : epsilon);

	float t1 = (Min.x - rayOrigin.x) * rayDirInv.x;
	float t2 = (Max.x - rayOrigin.x) * rayDirInv.x;
	float t3 = (Min.y - rayOrigin.y) * rayDirInv.y;
	float t4 = (Max.y - rayOrigin.y) * rayDirInv.y;
	float t5 = (Min.z - rayOrigin.z) * rayDirInv.z;
	float t6 = (Max.z - rayOrigin.z) * rayDirInv.z;

	float tmin = max(max(min(t1, t2), min(t3, t4)), min(t5, t6));
	float tmax = min(min(max(t1, t2), max(t3, t4)), max(t5, t6));

	float t = 0.f;
	// if tmax < 0, ray (line) is intersecting AABB, but the whole AABB is behind us
	if (tmax < 0)
	{
		t = tmax;
		return false;
	}

	// if tmin > tmax, ray doesn't intersect AABB
	if (tmin > tmax)
	{
		t = tmax;
		return false;
	}

	t = tmin;
	hitLocation = (t * rayDir) + rayOrigin;

	// computing the normal - https://blog.johnnovak.net/2016/10/22/the-nim-ray-tracer-project-part-4-calculating-box-normals/
	const V D = 0.5f * (Max - Min);
	const V P = hitLocation - boxOrigin;
	constexpr float bias = 1.00001;

	hitNormal.x = (int)((P.x / D.x) * bias);
	hitNormal.y = (int)((P.y / D.y) * bias);
	hitNormal.z = (int)((P.z / D.z) * bias);
	hitNormal.Normalize();
	return true;
}
