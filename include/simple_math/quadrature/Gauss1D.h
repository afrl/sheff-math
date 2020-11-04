#pragma once

#include "Quadrature.h"
#include "QuadratureDefines.h"
#include "utility/Exceptions.h"

#ifndef _USE_MATH_DEFINES
#define _USE_MATH_DEFINES
#endif
#include <math.h>

namespace simple_math
{
template<QuadratureOrder N>
class Gauss1D final
{
  public:
    /// @brief Constructs static memory for 1D Gauss quadrature with the given number of points.
    ///
    /// @template N - equivalent to the number of Gauss points
    ///
    /// Note: thread-safe for compilers that fully support C++11 with thread-safe function-local
    /// static initializers (or "magic statics").
    static const Gauss1D<N>& get_singleton()
    {
        static Gauss1D<N> singleton;
        return singleton;
    }

    inline const double* xis() const
    {
        return xis_;
    }
    inline const double* weights() const
    {
        return weights_;
    }
    inline QuadratureIndex n_points() const
    {
        return N;
    }

    /// @brief Returns a reference to the quadrature that is convenient to pass around.
    Quadrature get() const
    {
        return {
            QuadratureType::Gauss1D, N, false, n_points(), xis(), nullptr, nullptr, weights()
        };
    }

  private:
    Gauss1D();
    ~Gauss1D();

    Gauss1D(const Gauss1D&) = delete;
    Gauss1D(Gauss1D&&)      = delete;

    double* xis_;
    double* weights_;
};

//////////////////////////////////////////////////////////////////////////////////////////////////
// Template defintions

template<QuadratureOrder N>
Gauss1D<N>::Gauss1D()
{
    const double EPS = 3e-15;

    xis_      = new double[N];
    weights_  = new double[N];
    int m     = (N + 1) / 2;
    double xm = 0;
    double xl = 1;
    for (int i = 0; i < m; i++)
    {
        double z = cos(M_PI * ((double)i + 1 - .25) / ((double)N + .5));
        double z1;
        double PP;
        do
        {
            double P1 = 1;
            double P2 = 0;
            for (unsigned int j = 0; j < N; j++)
            {
                double P3 = P2;
                P2        = P1;
                P1        = ((2 * j + 1) * z * P2 - (j)*P3) / (j + 1);
            }
            PP = N * (z * P1 - P2) / (z * z - 1);
            z1 = z;
            z  = z1 - P1 / PP;
        } while (fabs(z - z1) > EPS);
        xis_[i]             = xm - xl * z;
        xis_[N - i - 1]     = xm + xl * z;
        weights_[i]         = 2 * xl / ((1 - z * z) * PP * PP);
        weights_[N - i - 1] = weights_[i];
    }
}

template<QuadratureOrder N>
Gauss1D<N>::~Gauss1D()
{
    delete[] xis_;
    delete[] weights_;
}
}