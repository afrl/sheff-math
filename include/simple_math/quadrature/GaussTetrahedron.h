#pragma once

#include "GaussTriangle.h"

namespace simple_math
{
	class GaussTetrahedron : public GaussTriangle
	{
	public:
        GaussTetrahedron();
        virtual ~GaussTetrahedron();

        virtual void setup(const unsigned int integration_order) override;
        const double* get_zetas(const unsigned int &integration_order) { return zeta[integration_order - 1]; }

	protected:
		double* zeta[MAX_GAUSS_TRI_ORDER];
	};
}