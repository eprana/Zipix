#ifndef _IMAC3_GRAPHBRAKEFORCE_HPP
#define _IMAC3_GRAPHBRAKEFORCE_HPP

#include "Force.hpp"

namespace imac3 {

typedef std::vector<std::pair<unsigned int, unsigned int>> ParticleGraph;

class GraphBrakeForce : public Force {

public:
	// Builder
	GraphBrakeForce(float fV, float fDt) : m_fV(fV), m_fDt(fDt), m_pGraph(NULL) {};

	// Getters & Setters
	inline void setDt(float dt) { m_fDt = dt;}

	inline void setGraph(const ParticleGraph* pGraph) {m_pGraph = pGraph;}

	// Apply force
	void apply(ParticleManager &pm);


private:
	float m_fV;
	float m_fDt;
	const ParticleGraph* m_pGraph;

};
}

#endif // _IMAC3_GRAPHBRAKEFORCE_HPP