#include "Game.hpp"
#include <iostream>
namespace imac3 {	

	// Create a particleGraph
	ParticleGraph createString(glm::vec2 A, glm::vec2 B, glm::vec3 color, glm::vec3 headColor, uint32_t discFactor, ParticleManager& particleManager) {
	    glm::vec2 AB = B - A;
	    glm::vec2 step = glm::vec2(AB[0]/discFactor, AB[1]/discFactor);
	    glm::vec2 position = A;
	    float mass = 1.f;

	    ParticleGraph graph;

	    unsigned int id = 0;

	    for(int i = 0; i < discFactor + 1; ++i) {
	        id = particleManager.addParticle(mass, position, glm::vec2(0.f, 0.f), glm::vec2(0.f, 0.f), color, ParticleManager::Type::P_SNAKE);
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
	    particleManager.getParticleColor(0) = headColor;
	    return graph;
	}

	// Copy a particle
	int copyParticle(ParticleManager& sourceManager, ParticleManager& destinationManager, int id) {
		int newId = destinationManager.addParticle(sourceManager.getParticleMass(id),
                            sourceManager.getParticlePosition(id),           
                            sourceManager.getParticleVelocity(id),
                            sourceManager.getParticleForce(id),
                            sourceManager.getParticleColor(id));

		return newId;
	}

	void updateParticle(ParticleManager& sourceManager, int sourceId, ParticleManager& destinationManager, int destinationId) {
		destinationManager.getParticleMass(destinationId) = sourceManager.getParticleMass(sourceId);
		destinationManager.getParticlePosition(destinationId) = sourceManager.getParticlePosition(sourceId);
		destinationManager.getParticleVelocity(destinationId) = sourceManager.getParticleVelocity(sourceId);
		destinationManager.getParticleForce(destinationId) = sourceManager.getParticleForce(sourceId);
		destinationManager.getParticleColor(destinationId) = sourceManager.getParticleColor(sourceId);
	}

	// Add a particle to the Snake
	void addParticletoSnake(ParticleGraph& graph, int id, ParticleManager& foodManager, ParticleManager& snakeManager) {

	    id = snakeManager.addParticleToHead(foodManager.getParticleMass(id), 
	                                    foodManager.getParticlePosition(id),
	                                    foodManager.getParticleVelocity(id),
	                                    foodManager.getParticleForce(id),
	                                    snakeManager.getHeadColor());

	    foodManager.clear();

	    std::pair<unsigned int, unsigned int> pair (id, snakeManager.getCount() - 2);
	    graph.push_back(pair);
	    snakeManager.getParticleColor(1) = snakeManager.getParticleColor(2);

	}

		// Add attractive force
	void addAttractiveForce(ParticleManager& foodManager, ParticleManager& snakeManager) {
		int attractiveCoeff = 50;
		glm::vec2 attractiveForce = foodManager.getParticlePosition(0) - snakeManager.getParticlePosition(0);
        float d = glm::length(attractiveForce);
        attractiveForce = glm::normalize(attractiveForce);
        
      
	    // Plus on divise et plus c'est petit : pluattractivecoeff augmente et mois l'attraction est forte au loin
        snakeManager.addForceToParticle(0, glm::vec2(attractiveForce[0]/(attractiveCoeff*d), attractiveForce[1]/(attractiveCoeff*d)));
        
	}

		// Add repulsive force
	void addRepulsiveForce(ParticleManager& repulsiveManager, ParticleManager& snakeManager) {
		if(repulsiveManager.getCount() > 0) {
			for(int i = 0; i < snakeManager.getCount(); ++i) {
				for(int j = 0; j < repulsiveManager.getCount(); ++j) {
	                glm::vec2 repulse = snakeManager.getParticlePosition(i) - repulsiveManager.getParticlePosition(j);
	                float d = glm::length(repulse);
	                repulse = glm::normalize(repulse);

	                int repulsiveCoeff = 40;

	                if(d < 0.2) {
	                   // Plus on divise et plus c'est petit : pluattractivecoeff augmente et mois l'attraction est forte au loin
	                    snakeManager.addForceToParticle(i, glm::vec2(repulse[0]/(repulsiveCoeff*d), repulse[1]/(repulsiveCoeff*d)));
	                }
				}	               
                
            }
		}
		
	}

	

	// Create bonus
	void addBonus(ParticleManager& bonusManager) {
		bonusManager.removeParticle(0);
		bonusManager.addRandomParticle(1, ParticleManager::Type::P_BONUS);

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
	int checkFoodCollision(ParticleGraph& snakeGraph, ParticleManager& snakeManager, ParticleManager& foodManager, ParticleManager& fireworkManager, float step, int init) {
	    for(int i = init; i < foodManager.getCount(); ++i) {

	        if(foodManager.getParticleX(i) - step * foodManager.getParticleMass(i) <= snakeManager.getParticleX(0) 
	            && snakeManager.getParticleX(0) <= foodManager.getParticleX(i) + step * foodManager.getParticleMass(i)
	            && foodManager.getParticleY(i) - step * foodManager.getParticleMass(i) <= snakeManager.getParticleY(0) 
	            && snakeManager.getParticleY(0) <= foodManager.getParticleY(i) + step* foodManager.getParticleMass(i)) {

	        	fireworkManager.addCircleParticlesAtPosition(3, snakeManager.getParticlePosition(0), snakeManager.getParticleColor(1), 6);

	            addParticletoSnake(snakeGraph, i, foodManager, snakeManager);
                foodManager.addRandomParticle(snakeManager.getCount());

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