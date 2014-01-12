#include <iostream>

#include "HookForce.hpp"

namespace imac3 {

	void HookForce::apply(ParticleManager& pm) {
		for(int i = 0; i < pm.getCount(); ++i) {
			for(int j = i + 1; j < pm.getCount(); ++j) {

				glm::vec2 A = pm.getParticlePosition(i);
				glm::vec2 B = pm.getParticlePosition(j);

				float d = glm::length( B - A);
				float e = 0.001;

				glm::vec2 F = m_fK * (1 - (m_fL/std::max(d, e))) * (B - A);

				pm.addForceToParticle(i, F);
				pm.addForceToParticle(j, -F);
			}
		}
		
	}

}
