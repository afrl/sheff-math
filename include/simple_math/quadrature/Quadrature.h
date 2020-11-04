#pragma once

#include "AdaptiveQuadratureData.h"
#include "QuadratureDefines.h"

namespace simple_math
{
// Forward declare (definition later in this file)
namespace detail
{
class QuadratureIterator;
}

/// @brief A simple struct that contains references to quadrature parametric coordinates and
/// weights.
///
/// This class does not own the pointers and thus this object is only valid as long as the object
/// pointed to lives.
class Quadrature
{
  public:
    Quadrature() = delete;
    Quadrature(const QuadratureScheme& quadrature_scheme,
               const AdaptiveQuadratureData* adaptive_quadrature);
    Quadrature(const QuadratureType type,
               const QuadratureOrder integration_order,
               const bool adaptive,
               const QuadratureIndex n_quad_points,
               const double* const xi_list,
               const double* const eta_list,
               const double* const zeta_list,
               const double* const weight_list);

    /// Returns the corresponding parametric point for the given quadrture point index.
    /// Note: quad_index must be less than n_quad_points().  No check performed.
    const Point parametric_point(const QuadratureIndex& quad_index) const;

    /// Returns the parametric points in the form a matrix, where the row corresponds to the
    /// respective quadrature point and the column corresponds to the component of the parametric
    /// point.
    const GeneralMatrix parametric_points() const;

    /// Returns the number of quadrature points that exist for the last scheme passed to setup()
    inline const QuadratureIndex& n_quad_points() const
    {
        return n_quad_points_;
    }

    /// Returns the first component of the parametic point for the given quad point.
    inline const double& xi(const QuadratureIndex& quad_index) const
    {
        return get(quad_index, xi_list_);
    }

    /// Returns the second component of the parametic point for the given quad point.
    /// Note: this should only be called for 2D or 3D quadrature schemes.
    inline const double& eta(const QuadratureIndex& quad_index) const
    {
        return get(quad_index, eta_list_);
    }

    /// Returns the third component of the parametic point for the given quad point.
    /// Note: this should only be called for 3D quadrature schemes.
    inline const double& zeta(const QuadratureIndex& quad_index) const
    {
        return get(quad_index, zeta_list_);
    }

    /// Returns the weight of the quad point.
    inline const double& weight(const QuadratureIndex& quad_index) const
    {
        return get(quad_index, weight_list_);
    }

    inline bool is_adaptive() const
    {
        return adaptive_;
    }

    inline QuadratureType type() const
    {
        return type_;
    }

    inline QuadratureOrder order() const
    {
        return integration_order_;
    }

    //////////////////////////////////////////////////////////////////////////////////////////////
    // Support for range-based loops

    detail::QuadratureIterator begin() const;
    const detail::QuadratureIterator end() const;

  private:
    QuadratureType type_;
    QuadratureOrder integration_order_;
    bool adaptive_;
    QuadratureIndex n_quad_points_;
    const double* xi_list_;
    const double* eta_list_;
    const double* zeta_list_;
    const double* weight_list_;

    /// Allows common error checks to be added or removed
    inline const double& get(const QuadratureIndex& quad_index, const double* valArray) const
    {
        _UTIL_ASSERT(
          valArray != nullptr,
          "Tried to get an invalid value. Either Quadrature was not setup or the type is invalid.");
        _UTIL_ASSERT(quad_index < n_quad_points_, "The quad point requested is out of bounds.");
        return valArray[quad_index];
    };
};

namespace detail
{
/// @brief Iterator for Quadrature
class QuadratureIterator
{
  public:
    struct QuadPoint
    {
        QuadratureIndex index;
        double xi;
        double eta;
        double zeta;
        double weight;
    };

    // Iterator traits
    using difference_type   = QuadratureIndex;
    using value_type        = QuadPoint;
    using pointer           = const QuadPoint*;
    using reference         = const QuadPoint&;
    using iterator_category = std::forward_iterator_tag;

    QuadratureIterator(const Quadrature& quadrature, QuadratureIndex curr_index = 0);

    QuadratureIterator& operator++();
    QuadratureIterator operator++(int);
    bool operator==(QuadratureIterator other) const;
    bool operator!=(QuadratureIterator other) const;
    value_type operator*() const;

  private:
    const Quadrature& quadrature_;
    QuadratureIndex curr_index_;
};
}
}