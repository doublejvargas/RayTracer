#include "Ray.hpp"

rt::Ray::Ray()
	: p1{ glm::dvec3{ 0.0 } },
	  p2{ glm::dvec3{ 0.0 } },
	  lab{ p2 - p1 }
{}

// There is no copy constructor to initialize a qbVector from another qbVector in qbVector.h...
// ... so how is this initializer list working?
rt::Ray::Ray(const glm::dvec3 &point1, const glm::dvec3 &point2) 
	: p1{ point1 }, 
	  p2{ point2 }, 
	  lab{ p2 - p1 } 
{}
