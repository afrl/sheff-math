#include "simple_math/quadrature/Quadrature.h"

namespace simple_math
{

	Quadrature::Quadrature() : gauss(), gauss2D(), gauss3D(), n_quad_points_(0)
	{
	}

	void Quadrature::setup(const QuadratureScheme& quadrature_scheme)
	{
		reset();

		type_ = quadrature_scheme.type;
		integration_order_ = quadrature_scheme.integration_order;

        switch (type_)
        {
        case QuadratureType::Gauss1D:
			xi_list_ = gauss.get_abscissae(integration_order_);
			weight_list_ = gauss.get_weights(integration_order_);
			n_quad_points_ = integration_order_;
            break;
        case QuadratureType::Gauss2D:
            gauss2D.setup(integration_order_, gauss);
			xi_list_ = gauss2D.get_xis(integration_order_);
			eta_list_ = gauss2D.get_etas(integration_order_);
			weight_list_ = gauss2D.get_weights(integration_order_);
			n_quad_points_ = integration_order_ * integration_order_;
            break;
        case QuadratureType::Gauss3D:
            gauss3D.setup(integration_order_, gauss);
			xi_list_ = gauss3D.get_xis(integration_order_);
			eta_list_ = gauss3D.get_etas(integration_order_);
			zeta_list_ = gauss3D.get_zetas(integration_order_);
			weight_list_ = gauss3D.get_weights(integration_order_);
			n_quad_points_ = integration_order_ * integration_order_ * integration_order_;
            break;
        case QuadratureType::Gauss2DTriangle:
            gaussTriangle2D.setup(integration_order_);
			xi_list_ = gaussTriangle2D.get_xis(integration_order_);
			eta_list_ = gaussTriangle2D.get_etas(integration_order_);
			weight_list_ = gaussTriangle2D.get_weights(integration_order_);
			n_quad_points_ = gaussTriangle2D.get_n_quads(integration_order_);
            break;
        case QuadratureType::Gauss3DTet:
            gaussTetrahedron3D.setup(integration_order_);
			xi_list_ = gaussTetrahedron3D.get_xis(integration_order_);
			eta_list_ = gaussTetrahedron3D.get_etas(integration_order_);
			zeta_list_ = gaussTetrahedron3D.get_zetas(integration_order_);
			weight_list_ = gaussTetrahedron3D.get_weights(integration_order_);
			n_quad_points_ = gaussTetrahedron3D.get_n_quads(integration_order_);
            break;
        case QuadratureType::Gauss3DWedge:
            gaussWedge3D.setup(integration_order_);
			xi_list_ = gaussWedge3D.get_xis(integration_order_);
			eta_list_ = gaussWedge3D.get_etas(integration_order_);
			zeta_list_ = gaussWedge3D.get_zetas(integration_order_);
			weight_list_ = gaussWedge3D.get_weights(integration_order_);
			n_quad_points_ = gaussWedge3D.get_n_quads(integration_order_);
            break;
        case QuadratureType::NewtonCotes1D:
            nc1D.setup(integration_order_);
			xi_list_ = nc1D.get_xis(integration_order_);
			weight_list_ = nc1D.get_weights(integration_order_);
			n_quad_points_ = integration_order_;
            break;
        case QuadratureType::NewtonCotes2D:
            nc2D.setup(integration_order_);
			xi_list_ = nc2D.get_xis(integration_order_);
			eta_list_ = nc2D.get_etas(integration_order_);
			weight_list_ = nc2D.get_weights(integration_order_);
			n_quad_points_ = integration_order_ * integration_order_;
            break;
        case QuadratureType::NewtonCotes3D:
            nc3D.setup(integration_order_);
			xi_list_ = nc3D.get_xis(integration_order_);
			eta_list_ = nc3D.get_etas(integration_order_);
			zeta_list_ = nc3D.get_zetas(integration_order_);
			weight_list_ = nc3D.get_weights(integration_order_);
			n_quad_points_ = integration_order_ * integration_order_ * integration_order_;
            break;
        case QuadratureType::RuntimeGenerated1D:
            xi_list_ = static_cast<PersistentQuadrature1D*>(
                quadrature_scheme.run_time_generated_quadrature.get())->xis_;
            weight_list_ = static_cast<PersistentQuadrature1D*>(
                quadrature_scheme.run_time_generated_quadrature.get())->weights_;
            n_quad_points_ = static_cast<PersistentQuadrature1D*>(
                quadrature_scheme.run_time_generated_quadrature.get())->n_quad_points_;
            break;
        case QuadratureType::RuntimeGenerated2D:
            xi_list_ = static_cast<PersistentQuadrature2D*>(
                quadrature_scheme.run_time_generated_quadrature.get())->xis_;
            eta_list_ = static_cast<PersistentQuadrature2D*>(
                quadrature_scheme.run_time_generated_quadrature.get())->etas_;
            weight_list_ = static_cast<PersistentQuadrature2D*>(
                quadrature_scheme.run_time_generated_quadrature.get())->weights_;
            n_quad_points_ = static_cast<PersistentQuadrature2D*>(
                quadrature_scheme.run_time_generated_quadrature.get())->n_quad_points_;
            break;
        case QuadratureType::RuntimeGenerated3D:
            xi_list_ = static_cast<PersistentQuadrature3D*>(
                quadrature_scheme.run_time_generated_quadrature.get())->xis_;
            eta_list_ = static_cast<PersistentQuadrature3D*>(
                quadrature_scheme.run_time_generated_quadrature.get())->etas_;
            zeta_list_ = static_cast<PersistentQuadrature3D*>(
                quadrature_scheme.run_time_generated_quadrature.get())->zetas_;
            weight_list_ = static_cast<PersistentQuadrature3D*>(
                quadrature_scheme.run_time_generated_quadrature.get())->weights_;
            n_quad_points_ = static_cast<PersistentQuadrature3D*>(
                quadrature_scheme.run_time_generated_quadrature.get())->n_quad_points_;
            break;
		}
	}

<<<<<<< HEAD
=======
    void Quadrature::setup(const PersistentQuadrature1D& persistent_quadrature)
    {
        xi_list_ = persistent_quadrature.xis_;
        eta_list_ = nullptr;
        zeta_list_ = nullptr;
        weight_list_ = persistent_quadrature.weights_;
        n_quad_points_ = persistent_quadrature.n_quad_points_;
    }

    void Quadrature::setup(const PersistentQuadrature2D& persistent_quadrature)
    {
        xi_list_ = persistent_quadrature.xis_;
        eta_list_ = persistent_quadrature.etas_;
        zeta_list_ = nullptr;
        weight_list_ = persistent_quadrature.weights_;
        n_quad_points_ = persistent_quadrature.n_quad_points_;
    }

    void Quadrature::setup(const PersistentQuadrature3D& persistent_quadrature)
    {
        xi_list_ = persistent_quadrature.xis_;
        eta_list_ = persistent_quadrature.etas_;
        zeta_list_ = persistent_quadrature.zetas_;
        weight_list_ = persistent_quadrature.weights_;
        n_quad_points_ = persistent_quadrature.n_quad_points_;
    }

>>>>>>> master
	void Quadrature::reset()
	{
		xi_list_ = nullptr;
		eta_list_ = nullptr;
		zeta_list_ = nullptr;
		n_quad_points_ = 0;
	}

	const Point Quadrature::parametric_point(const QuadratureIndex & quad_index) const
	{
		if (eta_list_ == nullptr)
			return { xi(quad_index), 0, 0 };
		else if (zeta_list_ == nullptr)
			return { xi(quad_index), eta(quad_index), 0 };
		else
			return { xi(quad_index), eta(quad_index), zeta(quad_index) };
	}

}
