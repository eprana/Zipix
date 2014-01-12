#include <iostream>
#include <cstdlib>

#include <glm/glm.hpp>
#include <glm/gtc/random.hpp>

#include "renderer/WindowManager.hpp"
#include "renderer/ParticleRenderer2D.hpp"
#include "ParticleManager.hpp"
#include "ConstantForce.hpp"
#include "PolygonForce.hpp"
#include "HookForce.hpp"
#include "BrakeForce.hpp"
#include "GraphBrakeForce.hpp"
#include "GraphHookForce.hpp"



#include <vector>

static const Uint32 WINDOW_WIDTH = 1024;
static const Uint32 WINDOW_HEIGHT = 1024;

using namespace imac3;


/* ParticleGraph */
typedef std::vector<std::pair<unsigned int, unsigned int>> ParticleGraph;

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
void addParticletoSnake(ParticleGraph& graph, int id, ParticleManager& particleManager, ParticleManager& snakeManager) {
    id = snakeManager.addParticle(particleManager.getParticleMass(id), 
                                    particleManager.getParticlePosition(id),
                                    particleManager.getParticleVelocity(id),
                                    particleManager.getParticleForce(id),
                                    particleManager.getParticleColor(id));

    particleManager.clear();
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
int checkCollision(ParticleManager& snakeManager, ParticleManager& foodManager) {
    for(int i = 0; i < foodManager.getCount(); ++i) {

        if(foodManager.getParticleX(i) - 0.05f * foodManager.getParticleMass(i) <= snakeManager.getParticleX(0) 
            && snakeManager.getParticleX(0) <= foodManager.getParticleX(i) + 0.05f * foodManager.getParticleMass(i)
            && foodManager.getParticleY(i) - 0.05f * foodManager.getParticleMass(i) <= snakeManager.getParticleY(0) 
            && snakeManager.getParticleY(0) <= foodManager.getParticleY(i) + 0.05f * foodManager.getParticleMass(i)) {

            return i;
        }
    }

    return -1;
}

int main() {
    WindowManager wm(WINDOW_WIDTH, WINDOW_HEIGHT, "Zipix");
    wm.setFramerate(30);

    // Managers et Renderer
    ParticleRenderer2D renderer;
    ParticleManager snakeManager;
    ParticleManager foodManager;

    int score = 0;

    // Forces
    GraphHookForce graphHook = GraphHookForce(1.f, 0.15f/4.f);
    GraphBrakeForce graphBrake = GraphBrakeForce(0.3f, 0.0001f); // 0.5 = viscosité max 
    
    // // Ajout des particules
    foodManager.addRandomParticles(1);

    // Forces
    //ConstantForce mg(glm::vec2(0.f, -0.01f));

    // LeapfrogSolver
    LeapfrogSolver leapfrog;

    // Snake's creation
    ParticleGraph snakeGraph = createString(glm::vec2(0.f, 0.0f), glm::vec2(0.f, -0.15f), glm::vec3(1.f, 0.f, 0.f), 4.f, snakeManager);
    

    // Temps s'écoulant entre chaque frame
    float dt = 0.f;

	bool done = false;
    while(!done) {
        wm.startMainLoop();

        // Renderer
        renderer.clear();

        foodManager.drawParticles(renderer);

        snakeManager.drawParticles(renderer);
        snakeManager.drawParticleGraph(snakeGraph, renderer);

        // Forces
        graphHook.setGraph(&snakeGraph);
        graphBrake.setGraph(&snakeGraph);
        

        // Simulation
        if(dt != 0) {

            // Collision
            if(isOutside(snakeManager)) {
                std::cout << "YOUR SCORE : "  << score << std::endl;
                snakeManager.clear();
                return EXIT_SUCCESS;
            }

            int idCollision = checkCollision(snakeManager, foodManager);
            if( idCollision != -1) {
                addParticletoSnake(snakeGraph, idCollision, foodManager, snakeManager);
                foodManager.addRandomParticles(1);
                score += snakeManager.getCount();
            }

            graphBrake.setDt(dt);
            graphHook.apply(snakeManager);
            graphBrake.apply(snakeManager);

            // brake.setDt(dt);
            // hook.apply(particleManager);
            // brake.apply(particleManager);

            // mg.apply(particleManager);

            leapfrog.solve(snakeManager, dt);
        }
        

        // Gestion des evenements
		SDL_Event e;
        while(wm.pollEvent(e)) {
			switch(e.type) {

				default:
					break;

				case SDL_QUIT:
					done = true;
					break;


                // Déplacement
                case SDL_KEYDOWN:
                    if(e.key.keysym.sym == SDLK_LEFT) {
                        snakeManager.getParticleVelocity(0) = glm::vec2(-0.03f, 0.f);
                        break;
                	}
                    else if(e.key.keysym.sym == SDLK_RIGHT) {
                        snakeManager.getParticleVelocity(0) = glm::vec2(0.03f, 0.f);
                        break;
                    }
                    else if(e.key.keysym.sym == SDLK_UP) {
                        snakeManager.getParticleVelocity(0) = glm::vec2(0.f, 0.03f);
                        break;
                    }
                    else if(e.key.keysym.sym == SDLK_DOWN) {
                        snakeManager.getParticleVelocity(0) = glm::vec2(0.f, -0.03f);
                        break;
                    }
                    break;
            }
		}

        // Mise à jour de la fenêtre
        dt = wm.update();
	}

	return EXIT_SUCCESS;
}
