#pragma once
#include "RayIntersection.h"
#include <vector>

struct RayIntersectionBuffer
{
public:
	std::vector<RayIntersection> rayIntersections;

	RayIntersectionBuffer(size_t screenXSize);
	~RayIntersectionBuffer();

};

