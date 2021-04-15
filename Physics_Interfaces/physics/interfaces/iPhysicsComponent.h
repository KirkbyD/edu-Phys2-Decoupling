#pragma once
#include <glm/mat4x4.hpp>
#include "eComponentType.h"

namespace nPhysics {
	class iPhysicsComponent	{
	public:
		virtual ~iPhysicsComponent() {}
		//Returns what soirt of shape this component is.
		inline const eComponentType& GetComponentType() { return mComponentType; }

		// Get the current physics transformation for rendering purposes.
		virtual void GetTransform(glm::mat4& transformOut) = 0;

	protected:
		iPhysicsComponent(eComponentType componentType)
			: mComponentType(componentType) {}

	private:
		// What type of shape this component is
		eComponentType mComponentType;

		// Delete constructors so that they cannot be used
		iPhysicsComponent() = delete;
		iPhysicsComponent(const iPhysicsComponent& other) = delete;
		iPhysicsComponent& operator=(const iPhysicsComponent& other) = delete;
	};
}