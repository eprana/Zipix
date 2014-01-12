#ifndef _IMAC3_POLYGONFORCE_HPP
#define _IMAC3_POLYGONFORCE_HPP

#include "Force.hpp"
#include "Polygon.hpp"
#include "LeapfrogSolver.hpp"

#include <glm/glm.hpp>


namespace imac3 {

class PolygonForce : public Force {

public:

	// Builder	
	PolygonForce(const Polygon& polygon, float elasticity, const LeapfrogSolver& solver) : m_Polygon(&polygon), m_fElasticity(elasticity), m_Solver(&solver), m_fDt(0.f) {};

	// Destructor
	~PolygonForce() {};
	
	/* Getters & Setters */
	inline void setDt(float Dt) { m_fDt = Dt;}

	void apply(ParticleManager& pm);

private:
	const Polygon* m_Polygon;
	float m_fElasticity;
	const LeapfrogSolver* m_Solver;
	float m_fDt;

};


}

#endif // _IMAC3_POLYGONFORCE_HPP