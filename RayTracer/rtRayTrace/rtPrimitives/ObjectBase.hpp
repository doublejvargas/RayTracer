#pragma once

#include "Ray.hpp"
#include "gtfm.hpp" 

// tools
#include "toolbox/qbVector.h"

//lib
#include <memory>

namespace rt
{

	/* Forward declare the material base class to avoid circular includes. This will be overriden later */
	class MaterialBase;

	class ObjectBase
	{
	public:
		// Constructor and destructor
		ObjectBase();
		virtual ~ObjectBase();

		// Function to test for intersections
		virtual bool TestIntersection(	const Ray &castRay, qbVector<double> &intPoint, 
										qbVector<double> &localNormal, 
										qbVector<double> &localColor);

		// Set the transform matrix
		void SetTransformMatrix(const rt::GTform &transformMatrix);

		// Function to test whether two floating-point numbers are close to being equal (should this be static?)
		bool CloseEnough(const double f1, const double f2);

		// Assign a material
		bool AssignMaterial(const std::shared_ptr<rt::MaterialBase> &objectMaterial);

	// Public member variables
	public:
		// Base color of the object
		qbVector<double> baseColor_{ 3 };

		// Geometric transform applied to the object
		rt::GTform transformMatrix_;

		// A reference to the material assigned to this object
		std::shared_ptr<rt::MaterialBase> m_pMaterial;

		// A flag to indicate whether this object has a material or not
		bool m_hasMaterial = false;
	};
}
