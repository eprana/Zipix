#include "Game.hpp"

namespace imac3 {	

	// Create a particleGraph
	ParticleGraph createString(glm::vec2 A, glm::vec2 B, glm::vec3 color, uint32_t discFactor, ParticleManager& particleManager) {
	    glm::vec2 AB = B - A;
	    glm::vec2 step = glm::vec2(AB[0]/discFactor, AB[1]/discFactor);
	    glm::vec2 position = A;
	    float mass = 1.f;

	    ParticleGraph graph;

	    unsigned int id = 0;

	    for(int i = 0; i < discFactor + 1; ++i) {
	        id = particleManager.addParticle(mass, position, glm::vec2(0.f, 0.f), glm::vec2(0.f, 0.f), color);
	        if(id != 0) {
	            std::pair<unsigned int, unsigned int> pair (id, id - 1);
	            graph.push_back(pair);    
	        }
	        else if(id == discFactor) {
	            std::pair<unsigned int, unsigned int> pair (id, 0);  
	            graph.push_back(pair);   

	        }
	  
	        position += step;
	    }
	    particleManager.getParticleColor(0) = glm::vec3(0.f, 0.f, 1.f);
	    return graph;
	}

	// Add a particle to the Snake
	void addParticletoSnake(ParticleGraph& graph, int id, ParticleManager& foodManager, ParticleManager& snakeManager) {
	    id = snakeManager.addParticleToHead(foodManager.getParticleMass(id), 
	                                    foodManager.getParticlePosition(id),
	                                    foodManager.getParticleVelocity(id),
	                                    foodManager.getParticleForce(id),
	                                    foodManager.getParticleColor(id));

	    foodManager.clear();
	    std::pair<unsigned int, unsigned int> pair (id, snakeManager.getCount() - 2);
	    graph.push_back(pair);

	}


	// Check if the snake is outside
	bool isOutside(ParticleManager& snakeManager) {
	    for(int  i = 0; i < snakeManager.getCount(); ++i) {
	        
	        //Right
	        if(snakeManager.getParticleX(i) >= 1.f) {
	            return true;
	        }

	        // Left
	        if(snakeManager.getParticleX(i) <= -1.f) {
	            return true;
	        }

	        // Up
	        if(snakeManager.getParticleY(i) >= 1.f) {
	            return true;
	        }

	        // Down
	        if(snakeManager.getParticleY(i) <= -1.f) {
	            return true;
	        }
	    }

	    return false;
	}

	// Check collision between snake and food
	int checkFoodCollision(ParticleManager& snakeManager, ParticleManager& foodManager, float step, int init) {
	    for(int i = init; i < foodManager.getCount(); ++i) {

	        if(foodManager.getParticleX(i) - step * foodManager.getParticleMass(i) <= snakeManager.getParticleX(0) 
	            && snakeManager.getParticleX(0) <= foodManager.getParticleX(i) + step * foodManager.getParticleMass(i)
	            && foodManager.getParticleY(i) - step * foodManager.getParticleMass(i) <= snakeManager.getParticleY(0) 
	            && snakeManager.getParticleY(0) <= foodManager.getParticleY(i) + step* foodManager.getParticleMass(i)) {

	            return i;
	        }
	    }

	    return -1;
	}

	// Check collision between snake and snake
	int checkSnakeCollision(ParticleManager& snakeManager, ParticleManager& foodManager, float step, int init) {
	    for(int i = init; i < foodManager.getCount() - 1; ++i) {

	        if(foodManager.getParticleX(i) - step * foodManager.getParticleMass(i) <= snakeManager.getParticleX(0) 
	            && snakeManager.getParticleX(0) <= foodManager.getParticleX(i) + step * foodManager.getParticleMass(i)
	            && foodManager.getParticleY(i) - step * foodManager.getParticleMass(i) <= snakeManager.getParticleY(0) 
	            && snakeManager.getParticleY(0) <= foodManager.getParticleY(i) + step* foodManager.getParticleMass(i)) {

	            return i;
	        }
	    }

	    return -1;
	}
}