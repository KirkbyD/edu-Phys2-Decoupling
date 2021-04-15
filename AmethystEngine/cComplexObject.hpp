#pragma once
#include "cModelObject.hpp"
#include "cLightObject.hpp"
#include "iGameObject.hpp"
#include <vector>
#include <string>
#include <glm/gtx/quaternion.hpp>

#include <physics/interfaces/iPhysicsComponent.h>
#include <physics/interfaces/iBallComponent.h>
#include <physics/interfaces/iPlaneComponent.h>

struct collisionPoint {
	glm::vec3 RelativePos;
	glm::vec3 Position;
	float Collided;
};

class cComplexObject : public iGameObject {
private:
	// Graphics
	float scale;
	// TRANSFORM MATRIX

	// Physics
	std::vector<nPhysics::iPhysicsComponent*> physicsComponents;	

	// Members
	std::vector<glm::vec3> modelPositions;
	std::vector<cModelObject*> modelVector;
	std::vector<glm::vec3> lightPositions;
	std::vector<cLightObject*> lightVector;

	void RotateChildren();

public:
	cComplexObject(std::vector<cComplexObject*> &vObjects);
	~cComplexObject();

	float GetScale();
	//glm::vec3 GetPosition();
	//glm::vec3 GetPreviousPosition();
	//glm::quat getQOrientation();
	//glm::vec3 getEulerAngle();
	//float GetRoll();
	//glm::vec3 GetVelocity();
	std::vector<cModelObject*> GetModels();
	std::vector<cLightObject*> GetLights();
	std::vector<nPhysics::iPhysicsComponent*> GetPhysicsComponents();
	//std::vector<collisionPoint*> GetCollisionPoints();

	void SetScale(float val);
	//void SetPosition(glm::vec3 posVec);
	//void SetPreviousPosition(glm::vec3 posVec);
	//void SetVelocity(glm::vec3 velocity);
	//void SetOrientation(glm::vec3 EulerAngleDegreesXYZ);
	//void SetOrientation(glm::quat qAngle);

	void AddLight(std::string);
	void AddLight(cLightObject* theLight);
	void AddModel(std::string);	
	void AddModel(cModelObject* theModel);
	void AddPhysicsComponent(nPhysics::iPhysicsComponent* component);
	//void AddCollisionPoint(glm::vec3 thePoint);
	//void AddPhysics(std::string);
	//void AddAudio(std::string);
	bool RemoveLight(cLightObject* theLight);

	//Apply an impulse to all member components
	void ApplyImpulse(glm::vec3 impulse);
	//Clear all existingg physics components from entity (used in physics terminate)
	void ClearPhysicsComponents();

	//void Move(glm::vec3 movement);
	//void Resize();
	//void UpdateOrientation(glm::vec3 EulerAngleDegreesXYZ);
	//void UpdateOrientation(glm::quat qAngle);
	//void UpdateRoll(float offset);

	/*Interface*/
	virtual unsigned int getUniqueID(void);
	virtual glm::vec3 getPosition(void);
};