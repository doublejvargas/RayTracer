#pragma once

#include "toolbox/qbVector.h"

namespace rt
{
	class Ray
	{
	public:
		// Constructor (we don't need a default constructor for this class) (?)
		Ray(const qbVector<double> &p1, const qbVector<double> &p2);

		// Getters
		inline qbVector<double> GetP1() const { return m_p1; }
		inline qbVector<double> GetP2() const { return m_p2; }

	public:
		qbVector<double> m_p1	{3};
		qbVector<double> m_p2	{3};
		qbVector<double> m_lab	{3};

	};
}