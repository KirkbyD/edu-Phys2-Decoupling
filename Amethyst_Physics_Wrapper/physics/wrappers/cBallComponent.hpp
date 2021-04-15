#pragma once
#include <physics/interfaces/iBallComponent.h>
#include <Shapes/cSphere.hpp>
#include "cPhysicsWorld.hpp"

namespace nPhysics {
	class cBallComponent : public iBallComponent {
	private:
		//Friends so the world can access these componen ts freely, it needs them a lot.
		friend nPhysics::cPhysicsWorld;
		phys::cRigidBody* mRigidBody;
		phys::cSphere* mBall;

	public:
		// Only contructor, builds the library's RigidBody and shape.
		cBallComponent(const sBallDef& def);
		virtual ~cBallComponent() {}

		//Retrieve physics transformation for rendering purposes.
		void GetTransform(glm::mat4& transformOut) override;

		// Inherited via iBallComponent
		virtual void ApplyImpulse(glm::vec3 impulseIn) override;
	};
}