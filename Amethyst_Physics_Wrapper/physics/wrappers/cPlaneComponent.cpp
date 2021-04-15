#include "cPlaneComponent.hpp"
#include <glm/vec3.hpp>

nPhysics::cPlaneComponent::cPlaneComponent(const sPlaneDef& def)
	: iPlaneComponent()
	, mPlane(new phys::cPlane(def.Normal, def.Constant)) {
	mRigidBody = new phys::cRigidBody(phys::sRigidBodyDef(), mPlane);	//Uses default rbDef as plane has no elements of it.
}

void nPhysics::cPlaneComponent::GetTransform(glm::mat4& transformOut) {
	mRigidBody->GetTransform(transformOut);
}