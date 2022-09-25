#pragma once
#include "Core/Object.h"
#include <glm/glm.hpp>

class UMath : public UObject
{
protected:
	IMPLEMENT_CONSTRUCTOR(UMath, UObject);

public:
	static bool RayPlaneIntersection(const glm::vec3& PlaneNormal, const glm::vec3& PlaneLocation, const glm::vec3& RayOrigin, const glm::vec3& RayDirection, float& T);
};

