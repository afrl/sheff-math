#pragma once

#include "Gauss1D.h"

namespace simple_math
{
template<QuadratureOrder N>
class NewtonCotes3D final
{
  public:
    /// @brief Constructs static memory for 3D Newton-Cotes quadrature.
    ///
    /// @template N - number of points on each axis, resulting in N^3 points
    ///
    /// Note: thread-safe for compilers that fully support C++11 with thread-safe function-local
    /// static initializers (or "magic statics").
    static const NewtonCotes3D<N>& get_singleton()
    {
        static NewtonCotes3D<N> singleton;
        return singleton;
    }

    inline const double* xis() const
    {
        return xis_;
    }
    inline const double* etas() const
    {
        return etas_;
    }
    inline const double* zetas() const
    {
        return zetas_;
    }
    inline const double* weights() const
    {
        return weights_;
    }
    inline QuadratureIndex n_points() const
    {
        return N * N;
    }

    /// @brief Returns a reference to the quadrature that is convenient to pass around.
    Quadrature get() const
    {
        return { QuadratureType::NewtonCotes3D, N, false, n_points(), xis(), etas(), zetas(), weights() };
    }

  private:
    NewtonCotes3D();
    ~NewtonCotes3D();

    NewtonCotes3D(const NewtonCotes3D&) = delete;
    NewtonCotes3D(NewtonCotes3D&&)      = delete;

    double* xis_;
    double* etas_;
    double* zetas_;
    double* weights_;
};

//////////////////////////////////////////////////////////////////////////////////////////////////
// Template defintions

template<QuadratureOrder N>
NewtonCotes3D<N>::NewtonCotes3D()
{
    const double* gauss_abscissae = Gauss1D<N>::get_singleton().xis();
    const double* gauss_weights   = Gauss1D<N>::get_singleton().weights();

    const auto length = N * N * N;
    xis_              = new double[length];
    etas_             = new double[length];
    zetas_            = new double[length];
    weights_          = new double[length];

    QuadratureIndex quad_index = 0;
    for (auto i = 0; i < N; i++)
    {
        for (auto j = 0; j < N; j++)
        {
            for (auto k = 0; k < N; k++)
            {
                xis_[quad_index]     = gauss_abscissae[i];
                etas_[quad_index]    = gauss_abscissae[j];
                zetas_[quad_index]   = gauss_abscissae[k];
                weights_[quad_index] = gauss_weights[i] * gauss_weights[j] * gauss_weights[k];
                quad_index++;
            }
        }
    }
}

template<QuadratureOrder N>
NewtonCotes3D<N>::~NewtonCotes3D()
{
    delete[] xis_;
    delete[] etas_;
    delete[] zetas_;
    delete[] weights_;
}
}