#include <iostream>
#include <cstdlib>

#include <glm/glm.hpp>
#include <glm/gtc/random.hpp>

#include "Game.hpp"
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

typedef enum {
    SNAKE_UP,
    SNAKE_DOWN,
    SNAKE_RIGHT, 
    SNAKE_LEFT
} SnakeDirection;


int main() {
    WindowManager wm(WINDOW_WIDTH, WINDOW_HEIGHT, "Zipix");
    wm.setFramerate(30);

    // Managers and Renderer
    ParticleRenderer2D renderer;
    ParticleManager snakeManager;
    ParticleManager foodManager;

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
    
    // Variables
    const float step = 0.04;
    const float viscosity = -0.1;
    int score = 0;
    SnakeDirection dir = SNAKE_UP;

    // Temps s'écoulant entre chaque frame
    float dt = 0.f;

	bool done = false;
    while(!done) {
        wm.startMainLoop();

        // Renderer
        renderer.clear();

        // Draw particles
        foodManager.drawParticles(renderer);
        snakeManager.drawParticles(renderer);
        //snakeManager.drawParticleGraph(snakeGraph, renderer);

        // Forces
        graphHook.setGraph(&snakeGraph);
        graphBrake.setGraph(&snakeGraph);
        

        // Simulation
        if(dt != 0) {

            // Move
            if(dir == SNAKE_UP) {
                snakeManager.getParticleVelocity(0) = glm::vec2(0.f, step);
            }
            else if (dir == SNAKE_DOWN) {
                snakeManager.getParticleVelocity(0) = glm::vec2(0.f, -step);
            }
            else if (dir == SNAKE_LEFT) {
                snakeManager.getParticleVelocity(0) = glm::vec2(-step, 0.f);
            }
            else if (dir == SNAKE_RIGHT) {
                snakeManager.getParticleVelocity(0) = glm::vec2(step, 0.f);
            }

            // Snake - Window
            if(isOutside(snakeManager) 
                || checkSnakeCollision(snakeManager, snakeManager, 0.02f, 1) != -1) {
                std::cout << "YOUR SCORE : "  << score << std::endl;
                snakeManager.clear();
                return EXIT_SUCCESS;
            }

            // Snake - Food
            int idCollision = checkFoodCollision(snakeManager, foodManager, 0.05f, 0);
            if( idCollision != -1) {
                addParticletoSnake(snakeGraph, idCollision, foodManager, snakeManager);
                foodManager.addRandomParticles(1);
                score += snakeManager.getCount();
            }

            // Apply forces
            graphBrake.setDt(dt);
            graphHook.apply(snakeManager);
            graphBrake.apply(snakeManager);


            // Viscosity 
            for(int i = 0; i < snakeManager.getCount() - 1; ++i) {
                snakeManager.addForceToParticle(i, snakeManager.getParticleVelocity(i) * viscosity);
            }

            // Leapfrog solver
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
                        dir = SNAKE_LEFT;
                        break;
                	}
                    else if(e.key.keysym.sym == SDLK_RIGHT) {
                        dir = SNAKE_RIGHT;
                        break;
                    }
                    else if(e.key.keysym.sym == SDLK_UP) {
                        dir = SNAKE_UP;
                        break;
                    }
                    else if(e.key.keysym.sym == SDLK_DOWN) {
                        dir = SNAKE_DOWN;
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
