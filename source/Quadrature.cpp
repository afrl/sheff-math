#include "simple_math/quadrature/Quadrature.h"

namespace simple_math
{

	Quadrature::Quadrature() : gauss(), gauss2D(), gauss3D(), n_quad_points_(0)
	{
	}

	void Quadrature::setup(const QuadratureType & qt, const unsigned int integrationOrder)
	{
		reset();

		type_ = qt;
		integration_order_ = integrationOrder;

		if (qt == QuadratureType::Gauss1D) {
			xi_list_ = gauss.getAbscissae(integrationOrder);
			weight_list_ = gauss.getWeights(integrationOrder);
			n_quad_points_ = integrationOrder;
		}
		else if (qt == QuadratureType::Gauss2D) {
			xi_list_ = gauss2D.getXis(integrationOrder, gauss);
			eta_list_ = gauss2D.getEtas(integrationOrder, gauss);
			weight_list_ = gauss2D.getWeights(integrationOrder, gauss);
			n_quad_points_ = integrationOrder * integrationOrder;
		}
		else if (qt == QuadratureType::Gauss3D) {
			xi_list_ = gauss3D.getXis(integrationOrder, gauss);
			eta_list_ = gauss3D.getEtas(integrationOrder, gauss);
			zeta_list_ = gauss3D.getZetas(integrationOrder, gauss);
			weight_list_ = gauss3D.getWeights(integrationOrder, gauss);
			n_quad_points_ = integrationOrder * integrationOrder*integrationOrder;
		}
		else if (qt == QuadratureType::Gauss2DTriangle) {
			xi_list_ = gaussTriangle2D.getXis(integrationOrder);
			eta_list_ = gaussTriangle2D.getEtas(integrationOrder);
			weight_list_ = gaussTriangle2D.getWeights(integrationOrder);
			n_quad_points_ = gaussTriangle2D.getNumQuads(integrationOrder);
		}
		else if (qt == QuadratureType::Gauss3DTet) {
			xi_list_ = gaussTetrahedron3D.getXis(integrationOrder);
			eta_list_ = gaussTetrahedron3D.getEtas(integrationOrder);
			zeta_list_ = gaussTetrahedron3D.getZetas(integrationOrder);
			weight_list_ = gaussTetrahedron3D.getWeights(integrationOrder);
			n_quad_points_ = gaussTetrahedron3D.getNumQuads(integrationOrder);
		}
		else if (qt == QuadratureType::Gauss3DWedge) {
			xi_list_ = gaussWedge3D.getXis(integrationOrder);
			eta_list_ = gaussWedge3D.getEtas(integrationOrder);
			zeta_list_ = gaussWedge3D.getZetas(integrationOrder);
			weight_list_ = gaussWedge3D.getWeights(integrationOrder);
			n_quad_points_ = gaussWedge3D.getNumQuads(integrationOrder);
		}
		else if (qt == QuadratureType::NewtonCotes1D) {
			xi_list_ = nc1D.getXis(integrationOrder);
			weight_list_ = nc1D.getWeights(integrationOrder);
			n_quad_points_ = integrationOrder;
		}
		else if (qt == QuadratureType::NewtonCotes2D) {
			xi_list_ = nc2D.getXis(integrationOrder);
			eta_list_ = nc2D.getEtas(integrationOrder);
			weight_list_ = nc2D.getWeights(integrationOrder);
			n_quad_points_ = integrationOrder * integrationOrder;
		}
		else if (qt == QuadratureType::NewtonCotes3D) {
			xi_list_ = nc3D.getXis(integrationOrder);
			eta_list_ = nc3D.getEtas(integrationOrder);
			zeta_list_ = nc3D.getZetas(integrationOrder);
			weight_list_ = nc3D.getWeights(integrationOrder);
			n_quad_points_ = integrationOrder * integrationOrder*integrationOrder;
		}
	}

	void Quadrature::reset()
	{
		xi_list_ = 0;
		eta_list_ = 0;
		zeta_list_ = 0;
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