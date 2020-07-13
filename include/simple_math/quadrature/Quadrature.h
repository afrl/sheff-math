#pragma once

#include "simple_math/Defines.h"
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
#include "PersistentQuadratureData.h"

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
        RuntimeGenerated1D, ///< Relies on the element storing a PersistentQuadratureScheme1D object
        RuntimeGenerated2D, ///< Relies on the element storing a PersistentQuadratureScheme2D object
        RuntimeGenerated3D, ///< Relies on the element storing a PersistentQuadratureScheme3D object
		UNKNOWN ///< Must be last! Used by Mapping class to know the last QuadratureType.
	};    

    /// Stores all of the information needed to describe a quadrature scheme.
    struct QuadratureScheme
    {
        QuadratureType type;
        QuadratureOrder integration_order;
    };

	class Quadrature
	{
	public:
		/// Constructor creates all the quadrature objects
		Quadrature();

		void setup(
            const QuadratureScheme& quadrature_scheme,
            const PersistentQuadrature* run_time_generated_quadrature = nullptr);
		void reset();

		const QuadratureType& get_type() const { return type_; }
		const QuadratureOrder& get_integration_order() const { return integration_order_; }

		/// Returns the number of quadrature points that exist for the last scheme passed to setup()
		const QuadratureIndex& n_quad_points() const { return n_quad_points_; }

		/// Returns the corresponding parametric point for the given quadrture point index.
		/// Note: quad_index must be less than n_quad_points().  No check performed.
		const Point parametric_point(const QuadratureIndex &quad_index) const;

		/// Returns the parametric points in the form a matrix, where the row corresponds to the
		/// respective quadrature point and the column corresponds to the component of the parametric
		/// point.
		const GeneralMatrix parametric_points() const;

		/// Returns the first component of the parametic point for the given quad point.
		inline const double& xi(const QuadratureIndex &quad_index) const { return get(quad_index, xi_list_); }

		/// Returns the second component of the parametic point for the given quad point.
		/// Note: this should only be called for 2D or 3D quadrature schemes.
		inline const double& eta(const QuadratureIndex &quad_index) const { return get(quad_index, eta_list_); }

		/// Returns the third component of the parametic point for the given quad point.
		/// Note: this should only be called for 3D quadrature schemes.
		inline const double& zeta(const QuadratureIndex &quad_index) const { return get(quad_index, zeta_list_); }

		/// Returns the weight of the quad point.
		inline const double& weight(const QuadratureIndex &quad_index) const { return get(quad_index, weight_list_); }


	private:
		/// The current quadrature type
		QuadratureType type_;
		/// The current integration order
        QuadratureOrder integration_order_;
		/// The total number of integration points for the current quadrature rule
		QuadratureIndex n_quad_points_;
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
		inline const double& get(const QuadratureIndex &quad_index, const double* valArray) const {
			_UTIL_ASSERT(valArray != nullptr, "Tried to get an invalid value. Either Quadrature was not setup or the type is invalid.");
			_UTIL_ASSERT(quad_index < n_quad_points_, "The quad point requested is out of bounds.");
			return valArray[quad_index];
		};
	};

}