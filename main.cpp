#include <iostream>
#include <cstdlib>
#include <ctime> 

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

//FMOD
#include "../libs/FMOD-4.44.27/linux/inc/fmod.h"
#include "fmod_errors.h"

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

    // ----- Fmod test ----- 
    FMOD_SYSTEM *system;
    FMOD_SOUND *test;
    
    FMOD_RESULT resultat;

    /* Création et initialisation d'un objet système */
    FMOD_System_Create(&system);
    FMOD_System_Init(system, 1, FMOD_INIT_NORMAL, NULL);
    
    /* Chargement du son et vérification du chargement */
    resultat = FMOD_System_CreateSound(system, "../music/Timer.mp3", FMOD_CREATESAMPLE, 0, &test);
    if (resultat != FMOD_OK)
    {
        std::cerr << "Impossible de lire le son test" << std::endl;
        exit(EXIT_FAILURE);
    }

    FMOD_System_PlaySound(system, FMOD_CHANNEL_FREE, test, 0, NULL);

    time_t beginTime;
    time_t currentTime;
    time(&beginTime);

    // Managers and Renderer
    ParticleRenderer2D renderer;

    ParticleManager snakeManager;
    ParticleManager redManager;
    ParticleManager blueManager;

    ParticleManager foodManager;
    ParticleManager bonusManager;
    ParticleManager fireworkManager;

    ParticleManager autoManager;

    // Graph Forces
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
    bonusManager.addParticle(1.5f, glm::vec2(0.5, 0.5), glm::vec2(0, 0), glm::vec2(0, 0), glm::vec3(1, 0, 0));
    bonusManager.addParticle(1.5f, glm::vec2(-0.2, 0.6), glm::vec2(0, 0), glm::vec2(0, 0), glm::vec3(1, 0, 0));
    bonusManager.addParticle(1.5f, glm::vec2(0.8, -0.7), glm::vec2(0, 0), glm::vec2(0, 0), glm::vec3(1, 0, 0));
    bonusManager.addParticle(1.5f, glm::vec2(-0.7, -0.5), glm::vec2(0, 0), glm::vec2(0, 0), glm::vec3(1, 0, 0));
    bonusManager.addParticle(1.5f, glm::vec2(0.3, -0.1), glm::vec2(0, 0), glm::vec2(0, 0), glm::vec3(1, 0, 0));

    // Copy the food to the autoManager
    copyParticle(foodManager, autoManager, id);

    // Forces
    ConstantForce mg(glm::vec2(0.f, -0.005));

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
    //const float viscosity = -0.05;

    int score = 0;
    int bonus = 0;

    // Temps s'écoulant entre chaque frame
    float dt = 0.f;

	bool done = false;
    while(!done) {

        time(&currentTime);
        wm.startMainLoop();

        // Renderer
        renderer.clear(); 

        // Draw particles
        foodManager.drawParticles(renderer);
        snakeManager.drawParticles(renderer);
        redManager.drawParticles(renderer);
        blueManager.drawParticles(renderer);
        fireworkManager.drawParticles(renderer);  
        bonusManager.drawParticles(renderer);
        
        mg.apply(fireworkManager);       

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

            // std::cout << "Begin : " << beginTime << std::endl;
            // std::cout << "Current : " << currentTime << std::endl;
 
            //Bonus
            if(difftime(currentTime, beginTime) == 10) {
                beginTime = currentTime;
                time(&currentTime);
                addBonus(bonusManager);
                bonus++;
            }

            // Snake - Food
            if( checkFoodCollision(snakeGraph, snakeManager, foodManager, fireworkManager, 0.05f, 0) != -1
                || checkFoodCollision(redGraph, redManager, foodManager, fireworkManager, 0.05f, 0) != -1
                || checkFoodCollision(blueGraph, blueManager, foodManager, fireworkManager, 0.05f, 0) != -1) {

                updateParticle(foodManager, 0, autoManager, 0);
                bonus++;
            }


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

            addRepulsiveForce(bonusManager, snakeManager);
            addRepulsiveForce(bonusManager, redManager);
            addRepulsiveForce(bonusManager, blueManager);       

            // Leapfrog solver
            leapfrog.solve(snakeManager, dt);
            leapfrog.solve(autoManager, dt);
            leapfrog.solve(redManager, dt);
            leapfrog.solve(blueManager, dt);
            leapfrog.solve(fireworkManager, dt);
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
            }
		}

        // Mise à jour de la fenêtre
        dt = wm.update();
	}

	return EXIT_SUCCESS;
}
