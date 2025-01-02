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
