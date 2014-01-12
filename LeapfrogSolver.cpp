#include "LeapfrogSolver.hpp"
#include <iostream>

namespace imac3 {

void LeapfrogSolver::solve(ParticleManager& pm, float dt) {
	for(int i = 0; i < pm.getCount(); ++i ) {

		pm.getParticleVelocity(i) += dt*pm.getParticleForce(i)/pm.getParticleMass(i);
		pm.getParticlePosition(i) += dt*pm.getParticleVelocity(i);
		
		pm.getParticleForce(i) = glm::vec2(0,0);
	}

}

LeapfrogSolver::ParticleState LeapfrogSolver::getNextState(uint32_t particleIdx, ParticleManager& pm, float dt) const {
	ParticleState nextState;
	
	if( dt != 0) {
		nextState.velocity = pm.getParticleVelocity(particleIdx) + dt*pm.getParticleForce(particleIdx)/pm.getParticleMass(particleIdx);
		nextState.position = pm.getParticlePosition(particleIdx) + dt*pm.getParticleVelocity(particleIdx);
	
	}

	
	return nextState;
}

}
