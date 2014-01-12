#include <iostream>
#include "ParticleManager.hpp"

#include <glm/gtc/random.hpp>

namespace imac3 {

// Add a particle to the ParticleManager
int ParticleManager::addParticle(float mass, glm::vec2 position, glm::vec2 velocity, glm::vec2 force , glm::vec3 color) {
	massArray.push_back(mass);
	positionArray.push_back(position);
	velocityArray.push_back(velocity);
	forceArray.push_back(force);
	colorArray.push_back(color);

	return massArray.size() -1;
}

// Add random particles
void ParticleManager::addRandomParticles(unsigned int count) {	
    	for(int i = 0; i < count; ++i) {
    		float x = glm::linearRand(-1.f, 1.f);
    		float y = glm::linearRand(-1.f, 1.f);

    		float r = glm::linearRand(0.f, 1.f);
    		float g = glm::linearRand(0.f, 1.f);
    		float b = glm::linearRand(0.f, 1.f);

    		float m = glm::linearRand(0.5f, 2.f);

    		addParticle(m, glm::vec2(x, y), glm::vec2(0, 0), glm::vec2(0, 0), glm::vec3(r, g, b));
    	}
}

// Add force to a particle
void ParticleManager::addForceToParticle(int id, glm::vec2 force) {
  getParticleForce(id) += force;
}

// Draw particles
void ParticleManager::drawParticles(ParticleRenderer2D& renderer) {
	renderer.drawParticles(positionArray.size(),
                               &positionArray[0],
                               &massArray[0],
                               &colorArray[0]);
}

// Draw particleGraph
void ParticleManager::drawParticleGraph(ParticleGraph& graph, ParticleRenderer2D& renderer) {
  renderer.drawLines(graph.size(), 
                      &graph[0],
                      getCount(),
                      &positionArray[0],
                      &colorArray[0]);
}

// Clear Manager
void ParticleManager::clear() {
  massArray = {};
  positionArray = {};
  velocityArray = {};
  forceArray = {};
  colorArray = {};  
}


}