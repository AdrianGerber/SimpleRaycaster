#include "RayIntersectionBuffer.h"

RayIntersectionBuffer::RayIntersectionBuffer(size_t screenXSize)
{
	rayIntersections.clear();
	rayIntersections.reserve(screenXSize);
	for (size_t index{ 0 }; index < screenXSize; index++) {
		rayIntersections.push_back(RayIntersection());
	}
}

RayIntersectionBuffer::~RayIntersectionBuffer()
{
}
