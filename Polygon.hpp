#ifndef _IMAC3_POLYGON_HPP
#define _IMAC3_POLYGON_HPP

#include <vector>
#include <glm/glm.hpp>

#include "renderer/ParticleRenderer2D.hpp"

namespace imac3 {

class Polygon {
public:

	// Builder
	Polygon(std::vector<glm::vec2> pointArray, glm::vec3 color, bool isInner) : m_pointArray(pointArray), m_color(color), m_bIsInner(isInner) {};

	// Getters & Setters
	inline const std::vector<glm::vec2>& getPointArray() const { return m_pointArray;}
	inline const glm::vec2& getPoint(int id) const { return m_pointArray[id];}
	inline const glm::vec3& getColor() const { return m_color;}
	inline bool isInner() const { return m_bIsInner;}

	inline int getCount() const { return getPointArray().size();}

	// Draw
	void draw(ParticleRenderer2D& renderer, float lineWidth = 1.f) const;


private:
	std::vector<glm::vec2>	m_pointArray;
	glm::vec3				m_color;
	bool					m_bIsInner;
};

// Build a box
Polygon buildBox(glm::vec3 color, glm::vec2 position, float width, float height, bool isInner = false);

// Build a circle
Polygon buildCircle(glm::vec3 color, glm::vec2 center, float radius, size_t discFactor, bool isInner = false);

// Check intersections
bool intersect(const glm::vec2& p1, const glm::vec2& p2, const glm::vec2& A, const glm::vec2& B, glm::vec2* intersection, glm::vec2* normal) ;

}

#endif // _IMAC3_POLYGON_HPP