#include <iostream>

#include "GraphHookForce.hpp"

namespace imac3 {

	void GraphHookForce::apply(ParticleManager& pm) {
		if(m_pGraph != NULL) {
			for(ParticleGraph::const_iterator it = m_pGraph->begin(); it != m_pGraph->end(); ++it) {
				int i = std::get<0>(*it);
				int j = std::get<1>(*it);
				
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
