#pragma once

#include "ObjectBase.hpp"
#include "gtfm.hpp"

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
		virtual bool TestIntersection(	const Ray &castRay, 
										qbVector3<double> &intPoint, 
										qbVector3<double> &localNormal, 
										qbVector3<double> &localColor) override;

	private:


	};
}
