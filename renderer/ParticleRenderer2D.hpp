#ifndef _IMAC3_PARTICLERENDERER2D_HPP
#define _IMAC3_PARTICLERENDERER2D_HPP

#include <GL/glew.h>
#include <glm/glm.hpp>

#include <utility>

namespace imac3 {

class ParticleRenderer2D {
public:
    ParticleRenderer2D(float massScale = 0.05);

    ~ParticleRenderer2D();

    ParticleRenderer2D(const ParticleRenderer2D&) = delete;

    ParticleRenderer2D& operator =(const ParticleRenderer2D&) = delete;

    void clear();

    void drawParticles(uint32_t count,
                       const glm::vec2* positionArray,
                       const float* massArray,
                       const glm::vec3* colorArray);

    void drawPolygon(uint32_t count,
                     const glm::vec2* position,
                     const glm::vec3& color,
                     float lineWidth = 1.f);

    void drawLines(uint32_t lineCount,
                   const std::pair<unsigned int, unsigned int>* lines,
                   uint32_t vertexCount,
                   const glm::vec2* positionArray,
                   const glm::vec3* colorArray,
                   float lineWidth = 1.f);

private:
    static const GLchar *VERTEX_SHADER, *FRAGMENT_SHADER;
    static const GLchar *POLYGON_VERTEX_SHADER, *POLYGON_FRAGMENT_SHADER;
    static const GLchar *LINE_VERTEX_SHADER, *LINE_FRAGMENT_SHADER;

    // Ressources OpenGL
    GLuint m_ProgramID, m_PolygonProgramID, m_LineProgramID;
    GLuint m_VBOID, m_VAOID;

    GLuint m_PolygonVBOID, m_PolygonVAOID;

    GLuint m_LinePositionVBOID, m_LineColorVBOID, m_LineIBOID, m_LineVAOID;

    // Uniform locations
    GLint m_uParticleColor;
    GLint m_uParticlePosition;
    GLint m_uParticleScale;

    GLint m_uPolygonColor;

    // Scale appliqué sur la masse de chaque particule pour obtenir sa taille
    float m_fMassScale;
};

}

#endif // _IMAC3_PARTICLERENDERER2D_HPP
