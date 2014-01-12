#ifndef _IMAC3_HOOKFORCE_HPP
#define _IMAC3_HOOKFORCE_HPP

#include "Force.hpp"

namespace imac3 {

class HookForce : public Force {

public:
	// Builder
	HookForce(float fK, float fL) : m_fK(fK), m_fL(fL) {};

	// ApplyForce
	void apply(ParticleManager& pm);

private:
	float m_fK;
	float m_fL;	

};
}

#endif // _IMAC3_HOOKFORCE_HPP