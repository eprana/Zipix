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
		srand (time(NULL));
		for(int i = 0; i < count; ++i) {

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

		// Position
		float x = (rand() % 18 - 9) / 10.f;
		float y = (rand() % 18 - 9) / 10.f;

		// Color
		glm::vec3 color;

		// Mass
		float m = 1.f;

		switch(type) {
			case Type::P_FOOD:
				color = foodColor;
				break;

			case Type::P_BONUS:
				color = bonusColor;
				m = 1.5f;
				break;

			default: 
				color = glm::vec3((rand() % 10) / 10.f, (rand() % 10) / 10.f, (rand() % 10) / 10.f);
				break;
		}  

		

		return addParticle(m, glm::vec2(x, y), glm::vec2(0, 0), glm::vec2(0, 0), color, type);
	}

	// Add random particle in a specific position 
	void ParticleManager::addCircleParticlesAtPosition(int random, glm::vec2 position, glm::vec3 color, int count) {
		srand(time(NULL)*random);
		float delta = 2 * 3.14f / count;

		for(int i = 0; i < count; ++i) {
			
			float c = cos(i * delta);
			float s = sin(i * delta);

			addParticle(1.f, position, glm::vec2(0, 0), glm::vec2(c/5, s/5), color);
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
		massArray.clear();
		positionArray.clear();
		velocityArray.clear();
		forceArray.clear();
		colorArray.clear(); 
	}
}