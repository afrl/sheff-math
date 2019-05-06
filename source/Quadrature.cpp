#include "simple_math/quadrature/Quadrature.h"

namespace simple_math
{

	Quadrature::Quadrature() : gauss(), gauss2D(), gauss3D(), n_quad_points_(0)
	{
	}

	void Quadrature::setup(const QuadratureType & qt, const unsigned int integration_order)
	{
		reset();

		type_ = qt;
		integration_order_ = integration_order;

        switch (qt)
        {
        case QuadratureType::Gauss1D:
			xi_list_ = gauss.get_abscissae(integration_order);
			weight_list_ = gauss.get_weights(integration_order);
			n_quad_points_ = integration_order;
        case QuadratureType::Gauss2D:
            gauss2D.setup(integration_order, gauss);
			xi_list_ = gauss2D.get_xis(integration_order);
			eta_list_ = gauss2D.get_etas(integration_order);
			weight_list_ = gauss2D.get_weights(integration_order);
			n_quad_points_ = integration_order * integration_order;
        case QuadratureType::Gauss3D:
            gauss3D.setup(integration_order, gauss);
			xi_list_ = gauss3D.get_xis(integration_order);
			eta_list_ = gauss3D.get_etas(integration_order);
			zeta_list_ = gauss3D.get_zetas(integration_order);
			weight_list_ = gauss3D.get_weights(integration_order);
			n_quad_points_ = integration_order * integration_order * integration_order;
        case QuadratureType::Gauss2DTriangle:
			xi_list_ = gaussTriangle2D.getXis(integration_order);
			eta_list_ = gaussTriangle2D.getEtas(integration_order);
			weight_list_ = gaussTriangle2D.getWeights(integration_order);
			n_quad_points_ = gaussTriangle2D.getNumQuads(integration_order);
        case QuadratureType::Gauss3DTet:
			xi_list_ = gaussTetrahedron3D.getXis(integration_order);
			eta_list_ = gaussTetrahedron3D.getEtas(integration_order);
			zeta_list_ = gaussTetrahedron3D.getZetas(integration_order);
			weight_list_ = gaussTetrahedron3D.getWeights(integration_order);
			n_quad_points_ = gaussTetrahedron3D.getNumQuads(integration_order);
        case QuadratureType::Gauss3DWedge:
			xi_list_ = gaussWedge3D.getXis(integration_order);
			eta_list_ = gaussWedge3D.getEtas(integration_order);
			zeta_list_ = gaussWedge3D.getZetas(integration_order);
			weight_list_ = gaussWedge3D.getWeights(integration_order);
			n_quad_points_ = gaussWedge3D.getNumQuads(integration_order);
        case QuadratureType::NewtonCotes1D:
            nc1D.setup(integration_order);
			xi_list_ = nc1D.get_xis(integration_order);
			weight_list_ = nc1D.get_weights(integration_order);
			n_quad_points_ = integration_order;
        case QuadratureType::NewtonCotes2D:
            nc2D.setup(integration_order);
			xi_list_ = nc2D.get_xis(integration_order);
			eta_list_ = nc2D.get_etas(integration_order);
			weight_list_ = nc2D.get_weights(integration_order);
			n_quad_points_ = integration_order * integration_order;
        case QuadratureType::NewtonCotes3D:
            nc3D.setup(integration_order);
			xi_list_ = nc3D.get_xis(integration_order);
			eta_list_ = nc3D.get_etas(integration_order);
			zeta_list_ = nc3D.get_zetas(integration_order);
			weight_list_ = nc3D.get_weights(integration_order);
			n_quad_points_ = integration_order * integration_order * integration_order;
		}
	}

	void Quadrature::reset()
	{
		xi_list_ = nullptr;
		eta_list_ = nullptr;
		zeta_list_ = nullptr;
		n_quad_points_ = 0;
	}

	const Point Quadrature::parametric_point(const unsigned int & quad_index) const
	{
		if (eta_list_ == nullptr)
			return { xi(quad_index), 0, 0 };
		else if (zeta_list_ == nullptr)
			return { xi(quad_index), eta(quad_index), 0 };
		else
			return { xi(quad_index), eta(quad_index), zeta(quad_index) };
	}

}