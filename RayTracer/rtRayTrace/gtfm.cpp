#include "gtfm.hpp"

rt::GTform::GTform()
{
	// Set forward and backward transforms to identity matrices
	m_Fwdtfm.SetToIdentity();
	m_Bcktfm.SetToIdentity();
}

rt::GTform::~GTform()
{

}

rt::GTform::GTform(const qbMatrix2<double> &fwd, const qbMatrix2<double> &bck)
{
	// Verify that the inputs are 4x4
	if ((fwd.GetNumRows() != 4) || (fwd.GetNumCols() != 4) ||
		(bck.GetNumRows() != 4) || (bck.GetNumCols() != 4))
	{
		throw std::invalid_argument("Cannot construct GTform, inputs are not all 4x4.");
	}

	m_Fwdtfm = fwd;
	m_Bcktfm = bck;
}

void rt::GTform::SetTransform(	const qbVector<double> &translation, 
								const qbVector<double> &rotation, 
								const qbVector<double> &scale)
{
	// Define a matrix for each component of the transform
	qbMatrix2<double> translationMatrix	{ 4,4 };
	qbMatrix2<double> rotationMatrixX	{ 4,4 };
	qbMatrix2<double> rotationMatrixY	{ 4,4 };
	qbMatrix2<double> rotationMatrixZ	{ 4,4 };
	qbMatrix2<double> scaleMatrix		{ 4,4 };

	// Set the matrices to identity
	translationMatrix.SetToIdentity();
	rotationMatrixX.SetToIdentity();
	rotationMatrixY.SetToIdentity();
	rotationMatrixZ.SetToIdentity();
	scaleMatrix.SetToIdentity();

	// Populate the matrices with the appropriate values
	// First, the translation matrix
	/*	| 1 0 0 tx |
	*	| 0 1 0 ty |
	*	| 0 0 1 tz |
	*	| 0 0 0 1  | */
	translationMatrix.SetElement(0, 3, translation.GetElement(0));
	translationMatrix.SetElement(1, 3, translation.GetElement(1));
	translationMatrix.SetElement(2, 3, translation.GetElement(2));

	// Then, rotation matrices
	/*  Rz
	*	| cos(θz) -sin(θz) 0 0 |
	*	| sin(θz)  cos(θz) 0 0 |
	*	|   0        0     1 0 |
	*	|   0        0     0 1 |
	*/
	rotationMatrixZ.SetElement(0, 0,  cos(rotation.GetElement(0)));
	rotationMatrixZ.SetElement(1, 0,  sin(rotation.GetElement(0)));
	rotationMatrixZ.SetElement(0, 1, -sin(rotation.GetElement(0)));
	rotationMatrixZ.SetElement(1, 1,  cos(rotation.GetElement(0)));
	
	/*  Ry
	*	|  cos(θy) 0 sin(θy) 0 |
	*	|    0     1    0	 0 |
	*	| -sin(θy)   cos(θy) 0 |
	*	|    0     0    0	 1 |
	*/
	rotationMatrixY.SetElement(0, 0,  cos(rotation.GetElement(1)));
	rotationMatrixY.SetElement(2, 0, -sin(rotation.GetElement(1)));
	rotationMatrixY.SetElement(0, 2,  sin(rotation.GetElement(1)));
	rotationMatrixY.SetElement(2, 2,  cos(rotation.GetElement(1)));

	/*  Rx
	*	| 1    0		0	 0 |
	*	| 0 cos(θx) -sin(θx) 0 |
	*	| 0 sin(θx)  cos(θx) 0 |
	*	| 0    0		0	 1 |
	*/
	rotationMatrixX.SetElement(1, 1,  cos(rotation.GetElement(2)));
	rotationMatrixX.SetElement(1, 2,  sin(rotation.GetElement(2)));
	rotationMatrixX.SetElement(2, 1, -sin(rotation.GetElement(2)));
	rotationMatrixX.SetElement(2, 2,  cos(rotation.GetElement(2)));

	// Finally, the scale matrix
	scaleMatrix.SetElement(0, 0, scale.GetElement(0));
	scaleMatrix.SetElement(1, 1, scale.GetElement(1));
	scaleMatrix.SetElement(2, 2, scale.GetElement(2));

	// Combine to give the final forward transformation matrix
	// Notice that the order in which the transformations are applied based on this multiplication is:
	//	rotZ, rotY, rotX, scale, translation
	m_Fwdtfm = translationMatrix *
			   scaleMatrix *
			   rotationMatrixX *
			   rotationMatrixY *
			   rotationMatrixZ;

	// Compute the backwards transform
	m_Bcktfm = m_Fwdtfm;
	m_Bcktfm.Inverse();
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

qbVector<double> rt::GTform::Apply(const qbVector<double> &inputVector, bool dirFlag)
{
	// Convert inputVector to a 4-element vector
	std::vector<double> tempData{	inputVector.GetElement(0),
									inputVector.GetElement(1),
									inputVector.GetElement(2),
									1.0 };

	qbVector<double> tempVector{ tempData };

	// Create a vector for the result
	qbVector<double> resultVector;

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
	qbVector<double> outputVector { std::vector<double> {	resultVector.GetElement(0),
															resultVector.GetElement(1),
															resultVector.GetElement(2) }};

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

void rt::GTform::PrintVector(const qbVector<double> &vector)
{
	int nRows = vector.GetNumDims();
	for (int row = 0; row < nRows; row++)
	{
		std::cout << std::fixed << std::setprecision(3) << vector.GetElement(row) << std::endl;
	}
}

void rt::GTform::Print(const qbMatrix2<double> &matrix)
{
	int nRows = matrix.GetNumRows();
	int nCols = matrix.GetNumCols();
	for (int row = 0; row < nRows; row++)
	{
		for (int col = 0; col < nCols; col++)
		{
			std::cout << std::fixed << std::setprecision(3) << matrix.GetElement(row, col) << " ";
		}
		std::cout << std::endl;
	}
}

namespace rt
{
	rt::GTform operator*(const rt::GTform &lhs, const rt::GTform &rhs)
	{
		// Form the product of the two forward transforms
		qbMatrix2<double> fwdResult = lhs.m_Fwdtfm * rhs.m_Fwdtfm;
		
		// Compute the backward transform as the inverse of the forward transform
		/* The "Inverse" function is a numerical function that computes inverse matrix to a significant amount of decimal places, but
			it is not perfectly accurate. By multiplying lhs's and rhs's backward transforms (which are each computed using inverse on their respective forwards)
			we run the risk of compounding the error by multiplying two results of the "Inverse" function. Instead, we call inverse on the product of the forward transforms 
		*/
		qbMatrix2<double> bckResult = fwdResult;
		bckResult.Inverse();

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