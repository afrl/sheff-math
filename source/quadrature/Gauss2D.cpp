#include "simple_math/quadrature/Gauss2D.h"

namespace simple_math
{
    Gauss2D::Gauss2D()
    {
        for (unsigned int i = 0; i < MAX_GAUSS_ORDER; i++)
        {
            xis[i] = nullptr;
            etas[i] = nullptr;
            weights[i] = nullptr;
        }
    }

    Gauss2D::~Gauss2D()
    {
        for (unsigned int i = 0; i < MAX_GAUSS_ORDER; i++)
        {
            if (xis[i])
            {
                delete[] xis[i];
            }
            if (etas[i])
            {
                delete[] etas[i];
            }
            if (weights[i])
            {
                delete[] weights[i];
            }
        }
    }

    void Gauss2D::setup(unsigned int integration_order, Gauss & gauss)
    {
        const unsigned int order_index = integration_order - 1;
        if (xis[order_index])
        {
            return;
        }

        gauss.setup(integration_order);

        // Setup some variables
        unsigned int length = integration_order * integration_order;
        const double * gauss_abscissae = gauss.get_abscissae(integration_order);
        const double * gauss_weights = gauss.get_weights(integration_order);
        // Allocate the vectors
        xis[order_index] = new double[length];
        etas[order_index] = new double[length];
        weights[order_index] = new double[length];
        // Set the values
        unsigned int quad_index = 0;
        for (unsigned int i = 0; i < integration_order; i++)
        {
            for (unsigned int j = 0; j < integration_order; j++)
            {
                xis[order_index][quad_index] = gauss_abscissae[i];
                etas[order_index][quad_index] = gauss_abscissae[j];
                weights[order_index][quad_index] = gauss_weights[i] * gauss_weights[j];
                quad_index++;
            }
        }
    }
}