#ifndef _IMAC3_GAME_HPP
#define _IMAC3_GAME_HPP

#include "ParticleManager.hpp"

namespace imac3 {

	typedef std::vector<std::pair<unsigned int, unsigned int>> ParticleGraph;

	// Create a particleGraph
	ParticleGraph createString(glm::vec2 A, glm::vec2 B, glm::vec3 color, uint32_t discFactor, ParticleManager& particleManager);
	
	// Add a particle to the Snake
	void addParticletoSnake(ParticleGraph& graph, int id, ParticleManager& particleManager, ParticleManager& snakeManager);
	
	// Check if the snake is outside
	bool isOutside(ParticleManager& snakeManager);

	// Check collision between snake and food
	int checkFoodCollision(ParticleManager& snakeManager, ParticleManager& foodManager, float step, int init);
	
	// Check collision between snake and snake
	int checkSnakeCollision(ParticleManager& snakeManager, ParticleManager& foodManager, float step, int init);

}
	  
#endif // _IMAC3_GAME_HPP