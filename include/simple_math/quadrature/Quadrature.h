#pragma once

#include "simple_math/EigenDefines.h"

#include "Gauss.h"
#include "Gauss2D.h"
#include "Gauss3D.h"
#include "GaussTriangle.h"
#include "GaussTetrahedron.h"
#include "GaussWedge.h"
#include "NewtonCotes1D.h"
#include "NewtonCotes2D.h"
#include "NewtonCotes3D.h"

namespace simple_math
{

	enum class QuadratureType : int8_t {
		Gauss1D, ///< Standard Gauss quadrature (order will refer to # of quad points)
		Gauss2D, ///< See Gauss1D
		Gauss3D, ///< See Gauss1D
		Gauss2DTriangle, ///< Gauss quadrature specialized for triangles, which use area coordinates (max order is 4)
		Gauss3DTet, ///< Gauss quadrature specialized for tetrahedrals (max order is 4)
		Gauss3DWedge, ///< Gauss quadrature specialized for wedges, due to area coordinates of triangles (max order is 4)
		NewtonCotes1D, ///< Evenly spaced quad-points from -1 to 1 (order will refer to # of quad points)
		NewtonCotes2D, ///< See NewtonCotes1D
		NewtonCotes3D, ///< See NewtonCotes1D
		UNKNOWN ///< Must be last! Used by Mapping class to know the last QuadratureType.
	};

	class Quadrature
	{
	public:
		/// Constructor creates all the quadrature objects
		Quadrature();

		void setup(const QuadratureType &qt, const unsigned int integration_order);
		void reset();

		const QuadratureType& get_type() const { return type_; }
		const unsigned int& get_integration_order() const { return integration_order_; }
		const unsigned int& n_quad_points() const { return n_quad_points_; }
		const Point parametric_point(const unsigned int &quad_index) const;
		const double& xi(const unsigned int &quad_index) const { return get(quad_index, xi_list_); };
		const double& eta(const unsigned int &quad_index) const { return get(quad_index, eta_list_); };
		const double& zeta(const unsigned int &quad_index) const { return get(quad_index, zeta_list_); };
		const double& weight(const unsigned int &quad_index) const { return get(quad_index, weight_list_); };

	private:
		/// The current quadrature type
		QuadratureType type_;
		/// The current integration order
		unsigned int integration_order_;
		/// The total number of integration points for the current quadrature rule
		unsigned int n_quad_points_;
		/// Each of these arrays will point to static memory
		const double *xi_list_, *eta_list_, *zeta_list_, *weight_list_;

		/// Calculates and stores the Gauss-Legrandre abscissae and weights
		Gauss gauss;
		/// Calcualtes and stores the list of master coords and weights for 2D
		/// elements with evenly distributed points (quadralaterals)
		Gauss2D gauss2D;
		/// Calcualtes and stores the list of master coords and weights for 3D
		/// elements with evenly distributed points
		Gauss3D gauss3D;
		GaussTriangle gaussTriangle2D;
		GaussTetrahedron gaussTetrahedron3D;
		GaussWedge gaussWedge3D;
		NewtonCotes1D nc1D;
		NewtonCotes2D nc2D;
		NewtonCotes3D nc3D;

		/// Allows common error checks to be added or removed
		inline const double& get(const unsigned int &quad_index, const double* valArray) const {
			_UTIL_ASSERT(valArray == nullptr, "Tried to get an invalid value. Either Quadrature was not setup or the type is invalid.");
			_UTIL_ASSERT(quad_index >= n_quad_points_, "The quad point requested is out of bounds.");
			return valArray[quad_index];
		};
	};

}