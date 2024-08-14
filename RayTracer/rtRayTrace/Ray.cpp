
#include "Ray.hpp"

rt::Ray::Ray(const qbVector<double> &p1, const qbVector<double> &p2) :
	m_p1{p1}, m_p2{p2}, m_lab{m_p2 - m_p1} {}
