#include <iostream>
#include <cstdlib>

#include <glm/glm.hpp>
#include <glm/gtc/random.hpp>

#include "Game.hpp"
#include "renderer/WindowManager.hpp"
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
    ParticleManager redManager;
    //ParticleManager blueManager;
    ParticleManager foodManager;
    // ParticleManager bonusManager;
    ParticleManager autoManager;

    // Forces
    GraphHookForce graphHook = GraphHookForce(1.f, 0.15f/4.f);
    GraphBrakeForce graphBrake = GraphBrakeForce(0.3f, 0.0001f); // 0.5 = viscosité max 
    
    GraphHookForce redGraphHook = GraphHookForce(1.f, 0.15f/4.f);
    GraphBrakeForce redGraphBrake = GraphBrakeForce(0.3f, 0.0001f);

    // // Ajout des particules
    int id = foodManager.addRandomParticle(1);

    // Copy the food to the autoManager
    copyParticle(foodManager, autoManager, id);

    // Forces
    ConstantForce mg(glm::vec2(0.f, -0.0005f));

    // LeapfrogSolver
    LeapfrogSolver leapfrog;

    // Snake's creation
    ParticleGraph snakeGraph = createString(glm::vec2(0.f, 0.0f), glm::vec2(0.f, -0.15f), glm::vec3(0.2f, 0.6f, 0.2f), 4.f, snakeManager);
    ParticleGraph redGraph = createString(glm::vec2(0.f, 0.2f), glm::vec2(0.15f, 0.2f), glm::vec3(0.6f, 0.2f, 0.2f), 4.f, redManager);
    ParticleGraph autoGraph;

    // Variables
    const float step = 0.04;
    const float viscosity = -0.05;
    const float attractiveCoeff = 5;
    int score = 0;
    //int bonus = 0;
    //SnakeDirection dir = SNAKE_UP;

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
        redManager.drawParticles(renderer);


        //mg.apply(bonusManager);

        // Mise à jour du graph autoGraph
        updateParticle(snakeManager, 0, autoManager, 1);
        updateParticle(redManager, 0, autoManager, 2);

        // Force attractive
        glm::vec2 attractiveForce = foodManager.getParticlePosition(0) - snakeManager.getParticlePosition(0);
        int d = attractiveForce.length();
        attractiveForce = glm::normalize(attractiveForce);
 
        snakeManager.addForceToParticle(0, glm::vec2(attractiveForce[0]/(attractiveCoeff*d), attractiveForce[1]/(attractiveCoeff*d)));
        
        glm::vec2 redAttractiveForce = foodManager.getParticlePosition(0) - redManager.getParticlePosition(0);
        int redDistance = redAttractiveForce.length();
        redAttractiveForce = glm::normalize(redAttractiveForce);
        redManager.addForceToParticle(0, glm::vec2(redAttractiveForce[0]/(attractiveCoeff*redDistance), redAttractiveForce[1]/(attractiveCoeff*redDistance)));


        // Forces
        graphHook.setGraph(&snakeGraph);
        graphBrake.setGraph(&snakeGraph);

        redGraphHook.setGraph(&redGraph);
        redGraphBrake.setGraph(&redGraph);
        

        // Simulation
        if(dt != 0) {

            // Move
            // if(dir == SNAKE_UP) {
            //     snakeManager.getParticleVelocity(0) = glm::vec2(0.f, step);
            // }
            // else if (dir == SNAKE_DOWN) {
            //     snakeManager.getParticleVelocity(0) = glm::vec2(0.f, -step);
            // }
            // else if (dir == SNAKE_LEFT) {
            //     snakeManager.getParticleVelocity(0) = glm::vec2(-step, 0.f);
            // }
            // else if (dir == SNAKE_RIGHT) {
            //     snakeManager.getParticleVelocity(0) = glm::vec2(step, 0.f);
            // }

            // // Bonus
            // if(bonus == 5) {
            //     addBonus(bonusManager);
            //     bonus++;
            // }

            // // Snake - Window
            // if(isOutside(snakeManager) ) {
            //     // || checkSnakeCollision(snakeManager, snakeManager, 0.02f, 1) != -1) {
            //     std::cout << "YOUR SCORE : "  << score << std::endl;
            //     snakeManager.clear();
            //     return EXIT_SUCCESS;
            // }

            // // Bonus - Window
            // if(isOutside(bonusManager)) {
            //     bonusManager.clear();
            //     bonus = 0;
            // }

            // Snake - Food
            int idCollision = checkFoodCollision(snakeManager, foodManager, 0.05f, 0);
            if( idCollision != -1) {
                addParticletoSnake(snakeGraph, idCollision, foodManager, snakeManager);
                foodManager.addRandomParticle(2);
                updateParticle(foodManager, 0, autoManager, 0);
                //bonus++;
                score += snakeManager.getCount();
            }

            int redCollision = checkFoodCollision(redManager, foodManager, 0.05f, 0);
            if( redCollision != -1) {
                addParticletoSnake(redGraph, redCollision, foodManager, redManager);
                foodManager.addRandomParticle(2);
                updateParticle(foodManager, 0, autoManager, 0);
                //bonus++;
                score += redManager.getCount();
            }


            // // Snake - Bonus
            // int idBonusCollision = checkFoodCollision(snakeManager, bonusManager, 0.05f, 0);
            // if( idBonusCollision != -1) {
            //     addParticletoSnake(snakeGraph, idBonusCollision, bonusManager, snakeManager);
            //     bonus = 0;
            //     score += snakeManager.getCount()*2;
            // }

            // Apply forces
            graphBrake.setDt(dt);
            redGraphBrake.setDt(dt);

            graphHook.apply(snakeManager);
            graphBrake.apply(snakeManager);

            redGraphHook.apply(redManager);
            redGraphBrake.apply(redManager);


            // Viscosity 
            // for(int i = 0; i < snakeManager.getCount() - 1; ++i) {
            //     snakeManager.addForceToParticle(i, snakeManager.getParticleVelocity(i) * viscosity);
            // }

            // Leapfrog solver
            leapfrog.solve(snakeManager, dt);
            leapfrog.solve(autoManager, dt);
            leapfrog.solve(redManager, dt);
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
                // case SDL_KEYDOWN:
                //     if(e.key.keysym.sym == SDLK_LEFT) {
                //         dir = SNAKE_LEFT;
                //         break;
                // 	}
                //     else if(e.key.keysym.sym == SDLK_RIGHT) {
                //         dir = SNAKE_RIGHT;
                //         break;
                //     }
                //     else if(e.key.keysym.sym == SDLK_UP) {
                //         dir = SNAKE_UP;
                //         break;
                //     }
                //     else if(e.key.keysym.sym == SDLK_DOWN) {
                //         dir = SNAKE_DOWN;
                //         break;
                //     }
                //     break;
            }
		}

        // Mise à jour de la fenêtre
        dt = wm.update();
	}

	return EXIT_SUCCESS;
}
