#include "simple_math/quadrature/GaussWedge.h"

namespace simple_math
{
    void GaussWedge::setup(const unsigned int integration_order)
    {
        const unsigned int order_index = integration_order - 1;
        if (xi[order_index])
        {
            return;
        }

        // Set the lengths
        switch (integration_order) {
        case 1:
            length[order_index] = 2;
            break;
        case 2:
            length[order_index] = 9;
            break;
        case 3:
            length[order_index] = 18;
            break;
        default:
            throw _UTIL_MSG_EXCEPT_C("Invalid integration order.");
        }

        xi[order_index] = new double[length[order_index]];
        eta[order_index] = new double[length[order_index]];
        zeta[order_index] = new double[length[order_index]];
        weight[order_index] = new double[length[order_index]];

        double val1, val2, val3, weight_val;
        switch (integration_order) {
        case 1:
            xi[order_index][0] = 1.0 / 3;
            eta[order_index][0] = 1.0 / 3;
            zeta[order_index][0] = 1.0 / sqrt(3.0);
            weight[order_index][0] = 1.0 / 2;

            xi[order_index][1] = 1.0 / 3;
            eta[order_index][1] = 1.0 / 3;
            zeta[order_index][1] = -1.0 / sqrt(3.0);
            weight[order_index][1] = 1.0 / 2;
            break;
        case 2:
            val1 = 1.0 / 6;
            val2 = 4.0 / 6;
            val3 = sqrt(3.0 / 5);
            weight_val = 5.0 / 54;

            xi[order_index][0] = val1;
            eta[order_index][0] = val1;
            zeta[order_index][0] = val3;
            weight[order_index][0] = weight_val;

            xi[order_index][1] = val1;
            eta[order_index][1] = val2;
            zeta[order_index][1] = val3;
            weight[order_index][1] = weight_val;

            xi[order_index][2] = val2;
            eta[order_index][2] = val1;
            zeta[order_index][2] = val3;
            weight[order_index][2] = weight_val;

            xi[order_index][3] = val1;
            eta[order_index][3] = val1;
            zeta[order_index][3] = -val3;
            weight[order_index][3] = weight_val;

            xi[order_index][4] = val1;
            eta[order_index][4] = val2;
            zeta[order_index][4] = -val3;
            weight[order_index][4] = weight_val;

            xi[order_index][5] = val2;
            eta[order_index][5] = val1;
            zeta[order_index][5] = -val3;
            weight[order_index][5] = weight_val;

            val3 = 0.0;
            weight_val = 8.0 / 54;

            xi[order_index][6] = val1;
            eta[order_index][6] = val1;
            zeta[order_index][6] = val3;
            weight[order_index][6] = weight_val;

            xi[order_index][7] = val1;
            eta[order_index][7] = val2;
            zeta[order_index][7] = val3;
            weight[order_index][7] = weight_val;

            xi[order_index][8] = val2;
            eta[order_index][8] = val1;
            zeta[order_index][8] = val3;
            weight[order_index][8] = weight_val;
            break;
        case 3:
            val1 = 1.0 / 6;
            val2 = 4.0 / 6;
            val3 = sqrt(3.0 / 5);
            weight_val = 1.0 / 12;

            xi[order_index][0] = val1;
            eta[order_index][0] = val1;
            zeta[order_index][0] = val3;
            weight[order_index][0] = weight_val;

            xi[order_index][1] = val1;
            eta[order_index][1] = val2;
            zeta[order_index][1] = val3;
            weight[order_index][1] = weight_val;

            xi[order_index][2] = val2;
            eta[order_index][2] = val1;
            zeta[order_index][2] = val3;
            weight[order_index][2] = weight_val;

            xi[order_index][3] = val1;
            eta[order_index][3] = val1;
            zeta[order_index][3] = -val3;
            weight[order_index][3] = weight_val;

            xi[order_index][4] = val1;
            eta[order_index][4] = val2;
            zeta[order_index][4] = -val3;
            weight[order_index][4] = weight_val;

            xi[order_index][5] = val2;
            eta[order_index][5] = val1;
            zeta[order_index][5] = -val3;
            weight[order_index][5] = weight_val;

            val3 = 0.0;
            weight_val = 2.0 / 15;

            xi[order_index][6] = val1;
            eta[order_index][6] = val1;
            zeta[order_index][6] = val3;
            weight[order_index][6] = weight_val;

            xi[order_index][7] = val1;
            eta[order_index][7] = val2;
            zeta[order_index][7] = val3;
            weight[order_index][7] = weight_val;

            xi[order_index][8] = val2;
            eta[order_index][8] = val1;
            zeta[order_index][8] = val3;
            weight[order_index][8] = weight_val;

            val1 = 1.0 / 2;
            val2 = 0.0;
            val3 = sqrt(3.0 / 5);
            weight_val = 1.0 / 108;

            xi[order_index][9] = val1;
            eta[order_index][9] = val1;
            zeta[order_index][9] = val3;
            weight[order_index][9] = weight_val;

            xi[order_index][10] = val1;
            eta[order_index][10] = val2;
            zeta[order_index][10] = val3;
            weight[order_index][10] = weight_val;

            xi[order_index][11] = val2;
            eta[order_index][11] = val1;
            zeta[order_index][11] = val3;
            weight[order_index][11] = weight_val;

            xi[order_index][12] = val1;
            eta[order_index][12] = val1;
            zeta[order_index][12] = -val3;
            weight[order_index][12] = weight_val;

            xi[order_index][13] = val1;
            eta[order_index][13] = val2;
            zeta[order_index][13] = -val3;
            weight[order_index][13] = weight_val;

            xi[order_index][14] = val2;
            eta[order_index][14] = val1;
            zeta[order_index][14] = -val3;
            weight[order_index][14] = weight_val;

            val3 = 0.0;
            weight_val = 2.0 / 135;

            xi[order_index][15] = val1;
            eta[order_index][15] = val1;
            zeta[order_index][15] = val3;
            weight[order_index][15] = weight_val;

            xi[order_index][16] = val1;
            eta[order_index][16] = val2;
            zeta[order_index][16] = val3;
            weight[order_index][16] = weight_val;

            xi[order_index][17] = val2;
            eta[order_index][17] = val1;
            zeta[order_index][17] = val3;
            weight[order_index][17] = weight_val;
            break;
        }
    }
}