
#include "Ray.hpp"

// There is no copy constructor to initialize a qbVector from another qbVector in qbVector.h...
// ... so how is this initializer list working?
rt::Ray::Ray(const qbVector<double> &point1, const qbVector<double> &point2) :
	p1{point1}, p2{point2}, lab{p2 - p1} {}
