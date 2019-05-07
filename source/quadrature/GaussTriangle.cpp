#include "simple_math/quadrature/GaussTriangle.h"

namespace simple_math
{
    GaussTriangle::GaussTriangle()
    {
        for(unsigned int i = 0; i < MAX_GAUSS_TRI_ORDER; i++)
        {
            length[i] = 0;
            xi[i] = nullptr;
            eta[i] = nullptr;
            weight[i] = nullptr;
        }
    }

    GaussTriangle::~GaussTriangle()
    {
        for (unsigned int i = 0; i < MAX_GAUSS_TRI_ORDER; i++)
        {
            if (xi[i]) delete[] xi[i];
            if (eta[i]) delete[] eta[i];
            if (weight[i]) delete[] weight[i];
        }
    }

    const unsigned int GaussTriangle::get_n_quads(const unsigned int & integration_order)
    {
        const unsigned int order_index = integration_order - 1;
        return length[order_index];
    }

    void GaussTriangle::setup(const unsigned int integration_order)
    {
        const unsigned int order_index = integration_order - 1;
        if (xi[order_index])
        {
            return;
        }

        // Set the lengths
        switch (integration_order) {
        case 1:
            length[order_index] = 1;
            break;
        case 2:
            length[order_index] = 3;
            break;
        case 3:
            length[order_index] = 4;
            break;
        case 4:
            length[order_index] = 7;
            break;
        default:
            throw _UTIL_MSG_EXCEPT_C("Invalid integration order.");
        }

        xi[order_index] = new double[length[order_index]];
        eta[order_index] = new double[length[order_index]];
        weight[order_index] = new double[length[order_index]];

        double val1, val2, weightVal;
        switch (integration_order) {
        case 1:
            xi[order_index][0] = 1.0 / 3;
            eta[order_index][0] = 1.0 / 3;
            weight[order_index][0] = 0.5;
            break;

        case 2:
            val1 = 0.5;
            val2 = 0.0;
            weightVal = 1.0 / 6;

            xi[order_index][0] = val1;
            eta[order_index][0] = val1;
            weight[order_index][0] = weightVal;

            xi[order_index][1] = val1;
            eta[order_index][1] = val2;
            weight[order_index][1] = weightVal;

            xi[order_index][2] = val2;
            eta[order_index][2] = val1;
            weight[order_index][2] = weightVal;
            break;

        case 3:
            val1 = 0.2;
            val2 = 0.6;
            weightVal = 25.0 / 96;

            xi[order_index][0] = val1;
            eta[order_index][0] = val1;
            weight[order_index][0] = weightVal;

            xi[order_index][1] = val1;
            eta[order_index][1] = val2;
            weight[order_index][1] = weightVal;

            xi[order_index][2] = val2;
            eta[order_index][2] = val1;
            weight[order_index][2] = weightVal;


            xi[order_index][3] = 1.0 / 3;
            eta[order_index][3] = 1.0 / 3;
            weight[order_index][3] = -27.0 / 96;
            break;

        case 4:
            xi[order_index][0] = 1.0 / 3;
            eta[order_index][0] = 1.0 / 3;
            weight[order_index][0] = 0.225*0.5;


            val1 = 0.101286507323;
            val2 = 0.797426985353;
            weightVal = 0.125939180544 * 0.5;

            xi[order_index][1] = val1;
            eta[order_index][1] = val1;
            weight[order_index][1] = weightVal;

            xi[order_index][2] = val1;
            eta[order_index][2] = val2;
            weight[order_index][2] = weightVal;

            xi[order_index][3] = val2;
            eta[order_index][3] = val1;
            weight[order_index][3] = weightVal;


            val1 = 0.470142064105;
            val2 = 0.059715871789;
            weightVal = 0.132394152788 * 0.5;

            xi[order_index][4] = val1;
            eta[order_index][4] = val1;
            weight[order_index][4] = weightVal;

            xi[order_index][5] = val1;
            eta[order_index][5] = val2;
            weight[order_index][5] = weightVal;

            xi[order_index][6] = val2;
            eta[order_index][6] = val1;
            weight[order_index][6] = weightVal;
            break;
        }
    }
}