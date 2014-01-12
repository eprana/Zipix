#include "BrakeForce.hpp"

namespace imac3 {

	void BrakeForce::apply(ParticleManager &pm) {
		for(int i = 0; i < pm.getCount(); ++i) {
			for(int j = i + 1; j < pm.getCount(); ++j) {

				glm::vec2 F = m_fV * (pm.getParticleVelocity(j) - pm.getParticleVelocity(i))/m_fDt;

				pm.addForceToParticle(i, F);
				pm.addForceToParticle(j, -F);
			}
		}

	}

}