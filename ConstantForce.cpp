#include "ConstantForce.hpp"
#include <iostream>

namespace imac3 {

void ConstantForce::apply(ParticleManager& pm) {
	for(int i = 0; i < pm.getCount(); ++i) {
		pm.addForceToParticle(i, this->getForce());
	}
}


}