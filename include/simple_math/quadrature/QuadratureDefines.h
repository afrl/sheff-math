#pragma once

#include "simple_math/Defines.h"
#include "simple_math/EigenDefines.h"

#include <array>

namespace simple_math
{
enum class QuadratureType : int8_t
{
    Gauss1D,         ///< Standard Gauss quadrature (order will refer to # of quad points)
    Gauss2D,         ///< See Gauss1D
    Gauss3D,         ///< See Gauss1D
    Gauss2DTriangle, ///< Gauss quadrature specialized for triangles, which use area coordinates
                     ///< (max order is 4)
    Gauss3DTet,      ///< Gauss quadrature specialized for tetrahedrals (max order is 4)
    Gauss3DWedge, ///< Gauss quadrature specialized for wedges, due to area coordinates of triangles
                  ///< (max order is 4)
    NewtonCotes1D, ///< Evenly spaced quad-points from -1 to 1 (order will refer to # of quad
                   ///< points)
    NewtonCotes2D, ///< See NewtonCotes1D
    NewtonCotes3D, ///< See NewtonCotes1D
    UNKNOWN        ///< Must be last! Used by Mapping class to know the last QuadratureType.
};

static std::array<double, 6> get_extent(QuadratureType type)
{
    switch (type)
    {
        case QuadratureType::Gauss1D:
        case QuadratureType::NewtonCotes1D: return { -1, 1, 0, 0, 0, 0 };
        case QuadratureType::Gauss2D:
        case QuadratureType::NewtonCotes2D: return { -1, 1, -1, 1, 0, 0 };
        case QuadratureType::Gauss3D:
        case QuadratureType::NewtonCotes3D: return { -1, 1, -1, 1, -1, 1 };
        case QuadratureType::Gauss2DTriangle: return { 0, 1, 0, 1, 0, 0 };
        case QuadratureType::Gauss3DTet: return { 0, 1, 0, 1, 0, 1 };
        case QuadratureType::Gauss3DWedge: return { 0, 1, 0, 1, -1, 1 };
        case QuadratureType::UNKNOWN:
        default:
            throw _UTIL_MSG_EXCEPT("Unrecognized type of quadrature.");
    }
}

/// Stores all of the information needed to describe a quadrature scheme.
struct QuadratureScheme
{
    QuadratureType type;
    QuadratureOrder integration_order;

    QuadratureScheme()
      : type(QuadratureType::UNKNOWN)
      , integration_order(0)
    {
    }
    QuadratureScheme(QuadratureType type_a, QuadratureOrder integration_order_a)
      : type(type_a)
      , integration_order(integration_order_a)
    {
    }

    bool operator==(const QuadratureScheme& other) const
    {
        return type == other.type && integration_order == other.integration_order;
    }

    template<typename SerializerType>
    void serialize(SerializerType& s) const
    {
        s(utility::named_value("type", type),
          utility::named_value("integration_order", integration_order));
    }

    template<typename SerializerType>
    void deserialize(SerializerType& s)
    {
        s(type, integration_order);
    }
};

}