#include "simple_math/quadrature/GaussTetrahedron.h"

#include <math.h>

namespace simple_math
{
    GaussTetrahedron::GaussTetrahedron() : GaussTriangle()
    {
        for (unsigned int i = 0; i < MAX_GAUSS_TRI_ORDER; i++)
        {
            zeta[i] = nullptr;
        }
    }

    GaussTetrahedron::~GaussTetrahedron()
    {
        for (unsigned int i = 0; i < MAX_GAUSS_TRI_ORDER; i++)
        {
            if (zeta[i])
            {
                delete[] zeta[i];
            }
        }
    };

    void GaussTetrahedron::setup(const unsigned int integration_order)
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
            length[order_index] = 4;
            break;
        case 3:
            length[order_index] = 15;
            break;
        default:
            throw _UTIL_MSG_EXCEPT_C("Invalid integration order.");
        }

        xi[order_index] = new double[length[order_index]];
        eta[order_index] = new double[length[order_index]];
        zeta[order_index] = new double[length[order_index]];
        weight[order_index] = new double[length[order_index]];

        double val1, val2, weightVal;
        switch (integration_order) {
        case 1:
            xi[order_index][0] = 0.25;
            eta[order_index][0] = 0.25;
            zeta[order_index][0] = 0.25;
            weight[order_index][0] = 1.0 / 6;
            break;
        case 2:
            val1 = (5.0 - sqrt(5.0)) / 20;
            val2 = (5.0 + 3 * sqrt(5.0)) / 20;
            xi[order_index][0] = val1;
            eta[order_index][0] = val1;
            zeta[order_index][0] = val1;
            weight[order_index][0] = 1.0 / 24;

            xi[order_index][1] = val2;
            eta[order_index][1] = val1;
            zeta[order_index][1] = val1;
            weight[order_index][1] = 1.0 / 24;

            xi[order_index][2] = val1;
            eta[order_index][2] = val2;
            zeta[order_index][2] = val1;
            weight[order_index][2] = 1.0 / 24;

            xi[order_index][3] = val1;
            eta[order_index][3] = val1;
            zeta[order_index][3] = val2;
            weight[order_index][3] = 1.0 / 24;
            break;
        case 3:
            xi[order_index][0] = 0.25;
            eta[order_index][0] = 0.25;
            zeta[order_index][0] = 0.25;
            weight[order_index][0] = 16.0 / 810;

            val1 = (7.0 - sqrt(15.0)) / 34;
            val2 = (13.0 + 3.0*sqrt(15.0)) / 34;
            weightVal = (2665.0 + 14.0*sqrt(15.0)) / 226800;

            xi[order_index][1] = val1;
            eta[order_index][1] = val1;
            zeta[order_index][1] = val1;
            weight[order_index][1] = weightVal;

            xi[order_index][2] = val2;
            eta[order_index][2] = val1;
            zeta[order_index][2] = val1;
            weight[order_index][2] = weightVal;

            xi[order_index][3] = val1;
            eta[order_index][3] = val2;
            zeta[order_index][3] = val1;
            weight[order_index][3] = weightVal;

            xi[order_index][4] = val1;
            eta[order_index][4] = val1;
            zeta[order_index][4] = val2;
            weight[order_index][4] = weightVal;

            val1 = (7.0 + sqrt(15.0)) / 34;
            val2 = (13.0 - 3.0*sqrt(15.0)) / 34;
            weightVal = (2665.0 - 14.0*sqrt(15.0)) / 226800;

            xi[order_index][5] = val1;
            eta[order_index][5] = val1;
            zeta[order_index][5] = val1;
            weight[order_index][5] = weightVal;

            xi[order_index][6] = val2;
            eta[order_index][6] = val1;
            zeta[order_index][6] = val1;
            weight[order_index][6] = weightVal;

            xi[order_index][7] = val1;
            eta[order_index][7] = val2;
            zeta[order_index][7] = val1;
            weight[order_index][7] = weightVal;

            xi[order_index][8] = val1;
            eta[order_index][8] = val1;
            zeta[order_index][8] = val2;
            weight[order_index][8] = weightVal;

            val1 = (10.0 - 2.0*sqrt(15.0)) / 40;
            val2 = (10.0 + 2.0*sqrt(15.0)) / 40;
            weightVal = 20.0 / 2268;

            xi[order_index][9] = val2;
            eta[order_index][9] = val1;
            zeta[order_index][9] = val1;
            weight[order_index][9] = weightVal;

            xi[order_index][10] = val1;
            eta[order_index][10] = val2;
            zeta[order_index][10] = val1;
            weight[order_index][10] = weightVal;

            xi[order_index][11] = val1;
            eta[order_index][11] = val1;
            zeta[order_index][11] = val2;
            weight[order_index][11] = weightVal;

            xi[order_index][12] = val1;
            eta[order_index][12] = val2;
            zeta[order_index][12] = val2;
            weight[order_index][12] = weightVal;

            xi[order_index][13] = val2;
            eta[order_index][13] = val1;
            zeta[order_index][13] = val2;
            weight[order_index][13] = weightVal;

            xi[order_index][14] = val2;
            eta[order_index][14] = val2;
            zeta[order_index][14] = val1;
            weight[order_index][14] = weightVal;
            break;
        }
    }
}
