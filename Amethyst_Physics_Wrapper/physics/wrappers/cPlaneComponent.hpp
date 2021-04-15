#pragma once
#include <physics/interfaces/iPlaneComponent.h>
#include <Shapes/cPlane.hpp>
#include "cPhysicsWorld.hpp"

namespace nPhysics {
	class cPlaneComponent : public iPlaneComponent {
	private:
		friend nPhysics::cPhysicsWorld;
		phys::cRigidBody* mRigidBody;
		phys::cPlane* mPlane;

	public:
		// Only contructor, builds the library's RigidBody and shape.
		cPlaneComponent(const sPlaneDef& def);
		virtual ~cPlaneComponent() {}

		//Retrieve physics transformation for rendering purposes.
		void GetTransform(glm::mat4& transformOut) override;
	};
}