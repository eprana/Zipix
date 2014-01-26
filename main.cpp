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
    ParticleManager blueManager;

    ParticleManager foodManager;
    ParticleManager bonusManager;

    ParticleManager autoManager;

    // Forces
    GraphHookForce graphHook = GraphHookForce(1.f, 0.15f/4.f);
    GraphBrakeForce graphBrake = GraphBrakeForce(0.3f, 0.0001f); // 0.5 = viscosité max 
    
    GraphHookForce redGraphHook = GraphHookForce(1.f, 0.15f/4.f);
    GraphBrakeForce redGraphBrake = GraphBrakeForce(0.3f, 0.0001f);

    GraphHookForce blueGraphHook = GraphHookForce(1.f, 0.15f/4.f);
    GraphBrakeForce blueGraphBrake = GraphBrakeForce(0.3f, 0.0001f);

    // Box
    Polygon box = buildBox(glm::vec3(1.f, 1.f, 1.f), glm::vec2(-1.f, -1.f), 2, 2, true);

    // // Ajout des particules
    int id = foodManager.addRandomParticle(1);

    // Copy the food to the autoManager
    copyParticle(foodManager, autoManager, id);

    // Forces
    ConstantForce mg(glm::vec2(0.f, -0.0005f));

    // LeapfrogSolver
    LeapfrogSolver leapfrog;
    PolygonForce boxForce(box, 1.5f, leapfrog);

    // Snake's creation
    ParticleGraph snakeGraph = createString(glm::vec2(0.f, 0.0f), glm::vec2(0.f, -0.15f), glm::vec3(0.2f, 0.6f, 0.2f), 4.f, snakeManager);
    ParticleGraph redGraph = createString(glm::vec2(0.f, 0.2f), glm::vec2(0.15f, 0.2f), glm::vec3(0.6f, 0.2f, 0.2f), 4.f, redManager);
    ParticleGraph blueGraph = createString(glm::vec2(0.f, -0.2f), glm::vec2(-0.15f, -0.2f), glm::vec3(0.2f, 0.2f, 0.6f), 4.f, blueManager);

    copyParticle(snakeManager, autoManager, 0);
    copyParticle(redManager, autoManager, 0);
    copyParticle(blueManager, autoManager, 0);

    // Variables
    //const float step = 0.04;
    //const float viscosity = -0.05;

    int score = 0;
    int bonus = 0;
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
        blueManager.drawParticles(renderer);
        bonusManager.drawParticles(renderer);

        //mg.apply(bonusManager);

        // Mise à jour du graph autoGraph
        updateParticle(snakeManager, 0, autoManager, 1);
        updateParticle(redManager, 0, autoManager, 2);
        updateParticle(blueManager, 0, autoManager, 3);

        // Force attractive
        addAttractiveForce(foodManager, snakeManager);
        addAttractiveForce(foodManager, redManager);
        addAttractiveForce(foodManager, blueManager);
        
        // Forces
        graphHook.setGraph(&snakeGraph);
        graphBrake.setGraph(&snakeGraph);

        redGraphHook.setGraph(&redGraph);
        redGraphBrake.setGraph(&redGraph);
        
        blueGraphHook.setGraph(&blueGraph);
        blueGraphBrake.setGraph(&blueGraph);

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

            // Bonus
            if(bonus == 5) {
                addBonus(bonusManager);
                bonus++;
            }

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
            if( checkFoodCollision(snakeGraph, snakeManager, foodManager, 0.05f, 0) != -1
                || checkFoodCollision(redGraph, redManager, foodManager, 0.05f, 0) != -1
                || checkFoodCollision(blueGraph, blueManager, foodManager, 0.05f, 0) != -1) {

                
                updateParticle(foodManager, 0, autoManager, 0);
                bonus++;
            }
            
           
            // int idCollision = checkFoodCollision(snakeManager, foodManager, 0.05f, 0);
            // if( idCollision != -1) {
            //     addParticletoSnake(snakeGraph, idCollision, foodManager, snakeManager);
            //     foodManager.addRandomParticle(2);
            //     updateParticle(foodManager, 0, autoManager, 0);
            //     //bonus++;
            //     score += snakeManager.getCount();
            // }

            // int redCollision = checkFoodCollision(redManager, foodManager, 0.05f, 0);
            // if( redCollision != -1) {
            //     addParticletoSnake(redGraph, redCollision, foodManager, redManager);
            //     foodManager.addRandomParticle(2);
            //     updateParticle(foodManager, 0, autoManager, 0);
            //     //bonus++;
            //     score += redManager.getCount();
            // }

            // int blueCollision = checkFoodCollision(blueManager, foodManager, 0.05f, 0);
            // if( blueCollision != -1) {
            //     addParticletoSnake(blueGraph, blueCollision, foodManager, blueManager);
            //     foodManager.addRandomParticle(2);
            //     updateParticle(foodManager, 0, autoManager, 0);
            //     //bonus++;
            //     score += blueManager.getCount();
            // }


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
            blueGraphBrake.setDt(dt);
            boxForce.setDt(dt);

            boxForce.apply(snakeManager);
            boxForce.apply(redManager);
            boxForce.apply(blueManager);

            graphHook.apply(snakeManager);
            graphBrake.apply(snakeManager);

            redGraphHook.apply(redManager);
            redGraphBrake.apply(redManager);

            blueGraphHook.apply(blueManager);
            blueGraphBrake.apply(blueManager);


            // Viscosity 
            // for(int i = 0; i < snakeManager.getCount() - 1; ++i) {
            //     snakeManager.addForceToParticle(i, snakeManager.getParticleVelocity(i) * viscosity);
            // }

            if(bonusManager.getCount() > 0) {
                //for(int i = 0; i < snakeManager.getCount() - 1; ++i) {
                    glm::vec2 repulse = snakeManager.getParticlePosition(0) - bonusManager.getParticlePosition(0);
                    int d = repulse.length();
                    repulse = glm::normalize(repulse);
                    snakeManager.addForceToParticle(0, glm::vec2(repulse[0]/(d*10), repulse[1]/(d*10)));
                //}
            }
            

            // Leapfrog solver
            leapfrog.solve(snakeManager, dt);
            leapfrog.solve(autoManager, dt);
            leapfrog.solve(redManager, dt);
            leapfrog.solve(blueManager, dt);
            leapfrog.solve(bonusManager, dt);
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
