#include "ConstantForce.hpp"
#include <iostream>

namespace imac3 {

void ConstantForce::apply(ParticleManager& pm) {
	for(int i = 0; i < pm.getCount(); ++i) {
		if(pm.getParticleType(i) == ParticleManager::Type::P_BONUS) {
			pm.addForceToParticle(i, this->getForce());
		}
	}
}


}