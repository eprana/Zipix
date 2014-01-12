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

/* ParticleManager */
// class StaticParticleManager {
//     std::vector<glm::vec2> m_PositionArray;
//     std::vector<float> m_MassArray;
//     std::vector<glm::vec3> m_ColorArray;

// public:
//     void addCircleParticles(float radius, uint32_t count) {
//         float delta = 2 * 3.14f / count; // 2pi / nombre de particules
//         for(size_t i = 0; i < count; ++i) {
//             float c = cos(i * delta), s = sin(i * delta);
//             addParticle(glm::vec2(radius * c, radius * s), 1.f, glm::vec3(c, s, c * s));
//         }
//     }

//     void addParticle(glm::vec2 position, float mass, glm::vec3 color) {
//         m_PositionArray.push_back(position);
//         m_MassArray.push_back(mass);
//         m_ColorArray.push_back(color);
//     }

//     void drawParticles(ParticleRenderer2D& renderer) {
//         renderer.drawParticles(m_PositionArray.size(),
//                                &m_PositionArray[0],
//                                &m_MassArray[0],
//                                &m_ColorArray[0]);
//     }

//     void move(float maxDist) {
//         for(uint32_t i = 0; i < m_PositionArray.size(); ++i) {
//             m_PositionArray[i] += glm::diskRand(maxDist);
//         }
//     }
    
//     void addRandomParticles(unsigned int count) {	
//     	for(int i = 0; i < count; ++i) {
//     		float x = glm::linearRand(-1.f, 1.f);
//     		float y = glm::linearRand(-1.f, 1.f);
//     		addParticle(glm::vec2(x, y), 1.f, glm::vec3(0, 0, 1));
//     	}
//     }
// };

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


bool isCollision(ParticleManager& snakeManager) {
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

typedef std::vector<std::pair<unsigned int, unsigned int>> ParticleGraph;

int main() {
    WindowManager wm(WINDOW_WIDTH, WINDOW_HEIGHT, "Zipix");
    wm.setFramerate(30);

    // Manager et Renderer
    ParticleRenderer2D renderer;
    ParticleManager snakeManager;

    ParticleManager foodManager;



    //particleManager.addParticle(3.f, glm::vec2(-0.3f, -0.3f), glm::vec2(0.f, 0.f), glm::vec2(0.f, 0.f), glm::vec3(1.f, 0.f, 0.f));  
    // particleManager.addParticle(3.f, glm::vec2(0.3f, -0.3f), glm::vec2(0.f, 0.f), glm::vec2(0.f, 0.f), glm::vec3(0.f, 1.f, 0.f));
    // particleManager.addParticle(3.f, glm::vec2(0.f, 0.3f), glm::vec2(0.f, 0.f), glm::vec2(0.f, 0.f), glm::vec3(0.f, 0.f, 1.f));

    // Forces
    GraphHookForce graphHook = GraphHookForce(1.f, 0.15f/4.f);
    GraphBrakeForce graphBrake = GraphBrakeForce(0.3f, 0.0001f); // 0.5 = viscosité max 
    
    // // Ajout des particules
    //particleManager.addCircleParticles(0.5f, 300);
    foodManager.addRandomParticles(2);

    // Forces
    //ConstantForce mg(glm::vec2(0.f, -0.01f));

    // LeapfrogSolver
    LeapfrogSolver leapfrog;

    // Snake's creation
    ParticleGraph particleGraph = createString(glm::vec2(0.f, 0.0f), glm::vec2(0.f, -0.15f), glm::vec3(1.f, 0.f, 0.f), 4.f, snakeManager);
    

    // Temps s'écoulant entre chaque frame
    float dt = 0.f;

	bool done = false;
    while(!done) {
        wm.startMainLoop();

        // Renderer
        renderer.clear();

        foodManager.drawParticles(renderer);

        snakeManager.drawParticles(renderer);
        //snakeManager.drawParticleGraph(particleGraph, renderer);

        // Forces
        graphHook.setGraph(&particleGraph);
        graphBrake.setGraph(&particleGraph);
        

        // Simulation
        if(dt != 0) {

            // Collision
            if(isCollision(snakeManager)) {
                std::cout << "YOU LOST ! " << std::endl;
                snakeManager.clear();
                return EXIT_SUCCESS;
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
