#pragma once

#include "Ray.hpp"

// tools
//#include "toolbox/qbMatrix.h" // includes qbVector.h
#include "toolbox/glm/glm.hpp"
#include "toolbox/glm/ext.hpp"

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
		GTform(const glm::dmat4 &fwd, const glm::dmat4 &bck);

		// Set translation, rotation and scale components
		void SetTransform(
			const glm::dvec3 &translation,
			const glm::dvec3 &rotation,
			const glm::dvec3 &scale
		);

		// Getters
		inline glm::dmat4 GetForward() const  { return m_Fwdtfm; }
		inline glm::dmat4 GetBackward() const { return m_Bcktfm; }

		// Overloaded "apply" function to apply the transformation
		rt::Ray Apply(const rt::Ray &inputRay, bool dirFlag);
		glm::dvec3 Apply(const glm::dvec3 &inputVector, bool dirFlag);

		// Overloaded operators
		friend GTform operator*(const rt::GTform &lhs, const rt::GTform &rhs);
		GTform operator=(const GTform &rhs); // Why not return reference here?

		// Print transform matrix to STDOUT
		void PrintMatrix(bool dirFlag);
		// Print vectors
		static void PrintVector(const glm::dvec3 &vector);

	// PRIVATE METHODS
	private:
		void Print(const glm::dmat4 &matrix);

	// PRIVATE MEMBERS
	private:
		// Forward and backward transforms
		glm::dmat4 m_Fwdtfm;
		glm::dmat4 m_Bcktfm;
	};
}
