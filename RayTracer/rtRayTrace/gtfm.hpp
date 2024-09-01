#pragma once

#include "Ray.hpp"

// tools
#include "toolbox/qbMatrix.h" // includes qbVector.h

namespace rt
{
	constexpr bool FWDTFORM = true;
	constexpr bool BCKTFORM = false;

	class GTform
	{
	public:
		// Default Constructor & destructor
		GTform();
		~GTform();

		// Parametrized constructors
		// Construct from a pair of matrices
		GTform(const qbMatrix2<double> &fwd, const qbMatrix2<double> &bck);

		// Set translation, rotation and scale components
		void SetTransform(
			const qbVector3<double> &translation,
			const qbVector3<double> &rotation,
			const qbVector3<double> &scale
		);

		// Getters
		inline qbMatrix2<double> GetForward() const	 { return m_Fwdtfm; }
		inline qbMatrix2<double> GetBackward() const { return m_Bcktfm; }

		// Overloaded "apply" function to apply the transformation
		rt::Ray Apply(const rt::Ray &inputRay, bool dirFlag);
		qbVector3<double> Apply(const qbVector3<double> &inputVector, bool dirFlag);

		// Overloaded operators
		friend GTform operator*(const rt::GTform &lhs, const rt::GTform &rhs);
		GTform operator=(const GTform &rhs); // Why not return reference here?

		// Print transform matrix to STDOUT
		void PrintMatrix(bool dirFlag);
		// Print vectors
		static void PrintVector(const qbVector3<double> &vector);

	// PRIVATE METHODS
	private:
		void Print(const qbMatrix2<double> &matrix);

	// PRIVATE MEMBERS
	private:
		qbMatrix2<double> m_Fwdtfm{ 4, 4 };
		qbMatrix2<double> m_Bcktfm{ 4, 4 };
	};
}
