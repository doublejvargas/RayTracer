#pragma once

#include "toolbox/qbVector3.hpp"

namespace rt
{
	class Ray
	{
	public:
		// Default constructor
		Ray();
		// Prereq. of point1 and point2 is that they are 3D vectors
		Ray(const qbVector3<double> &point1, const qbVector3<double> &point2);

		// Getters
		inline qbVector3<double> GetP1() const { return p1; }
		inline qbVector3<double> GetP2() const { return p2; }

	public:
		// The assumption here is 3-D space, and that vectors are 3-dimensional. This is hard-coded.
		qbVector3<double> p1;
		qbVector3<double> p2;
		qbVector3<double> lab; // lab = line from point a to point b (analogous to point1 and point2).

	};
}