#ifndef _IMAC3_FORCE_HPP
#define _IMAC3_FORCE_HPP

#include "ParticleManager.hpp"

namespace imac3 {

class Force {

	public:

		virtual ~Force() {};

		virtual void apply(ParticleManager& pm) = 0;

	private:

};

}

#endif // _IMAC3_FORCE_HPP