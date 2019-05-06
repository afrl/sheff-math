#pragma once

#include "NewtonCotes1D.h"

namespace simple_math
{
	class NewtonCotes2D : public NewtonCotes1D
	{
	public:
        NewtonCotes2D();
        virtual ~NewtonCotes2D();

        virtual void setup(const unsigned int integration_order) override;
        const double* get_etas(const unsigned int &integration_order) { return etas[integration_order - 1]; }

	protected:
		double *etas[MAX_NC_ORDER];
	};
}