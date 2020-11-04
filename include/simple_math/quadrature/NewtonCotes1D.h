#pragma once

#include "Quadrature.h"
#include "QuadratureDefines.h"
#include "utility/Exceptions.h"

namespace simple_math
{
template<QuadratureOrder N>
class NewtonCotes1D final
{
  public:
    /// @brief Constructs static memory for 1D Newton-Cotes quadrature with the given number of
    /// points.
    ///
    /// @template N - equivalent to the number of points
    ///
    /// Note: thread-safe for compilers that fully support C++11 with thread-safe function-local
    /// static initializers (or "magic statics").
    static const NewtonCotes1D<N>& get_singleton()
    {
        static NewtonCotes1D<N> singleton;
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
            QuadratureType::NewtonCotes1D, N, false, n_points(), xis(), nullptr, nullptr, weights()
        };
    }

  private:
    NewtonCotes1D();
    ~NewtonCotes1D();

    NewtonCotes1D(const NewtonCotes1D&) = delete;
    NewtonCotes1D(NewtonCotes1D&&)      = delete;

    double* xis_;
    double* weights_;
};

//////////////////////////////////////////////////////////////////////////////////////////////////
// Template defintions

template<QuadratureOrder N>
NewtonCotes1D<N>::~NewtonCotes1D()
{
    delete[] xis_;
    delete[] weights_;
}

///////////////////////////////////////////////////////////////////////////////////////////////////
// Fall back for other values of N not implemented

template<QuadratureOrder N>
NewtonCotes1D<N>::NewtonCotes1D()
{
    throw _UTIL_MSG_EXCEPT_C("Integration not implemented for N = " + std::to_string(N));
}

//////////////////////////////////////////////////////////////////////////////////////////////////
// 1-point rule (N=1)

template<>
NewtonCotes1D<1>::NewtonCotes1D()
{
    xis_     = new double[1];
    weights_ = new double[1];

    xis_[0]     = 0.0;
    weights_[0] = 2.0;
}

//////////////////////////////////////////////////////////////////////////////////////////////////
// 2-point rule (N=2)

template<>
NewtonCotes1D<2>::NewtonCotes1D()
{
    const auto N = 2;
    xis_         = new double[N];
    weights_     = new double[N];

    for (auto i = 0; i < N; i++)
    {
        xis_[i] = -1.0 + i * (2.0 / (N - 1));
    }
    weights_[0] = 1.0;
    weights_[1] = 1.0;
}

//////////////////////////////////////////////////////////////////////////////////////////////////
// 3-point rule (N=3)

template<>
NewtonCotes1D<3>::NewtonCotes1D()
{
    const auto N = 3;
    xis_         = new double[N];
    weights_     = new double[N];

    for (auto i = 0; i < N; i++)
    {
        xis_[i] = -1.0 + i * (2.0 / (N - 1));
    }
    weights_[0] = 1.0 / 3.0;
    weights_[1] = 4.0 / 3.0;
    weights_[2] = 1.0 / 3.0;
}

//////////////////////////////////////////////////////////////////////////////////////////////////
// 4-point rule (N=4)

template<>
NewtonCotes1D<4>::NewtonCotes1D()
{
    const auto N = 4;
    xis_         = new double[N];
    weights_     = new double[N];

    for (auto i = 0; i < N; i++)
    {
        xis_[i] = -1.0 + i * (2.0 / (N - 1));
    }
    weights_[0] = 0.25;
    weights_[1] = 0.75;
    weights_[2] = 0.75;
    weights_[3] = 0.25;
}

//////////////////////////////////////////////////////////////////////////////////////////////////
// 5-point rule (N=5)

template<>
NewtonCotes1D<5>::NewtonCotes1D()
{
    const auto N = 5;
    xis_         = new double[N];
    weights_     = new double[N];

    for (auto i = 0; i < N; i++)
    {
        xis_[i] = -1.0 + i * (2.0 / (N - 1));
    }
    weights_[0] = 7.0 / 45.0;
    weights_[1] = 32.0 / 45.0;
    weights_[2] = 4.0 / 15.0;
    weights_[3] = 32.0 / 45.0;
    weights_[4] = 7.0 / 45.0;
}
}