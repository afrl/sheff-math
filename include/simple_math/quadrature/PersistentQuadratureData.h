#pragma once

#include "utility/Exceptions.h"

namespace simple_math
{
    /// An abstract base class for PersistentQuadrature1D, PersistentQuadrature2D,
    /// and PersistentQuadrature3D.  This is only provided for elements that can be
    /// 1D, 2D, or 3D, allowing them to store a std::unique_ptr<PersistentQuadrature>.
    /// However, note that Quadrature::setup does not accept the abstract class, so
    /// it will need to be cast before a setup call.  If the element knows it will be
    /// 2D or 3D, then store the appropriate type to avoid casts.
    class PersistentQuadrature
    {
    public:
        PersistentQuadrature() {}
        virtual ~PersistentQuadrature() {}
    };

    /// Stores persistent quadrature locations and weights for 1D.
    /// This is used where adaptive quadrature is used, since there is not static
    /// quadrature for those cases.  Each case is specialized for 1D, 2D, and 3D
    /// to prevent programming error the future.  If an element has a persistent
    /// quadrature rule that is specailized to it, then it should store one of
    /// these objects.  After the quad points are set, it passed to Quadrature::setup.
    class PersistentQuadrature1D : public PersistentQuadrature
    {
    public:
        PersistentQuadrature1D(const unsigned int n_quad_points)
        {
            n_quad_points_ = n_quad_points;
            xis_ = new double[n_quad_points_];
            weights_ = new double[n_quad_points_];
        }

        virtual ~PersistentQuadrature1D()
        {
            if (xis_) delete[] xis_;
            if (weights_) delete[] weights_;
        }

        void set_quad_point(const unsigned int index, const double xi)
        {
            xis_[index] = xi;
        }

        /// Performs a simple check to ensure all the weights are in the range [0,1]
        /// and all of the components of each parametic coordinate is in the range [-1,1].
        /// Only checks in debug mode.
        void check()
        {
            for (unsigned int i = 0; i < n_quad_points_; i++)
            {
                _UTIL_ASSERT(xis_[i] >= -1.0 && xis_[i] <= 1.0, "Xi for quad point "
                    + std::to_string(i) + " is outside of bounds with value "
                    + std::to_string(xis_[i]));
                _UTIL_ASSERT(weights_[i] >= 0.0 && weights_[i] <= 1.0, "Weight for quad point "
                    + std::to_string(i) + " is outside of bounds with value "
                    + std::to_string(weights_[i]));
            }
        }

        friend class Quadrature;

    private:
        unsigned int n_quad_points_;
        double* xis_;
        double* weights_;
    };

    /// Stores persistent quadrature locations and weights for 2D.
    /// \see PersistentQuadrature1D for further documentation
    class PersistentQuadrature2D : public PersistentQuadrature
    {
    public:
        PersistentQuadrature2D(const unsigned int n_quad_points)
        {
            n_quad_points_ = n_quad_points;
            xis_ = new double[n_quad_points_];
            etas_ = new double[n_quad_points_];
            weights_ = new double[n_quad_points_];
        }

        ~PersistentQuadrature2D()
        {
            if (xis_) delete[] xis_;
            if (etas_) delete[] etas_;
            if (weights_) delete[] weights_;
        }

        void set_quad_point(const unsigned int index, const double xi, const double eta)
        {
            xis_[index] = xi;
            etas_[index] = eta;
        }

        /// Performs a simple check to ensure all the weights are in the range [0,1]
        /// and all of the components of each parametic coordinate is in the range [-1,1].
        /// Only checks in debug mode.
        void check()
        {
            for (unsigned int i = 0; i < n_quad_points_; i++)
            {
                _UTIL_ASSERT(xis_[i] >= -1.0 && xis_[i] <= 1.0, "Xi for quad point "
                    + std::to_string(i) + " is outside of bounds with value "
                    + std::to_string(xis_[i]));
                _UTIL_ASSERT(etas_[i] >= -1.0 && etas_[i] <= 1.0, "Eta for quad point "
                    + std::to_string(i) + " is outside of bounds with value "
                    + std::to_string(etas_[i]));
                _UTIL_ASSERT(weights_[i] >= 0.0 && weights_[i] <= 1.0, "Weight for quad point "
                    + std::to_string(i) + " is outside of bounds with value "
                    + std::to_string(weights_[i]));
            }
        }

        friend class Quadrature;

    private:
        unsigned int n_quad_points_;
        double* xis_;
        double* etas_;
        double* weights_;
    };

    /// Stores persistent quadrature locations and weights for 3D.
    /// \see PersistentQuadrature1D for further documentation
    class PersistentQuadrature3D : public PersistentQuadrature
    {
    public:
        PersistentQuadrature3D(const unsigned int n_quad_points)
        {
            n_quad_points_ = n_quad_points;
            xis_ = new double[n_quad_points_];
            etas_ = new double[n_quad_points_];
            zetas_ = new double[n_quad_points_];
            weights_ = new double[n_quad_points_];
        }

        ~PersistentQuadrature3D()
        {
            if (xis_) delete[] xis_;
            if (etas_) delete[] etas_;
            if (zetas_) delete[] zetas_;
            if (weights_) delete[] weights_;
        }

        void set_quad_point(const unsigned int index, const double xi, const double eta, const double zeta)
        {
            xis_[index] = xi;
            etas_[index] = eta;
            zetas_[index] = zeta;
        }

        /// Performs a simple check to ensure all the weights are in the range [0,1]
        /// and all of the components of each parametic coordinate is in the range [-1,1].
        /// Only checks in debug mode.
        void check()
        {
            for (unsigned int i = 0; i < n_quad_points_; i++)
            {
                _UTIL_ASSERT(xis_[i] >= -1.0 && xis_[i] <= 1.0, "Xi for quad point "
                    + std::to_string(i) + " is outside of bounds with value "
                    + std::to_string(xis_[i]));
                _UTIL_ASSERT(etas_[i] >= -1.0 && etas_[i] <= 1.0, "Eta for quad point "
                    + std::to_string(i) + " is outside of bounds with value "
                    + std::to_string(etas_[i]));
                _UTIL_ASSERT(zetas_[i] >= -1.0 && zetas_[i] <= 1.0, "Zeta for quad point "
                    + std::to_string(i) + " is outside of bounds with value "
                    + std::to_string(zetas_[i]));
                _UTIL_ASSERT(weights_[i] >= 0.0 && weights_[i] <= 1.0, "Weight for quad point "
                    + std::to_string(i) + " is outside of bounds with value "
                    + std::to_string(weights_[i]));
            }
        }

        friend class Quadrature;

    private:
        unsigned int n_quad_points_;
        double* xis_;
        double* etas_;
        double* zetas_;
        double* weights_;
    };
}