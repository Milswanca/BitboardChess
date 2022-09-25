#include "Core/Math.h"

bool UMath::RayPlaneIntersection(const glm::vec3& PlaneNormal, const glm::vec3& PlaneLocation, const glm::vec3& RayOrigin, const glm::vec3& RayDirection, float& T)
{
	// assuming vectors are all normalized
	float Denom = glm::dot(PlaneNormal, RayDirection);
	
	if (Denom > 1e-6)
	{
		glm::vec3 p0l0 = PlaneLocation - RayOrigin;
		T = glm::dot(p0l0, PlaneNormal) / Denom;
		return (T >= 0);
	}

	return false;
}
