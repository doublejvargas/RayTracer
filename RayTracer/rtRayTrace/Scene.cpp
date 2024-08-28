#include "Scene.hpp"

rt::Scene::Scene()
{
	// Configure the camera 
	m_Camera.SetPosition(qbVector<double>{std::vector<double>{0.0, -10.0, 1.0}});
	m_Camera.SetLookAt	(qbVector<double>{std::vector<double>{0.0, 0.0, 0.0}});
	m_Camera.SetUpVector(qbVector<double>{std::vector<double>{0.0, 0.0, 1.0}});
	m_Camera.SetProjScreenWidth(0.25);
	m_Camera.SetAspect(16.0 / 9.0);
	m_Camera.UpdateCameraGeometry();
	
	// make a sphere
// 	m_objectList.push_back(std::make_shared<rt::ObjSphere>(rt::ObjSphere{}));
// 	m_objectList.push_back(std::make_shared<rt::ObjSphere>(rt::ObjSphere{}));
// 	m_objectList.push_back(std::make_shared<rt::ObjSphere>(rt::ObjSphere{}));

	// make a plane
	m_objectList.push_back(std::make_shared<rt::ObjPlane>(rt::ObjPlane{}));
	m_objectList.at(0)->baseColor_ = qbVector<double>{ std::vector<double>{128.0, 128.0, 128.0} };

	// Modify the spheres
	// Set transformations
	rt::GTform matrix1, matrix2, matrix3;
	matrix1.SetTransform(	qbVector<double>{std::vector<double>{-1.5, 0.0, 0.0}},
							qbVector<double>{std::vector<double>{0.0, 0.0, 0.0}},
							qbVector<double>{std::vector<double>{0.5, 0.5, 0.75}});

	matrix2.SetTransform(	qbVector<double>{std::vector<double>{0.0, 0.0, 0.0}},
							qbVector<double>{std::vector<double>{0.0, 0.0, 0.0}},
							qbVector<double>{std::vector<double>{0.75, 0.5, 0.5}});

	matrix3.SetTransform(	qbVector<double>{std::vector<double>{1.5, 0.0, 0.0}},
							qbVector<double>{std::vector<double>{0.0, 0.0, 0.0}},
							qbVector<double>{std::vector<double>{0.75, 0.75, 0.75}});

// 	m_objectList.at(0)->SetTransformMatrix(matrix1);
// 	m_objectList.at(1)->SetTransformMatrix(matrix2);
// 	m_objectList.at(2)->SetTransformMatrix(matrix3);
	
	// Set light color 
// 	m_objectList.at(0)->baseColor_ = qbVector<double>{ std::vector<double>{64.0, 128.0, 200.0} };
// 	m_objectList.at(1)->baseColor_ = qbVector<double>{ std::vector<double>{255.0, 128.0, 0.0} };
// 	m_objectList.at(2)->baseColor_ = qbVector<double>{ std::vector<double>{255.0, 200.0, 0.0} };

	// make a point light
	m_lightList.push_back(std::make_shared<rt::PointLight>(rt::PointLight()));
	m_lightList.at(0)->m_Location = qbVector<double>{ std::vector<double>{5.0, -10.0, -5.0} }; // -z value because currently the z axis is inverted in our application.
	m_lightList.at(0)->m_Color = qbVector<double>{ std::vector<double>{255.0, 255.0, 255.0} };
}

bool rt::Scene::Render(rtImage &outputImage)
{
	// Get the dimensions of the output image
	int width = outputImage.GetWidth();
	int height = outputImage.GetHeight();

	// Loop over each pixel in our image
	rt::Ray cameraRay;
	qbVector<double> intPoint	(3);
	qbVector<double> localNormal(3);
	qbVector<double> localColor	(3);
	// used to normalize the pixels from our typical dimensions (eg 1280 x 720) to range between [0, 1].
	double xFactor = 1.0 / (static_cast<double>(width) / 2.0);	// [0, 2]
	double yFactor = 1.0 / (static_cast<double>(height) / 2.0); // [0, 2]
	double minDist = 1e6;
	double maxDist = 0.0;
	for (int x = 0; x < width; x++)
	{
		for (int y = 0; y < height; y++)
		{
			// Normalize x and y coordinates
			double normX = (static_cast<double>(x) * xFactor) - 1.0; // x * xFactor is in range [0, 2], so subtracting one yields range [-1, 1]
			double normY = (static_cast<double>(y) * yFactor) - 1.0;//  [-1, 1]

			// Generate the ray for this pixel
			m_Camera.GenerateRay(static_cast<float>(normX), static_cast<float>(normY), cameraRay); //note this returns a boolean! we might want to use that

			// Test for intersections with all objects in the scene
			for (auto currentObject : m_objectList)
			{
				bool validIntersection = currentObject->TestIntersection(cameraRay, intPoint, localNormal, localColor);
				// If we have a valid intersection, change pixel color to red
				if (validIntersection)
				{
					// Compute intensity of illumination
					double intensity;
					qbVector<double> color{ 3 };
					bool validIllum = false;
					for (auto currentLight : m_lightList)
					{
						validIllum = currentLight->ComputeIllumination(intPoint, localNormal, m_objectList, currentObject, color, intensity);
					}

					// Compute the distance between the camera and the point of intersection
					double dist = (intPoint - cameraRay.p1).norm();
					if (dist > maxDist)
						maxDist = dist;

					if (dist < minDist)
						minDist = dist;

					//outputImage.SetPixel(x, y, 255.0 - ((dist - 9.0) / 0.94605) * 255.0, .0, .0);
					if (validIllum)
					{
						//outputImage.SetPixel(x, y, 255.0 * intensity, 0.0, 0.0); // shades of red
						outputImage.SetPixel(x, y,	localColor.GetElement(0) * intensity,
													localColor.GetElement(1) * intensity,
													localColor.GetElement(2) * intensity);
					}
					else
					{
						// Leave pixel unchanged
						//outputImage.SetPixel(x, y, 0.0, 0.0, 0.0);  // black shadow
					}
				}
				else
				{
					// Leave pixel unchanged
					//outputImage.SetPixel(x, y, .0, .0, .0);
				}
			}
		}
	}

	std::cout << "Minimum distance: " << minDist << std::endl;
	std::cout << "Maximum distance: " << maxDist << std::endl;

	return true;
}