#include "simple_math/quadrature/NewtonCotes3D.h"

namespace simple_math
{
    NewtonCotes3D::NewtonCotes3D()
        : NewtonCotes2D()
    {
        for (unsigned int i = 0; i < MAX_NC_ORDER; i++)
        {
            zetas[i] = nullptr;
        }
    }

    NewtonCotes3D::~NewtonCotes3D()
    {
        for (unsigned int i = 0; i < MAX_NC_ORDER; i++)
        {
            if (zetas[i])
            {
                delete[] zetas[i];
            }
        }
    }

    void NewtonCotes3D::setup(unsigned int integration_order)
    {
        if (integration_order > MAX_NC_ORDER || integration_order < 1)
            throw _UTIL_MSG_EXCEPT("Newton Cotes was asked to calculate either too small or too large of an integration order: " + std::to_string(integration_order));

        const unsigned int order_index = integration_order - 1;
        if (xis[order_index])
        {
            return;
        }
        
        // Setup some variables
        const unsigned int length = integration_order * integration_order * integration_order;

        // Allocate the vectors
        xis[order_index] = new double[length];
        etas[order_index] = new double[length];
        zetas[order_index] = new double[length];
        weights[order_index] = new double[length];
        unsigned int quad_index = 0;
        for (unsigned int i = 0; i < integration_order; i++)
        {
            for (unsigned int j = 0; j < integration_order; j++)
            {
                for (unsigned int k = 0; k < integration_order; k++)
                {
                    xis[order_index][quad_index] = get_coord(i, integration_order);
                    etas[order_index][quad_index] = get_coord(j, integration_order);
                    zetas[order_index][quad_index] = get_coord(k, integration_order);
                    weights[order_index][quad_index] = get_weight(i, integration_order)
                        * get_weight(j, integration_order) * get_weight(k, integration_order);
                    quad_index++;
                }
            }
        }
    }
}