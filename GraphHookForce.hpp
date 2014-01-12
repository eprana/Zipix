#ifndef _IMAC3_GRAPHHOOKFORCE_HPP
#define _IMAC3_GRAPHHOOKFORCE_HPP

#include "Force.hpp"

namespace imac3 {

typedef std::vector<std::pair<unsigned int, unsigned int>> ParticleGraph;

class GraphHookForce : public Force {

public:
	// Builder
	GraphHookForce(float fK, float fL) : m_fK(fK), m_fL(fL), m_pGraph(NULL) {};

	/* Getters & Setters */
	inline void setGraph(const ParticleGraph* pGraph) {m_pGraph = pGraph;}

	// ApplyForce
	void apply(ParticleManager& pm);

private:
	float m_fK;
	float m_fL;	
	const ParticleGraph* m_pGraph;
};
}

#endif // _IMAC3_GRAPHHOOKFORCE_HPP