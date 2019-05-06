#pragma once

#include "Gauss.h"

namespace simple_math
{
	/// A helper class that uses the Gauss class to create a list of
	/// locations and weights for each quad point.  In the case of 2D
	/// this would create a list of integrationOrder^2 long.
	class Gauss2D
	{
	public:

        Gauss2D();
        virtual ~Gauss2D();

        virtual void setup(const unsigned int integration_order, Gauss & gauss);
		const double* get_xis(const unsigned int &integration_order) { return xis[integration_order - 1]; }
        const double* get_etas(const unsigned int &integration_order) { return etas[integration_order - 1]; }
        const double* get_weights(const unsigned int &integration_order) { return weights[integration_order - 1]; }

	protected:
		double * xis[MAX_GAUSS_ORDER];
		double * etas[MAX_GAUSS_ORDER];
		double * weights[MAX_GAUSS_ORDER];
	};
}