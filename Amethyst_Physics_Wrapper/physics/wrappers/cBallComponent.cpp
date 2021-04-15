#include "cBallComponent.hpp"

nPhysics::cBallComponent::cBallComponent(const sBallDef& def)
	: iBallComponent()
	, mBall(new phys::cSphere(def.Radius)) {
	phys::sRigidBodyDef rbDef(def.Mass, def.Position, def.Velocity);
	mRigidBody = new phys::cRigidBody(rbDef, mBall);
}

void nPhysics::cBallComponent::GetTransform(glm::mat4& transformOut) {
	mRigidBody->GetTransform(transformOut);
}

void nPhysics::cBallComponent::ApplyImpulse(glm::vec3 impulseIn) {
	mRigidBody->ApplyImpulse(impulseIn);
}
