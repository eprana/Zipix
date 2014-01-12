#ifndef _IMAC3_LEAPFROGSOLVER_HPP
#define _IMAC3_LEAPFROGSOLVER_HPP

#include "ParticleManager.hpp"

namespace imac3 {

class LeapfrogSolver {

public:

	struct ParticleState {
		glm::vec2 position;
		glm::vec2 velocity;

	};
	void solve(ParticleManager& pm, float dt);

	ParticleState getNextState(uint32_t particleIdx, ParticleManager& pm, float dt) const;

};
}

#endif //_IMAC3_LEAPFROGSOLVER_HPP