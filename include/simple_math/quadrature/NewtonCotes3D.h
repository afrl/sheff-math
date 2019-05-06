#pragma once

#include "NewtonCotes2D.h"

namespace simple_math
{
	class NewtonCotes3D : public NewtonCotes2D
	{
	public:
        NewtonCotes3D();
        virtual ~NewtonCotes3D();

        virtual void setup(const unsigned int integration_order) override;
        const double* get_zetas(const unsigned int &integration_order) { return zetas[integration_order - 1]; }

	protected:
		double *zetas[MAX_NC_ORDER];
	};
}