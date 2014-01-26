#include "WindowManager.hpp"

#include <GL/glew.h>
#include <stdexcept>

namespace imac3 {

WindowManager::WindowManager(uint32_t w, uint32_t h, const char *title) {
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

void WindowManager::loadText() {

    if(!(font = TTF_OpenFont("velvenda chill.ttf", 200))) {
        throw std::runtime_error(TTF_GetError());
    }  
    white = {1, 1, 1};
    text = TTF_RenderText_Solid(font, "Zipix !", white);
    TTF_CloseFont(font);

    Uint8 colors = text->format->BytesPerPixel;
    int texture_format;
    if (colors == 4) {   // alpha
        if (text->format->Rmask == 0x000000ff)
            texture_format = GL_RGBA;
        else
            texture_format = GL_BGRA;
    } else {             // no alpha
        if (text->format->Rmask == 0x000000ff)
            texture_format = GL_RGB;
        else
            texture_format = GL_BGR;
    }

    int x = text->w;
    int y = text->h;

    // Load into a GL texture
    glGenTextures(1, &textureText);
    glBindTexture(GL_TEXTURE_2D, textureText);
    // glTexImage2D(GL_TEXTURE_2D, 0, colors, x, y, 0, texture_format, GL_UNSIGNED_BYTE, text->pixels);
    // glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    // glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    // glBindTexture(GL_TEXTURE_2D, 0);
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

}
