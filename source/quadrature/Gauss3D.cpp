#include "simple_math/quadrature/Gauss3D.h"

namespace simple_math
{
    Gauss3D::Gauss3D()
        : Gauss2D()
    {
        for (unsigned int i = 0; i < MAX_GAUSS_ORDER; i++)
        {
            zetas[i] = nullptr;
        }
    }

    Gauss3D::~Gauss3D()
    {
        for (unsigned int i = 0; i < MAX_GAUSS_ORDER; i++)
        {
            if (zetas[i])
            {
                delete[] zetas[i];
            }
        }
    }

    void Gauss3D::setup(const unsigned int integration_order, Gauss & gauss)
    {
        const unsigned int order_index = integration_order - 1;
        if (xis[order_index])
        {
            return;
        }

        gauss.setup(integration_order);

        // Setup some variables
        unsigned int length = integration_order * integration_order * integration_order;
        const double * gauss_abscissae = gauss.get_abscissae(integration_order);
        const double * gauss_weights = gauss.get_weights(integration_order);
        // Allocate the vectors
        xis[order_index] = new double[length];
        etas[order_index] = new double[length];
        zetas[order_index] = new double[length];
        weights[order_index] = new double[length];
        // Set the values
        unsigned int quad_index = 0;
        for (unsigned int i = 0; i < integration_order; i++)
        {
            for (unsigned int j = 0; j < integration_order; j++)
            {
                for (unsigned int k = 0; k < integration_order; k++)
                {
                    xis[order_index][quad_index] = gauss_abscissae[i];
                    etas[order_index][quad_index] = gauss_abscissae[j];
                    zetas[order_index][quad_index] = gauss_abscissae[k];
                    weights[order_index][quad_index] = gauss_weights[i] * gauss_weights[j] * gauss_weights[k];
                    quad_index++;
                }
            }
        }
    }
}