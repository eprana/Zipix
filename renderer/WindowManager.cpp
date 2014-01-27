#include "WindowManager.hpp"

#include <GL/glew.h>
#include <stdexcept>

namespace imac3 {

WindowManager::WindowManager(uint32_t w, uint32_t h, const char *title)/*:
    m_TextProgramID(buildProgram(TEXT_VERTEX_SHADER, TEXT_FRAGMENT_SHADER))*/
{
    setFramerate(30);

    if(-1 == SDL_Init(SDL_INIT_VIDEO)) {
        throw std::runtime_error("Unable to initialize SDL");
    }

    if(!(window = SDL_SetVideoMode(w, h, 32, SDL_OPENGL))) {
        throw std::runtime_error("Unable to open a window");
    }
    SDL_WM_SetCaption(title, 0);

    if(-1 == TTF_Init()){
        throw std::runtime_error("Unable to initialize SDL_ttf");
    }

    loadText();
    
    GLenum error = glewInit();
    if(error != GLEW_OK) {
        throw std::runtime_error("Unable to init GLEW: " + std::string((const char*) glewGetErrorString(error)));
    }
}

WindowManager::~WindowManager() {
    SDL_Quit();
}

float WindowManager::update() {
    SDL_GL_SwapBuffers();

    Uint32 currentTime = SDL_GetTicks();
    Uint32 d = currentTime - m_nStartTime;
    if(d < m_nFrameDuration) {
        SDL_Delay(m_nFrameDuration - d);
    }
    return 0.01f * (SDL_GetTicks() - m_nStartTime);
}

void WindowManager::loadText() { 
    white = {1,1,1};
    font = TTF_OpenFont("albas.TTF", 65);
    text = TTF_RenderText_Blended(font, "Zipix !", white);
}

void WindowManager::drawText() {

    SDL_FillRect(window, NULL, SDL_MapRGB(window->format, 255, 255, 255));
    position.x = 500;
    position.y = 500;
    SDL_BlitSurface(text, NULL, window, &position);
    SDL_Flip(ecran);
}

}
