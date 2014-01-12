#include "PolygonForce.hpp"
#include "LeapfrogSolver.hpp"
#include <iostream>

//#include "maths.hpp"

namespace imac3 {

	void PolygonForce::apply(ParticleManager& pm) {

		glm::vec2 intersection = glm::vec2(0,0);
		glm::vec2 normal = glm::vec2(0,0);	
		
		if(m_fDt != 0) {

			// Inner
			if(m_Polygon->isInner() == true) {
				// Pour chaque arrête du polygone
				for(int i = 0; i < m_Polygon->getCount(); ++i) {

					// Pour chaque particule 
					for(int j = 0; j < pm.getCount(); ++j) {

						LeapfrogSolver::ParticleState nextState = m_Solver->getNextState(j, pm, m_fDt);

						if(intersect(pm.getParticlePosition(j), nextState.position, m_Polygon->getPoint(i), m_Polygon->getPoint((i + 1) % m_Polygon->getCount()),  &intersection, &normal)) {

							glm::vec2 F = this->m_fElasticity * glm::dot(nextState.velocity, -normal) * (pm.getParticleMass(j)/m_fDt) * normal;
							
							pm.addForceToParticle(j, F);
						}
					}
				}
			}
			// Not inner
			else {
				// Pour chaque arrête du polygone
				for(int i = m_Polygon->getCount() - 1; i >= 0; --i) {
					// Pour chaque particule 
					for(int j = 0; j < pm.getCount(); ++j) {
						
						LeapfrogSolver::ParticleState nextState = m_Solver->getNextState(j, pm, m_fDt);
						if(intersect(pm.getParticlePosition(j), nextState.position, m_Polygon->getPoint((i + 1) % m_Polygon->getCount()), m_Polygon->getPoint(i), &intersection, &normal)) {
							
							// std::cout << " #### Intersection #### " << std::endl;

							// std::cout << "Position : " << nextState.position[0] << " - " << nextState.position[1] << std::endl;
							// std::cout << "Velocity : " << nextState.velocity[0] << " - " << nextState.velocity[1] << std::endl;
							//std::cout << "Normal : " << normal[0] << " - " << normal[1] << std::endl;

							glm::vec2 F = this->m_fElasticity * glm::dot(nextState.velocity, -normal) * (pm.getParticleMass(j)/m_fDt) * normal;
							//std::cout << "F : " << F[0] << " - " << F[1] << std::endl;
							
							pm.addForceToParticle(j, F);

						}
					}
				}
			}
		}	
	}
}