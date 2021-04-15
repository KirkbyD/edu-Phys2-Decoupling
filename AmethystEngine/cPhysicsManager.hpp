#pragma once
#include <vector>
#include "iMediatorInterface.hpp"
#include "MediatorGlobals.hpp"
#include <map>
#include "cError.hpp"
#include "cPhysics.hpp"
#include "cAABB.hpp"
#include "cModelObject.hpp"
#include "DebugRenderer/cDebugRenderer.hpp"

//Physics P2
#include <physics/interfaces/PhysicsInterfaces.h>
#include <Windows.h>
#include "jsonUtility.hpp"

struct sPhysicsLibs : public iSerializable {
	std::vector<std::string> DLLs;
	std::string FactoryFunction;

	// Inherited via iSerializable
	virtual bool serialize(rapidjson::PrettyWriter<rapidjson::StringBuffer>* writer) override {
		return true;
	}

	virtual bool deserialize(const rapidjson::Document* document) override {
		const auto& doc = *document;

		if (doc["DLLs"].IsArray()) {
			int numDLLs = doc["DLLs"].Size();
			if (numDLLs == 0) return false;
			DLLs.resize(numDLLs);
			for (int i = 0; i < numDLLs; i++) {
				if (doc["DLLs"][i]["Library"].IsString()) {
					DLLs[i] = doc["DLLs"][i]["Library"].GetString();
				}
				else {
					std::cout << "sPhysicsLibs: " << " \"Library\" " << i << " is not properly defined" << std::endl;
				}
			}
		}
		if (doc["MakeFactory"].IsString())
			FactoryFunction = doc["MakeFactory"].GetString();
		else {
			std::cout << "sPhysicsLibs: " << " \"MakeFactory\" " << " is not properly defined" << std::endl;
			return false;
		}

		return true;
	}
};

struct BoundingBox {
	glm::vec3 minCorner;
	glm::vec3 maxCorner;
	glm::quat orientation;
};

class cPhysicsManager : iMediatorInterface {
	static cPhysicsManager stonPhysMngr;		// Singleton Enforcement Pointer
	iMediatorInterface* pMediator;				// Mediator Pointer
	cError error;								// Error class for reporting errors via mediator

	HMODULE hModule;							// Windows variable for loading variable .dll files
	int PhysicsDLLIndex;						// Keep track of what .dll file is currently in use.
	sPhysicsLibs PhysicsDLLs;					// Struct that records .dll names and the MakeFactoryFunction string
	
	typedef nPhysics::iPhysicsFactory* (*func_createPhysicsFactory)();
	nPhysics::iPhysicsFactory* PhysicsFactory;
	nPhysics::iPhysicsWorld* PhysicsWorld;


	cPhysicsManager();							// Private constructor only called by singleton pattern
	
public:
	static cPhysicsManager* GetPhysicsManager();
	sPhysicsLibs* GetDLLStruct() { return &PhysicsDLLs; }

	// Create necessary structures from the current library
	void InitializePhysicsSystem();
	// Destroy all library relevant components before the information is lost to the porogram
	void TerminatePhysicsSystem();

	// World Functions
	void WorldUpdate(float dt);
	void WorldAddRigidBody(nPhysics::iPhysicsComponent* component);
	void WorldRemoveRigidBody(nPhysics::iPhysicsComponent* component);
	// World Functions
	
	// Factory Functions
	// PhysicsWorld->CreateWorld() occurs in InitializePhysicsSystem();
	nPhysics::iBallComponent* FactoryCreateBall(const nPhysics::sBallDef& def);
	nPhysics::iBallComponent* FactoryCreateBall(const glm::vec3 position, const glm::vec3 velocity, const float radius, const float mass);
	nPhysics::iPlaneComponent* FactoryCreatePlane(const nPhysics::sPlaneDef& def);
	nPhysics::iPlaneComponent* FactoryCreatePlane(const glm::vec3 point, const glm::vec3 normal);
	// Factory Functions


	// Mediator Functions
	void setMediatorPointer(iMediatorInterface* pMediator);
	virtual sData RecieveMessage(sData& data);

	// PHYSICS CALCULATION FUNCTIONS
	void CalcDistance(glm::vec3 targetPos, glm::vec3 sourcePos, float& output);

#pragma region Semester_1
//	cPhysics* pPhysics;
//	std::map<unsigned long long /*ID*/, cAABB*> g_mapAABBs_World;
//	std::vector<glm::vec3> vec_BoundingBox_Test_Points;
//	std::vector<glm::vec3> vec_BoundingBox_ClosestPoints;
//	//debug aabb cubes with rendered cubes
//	cGameObjectFactory ObjectFactory;
//
//	unsigned long long prevAABB;
//	std::string currAABB;
//
//public:
//	
//
//	void IntegrationStep(std::vector<cModelObject*>& vec_pGameObjects, float deltaTime);
//	void TestForCollisions(std::vector<cModelObject*>& vec_pGameObjects);
//	void TestForComplexCollisions(std::vector<cComplexObject*> g_vec_pComplexObjects, cDebugRenderer* pDebugRenderer);
//
//	void CalcAABBsForMeshModel(cModelObject* theModel);
//	void AssignTriangle(std::vector<sPlyTriangle>::iterator itTri, glm::vec3 v1, glm::vec3 v2, glm::vec3 v3, float maxLength);
//	
//	void CalculateTransformedMesh(cMesh& originalMesh, glm::mat4 matWorld, cMesh& mesh_transformedInWorld);
//	BoundingBox CalculateBoundingBox(cModelObject* theModel);
//
//	void DrawAABBs();
//	int TrianglesInCurrentAABB(glm::vec3 pos);
//	std::string CurrentAABB(glm::vec3 pos);
//	cAABB* GetCurrentAABB();
//	cAABB* GetPointAABB(glm::vec3 pos);
//	unsigned long long GetPreviousAABB();
//
//	void DrawTestPoints(cComplexObject* Parent);
//
#pragma endregion
};