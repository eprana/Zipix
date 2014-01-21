#include <iostream>
#include "ParticleManager.hpp"

#include <glm/gtc/random.hpp>

namespace imac3 {

// Add a particle to the ParticleManager
int ParticleManager::addParticle(float mass, glm::vec2 position, glm::vec2 velocity, glm::vec2 force , glm::vec3 color, Type type) {
	massArray.push_back(mass);
	positionArray.push_back(position);
	velocityArray.push_back(velocity);
	forceArray.push_back(force);
	colorArray.push_back(color);
  typeArray.push_back(type);

  return massArray.size() -1;
}

int ParticleManager::addParticleToHead(float mass, glm::vec2 position, glm::vec2 velocity, glm::vec2 force , glm::vec3 color) {

  massArray.insert(massArray.begin(), mass);
  positionArray.insert(positionArray.begin(), position);
  velocityArray.insert(velocityArray.begin(), velocity);
  forceArray.insert(forceArray.begin(), force);
  colorArray.insert(colorArray.begin(), color);

  return massArray.size() -1;
}

// Add random particles
void ParticleManager::addRandomParticles(unsigned int count) {	
	for(int i = 0; i < count; ++i) {

    /* initialize random seed: */
    srand (time(NULL));

    float x = (rand() % 18 - 9) / 10.f;
    float y = (rand() % 18 - 9) / 10.f;


    float r = (rand() % 10) / 10.f;
    float g = (rand() % 10) / 10.f;
    float b = (rand() % 10) / 10.f;

    float m = 1.f; //(rand() % 15) / 10.f + 0.5f;

		addParticle(m, glm::vec2(x, y), glm::vec2(0, 0), glm::vec2(0, 0), glm::vec3(r, g, b));
	}
}

// Add random particle
int ParticleManager::addRandomParticle(int random, Type type) {  
    srand (time(NULL)*random);

    float x = (rand() % 18 - 9) / 10.f;
    float y = (rand() % 18 - 9) / 10.f;


    float r = (rand() % 10) / 10.f;
    float g = (rand() % 10) / 10.f;
    float b = (rand() % 10) / 10.f;

    float m = 1.f;

    return addParticle(m, glm::vec2(x, y), glm::vec2(0, 0), glm::vec2(0, 0), glm::vec3(r, g, b), type);
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