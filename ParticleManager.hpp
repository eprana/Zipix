#ifndef _IMAC3_PARTICLEMANAGER_HPP
#define _IMAC3_PARTICLEMANAGER_HPP

#include <vector>
#include <glm/glm.hpp>

#include "renderer/ParticleRenderer2D.hpp"

namespace imac3 {

typedef std::vector<std::pair<unsigned int, unsigned int>> ParticleGraph;

const glm::vec3 snakeColor = glm::vec3(0.2f, 0.6f, 0.2f);
const glm::vec3 headColor = glm::vec3(0.4f, 0.8f, 0.2f); 
const glm::vec3 foodColor = glm::vec3(0.2f, 0.2f, 0.6f);
const glm::vec3 bonusColor = glm::vec3(1.f, 0.f, 0.f);
const glm::vec3 speedColor = glm::vec3(0.f, 1.f, 0.f);
const glm::vec3 malusColor =  glm::vec3(0.f, 1.f, 0.f);

class ParticleManager {

	public:

		typedef enum {
			P_SNAKE = 0,
			P_FOOD = 1,
			P_BONUS = 2,
		} Type;

		/* Getters & Setters */
		inline std::vector<float>& getMassArray() { return massArray;}
		inline std::vector<glm::vec2>& getPositionArray() { return positionArray; }
		inline std::vector<glm::vec2>& getVelocityArray() { return velocityArray;}
		inline std::vector<glm::vec2>& getForceArray() { return forceArray;}
		inline std::vector<glm::vec3>& getColorArray() { return colorArray;}
		inline std::vector<Type>& getTypeArray() { return typeArray;}

		inline float& getParticleMass(int id) { return massArray[id];}
		inline glm::vec2& getParticlePosition(int id) { return positionArray[id];}
		inline float& getParticleX(int id) { return positionArray[id][0];}
		inline float& getParticleY(int id) { return positionArray[id][1];}
		inline glm::vec2& getParticleVelocity(int id) { return velocityArray[id];}
		inline glm::vec2& getParticleForce(int id) { return forceArray[id];}
		inline glm::vec3& getParticleColor(int id) { return colorArray[id];}
		inline Type& getParticleType(int id) { return typeArray[id];}

		inline int getCount() { return getMassArray().size();}


		// Add a particle to the ParticleManager
		int addParticle(float mass, glm::vec2 position, glm::vec2 velocity, glm::vec2 force, glm::vec3 color, Type type = Type::P_FOOD);

		// Add a particle in the first position to the ParticleManager
		int addParticleToHead(float mass, glm::vec2 position, glm::vec2 velocity, glm::vec2 force, glm::vec3 color);

		// Add random particles
		void addRandomParticles(unsigned int count);

		// Add random particle
		int addRandomParticle(int random, Type type = P_FOOD);

		// Add random particle in a specific position 
		void addCircleParticlesAtPosition(int random, glm::vec2 position, glm::vec3 color, int count);

		// Add force to a particle
		void addForceToParticle(int id, glm::vec2 force);

		// Draw particles
		void drawParticles(ParticleRenderer2D& renderer);

		// Draw particleGraph
		void drawParticleGraph(ParticleGraph& graph, ParticleRenderer2D& renderer);

		// Remove particle
		void removeParticle(int id);

		// Clear Manager
		void clear();




	private:
		std::vector<float> 		massArray;
		std::vector<glm::vec2>	positionArray;
		std::vector<glm::vec2> 	velocityArray;
		std::vector<glm::vec2>	forceArray;
		std::vector<glm::vec3>	colorArray;
		std::vector<Type> 		typeArray;
};

}

#endif // _IMAC3_PARTICLEMANAGER_HPP