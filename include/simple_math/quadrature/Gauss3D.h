#pragma once

#include "Gauss2D.h"

namespace simple_math
{
	/// A helper class that uses the Gauss class to create a list of
	/// locations and weights for each quad point.  In the case of 2D
	/// this would create a list of integrationOrder^2 long.
    class Gauss3D : public Gauss2D
    {
    public:

        Gauss3D();
        virtual ~Gauss3D();

        virtual void setup(const unsigned int integration_order, Gauss & gauss) override;
        const double* get_zetas(const unsigned int &integration_order) { return zetas[integration_order - 1]; }

    protected:
        double * zetas[MAX_GAUSS_ORDER];
    };
}