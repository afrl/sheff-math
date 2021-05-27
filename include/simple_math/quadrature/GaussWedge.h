#pragma once

#include "Quadrature.h"
#include "QuadratureDefines.h"
#include "utility/Exceptions.h"

namespace simple_math
{
template<QuadratureOrder N>
class GaussWedge final
{
  public:
    /// @brief Constructs static memory for 3D Gauss quadrature specialized for wedges.
    ///
    /// @template N - order of integration (1 results in 2 Gauss points, 2 results in 9 Gauss
    /// points, 3 results in 18 Gauss points)
    ///
    /// Note: thread-safe for compilers that fully support C++11 with thread-safe function-local
    /// static initializers (or "magic statics").
    static const GaussWedge<N>& get_singleton()
    {
        static GaussWedge<N> singleton;
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
    QuadratureIndex n_points() const;

    /// @brief Returns a reference to the quadrature that is convenient to pass around.
    Quadrature get() const
    {
        return {
            QuadratureType::Gauss3DWedge, N, false, n_points(), xis(), etas(), zetas(), weights()
        };
    }

  private:
    GaussWedge();
    ~GaussWedge();

    GaussWedge(const GaussWedge&) = delete;
    GaussWedge(GaussWedge&&)      = delete;

    double* xis_;
    double* etas_;
    double* zetas_;
    double* weights_;
};

//////////////////////////////////////////////////////////////////////////////////////////////////
// Template defintions

template<QuadratureOrder N>
GaussWedge<N>::~GaussWedge()
{
    delete[] xis_;
    delete[] etas_;
    delete[] zetas_;
    delete[] weights_;
}

///////////////////////////////////////////////////////////////////////////////////////////////////
// Fall back for other values of N not implemented

template<QuadratureOrder N>
inline QuadratureIndex GaussWedge<N>::n_points() const
{
    throw _UTIL_MSG_EXCEPT_C("Integration not implemented for N = " + std::to_string(N));
}

template<QuadratureOrder N>
inline GaussWedge<N>::GaussWedge()
{
    throw _UTIL_MSG_EXCEPT_C("Integration not implemented for N = " + std::to_string(N));
}

//////////////////////////////////////////////////////////////////////////////////////////////////
// 2-point rule (N=1)

template<>
inline QuadratureIndex GaussWedge<1>::n_points() const
{
    return 2;
}

template<>
inline GaussWedge<1>::GaussWedge()
{
    xis_     = new double[n_points()];
    etas_    = new double[n_points()];
    zetas_    = new double[n_points()];
    weights_ = new double[n_points()];

    xis_[0]     = 1.0 / 3;
    etas_[0]    = 1.0 / 3;
    zetas_[0]   = 1.0 / sqrt(3.0);
    weights_[0] = 1.0 / 2;

    xis_[1]     = 1.0 / 3;
    etas_[1]    = 1.0 / 3;
    zetas_[1]   = -1.0 / sqrt(3.0);
    weights_[1] = 1.0 / 2;
}

//////////////////////////////////////////////////////////////////////////////////////////////////
// 9-point rule (N=2)

template<>
inline QuadratureIndex GaussWedge<2>::n_points() const
{
    return 9;
}

template<>
inline GaussWedge<2>::GaussWedge()
{
    xis_     = new double[n_points()];
    etas_    = new double[n_points()];
    zetas_   = new double[n_points()];
    weights_ = new double[n_points()];

    auto val1       = 1.0 / 6;
    auto val2       = 4.0 / 6;
    auto val3       = sqrt(3.0 / 5);
    auto weight_val = 5.0 / 54;

    xis_[0]     = val1;
    etas_[0]    = val1;
    zetas_[0]   = val3;
    weights_[0] = weight_val;

    xis_[1]     = val1;
    etas_[1]    = val2;
    zetas_[1]   = val3;
    weights_[1] = weight_val;

    xis_[2]     = val2;
    etas_[2]    = val1;
    zetas_[2]   = val3;
    weights_[2] = weight_val;

    xis_[3]     = val1;
    etas_[3]    = val1;
    zetas_[3]   = -val3;
    weights_[3] = weight_val;

    xis_[4]     = val1;
    etas_[4]    = val2;
    zetas_[4]   = -val3;
    weights_[4] = weight_val;

    xis_[5]     = val2;
    etas_[5]    = val1;
    zetas_[5]   = -val3;
    weights_[5] = weight_val;

    val3       = 0.0;
    weight_val = 8.0 / 54;

    xis_[6]     = val1;
    etas_[6]    = val1;
    zetas_[6]   = val3;
    weights_[6] = weight_val;

    xis_[7]     = val1;
    etas_[7]    = val2;
    zetas_[7]   = val3;
    weights_[7] = weight_val;

    xis_[8]     = val2;
    etas_[8]    = val1;
    zetas_[8]   = val3;
    weights_[8] = weight_val;
}

//////////////////////////////////////////////////////////////////////////////////////////////////
// 18-point rule (N=3)

template<>
inline QuadratureIndex GaussWedge<3>::n_points() const
{
    return 18;
}

template<>
inline GaussWedge<3>::GaussWedge()
{
    xis_     = new double[n_points()];
    etas_    = new double[n_points()];
    zetas_    = new double[n_points()];
    weights_ = new double[n_points()];

    auto val1       = 1.0 / 6;
    auto val2       = 4.0 / 6;
    auto val3       = sqrt(3.0 / 5);
    auto weight_val = 1.0 / 12;

    xis_[0]     = val1;
    etas_[0]    = val1;
    zetas_[0]   = val3;
    weights_[0] = weight_val;

    xis_[1]     = val1;
    etas_[1]    = val2;
    zetas_[1]   = val3;
    weights_[1] = weight_val;

    xis_[2]     = val2;
    etas_[2]    = val1;
    zetas_[2]   = val3;
    weights_[2] = weight_val;

    xis_[3]     = val1;
    etas_[3]    = val1;
    zetas_[3]   = -val3;
    weights_[3] = weight_val;

    xis_[4]     = val1;
    etas_[4]    = val2;
    zetas_[4]   = -val3;
    weights_[4] = weight_val;

    xis_[5]     = val2;
    etas_[5]    = val1;
    zetas_[5]   = -val3;
    weights_[5] = weight_val;

    val3       = 0.0;
    weight_val = 2.0 / 15;

    xis_[6]     = val1;
    etas_[6]    = val1;
    zetas_[6]   = val3;
    weights_[6] = weight_val;

    xis_[7]     = val1;
    etas_[7]    = val2;
    zetas_[7]   = val3;
    weights_[7] = weight_val;

    xis_[8]     = val2;
    etas_[8]    = val1;
    zetas_[8]   = val3;
    weights_[8] = weight_val;

    val1       = 1.0 / 2;
    val2       = 0.0;
    val3       = sqrt(3.0 / 5);
    weight_val = 1.0 / 108;

    xis_[9]     = val1;
    etas_[9]    = val1;
    zetas_[9]   = val3;
    weights_[9] = weight_val;

    xis_[10]     = val1;
    etas_[10]    = val2;
    zetas_[10]   = val3;
    weights_[10] = weight_val;

    xis_[11]     = val2;
    etas_[11]    = val1;
    zetas_[11]   = val3;
    weights_[11] = weight_val;

    xis_[12]     = val1;
    etas_[12]    = val1;
    zetas_[12]   = -val3;
    weights_[12] = weight_val;

    xis_[13]     = val1;
    etas_[13]    = val2;
    zetas_[13]   = -val3;
    weights_[13] = weight_val;

    xis_[14]     = val2;
    etas_[14]    = val1;
    zetas_[14]   = -val3;
    weights_[14] = weight_val;

    val3       = 0.0;
    weight_val = 2.0 / 135;

    xis_[15]     = val1;
    etas_[15]    = val1;
    zetas_[15]   = val3;
    weights_[15] = weight_val;

    xis_[16]     = val1;
    etas_[16]    = val2;
    zetas_[16]   = val3;
    weights_[16] = weight_val;

    xis_[17]     = val2;
    etas_[17]    = val1;
    zetas_[17]   = val3;
    weights_[17] = weight_val;
}

}