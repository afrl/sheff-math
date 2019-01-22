#pragma once

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

	enum class QuadratureType {
		Gauss1D,
		Gauss2D,
		Gauss3D,
		Gauss2DTriangle,
		Gauss3DTet,
		Gauss3DWedge,
		NewtonCotes1D,
		NewtonCotes2D,
		NewtonCotes3D
	};

	class Quadrature
	{
	public:
		/// Constructor creates all the quadrature objects
		Quadrature();

		void setup(const QuadratureType &qt, const unsigned int integration_order);
		void reset();

		const unsigned int& n_quad_points() const { return n_quad_points_; }
		const double& xi(const unsigned int &quadIndex) const { return get(quadIndex, xi_list_); };
		const double& eta(const unsigned int &quadIndex) const { return get(quadIndex, eta_list_); };
		const double& zeta(const unsigned int &quadIndex) const { return get(quadIndex, zeta_list_); };
		const double& weight(const unsigned int &quadIndex) const { return get(quadIndex, weight_list_); };

	private:

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
		inline const double& get(const unsigned int &quadIndex, const double* valArray) const {
			_UTIL_ASSERT(valArray == nullptr, "Tried to get an invalid value. Either Quadrature was not setup or the type is invalid.");
			_UTIL_ASSERT(quadIndex >= n_quad_points_, "The quad point requested is out of bounds.");
			return valArray[quadIndex];
		};
	};

}