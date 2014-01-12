#ifndef _IMAC3_CONSTANTFORCE_HPP
#define _IMAC3_CONSTANTFORCE_HPP

#include <glm/glm.hpp>

#include "Force.hpp"
#include "ParticleManager.hpp"

namespace imac3 {

class ConstantForce : public Force {

	public:
		// Builder
		ConstantForce(glm::vec2 force) : m_Force(force) {};

		// Destructor
		~ConstantForce() {};

		/* Getters & Setters */
		inline glm::vec2 getForce() { return m_Force; }

		void apply(ParticleManager& pm);


	private:
		glm::vec2 m_Force;


};

}

#endif // _IMAC3_CONSTANTFORCE_HPP