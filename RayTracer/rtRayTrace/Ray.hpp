#pragma once

//#include "toolbox/qbVector3.hpp"
#include "toolbox/glm/glm.hpp"

namespace rt
{
	class Ray
	{
	public:
		// Default constructor
		Ray();
		// Prereq. of point1 and point2 is that they are 3D vectors
		Ray(const glm::dvec3 &point1, const glm::dvec3 &point2);

		// Getters
		inline glm::dvec3 GetP1() const { return p1; }
		inline glm::dvec3 GetP2() const { return p2; }

	public:
		// The assumption here is 3-D space, and that vectors are 3-dimensional. This is hard-coded.
		glm::dvec3 p1;
		glm::dvec3 p2;
		glm::dvec3 lab; // lab = line from point a to point b (analogous to point1 and point2).

	};
}