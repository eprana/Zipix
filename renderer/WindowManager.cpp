#include "WindowManager.hpp"

#include <GL/glew.h>
#include <stdexcept>

namespace imac3 {

// const GLchar* WindowManager::TEXT_VERTEX_SHADER =
// "#version 330 core\n"
// GL_STRINGIFY(
//     in vec3 vert;
//     in vec4 color;
//     in vec2 texcoord;
     
//     out vec4 fragColor;
//     out vec2 texCoord;
     
//     void main() {
//         fragColor = color;
//         gl_Position = vec4(vert, 1);
//         texCoord = texcoord;
//     }
// );

// const GLchar* WindowManager::TEXT_FRAGMENT_SHADER =
// "#version 330 core\n"
// GL_STRINGIFY(
//     in vec2 texCoord;
//     in vec4 fragColor;
     
//     out vec3 finalColor;
     
//     uniform sampler2D uTextureSampler;
//     void main() {
//         finalColor = texture( uTextureSampler, texCoord ).rgb;
//     }
// );

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

    //loadText();
    
    GLenum error = glewInit();
    if(error != GLEW_OK) {
        throw std::runtime_error("Unable to init GLEW: " + std::string((const char*) glewGetErrorString(error)));
    }
}

WindowManager::~WindowManager() {
    SDL_Quit();
}

void WindowManager::loadText() {

    if(!(font = TTF_OpenFont("albas.TTF", 200))) {
        throw std::runtime_error(TTF_GetError());
    }  
    white = {1, 1, 1};
    text = TTF_RenderUTF8_Blended(font, "Zipix !", white);
    TTF_CloseFont(font);

    Uint8 colors = text->format->BytesPerPixel;
    int texture_format;
    // Alpha
    if (colors == 4) {
        if (text->format->Rmask == 0x000000ff)
            texture_format = GL_RGBA;
        else
            texture_format = GL_BGRA;
    // No alpha
    } else {
        if (text->format->Rmask == 0x000000ff)
            texture_format = GL_RGB;
        else
            texture_format = GL_BGR;
    }

    glGenTextures(1, &textureText);
    //glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, textureText);
        glTexImage2D(GL_TEXTURE_2D, 0, colors, text->w, text->h, 0, texture_format, GL_UNSIGNED_BYTE, text->pixels);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glBindTexture(GL_TEXTURE_2D, 0);
    // SDL_FreeSurface(text);

    //m_TextProgramID = buildProgram(TEXT_VERTEX_SHADER, TEXT_FRAGMENT_SHADER);
    // glUseProgram(m_TextProgramID);
    // m_uTextureSampler = glGetUniformLocation(m_TextProgramID, "uTextureSampler");

}

void WindowManager::drawText() {
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, textureText);
    glUseProgram(m_TextProgramID);
    glUniform1i(m_uTextureSampler, 0);
    // SDL_GL_SwapBuffers();
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
