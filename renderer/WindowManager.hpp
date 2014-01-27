#ifndef _IMAC3_WINDOWMANAGER_HPP
#define _IMAC3_WINDOWMANAGER_HPP

#include <SDL/SDL.h>
#include <SDL/SDL_ttf.h>

#include "GLtools.hpp"

namespace imac3 {

class WindowManager {
public:
    WindowManager(uint32_t w, uint32_t h, const char* title);

    ~WindowManager();

    WindowManager(const WindowManager&) = delete;

    WindowManager& operator =(const WindowManager&) = delete;

    bool pollEvent(SDL_Event& e) const {
        return SDL_PollEvent(&e);
    }

    void loadText();
    void drawText();

    // A appeler en début de boucle de rendu
    void startMainLoop() {
        m_nStartTime = SDL_GetTicks();
        drawText();
    }

    // Met à jour la fenetre et renvoit le temps écoulé depuis le dernier appel à startMainLoop (en secondes)
    float update();

    void setFramerate(uint32_t fps) {
        m_nFPS = fps;
        m_nFrameDuration = 1000.f / m_nFPS;
    }
private:
    static const GLchar *TEXT_VERTEX_SHADER, *TEXT_FRAGMENT_SHADER;

    uint32_t m_nFPS;
    uint32_t m_nFrameDuration;

    uint32_t m_nStartTime;

    SDL_Surface *window;
    SDL_Surface *text;
    SDL_Rect position;
    SDL_Color white;
    TTF_Font *font;
    
    GLuint textureText;
    GLuint m_TextProgramID;
    GLuint m_uTextureSampler;

};

}

#endif // IMAC3_SMP_WINDOWMANAGER_HPP
