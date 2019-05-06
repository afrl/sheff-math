#include "simple_math/quadrature/Gauss.h"

namespace simple_math
{
    Gauss::Gauss()
    {
        for (unsigned int i = 0; i < MAX_GAUSS_ORDER; i++)
        {
            abscissae[i] = nullptr;
            weights[i] = nullptr;
        }
    }

    Gauss::~Gauss()
    {
        for (unsigned int i = 0; i < MAX_GAUSS_ORDER; i++)
        {
            if (abscissae[i])
            {
                delete[] abscissae[i];
            }
            if (weights[i])
            {
                delete[] weights[i];
            }
        }
    }

    void Gauss::setup(unsigned int desired_order)
    {
        if (desired_order > MAX_GAUSS_ORDER || desired_order < 1)
            throw _UTIL_MSG_EXCEPT("Gauss was asked to calculate either too small or too large of an integration order: " + std::to_string(desired_order));

        const unsigned int order_index = desired_order - 1;
        if (abscissae[order_index])
        {
            return;
        }
        const double EPS = 3e-15;

        abscissae[order_index] = new double[desired_order];
        weights[order_index] = new double[desired_order];
        int m = (desired_order + 1) / 2;
        double xm = 0;
        double xl = 1;
        for (int i = 0; i < m; i++)
        {
            double z = cos(M_PI*((double)i + 1 - .25) / ((double)desired_order + .5));
            double z1;
            double PP;
            do {
                double P1 = 1;
                double P2 = 0;
                for (unsigned int j = 0; j < desired_order; j++)
                {
                    double P3 = P2;
                    P2 = P1;
                    P1 = ((2 * j + 1)*z*P2 - (j)*P3) / (j + 1);
                }
                PP = desired_order * (z*P1 - P2) / (z*z - 1);
                z1 = z;
                z = z1 - P1 / PP;
            } while (fabs(z - z1) > EPS);
            abscissae[order_index][i] = xm - xl * z;
            abscissae[order_index][desired_order - i - 1] = xm + xl * z;
            weights[order_index][i] = 2 * xl / ((1 - z * z)*PP*PP);
            weights[order_index][desired_order - i - 1] = weights[order_index][i];
        }
    }
}