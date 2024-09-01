#include "Scene.hpp"
#include "rtMaterials/simplematerial.hpp"

rt::Scene::Scene()
{
	// Create some materials
	auto material = std::make_shared<rt::SimpleMaterial>(rt::SimpleMaterial());
	auto material2 = std::make_shared<rt::SimpleMaterial>(rt::SimpleMaterial());
	auto material3 = std::make_shared<rt::SimpleMaterial>(rt::SimpleMaterial());
	auto floorMaterial = std::make_shared<rt::SimpleMaterial>(rt::SimpleMaterial());

	// Set up the materials
	material->baseColor_ = qbVector<double>{ std::vector<double>{0.25, 0.5, 0.8} };
	material->reflectivity_ = 0.1;
	material->shininess_ = 10.0;

	material2->baseColor_ = qbVector<double>{ std::vector<double>{1.0, 0.5, 0.0} };
	material2->reflectivity_ = 0.75;
	material2->shininess_ = 10.0;

	material3->baseColor_ = qbVector<double>{ std::vector<double>{1.0, 0.8, 0.0} };
	material3->reflectivity_ = 0.25;
	material3->shininess_ = 10.0;

	floorMaterial->baseColor_ = qbVector<double>{ std::vector<double>{1.0, 1.0, 1.0} };
	floorMaterial->reflectivity_ = 0.5;
	floorMaterial->shininess_ = 0.0;

	// Configure the camera 
	m_Camera.SetPosition(qbVector<double>{std::vector<double>{0.0, -10.0, -1.0}});
	m_Camera.SetLookAt	(qbVector<double>{std::vector<double>{0.0, 0.0, 0.0}});
	m_Camera.SetUpVector(qbVector<double>{std::vector<double>{0.0, 0.0, 1.0}});
	m_Camera.SetProjScreenWidth(0.25);
	m_Camera.SetAspect(16.0 / 9.0);
	m_Camera.UpdateCameraGeometry();
	
	// make a sphere
	m_objectList.push_back(std::make_shared<rt::ObjSphere>(rt::ObjSphere{}));
	m_objectList.push_back(std::make_shared<rt::ObjSphere>(rt::ObjSphere{}));
	m_objectList.push_back(std::make_shared<rt::ObjSphere>(rt::ObjSphere{}));

	// make a plane
	m_objectList.push_back(std::make_shared<rt::ObjPlane>(rt::ObjPlane{}));
	m_objectList.at(3)->baseColor_ = qbVector<double>{ std::vector<double>{0.5, 0.5, 0.5} };

	rt::GTform planeTransform;
	planeTransform.SetTransform(	qbVector<double>{std::vector<double>{0.0, 0.0, 0.75}},
									qbVector<double>{std::vector<double>{0.0, 0.0, 0.0}},
									qbVector<double>{std::vector<double>{4.0, 4.0, 1.0}});
	m_objectList.at(3)->SetTransformMatrix(planeTransform);

	// Modify the spheres
	// Set transformations
	rt::GTform matrix1, matrix2, matrix3;
	matrix1.SetTransform(	qbVector<double>{std::vector<double>{-1.5, 0.0, 0.0}}, // translation
							qbVector<double>{std::vector<double>{0.0, 0.0, 0.0}},  // rotation
							qbVector<double>{std::vector<double>{0.5, 0.5, 0.5}}); // scaling

	matrix2.SetTransform(	qbVector<double>{std::vector<double>{0.0, 0.0, 0.0}},
							qbVector<double>{std::vector<double>{0.0, 0.0, 0.0}},
							qbVector<double>{std::vector<double>{0.5, 0.5, 0.5}});

	matrix3.SetTransform(	qbVector<double>{std::vector<double>{1.5, 0.0, 0.0}},
							qbVector<double>{std::vector<double>{0.0, 0.0, 0.0}},
							qbVector<double>{std::vector<double>{0.5, 0.5, 0.5}});

 	m_objectList.at(0)->SetTransformMatrix(matrix1);
 	m_objectList.at(1)->SetTransformMatrix(matrix2);
 	m_objectList.at(2)->SetTransformMatrix(matrix3);
	
	// Set spheres base color 
	m_objectList.at(0)->baseColor_ = qbVector<double>{ std::vector<double>{0.25, 0.5, 0.8} };
	m_objectList.at(1)->baseColor_ = qbVector<double>{ std::vector<double>{1.0, 0.5, 0.0} };
	m_objectList.at(2)->baseColor_ = qbVector<double>{ std::vector<double>{1.0, 0.8, 0.0} };

	// Assign materials to objects
	m_objectList.at(0)->AssignMaterial(material3);
	m_objectList.at(1)->AssignMaterial(material);
	m_objectList.at(2)->AssignMaterial(material2);
	m_objectList.at(3)->AssignMaterial(floorMaterial);

	// Make point lights
	m_lightList.push_back(std::make_shared<rt::PointLight>(rt::PointLight()));
	m_lightList.at(0)->m_Location = qbVector<double>{ std::vector<double>{5.0, -10.0, -5.0} }; // -z value because currently the z axis is inverted in our application.
	m_lightList.at(0)->m_Color = qbVector<double>{ std::vector<double>{0.0, 0.0, 1.0} };

	m_lightList.push_back(std::make_shared<rt::PointLight>(rt::PointLight()));
	m_lightList.at(1)->m_Location = qbVector<double>{ std::vector<double>{-5.0, -10.0, -5.0} };
	m_lightList.at(1)->m_Color = qbVector<double>{ std::vector<double>{1.0, 0.0, 0.0} };
	
	m_lightList.push_back(std::make_shared<rt::PointLight>(rt::PointLight()));
	m_lightList.at(2)->m_Location = qbVector<double>{ std::vector<double>{0.0, -10.0, -5.0} };
	m_lightList.at(2)->m_Color = qbVector<double>{ std::vector<double>{0.0, 1.0, 0.0} };
}

bool rt::Scene::Render(rtImage &outputImage)
{
	// Get the dimensions of the output image
	int width = outputImage.GetWidth();
	int height = outputImage.GetHeight();

	// Loop over each pixel in our image
	rt::Ray cameraRay;
	qbVector<double> intPoint	{3};
	qbVector<double> localNormal{3};
	qbVector<double> localColor	{3};
	// used to normalize the pixels from our typical dimensions (eg 1280 x 720) to range between [-1, 1].
	double xFactor = 1.0 / (static_cast<double>(width) / 2.0);
	double yFactor = 1.0 / (static_cast<double>(height) / 2.0);
	double minDist = 1e6;
	double maxDist = 0.0;
	for (int y = 0; y < height; y++)
	{
		// Display progress
		std::cout << "Processing line " << y << " of " << height << "." << std::endl;
		for (int x = 0; x < width; x++)
		{
			// Normalize x and y coordinates
			double normX = (static_cast<double>(x) * xFactor) - 1.0; // x * xFactor is in range [0, 2], so subtracting one yields range [-1, 1]
			double normY = (static_cast<double>(y) * yFactor) - 1.0;//  [-1, 1]

			// Generate the ray for this pixel
			m_Camera.GenerateRay(static_cast<float>(normX), static_cast<float>(normY), cameraRay); //note this returns a boolean! we might want to use that

			// Test for intersections with all objects in the scene
			std::shared_ptr<rt::ObjectBase> closestObject;
			qbVector<double> closestIntPoint	{ 3 };
			qbVector<double> closestLocalNormal	{ 3 };
			qbVector<double> closestLocalColor	{ 3 };
			bool intersectionFound = CastRay(cameraRay, closestObject, closestIntPoint, closestLocalNormal, closestLocalColor);
			
			/* Compute the illumination for the closest object, assuming that there
			   was a valid intersection */
			if (intersectionFound)
			{
				// Check if object has a material
				if (closestObject->hasMaterial_)
				{
					// Use material to compute color
					rt::MaterialBase::reflectionRayCount_ = 0;
					qbVector<double> color = closestObject->pMaterial_->ComputeColor(m_objectList, m_lightList, closestObject, closestIntPoint, closestLocalNormal, cameraRay);

					outputImage.SetPixel(x, y, color.GetElement(0), color.GetElement(1), color.GetElement(2));
				}
				else
				{
					// Use basic method to compute color
					qbVector<double> matColor = rt::MaterialBase::ComputeDiffuseColor(	m_objectList, m_lightList,
																						closestObject, closestIntPoint, 
																						closestLocalNormal, closestObject->baseColor_
																					 );

					outputImage.SetPixel(x, y, matColor.GetElement(0), matColor.GetElement(1), matColor.GetElement(2));
				}
			}
		}
	}

	return true;
}

bool rt::Scene::CastRay(	rt::Ray &castRay, 
							std::shared_ptr<rt::ObjectBase> &closestObject,
							qbVector<double> &closestIntPoint,
							qbVector<double> &closestLocalNormal, 
							qbVector<double> &closestLocalColor
						)
{
	qbVector<double> intPoint	{ 3 };
	qbVector<double> localNormal{ 3 };
	qbVector<double> localColor	{ 3 };
	double minDist = 1e6;
	bool intersectionFound = false;
	for (auto currentObject : m_objectList)
	{
		bool validInt = currentObject->TestIntersection(castRay, intPoint, localNormal, localColor);

		//If we have a valid intersection
		if (validInt)
		{
			// Set the flag to indicate that we found an intersection
			intersectionFound = true;

			// Compute distance between the camera and the point of intersection
			double dist = (intPoint - castRay.p1).norm();

			/* If this object is closer to the camera than any one that we have
			   seen before, then store a reference to it */
			if (dist < minDist)
			{
				minDist = dist;
				closestObject = currentObject;
				closestIntPoint = intPoint;
				closestLocalNormal = localNormal;
				closestLocalColor = localColor;
			}
		}
	}

	return intersectionFound;
}
