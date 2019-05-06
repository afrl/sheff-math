#include "simple_math/quadrature/NewtonCotes1D.h"

namespace simple_math
{
    NewtonCotes1D::NewtonCotes1D()
    {
        for (unsigned int i = 0; i < MAX_NC_ORDER; i++)
        {
            xis[i] = nullptr;
            weights[i] = nullptr;
        }
    }

    NewtonCotes1D::~NewtonCotes1D()
    {
        for (unsigned int i = 0; i < MAX_NC_ORDER; i++)
        {
            if (xis[i])
            {
                delete[] xis[i];
            }
            if (weights[i])
            {
                delete[] weights[i];
            }
        }
    }

    void NewtonCotes1D::setup(unsigned int integration_order)
    {
        if (integration_order > MAX_NC_ORDER || integration_order < 1)
            throw _UTIL_MSG_EXCEPT("Newton Cotes was asked to calculate either too small or too large of an integration order: " + std::to_string(integration_order));

        const unsigned int order_index = integration_order - 1;
        if (xis[order_index])
        {
            return;
        }
        
        // Setup some variables
        const unsigned int length = integration_order;

        // Allocate the vectors
        xis[order_index] = new double[length];
        weights[order_index] = new double[length];
        unsigned int quad_index = 0;
        for (unsigned int i = 0; i < integration_order; i++)
        {
            xis[order_index][quad_index] = get_coord(i, integration_order);
            weights[order_index][quad_index] = get_weight(i, integration_order);
            quad_index++;
        }
    }

    double NewtonCotes1D::get_coord(const unsigned int & k, const unsigned int & integration_order) const
    {
        if (k > integration_order) throw _UTIL_MSG_EXCEPT_C("Invalid integration order.");
        if (integration_order == 1) return 0.0; //Single point, midpoint.
        return -1.0 + k * (2.0 / (integration_order - 1)); //Return the kth point of N=Order points evenly spaced on interval [-1,1]
    }

    double NewtonCotes1D::get_weight(const unsigned int & k, const unsigned int & integration_order) const
    {
        switch (integration_order) {
        case 1:
            return 2.0;
            break;
        case 2:
            return 1.0;
            break;
        case 3:
            if (k == 1) return 4.0 / 3.0;
            else return 1.0 / 3.0;
            break;
        case 4:
            if (k == 1 || k == 2) return 0.75;
            else return 0.25;
            break;
        case 5:
            if (k == 2) return 4.0 / 15.0;
            else if (k == 1 || k == 3) return 32.0 / 45.0;
            else return 7.0 / 45.0;
            break;
        default:
            throw _UTIL_MSG_EXCEPT_C("Invalid integration order.");
        }
    }
}