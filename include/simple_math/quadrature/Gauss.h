#pragma once

#include "utility/Exceptions.h"

#ifndef _USE_MATH_DEFINES
#define _USE_MATH_DEFINES
#endif
#include <math.h>

namespace simple_math
{
	static const unsigned int MAX_GAUSS_ORDER = 5;

	class Gauss
	{
	public:
		/// Default constructor just iniailizes the arrays of pointers to be null
        Gauss();
		/// Destructor cleans up any memory allocated
        ~Gauss();

        void setup(const unsigned int integration_order);
		/// Get a pointer to the abscissae.  Make a copy of it or use, but
		/// don't try to modify it outside this class.
		const double * get_abscissae(const unsigned int integration_order) { return abscissae[integration_order - 1]; }
		/// Get a pointer to the weights.  Make a copy of it or use, but
		/// don't try to modify it outside this class.
		const double * get_weights(const unsigned int integration_order) { return weights[integration_order - 1]; }

	private:
		double * abscissae[MAX_GAUSS_ORDER];
		double * weights[MAX_GAUSS_ORDER];
	};

}