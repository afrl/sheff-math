#pragma once

#include "simple_math/Defines.h"
#include "simple_math/quadrature/QuadratureDefines.h"
#include "utility/Exceptions.h"

namespace simple_math
{
/// Stores quadrature locations and weights for a scheme that is generated at run-time (likely
/// dependent on XFEM enrichment and element topology).
///
/// One of these objects should be store within any fields where adaptive quadrature is used.  It
/// can also be passed into the QuadratureCache so that the interface of querying quadrature
/// information remains consistent.
class AdaptiveQuadratureData
{
  public:
    AdaptiveQuadratureData()
    {
        n_quad_points_ = 0;
        xis_           = nullptr;
        etas_          = nullptr;
        zetas_         = nullptr;
        weights_       = nullptr;
        scheme_        = simple_math::QuadratureScheme();
    }
    AdaptiveQuadratureData(QuadratureScheme scheme, const QuadratureIndex n_quad_points)
    {
        n_quad_points_ = n_quad_points;
        xis_           = new double[n_quad_points_];
        etas_          = new double[n_quad_points_];
        zetas_         = new double[n_quad_points_];
        weights_       = new double[n_quad_points_];
        scheme_        = scheme;
    }
    AdaptiveQuadratureData(AdaptiveQuadratureData&& other) noexcept
    {
        n_quad_points_ = other.n_quad_points_;
        xis_           = other.xis_;
        etas_          = other.etas_;
        zetas_         = other.zetas_;
        weights_       = other.weights_;
        scheme_        = other.scheme_;

        other.n_quad_points_ = 0;
        other.xis_           = nullptr;
        other.etas_          = nullptr;
        other.zetas_         = nullptr;
        other.weights_       = nullptr;
    }

    AdaptiveQuadratureData(const AdaptiveQuadratureData& other)
    {
        n_quad_points_ = other.n_quad_points_;

        xis_ = new double[n_quad_points_];
        memcpy(xis_, other.xis_, n_quad_points_ * sizeof(double));

        etas_ = new double[n_quad_points_];
        memcpy(etas_, other.etas_, n_quad_points_ * sizeof(double));

        zetas_ = new double[n_quad_points_];
        memcpy(zetas_, other.zetas_, n_quad_points_ * sizeof(double));

        weights_ = new double[n_quad_points_];
        memcpy(weights_, other.weights_, n_quad_points_ * sizeof(double));

        scheme_ = other.scheme_;
    }

    ~AdaptiveQuadratureData()
    {
        if (xis_)
            delete[] xis_;
        if (etas_)
            delete[] etas_;
        if (zetas_)
            delete[] zetas_;
        if (weights_)
            delete[] weights_;
    }

    void operator=(AdaptiveQuadratureData&& other) noexcept
    {
        std::swap(n_quad_points_, other.n_quad_points_);
        std::swap(xis_, other.xis_);
        std::swap(etas_, other.etas_);
        std::swap(zetas_, other.zetas_);
        std::swap(weights_, other.weights_);
        std::swap(scheme_, other.scheme_);
    }

    bool operator==(const AdaptiveQuadratureData& other) const
    {
        if (n_quad_points_ != other.n_quad_points_ || !(scheme_ == other.scheme_))
        {
            return false;
        }
        bool comparison = true;
        for (auto i : utility::range(n_quad_points_)) {
            comparison &= xis_[i] == other.xis_[i];
            comparison &= etas_[i] == other.etas_[i];
            comparison &= zetas_[i] == other.zetas_[i];
            comparison &= weights_[i] == other.weights_[i];
        }
        return comparison;
    }

    inline QuadratureIndex n_quad_points() const
    {
        return n_quad_points_;
    }

    inline void set_quad_point(const unsigned int index,
                               const double xi,
                               const double eta,
                               const double zeta)
    {
        xis_[index]   = xi;
        etas_[index]  = eta;
        zetas_[index] = zeta;
    }

    inline simple_math::Point get_quad_point(const unsigned int index) const
    {
        return { xis_[index], etas_[index], zetas_[index] };
    }

    inline void set_quad_weight(const unsigned int index, const double weight)
    {
        weights_[index] = weight;
    }

    inline double get_quad_weight(const unsigned int index) const
    {
        return weights_[index];
    }

    inline const QuadratureScheme& get_scheme() const
    {
        return scheme_;
    }

    inline void multiply_all_weights(const double factor_to_multiply)
    {
        for (unsigned int i = 0; i < n_quad_points_; i++)
        {
            weights_[i] *= factor_to_multiply;
        }
    }

    /// Performs a simple check to ensure all the weights are in the range [0,1]
    /// and all of the components of each parametic coordinate is in the range [-1,1].
    /// Only checks in debug mode.
    void check()
    {
        for (unsigned int i = 0; i < n_quad_points_; i++)
        {
            _UTIL_ASSERT(xis_[i] >= -1.0 && xis_[i] <= 1.0,
                         "Xi for quad point " + std::to_string(i) +
                           " is outside of bounds with value " + std::to_string(xis_[i]));
            _UTIL_ASSERT(etas_[i] >= -1.0 && etas_[i] <= 1.0,
                         "Eta for quad point " + std::to_string(i) +
                           " is outside of bounds with value " + std::to_string(etas_[i]));
            _UTIL_ASSERT(zetas_[i] >= -1.0 && zetas_[i] <= 1.0,
                         "Zeta for quad point " + std::to_string(i) +
                           " is outside of bounds with value " + std::to_string(zetas_[i]));
            _UTIL_ASSERT(weights_[i] >= 0.0 && weights_[i] <= 1.0,
                         "Weight for quad point " + std::to_string(i) +
                           " is outside of bounds with value " + std::to_string(weights_[i]));
        }
    }

    template<typename SerializerType>
    void serialize(SerializerType& s) const
    {
        s(utility::named_value("n_quad_points", n_quad_points_),
          utility::named_value("xis", utility::slice(xis_, n_quad_points_)),
          utility::named_value("etas", utility::slice(etas_, n_quad_points_)),
          utility::named_value("zetas", utility::slice(zetas_, n_quad_points_)),
          utility::named_value("weights", utility::slice(weights_, n_quad_points_)),
          utility::named_value("scheme", scheme_));
    }

    template<typename SerializerType>
    void deserialize(SerializerType& s)
    {
        s(n_quad_points_);

        xis_ = new double[n_quad_points_];
        s(utility::slice(xis_, n_quad_points_));

        etas_ = new double[n_quad_points_];
        s(utility::slice(etas_, n_quad_points_));

        zetas_ = new double[n_quad_points_];
        s(utility::slice(zetas_, n_quad_points_));

        weights_ = new double[n_quad_points_];
        s(utility::slice(weights_, n_quad_points_));

        s(scheme_);
    }

    friend class Quadrature;

  private:
    QuadratureIndex n_quad_points_;
    double* xis_;
    double* etas_;
    double* zetas_;
    double* weights_;

    QuadratureScheme scheme_;
};
}
