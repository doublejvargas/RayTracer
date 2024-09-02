#include "gtfm.hpp"

// lib
#include <iostream>
#include <iomanip>

rt::GTform::GTform()
	: m_Fwdtfm{ glm::dmat4(1.0) }, m_Bcktfm{ glm::dmat4(1.0) }
{
	// Set forward and backward transforms to identity matrices (done in initializer list above)
}

rt::GTform::~GTform()
{

}

rt::GTform::GTform(const glm::dmat4 &fwd, const glm::dmat4 &bck)
{
	/* No need to verify dimensions because parameter type 
		guarantees that both matrices are 4x4 */
	m_Fwdtfm = fwd;
	m_Bcktfm = bck;
}

void rt::GTform::SetTransform(	const glm::dvec3 &translation, 
								const glm::dvec3 &rotation, 
								const glm::dvec3 &scale)
{
	// Define a matrix for each component of the transform
	//  and initialize to identity matrix
	glm::dmat4 translationMatrix	{ 1.0 };
	glm::dmat4 rotationMatrixX		{ 1.0 };
	glm::dmat4 rotationMatrixY		{ 1.0 };
	glm::dmat4 rotationMatrixZ		{ 1.0 };
	glm::dmat4 scaleMatrix			{ 1.0 };

	// Populate the matrices with the appropriate values
	// First, the translation matrix
	/*	| 1 0 0 tx |
	*	| 0 1 0 ty |
	*	| 0 0 1 tz |
	*	| 0 0 0 1  | */

	translationMatrix = glm::translate(translationMatrix, translation);
	
	// Then, rotation matrices
	/*  Rz
	*	| cos(θz) -sin(θz) 0 0 |
	*	| sin(θz)  cos(θz) 0 0 |
	*	|   0        0     1 0 |
	*	|   0        0     0 1 |
	*/
	rotationMatrixZ = glm::rotate(rotationMatrixZ, glm::radians(rotation.x), glm::dvec3{ 0.0, 0.0, 1.0 });
	
	/*  Ry
	*	|  cos(θy) 0 sin(θy) 0 |
	*	|    0     1    0	 0 |
	*	| -sin(θy)   cos(θy) 0 |
	*	|    0     0    0	 1 |
	*/
	rotationMatrixY = glm::rotate(rotationMatrixY, glm::radians(rotation.y), glm::dvec3{ 0.0, 1.0, 0.0 });

	/*  Rx
	*	| 1    0		0	 0 |
	*	| 0 cos(θx) -sin(θx) 0 |
	*	| 0 sin(θx)  cos(θx) 0 |
	*	| 0    0		0	 1 |
	*/
	rotationMatrixX = glm::rotate(rotationMatrixX, glm::radians(rotation.z), glm::dvec3{1.0, 0.0, 0.0});

	// Finally, the scale matrix
	scaleMatrix = glm::scale(scaleMatrix, glm::dvec3{ scale.x, scale.y, scale.z });

	// Combine to give the final forward transformation matrix
	// Notice that the order in which the transformations are applied based on this multiplication is:
	//	rotZ, rotY, rotX, scale, translation
	m_Fwdtfm = translationMatrix *
			   scaleMatrix *
			   rotationMatrixX *
			   rotationMatrixY *
			   rotationMatrixZ;

	// Compute the backwards transform
	m_Bcktfm = glm::inverse(m_Fwdtfm);
}

rt::Ray rt::GTform::Apply(const rt::Ray &inputRay, bool dirFlag)
{
	// Create output object
	rt::Ray outputRay{};

	if (dirFlag)
	{
		// Apply the forward transform
		outputRay.p1 = this->Apply(inputRay.p1, rt::FWDTFORM);
		outputRay.p2 = this->Apply(inputRay.p2, rt::FWDTFORM);
		outputRay.lab = outputRay.p2 - outputRay.p1;
	}
	else
	{
		// Apply the backward transform
		outputRay.p1 = this->Apply(inputRay.p1, rt::BCKTFORM);
		outputRay.p2 = this->Apply(inputRay.p2, rt::BCKTFORM);
		outputRay.lab = outputRay.p2 - outputRay.p1;
	}

	return outputRay;
}

glm::dvec3 rt::GTform::Apply(const glm::dvec3 &inputVector, bool dirFlag)
{
	// Convert inputVector to a 4-element vector
	glm::dvec4 tempVector{ inputVector, 1.0 };

	// Create a vector for the result
	glm::dvec4 resultVector{ 0.0 };

	if (dirFlag)
	{
		// Apply the forward transform
		resultVector = m_Fwdtfm * tempVector;
	}
	else
	{
		// Apply the backward transform
		resultVector = m_Bcktfm * tempVector;
	}

	// Reform the output as a 3-element vector
	glm::dvec3 outputVector{ resultVector };

	return outputVector;
}

void rt::GTform::PrintMatrix(bool dirFlag)
{
	if (dirFlag)
	{
		Print(m_Fwdtfm);
	}
	else
	{
		Print(m_Bcktfm);
	}
}

void rt::GTform::PrintVector(const glm::dvec3 &vector)
{;
	for (int row = 0; row < 3; row++)
	{
		std::cout << std::fixed << std::setprecision(3) << vector[row] << std::endl;
	}
}

void rt::GTform::Print(const glm::dmat4 &matrix)
{
	for (int row = 0; row < 4; row++)
	{
		for (int col = 0; col < 4; col++)
		{
			std::cout << std::fixed << std::setprecision(3) << matrix[row][col] << " ";
		}
		std::cout << std::endl;
	}
}

namespace rt
{
	rt::GTform operator*(const rt::GTform &lhs, const rt::GTform &rhs)
	{
		// Form the product of the two forward transforms
		glm::dmat4 fwdResult = lhs.m_Fwdtfm * rhs.m_Fwdtfm;
		
		// Compute the backward transform as the inverse of the forward transform
		/* The "Inverse" function is a numerical function that computes inverse matrix to a significant amount of decimal places, but
			it is not perfectly accurate. By multiplying lhs's and rhs's backward transforms (which are each computed using inverse on their respective forwards)
			we run the risk of compounding the error by multiplying two results of the "Inverse" function. Instead, we call inverse on the product of the forward transforms 
		*/
		glm::dmat4 bckResult = glm::inverse(fwdResult);

		// Form the final result
		rt::GTform finalResult{ fwdResult, bckResult };
		
		return finalResult;
	}
}

// Why not return a reference here?
rt::GTform rt::GTform::operator=(const GTform &rhs)
{
	// Make sure we are not assigning to ourself
	if (this != &rhs)
	{
		m_Fwdtfm = rhs.m_Fwdtfm;
		m_Bcktfm = rhs.m_Bcktfm;
	}

	return *this;
}