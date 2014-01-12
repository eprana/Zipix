#include "GraphBrakeForce.hpp"

namespace imac3 {

	void GraphBrakeForce::apply(ParticleManager &pm) {
		if(m_pGraph != NULL) {
			for(ParticleGraph::const_iterator it = m_pGraph->begin(); it != m_pGraph->end(); ++it) {
				int i = std::get<0>(*it);
				int j = std::get<1>(*it);
				glm::vec2 F = m_fV * (pm.getParticleVelocity(j) - pm.getParticleVelocity(i))/m_fDt;

				pm.addForceToParticle(i, F);
				pm.addForceToParticle(j, -F);
			}
		}
		

	}

}