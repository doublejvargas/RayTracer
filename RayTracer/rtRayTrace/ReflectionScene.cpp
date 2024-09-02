#include "ReflectionScene.hpp"
#include "rtMaterials/simplematerial.hpp"

rt::ReflectionScene::ReflectionScene()
{
	SetupSceneObjects();
	
}

rt::ReflectionScene::~ReflectionScene()
{

}

void rt::ReflectionScene::SetupSceneObjects()
{
	// ****************************************************************************
	// Configure the camera
	// ****************************************************************************
	camera_.SetPosition(glm::dvec3{0.0, -10.0, -2.0});
	camera_.SetLookAt(glm::dvec3{0.0, 0.0, 0.0});
	camera_.SetUpVector(glm::dvec3{0.0, 0.0, 1.0});
	camera_.SetProjScreenWidth(0.25);
	camera_.SetAspect(16.0 / 9.0);
	camera_.UpdateCameraGeometry();

	// ****************************************************************************
	// Set up materials
	// ****************************************************************************
	auto material = std::make_shared<rt::SimpleMaterial>(rt::SimpleMaterial());
	auto material2 = std::make_shared<rt::SimpleMaterial>(rt::SimpleMaterial());
	auto material3 = std::make_shared<rt::SimpleMaterial>(rt::SimpleMaterial());
	auto floorMaterial = std::make_shared<rt::SimpleMaterial>(rt::SimpleMaterial());

	material->baseColor_ = glm::dvec3{ 0.25, 0.5, 0.8 };
	material->reflectivity_ = 0.1;
	material->shininess_ = 10.0;

	material2->baseColor_ = glm::dvec3{ 1.0, 0.5, 0.0 };
	material2->reflectivity_ = 0.75;
	material2->shininess_ = 10.0;

	material3->baseColor_ = glm::dvec3{ 1.0, 0.8, 0.0 };
	material3->reflectivity_ = 0.25;
	material3->shininess_ = 10.0;

	floorMaterial->baseColor_ = glm::dvec3{ 1.0, 1.0, 1.0 };
	floorMaterial->reflectivity_ = 0.5;
	floorMaterial->shininess_ = 0.0;


	// ****************************************************************************
	// Make Objects
	// ****************************************************************************

	// make a sphere
	objectList_.push_back(std::make_shared<rt::ObjSphere>(rt::ObjSphere{}));
	objectList_.push_back(std::make_shared<rt::ObjSphere>(rt::ObjSphere{}));
	objectList_.push_back(std::make_shared<rt::ObjSphere>(rt::ObjSphere{}));

	// make a plane
	objectList_.push_back(std::make_shared<rt::ObjPlane>(rt::ObjPlane{}));
	objectList_.at(3)->baseColor_ = glm::dvec3{ 0.5, 0.5, 0.5 };

	rt::GTform planeTransform;
	planeTransform.SetTransform(glm::dvec3{0.0, 0.0, 0.75},
		glm::dvec3{0.0, 0.0, 0.0},
		glm::dvec3{4.0, 4.0, 1.0});
	objectList_.at(3)->SetTransformMatrix(planeTransform);

	// Modify the spheres
	// Set transformations
	rt::GTform matrix1, matrix2, matrix3;
	matrix1.SetTransform(glm::dvec3{-1.5, 0.0, 0.0}, // translation
		glm::dvec3{0.0, 0.0, 0.0},  // rotation
		glm::dvec3{0.5, 0.5, 0.5}); // scaling

	matrix2.SetTransform(glm::dvec3{0.0, 0.0, 0.0},
		glm::dvec3{0.0, 0.0, 0.0},
		glm::dvec3{0.5, 0.5, 0.5});

	matrix3.SetTransform(glm::dvec3{1.5, 0.0, 0.0},
		glm::dvec3{0.0, 0.0, 0.0},
		glm::dvec3{0.5, 0.5, 0.5});

	objectList_.at(0)->SetTransformMatrix(matrix1);
	objectList_.at(1)->SetTransformMatrix(matrix2);
	objectList_.at(2)->SetTransformMatrix(matrix3);

	// Set spheres base color
	objectList_.at(0)->baseColor_ = glm::dvec3{ 0.25, 0.5, 0.8 };
	objectList_.at(1)->baseColor_ = glm::dvec3{ 1.0, 0.5, 0.0 };
	objectList_.at(2)->baseColor_ = glm::dvec3{ 1.0, 0.8, 0.0 };

	// Assign materials to objects
	objectList_.at(0)->AssignMaterial(material3);
	objectList_.at(1)->AssignMaterial(material);
	objectList_.at(2)->AssignMaterial(material2);
	objectList_.at(3)->AssignMaterial(floorMaterial);

	// Make point lights
	lightList_.push_back(std::make_shared<rt::PointLight>(rt::PointLight()));
	lightList_.at(0)->m_Location = glm::dvec3{ 5.0, -10.0, -5.0 }; // -z value because currently the z axis is inverted in our application.
	lightList_.at(0)->m_Color = glm::dvec3{ 0.0, 0.0, 1.0 };
 
	lightList_.push_back(std::make_shared<rt::PointLight>(rt::PointLight()));
	lightList_.at(1)->m_Location = glm::dvec3{ -5.0, -10.0, -5.0 };
	lightList_.at(1)->m_Color = glm::dvec3{ 1.0, 0.0, 0.0 };
 
	lightList_.push_back(std::make_shared<rt::PointLight>(rt::PointLight()));
	lightList_.at(2)->m_Location = glm::dvec3{ 0.0, -10.0, -5.0 };
	lightList_.at(2)->m_Color = glm::dvec3{ 0.0, 1.0, 0.0 };
}