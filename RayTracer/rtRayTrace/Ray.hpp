#pragma once

#include "toolbox/qbVector.h"

namespace rt
{
	class Ray
	{
	public:
		// Default constructor
		Ray();
		// Prereq. of point1 and point2 is that they are 3D vectors
		Ray(const qbVector<double> &point1, const qbVector<double> &point2);

		// Getters
		inline qbVector<double> GetP1() const { return p1; }
		inline qbVector<double> GetP2() const { return p2; }

	public:
		// The assumption here is 3-D space, and that vectors are 3-dimensional. This is hard-coded.
		qbVector<double> p1		{3};
		qbVector<double> p2		{3};
		qbVector<double> lab	{3}; // lab = line from point a to point b (analogous to point1 and point2).

	};
}