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

static const Uint32 WINDOW_WIDTH = 512;
static const Uint32 WINDOW_HEIGHT = 512;

using namespace imac3;

typedef enum {
    SNAKE_UP,
    SNAKE_DOWN,
    SNAKE_RIGHT, 
    SNAKE_LEFT
} SnakeDirection;


int main() {
    WindowManager wm(WINDOW_WIDTH, WINDOW_HEIGHT, "ZIPIX");
    wm.setFramerate(30);

    // ----- Fmod test ----- 
    FMOD_SYSTEM *system;

    /* Création et initialisation d'un objet système */
    FMOD_System_Create(&system);
    FMOD_System_Init(system, 2, FMOD_INIT_NORMAL, NULL);

    FMOD_SOUND* background = NULL;
    FMOD_SOUND* firework = NULL;

    FMOD_RESULT check;

    check = FMOD_System_CreateSound(system, "../music/firework_explode_and_crackle.mp3", FMOD_CREATESAMPLE, 0, &firework);
    if (check != FMOD_OK)
    {
        fprintf(stderr, "Impossible de lire le fichier firework.wav\n");
        exit(EXIT_FAILURE);
    }
    check = FMOD_System_CreateSound(system, "../music/Timer.mp3", FMOD_SOFTWARE | FMOD_2D | FMOD_CREATESTREAM, 0, &background);
    
    if (check != FMOD_OK)
    {
        fprintf(stderr, "Impossible de lire le fichier Timer.mp3\n");
        exit(EXIT_FAILURE);
    }
    
    FMOD_CHANNEL *backgroundChannel = NULL;
    FMOD_CHANNEL *fireworkChannel = NULL;
    FMOD_System_GetChannel(system, 0, &backgroundChannel);
    FMOD_System_GetChannel(system, 1, &fireworkChannel);
    FMOD_Sound_SetLoopCount(background, -1);


    FMOD_System_PlaySound(system, FMOD_CHANNEL_FREE, background, 0, &backgroundChannel);

    time_t beginTime;
    time_t currentTime;
    time(&beginTime);

    // Managers and Renderer
    ParticleRenderer2D renderer;

    ParticleManager snakeManager;
    snakeManager.setHeadColor(glm::vec3(0.4f, 0.8f, 0.3f));
    ParticleManager redManager;
    redManager.setHeadColor(glm::vec3(0.9f, 0.4f, 0.3f));
    ParticleManager blueManager;
    blueManager.setHeadColor(glm::vec3(0.2f, 0.4f, 0.9f));

    ParticleManager foodManager;
    ParticleManager bonusManager;
    ParticleManager fireworkManager;

    ParticleManager autoManager;

    // Graph Forces
    GraphHookForce graphHook = GraphHookForce(1.f, 0.15f/4.f);
    GraphBrakeForce graphBrake = GraphBrakeForce(0.3f, 10.f); // 0.5 = viscosité max 
    
    GraphHookForce redGraphHook = GraphHookForce(1.f, 0.15f/4.f);
    GraphBrakeForce redGraphBrake = GraphBrakeForce(0.3f, 0.0001f);

    GraphHookForce blueGraphHook = GraphHookForce(1.f, 0.15f/4.f);
    GraphBrakeForce blueGraphBrake = GraphBrakeForce(0.3f, 0.0001f);

    // Box
    Polygon box = buildBox(glm::vec3(1.f, 1.f, 1.f), glm::vec2(-1.f, -1.f), 2, 2, true);

    // // Ajout des particules
    int id = foodManager.addRandomParticle(1);
    bonusManager.addParticle(1.5f, glm::vec2(0.5, 0.5), glm::vec2(0, 0), glm::vec2(0, 0), glm::vec3(0.8f, 0.6f, 0.f));
    bonusManager.addParticle(1.5f, glm::vec2(-0.2, 0.6), glm::vec2(0, 0), glm::vec2(0, 0), glm::vec3(0.8f, 0.6f, 0.f));
    bonusManager.addParticle(1.5f, glm::vec2(0.8, -0.7), glm::vec2(0, 0), glm::vec2(0, 0), glm::vec3(0.8f, 0.6f, 0.f));
    bonusManager.addParticle(1.5f, glm::vec2(-0.7, -0.5), glm::vec2(0, 0), glm::vec2(0, 0), glm::vec3(0.8f, 0.6f, 0.f));
    bonusManager.addParticle(1.5f, glm::vec2(0.3, -0.1), glm::vec2(0, 0), glm::vec2(0, 0), glm::vec3(10.8f, 0.6f, 0.f));

    // Copy the food to the autoManager
    copyParticle(foodManager, autoManager, id);

    // Forces
    ConstantForce mg(glm::vec2(0.f, -0.005));

    // LeapfrogSolver
    LeapfrogSolver leapfrog;
    PolygonForce boxForce(box, 1.5f, leapfrog);

    // Snake's creation
    ParticleGraph snakeGraph = createString(glm::vec2(0.f, 0.0f), glm::vec2(0.f, -0.15f), glm::vec3(0.2f, 0.6f, 0.2f), glm::vec3(0.4f, 0.8f, 0.3f), 4.f, snakeManager);
    ParticleGraph redGraph = createString(glm::vec2(0.f, 0.2f), glm::vec2(0.15f, 0.2f), glm::vec3(0.9f, 0.2f, 0.2f), glm::vec3(0.9f, 0.4f, 0.3f), 4.f, redManager);
    ParticleGraph blueGraph = createString(glm::vec2(0.f, -0.2f), glm::vec2(-0.15f, -0.2f), glm::vec3(0.2f, 0.2f, 0.6f), glm::vec3(0.2f, 0.4f, 0.9f), 4.f, blueManager);

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

                FMOD_System_PlaySound(system, FMOD_CHANNEL_FREE, firework, 0, &fireworkChannel);
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

                    //FMOD
                    /* On libère le son et on ferme et libère l'objet système */
                    FMOD_Sound_Release(background);
                    FMOD_Sound_Release(firework);
                    FMOD_System_Close(system);
                    FMOD_System_Release(system);

					done = true;
					break;

                //Déplacement
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
