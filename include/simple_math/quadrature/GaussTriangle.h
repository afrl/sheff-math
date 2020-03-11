#pragma once

#include "utility/Exceptions.h"

namespace simple_math
{
	class GaussTriangle
	{
	public:
        GaussTriangle();
        virtual ~GaussTriangle();

        virtual void setup(const unsigned int integration_order);
        const double* get_xis(const unsigned int &integration_order) { return xi[integration_order - 1]; }
        const double* get_etas(const unsigned int &integration_order) { return eta[integration_order - 1]; }
        const double* get_weights(const unsigned int &integration_order) { return weight[integration_order - 1]; }

        unsigned int get_n_quads(const unsigned int &integration_order);

	protected:
		static const int MAX_GAUSS_TRI_ORDER = 4;

		unsigned int length[MAX_GAUSS_TRI_ORDER];
		double *xi[MAX_GAUSS_TRI_ORDER];
		double *eta[MAX_GAUSS_TRI_ORDER];
		double *weight[MAX_GAUSS_TRI_ORDER];
	};
}
