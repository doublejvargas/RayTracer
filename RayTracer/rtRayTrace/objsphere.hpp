#pragma once

#include "ObjectBase.hpp"

namespace rt
{
	class ObjSphere : public ObjectBase
	{
	public:
		/* The default constructor
		*   Note that this will define a unit sphere at the origin */
		ObjSphere();

		// Override destructor
		virtual ~ObjSphere() override;

		// Override function that tests intersections
		virtual bool TestIntersection(const Ray &castRay, qbVector<double> &intPoint, qbVector<double> &localNormal, qbVector<double> &localColor) override;

	private:


	};
}
