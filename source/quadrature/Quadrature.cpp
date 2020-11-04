#include "simple_math/quadrature/Quadrature.h"

#include "simple_math/quadrature/Gauss1D.h"
#include "simple_math/quadrature/Gauss2D.h"
#include "simple_math/quadrature/Gauss3D.h"
#include "simple_math/quadrature/GaussTetrahedron.h"
#include "simple_math/quadrature/GaussTriangle.h"
#include "simple_math/quadrature/GaussWedge.h"
#include "simple_math/quadrature/NewtonCotes1D.h"
#include "simple_math/quadrature/NewtonCotes2D.h"
#include "simple_math/quadrature/NewtonCotes3D.h"

namespace simple_math
{
Quadrature::Quadrature(const QuadratureScheme& quadrature_scheme,
                       const AdaptiveQuadratureData* adaptive_quadrature)
{
    if (!adaptive_quadrature)
    {
        switch (quadrature_scheme.type)
        {
            case QuadratureType::Gauss1D:
                switch (quadrature_scheme.integration_order)
                {
                    case 1: *this = Gauss1D<1>::get_singleton().get(); return;
                    case 2: *this = Gauss1D<2>::get_singleton().get(); return;
                    case 3: *this = Gauss1D<3>::get_singleton().get(); return;
                    case 4: *this = Gauss1D<4>::get_singleton().get(); return;
                    case 5: *this = Gauss1D<5>::get_singleton().get(); return;
                    default:
                        throw _UTIL_MSG_EXCEPT("Switch not implemented for Gauss1D order " +
                                               std::to_string(quadrature_scheme.integration_order));
                }
            case QuadratureType::Gauss2D:
                switch (quadrature_scheme.integration_order)
                {
                    case 1: *this = Gauss2D<1>::get_singleton().get(); return;
                    case 2: *this = Gauss2D<2>::get_singleton().get(); return;
                    case 3: *this = Gauss2D<3>::get_singleton().get(); return;
                    case 4: *this = Gauss2D<4>::get_singleton().get(); return;
                    case 5: *this = Gauss2D<5>::get_singleton().get(); return;
                    default:
                        throw _UTIL_MSG_EXCEPT("Switch not implemented for Gauss2D order " +
                                               std::to_string(quadrature_scheme.integration_order));
                }
            case QuadratureType::Gauss3D:
                switch (quadrature_scheme.integration_order)
                {
                    case 1: *this = Gauss3D<1>::get_singleton().get(); return;
                    case 2: *this = Gauss3D<2>::get_singleton().get(); return;
                    case 3: *this = Gauss3D<3>::get_singleton().get(); return;
                    case 4: *this = Gauss3D<4>::get_singleton().get(); return;
                    case 5: *this = Gauss3D<5>::get_singleton().get(); return;
                    default:
                        throw _UTIL_MSG_EXCEPT("Switch not implemented for Gauss3D order " +
                                               std::to_string(quadrature_scheme.integration_order));
                }
            case QuadratureType::Gauss2DTriangle:
                switch (quadrature_scheme.integration_order)
                {
                    case 1: *this = GaussTriangle<1>::get_singleton().get(); return;
                    case 2: *this = GaussTriangle<2>::get_singleton().get(); return;
                    case 3: *this = GaussTriangle<3>::get_singleton().get(); return;
                    case 4: *this = GaussTriangle<4>::get_singleton().get(); return;
                    default:
                        throw _UTIL_MSG_EXCEPT("Switch not implemented for GaussTriangle order " +
                                               std::to_string(quadrature_scheme.integration_order));
                }
            case QuadratureType::Gauss3DTet:
                switch (quadrature_scheme.integration_order)
                {
                    case 1: *this = GaussTetrahedron<1>::get_singleton().get(); return;
                    case 2: *this = GaussTetrahedron<2>::get_singleton().get(); return;
                    case 3: *this = GaussTetrahedron<3>::get_singleton().get(); return;
                    default:
                        throw _UTIL_MSG_EXCEPT(
                          "Switch not implemented for GaussTetrahedron order " +
                          std::to_string(quadrature_scheme.integration_order));
                }
            case QuadratureType::Gauss3DWedge:
                switch (quadrature_scheme.integration_order)
                {
                    case 1: *this = GaussWedge<1>::get_singleton().get(); return;
                    case 2: *this = GaussWedge<2>::get_singleton().get(); return;
                    case 3: *this = GaussWedge<3>::get_singleton().get(); return;
                    default:
                        throw _UTIL_MSG_EXCEPT("Switch not implemented for GaussWedge order " +
                                               std::to_string(quadrature_scheme.integration_order));
                }
            case QuadratureType::NewtonCotes1D:
                switch (quadrature_scheme.integration_order)
                {
                    case 1: *this = NewtonCotes1D<1>::get_singleton().get(); return;
                    case 2: *this = NewtonCotes1D<2>::get_singleton().get(); return;
                    case 3: *this = NewtonCotes1D<3>::get_singleton().get(); return;
                    case 4: *this = NewtonCotes1D<4>::get_singleton().get(); return;
                    case 5: *this = NewtonCotes1D<5>::get_singleton().get(); return;
                    default:
                        throw _UTIL_MSG_EXCEPT("Switch not implemented for NewtonCotes1D order " +
                                               std::to_string(quadrature_scheme.integration_order));
                }
            case QuadratureType::NewtonCotes2D:
                switch (quadrature_scheme.integration_order)
                {
                    case 1: *this = NewtonCotes2D<1>::get_singleton().get(); return;
                    case 2: *this = NewtonCotes2D<2>::get_singleton().get(); return;
                    case 3: *this = NewtonCotes2D<3>::get_singleton().get(); return;
                    case 4: *this = NewtonCotes2D<4>::get_singleton().get(); return;
                    case 5: *this = NewtonCotes2D<5>::get_singleton().get(); return;
                    default:
                        throw _UTIL_MSG_EXCEPT("Switch not implemented for NewtonCotes2D order " +
                                               std::to_string(quadrature_scheme.integration_order));
                }
            case QuadratureType::NewtonCotes3D:
                switch (quadrature_scheme.integration_order)
                {
                    case 1: *this = NewtonCotes3D<1>::get_singleton().get(); return;
                    case 2: *this = NewtonCotes3D<2>::get_singleton().get(); return;
                    case 3: *this = NewtonCotes3D<3>::get_singleton().get(); return;
                    case 4: *this = NewtonCotes3D<4>::get_singleton().get(); return;
                    case 5: *this = NewtonCotes3D<5>::get_singleton().get(); return;
                    default:
                        throw _UTIL_MSG_EXCEPT("Switch not implemented for NewtonCotes3D order " +
                                               std::to_string(quadrature_scheme.integration_order));
                }
        }
    }
    else
    {
        *this = { quadrature_scheme.type,
                  quadrature_scheme.integration_order,
                  true,
                  adaptive_quadrature->n_quad_points_,
                  adaptive_quadrature->xis_,
                  adaptive_quadrature->etas_,
                  adaptive_quadrature->zetas_,
                  adaptive_quadrature->weights_ };
    }
}

Quadrature::Quadrature(const QuadratureType type,
                             const QuadratureOrder integration_order,
                             const bool adaptive,
                             const QuadratureIndex n_quad_points,
                             const double* const xi_list,
                             const double* const eta_list,
                             const double* const zeta_list,
                             const double* const weight_list)
  : type_(type)
  , integration_order_(integration_order)
  , adaptive_(adaptive)
  , n_quad_points_(n_quad_points)
  , xi_list_(xi_list)
  , eta_list_(eta_list)
  , zeta_list_(zeta_list)
  , weight_list_(weight_list)
{
}

const Point Quadrature::parametric_point(const QuadratureIndex& quad_index) const
{
    if (eta_list_ == nullptr)
        return { xi(quad_index), 0, 0 };
    else if (zeta_list_ == nullptr)
        return { xi(quad_index), eta(quad_index), 0 };
    else
        return { xi(quad_index), eta(quad_index), zeta(quad_index) };
}

const GeneralMatrix Quadrature::parametric_points() const
{
    if (eta_list_ == nullptr) // 1D
    {
        GeneralMatrix points(n_quad_points_, 1);
        for (auto i = 0; i < n_quad_points_; i++)
        {
            points(i, 0) = xi(i);
        }
        return points;
    }
    else if (zeta_list_ == nullptr) // 2D
    {
        GeneralMatrix points(n_quad_points_, 2);
        for (auto i = 0; i < n_quad_points_; i++)
        {
            points(i, 0) = xi(i);
            points(i, 1) = eta(i);
        }
        return points;
    }
    else // 3D
    {
        GeneralMatrix points(n_quad_points_, 3);
        for (auto i = 0; i < n_quad_points_; i++)
        {
            points(i, 0) = xi(i);
            points(i, 1) = eta(i);
            points(i, 2) = zeta(i);
        }
        return points;
    }
}

detail::QuadratureIterator Quadrature::begin() const
{
    return detail::QuadratureIterator(*this, 0);
}

const detail::QuadratureIterator Quadrature::end() const
{
    return detail::QuadratureIterator(*this, n_quad_points_);
}


//////////////////////////////////////////////////////////////////////////////////////////////////
// QuadratureIterator Definitions

detail::QuadratureIterator::QuadratureIterator(const Quadrature& quadrature,
                                                     QuadratureIndex curr_index)
  : quadrature_(quadrature)
  , curr_index_(curr_index)
{
}

detail::QuadratureIterator& detail::QuadratureIterator::operator++()
{
    curr_index_++;
    return *this;
}

detail::QuadratureIterator detail::QuadratureIterator::operator++(int)
{
    detail::QuadratureIterator retval = *this;
    ++(*this);
    return retval;
}

bool detail::QuadratureIterator::operator==(detail::QuadratureIterator other) const
{
    return curr_index_ == other.curr_index_;
}

bool detail::QuadratureIterator::operator!=(detail::QuadratureIterator other) const
{
    return !(*this == other);
}

detail::QuadratureIterator::value_type detail::QuadratureIterator::operator*() const
{
    return { curr_index_,
             quadrature_.xi(curr_index_),
             quadrature_.eta(curr_index_),
             quadrature_.zeta(curr_index_),
             quadrature_.weight(curr_index_) };
}
}
