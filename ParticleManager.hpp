#ifndef _IMAC3_PARTICLEMANAGER_HPP
#define _IMAC3_PARTICLEMANAGER_HPP

#include <vector>
#include <glm/glm.hpp>

#include "renderer/ParticleRenderer2D.hpp"

namespace imac3 {

typedef std::vector<std::pair<unsigned int, unsigned int>> ParticleGraph;

class ParticleManager {

	public:

		/* Getters & Setters */
		inline std::vector<float>& getMassArray() { return massArray;}
		inline std::vector<glm::vec2>& getPositionArray() { return positionArray; }
		inline std::vector<glm::vec2>& getVelocityArray() { return velocityArray;}
		inline std::vector<glm::vec2>& getForceArray() { return forceArray;}
		inline std::vector<glm::vec3>& getColorArray() { return colorArray;}

		inline float& getParticleMass(int id) { return massArray[id];}
		inline glm::vec2& getParticlePosition(int id) { return positionArray[id];}
		inline float& getParticleX(int id) { return positionArray[id][0];}
		inline float& getParticleY(int id) { return positionArray[id][1];}
		inline glm::vec2& getParticleVelocity(int id) { return velocityArray[id];}
		inline glm::vec2& getParticleForce(int id) { return forceArray[id];}
		inline glm::vec3& getParticleColor(int id) { return colorArray[id];}

		inline int getCount() { return getMassArray().size();}


		// Add a particle to the ParticleManager
		int addParticle(float mass, glm::vec2 position, glm::vec2 velocity, glm::vec2 force, glm::vec3 color);

		// Add random particles
		void addRandomParticles(unsigned int count);

		// Add force to a particle
		void addForceToParticle(int id, glm::vec2 force);

		// Draw particles
		void drawParticles(ParticleRenderer2D& renderer);

		// Draw particleGraph
		void drawParticleGraph(ParticleGraph& graph, ParticleRenderer2D& renderer);

		// Clear Manager
		void clear();




	private:

		std::vector<float> 		massArray;
		std::vector<glm::vec2>	positionArray;
		std::vector<glm::vec2> 	velocityArray;
		std::vector<glm::vec2>	forceArray;
		std::vector<glm::vec3>	colorArray;
};

}

#endif // _IMAC3_PARTICLEMANAGER_HPP