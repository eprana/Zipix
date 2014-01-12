#include "Polygon.hpp"
#include <iostream>



namespace imac3 {

	/* ------------------------------- */
	/* ---------- Functions ---------- */
	/* ------------------------------- */

	bool intersect(const glm::vec2& p1, const glm::vec2& p2, const glm::vec2& A, const glm::vec2& B, glm::vec2* intersection, glm::vec2* normal) {
		// Permet de tricher en allongeant la taille des coté
		static const float epsilon = 0.001;

		if(intersection == NULL && normal == NULL) {
			return false;
		}
		
		// Calcul of k
		glm::vec2 AP1 = p1 - A;
		glm::vec2 P1P2 = p2 - p1;
		glm::vec2 AB = B - A;

		glm::vec2 n = glm::normalize(glm::vec2(-AB.y, AB.x));

		// Si le vecteur P1P2 et le vecteur n on la meme direction, il n'y a pas d'intersection
		if(glm::dot(P1P2, n) >= 0) {
			return false;
		}

		// Calcul du parametre k tel que I = p1 + k x P1P2
		float k = - glm::dot(n, AP1) / glm::dot(n, P1P2);

		// Si k n'est pas dans [0,1], le point d'intersection n'est pas dans le segment [p1,p2]
		if(k < 0.f || k > 1) {
			return false;
		}

		// Point d'intersection
		glm::vec2 M = p1 + k*P1P2;

		float dot = glm::dot(M - A, AB);
		if(dot < -epsilon || dot > glm::dot(AB, AB) + epsilon) {
			return false;
		}

		*intersection = M;
		*normal = n;

		return true;
}

	// Build a box
	Polygon builBox(glm::vec3 color, glm::vec2 position, float width, float height, bool isInner) {
		std::vector<glm::vec2> pointArray;

		// Coin inférieur gauche = position : 
		pointArray.push_back(position);
		pointArray.push_back(position + glm::vec2(width, 0));
		pointArray.push_back(position + glm::vec2(width, height));
		pointArray.push_back(position + glm::vec2(0, height));

		return Polygon(pointArray, color, isInner);
	}
	
	// Build a circle
	Polygon buildCircle(glm::vec3 color, glm::vec2 center, float radius, size_t discFactor, bool isInner) {
		std::vector<glm::vec2> pointArray;

		float delta = 2 * 3.14f / discFactor;
		for(size_t i = 0;  i < discFactor; ++i) {
			float c = cos(i*delta);
			float s = sin(i*delta);
			pointArray.push_back(center + glm::vec2( radius*c, radius*s));
		}

		return Polygon(pointArray, color, isInner);;
	}


	/* ------------------------------- */
	/* ---------- Polygon methods ---------- */
	/* ------------------------------- */
	void Polygon::draw(ParticleRenderer2D& renderer, float lineWidth) const {
		  renderer.drawPolygon(getCount(), &m_pointArray[0], getColor(), lineWidth);
	}

}