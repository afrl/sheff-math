#include "simple_math/quadrature/Quadrature.h"

namespace simple_math
{

	Quadrature::Quadrature() : gauss(), gauss2D(), gauss3D(), n_quad_points_(0)
	{
	}

	void Quadrature::setup(
        const QuadratureScheme& quadrature_scheme,
        const PersistentQuadrature* run_time_generated_quadrature)
	{
		reset();

		type_ = quadrature_scheme.type;
		integration_order_ = quadrature_scheme.integration_order;

        switch (type_)
        {
        case QuadratureType::Gauss1D:
            assert(!run_time_generated_quadrature);
			xi_list_ = gauss.get_abscissae(integration_order_);
			weight_list_ = gauss.get_weights(integration_order_);
			n_quad_points_ = integration_order_;
            break;
        case QuadratureType::Gauss2D:
            assert(!run_time_generated_quadrature);
            gauss2D.setup(integration_order_, gauss);
			xi_list_ = gauss2D.get_xis(integration_order_);
			eta_list_ = gauss2D.get_etas(integration_order_);
			weight_list_ = gauss2D.get_weights(integration_order_);
			n_quad_points_ = integration_order_ * integration_order_;
            break;
        case QuadratureType::Gauss3D:
            assert(!run_time_generated_quadrature);
            gauss3D.setup(integration_order_, gauss);
			xi_list_ = gauss3D.get_xis(integration_order_);
			eta_list_ = gauss3D.get_etas(integration_order_);
			zeta_list_ = gauss3D.get_zetas(integration_order_);
			weight_list_ = gauss3D.get_weights(integration_order_);
			n_quad_points_ = integration_order_ * integration_order_ * integration_order_;
            break;
        case QuadratureType::Gauss2DTriangle:
            assert(!run_time_generated_quadrature);
            gaussTriangle2D.setup(integration_order_);
			xi_list_ = gaussTriangle2D.get_xis(integration_order_);
			eta_list_ = gaussTriangle2D.get_etas(integration_order_);
			weight_list_ = gaussTriangle2D.get_weights(integration_order_);
			n_quad_points_ = gaussTriangle2D.get_n_quads(integration_order_);
            break;
        case QuadratureType::Gauss3DTet:
            assert(!run_time_generated_quadrature);
            gaussTetrahedron3D.setup(integration_order_);
			xi_list_ = gaussTetrahedron3D.get_xis(integration_order_);
			eta_list_ = gaussTetrahedron3D.get_etas(integration_order_);
			zeta_list_ = gaussTetrahedron3D.get_zetas(integration_order_);
			weight_list_ = gaussTetrahedron3D.get_weights(integration_order_);
			n_quad_points_ = gaussTetrahedron3D.get_n_quads(integration_order_);
            break;
        case QuadratureType::Gauss3DWedge:
            assert(!run_time_generated_quadrature);
            gaussWedge3D.setup(integration_order_);
			xi_list_ = gaussWedge3D.get_xis(integration_order_);
			eta_list_ = gaussWedge3D.get_etas(integration_order_);
			zeta_list_ = gaussWedge3D.get_zetas(integration_order_);
			weight_list_ = gaussWedge3D.get_weights(integration_order_);
			n_quad_points_ = gaussWedge3D.get_n_quads(integration_order_);
            break;
        case QuadratureType::NewtonCotes1D:
            assert(!run_time_generated_quadrature);
            nc1D.setup(integration_order_);
			xi_list_ = nc1D.get_xis(integration_order_);
			weight_list_ = nc1D.get_weights(integration_order_);
			n_quad_points_ = integration_order_;
            break;
        case QuadratureType::NewtonCotes2D:
            assert(!run_time_generated_quadrature);
            nc2D.setup(integration_order_);
			xi_list_ = nc2D.get_xis(integration_order_);
			eta_list_ = nc2D.get_etas(integration_order_);
			weight_list_ = nc2D.get_weights(integration_order_);
			n_quad_points_ = integration_order_ * integration_order_;
            break;
        case QuadratureType::NewtonCotes3D:
            assert(!run_time_generated_quadrature);
            nc3D.setup(integration_order_);
			xi_list_ = nc3D.get_xis(integration_order_);
			eta_list_ = nc3D.get_etas(integration_order_);
			zeta_list_ = nc3D.get_zetas(integration_order_);
			weight_list_ = nc3D.get_weights(integration_order_);
			n_quad_points_ = integration_order_ * integration_order_ * integration_order_;
            break;
        case QuadratureType::RuntimeGenerated1D:
            assert(run_time_generated_quadrature);
            xi_list_ = static_cast<const PersistentQuadrature1D*>(
                run_time_generated_quadrature)->xis_;
            weight_list_ = static_cast<const PersistentQuadrature1D*>(
                run_time_generated_quadrature)->weights_;
            n_quad_points_ = static_cast<const PersistentQuadrature1D*>(
                run_time_generated_quadrature)->n_quad_points_;
            break;
        case QuadratureType::RuntimeGenerated2D:
            assert(run_time_generated_quadrature);
            xi_list_ = static_cast<const PersistentQuadrature2D*>(
                run_time_generated_quadrature)->xis_;
            eta_list_ = static_cast<const PersistentQuadrature2D*>(
                run_time_generated_quadrature)->etas_;
            weight_list_ = static_cast<const PersistentQuadrature2D*>(
                run_time_generated_quadrature)->weights_;
            n_quad_points_ = static_cast<const PersistentQuadrature2D*>(
                run_time_generated_quadrature)->n_quad_points_;
            break;
        case QuadratureType::RuntimeGenerated3D:
            assert(run_time_generated_quadrature);
            xi_list_ = static_cast<const PersistentQuadrature3D*>(
                run_time_generated_quadrature)->xis_;
            eta_list_ = static_cast<const PersistentQuadrature3D*>(
                run_time_generated_quadrature)->etas_;
            zeta_list_ = static_cast<const PersistentQuadrature3D*>(
                run_time_generated_quadrature)->zetas_;
            weight_list_ = static_cast<const PersistentQuadrature3D*>(
                run_time_generated_quadrature)->weights_;
            n_quad_points_ = static_cast<const PersistentQuadrature3D*>(
                run_time_generated_quadrature)->n_quad_points_;
            break;
		}
	}

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

}
