#ifndef _IMAC3_BRAKEFORCE_HPP
#define _IMAC3_BRAKEFORCE_HPP

#include "Force.hpp"

namespace imac3 {

class BrakeForce : public Force {

public:
	// Builder
	BrakeForce(float fV, float fDt) : m_fV(fV), m_fDt(fDt) {};

	// Getters & Setters
	inline void setDt(float dt) { m_fDt = dt;}

	// Apply force
	void apply(ParticleManager &pm);


private:
	float m_fV;
	float m_fDt;

};
}

#endif // _IMAC3_BRAKEFORCE_HPP