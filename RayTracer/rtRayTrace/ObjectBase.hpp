#pragma once

#include "Ray.hpp"
#include "toolbox/qbVector.h"

namespace rt
{
	class ObjectBase
	{
	public:
		// Constructor and destructor
		ObjectBase();
		virtual ~ObjectBase();

		// Function to test for intersections
		virtual bool TestIntersection(const Ray &castRay, qbVector<double> &intPoint, qbVector<double> &localNormal, qbVector<double> &localColor);

		// Function to test whether two floating-point numbers are close to being equal (should this be static?)
		bool CloseEnough(const double f1, const double f2);

	// Public member variables
	public:
		qbVector<double> baseColor{ 3 };
	};
}
