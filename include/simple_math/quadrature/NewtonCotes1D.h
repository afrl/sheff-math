#pragma once

#include "utility/Exceptions.h"

namespace simple_math
{
	class NewtonCotes1D
	{
	public:
        NewtonCotes1D();
        virtual ~NewtonCotes1D();

        virtual void setup(const unsigned int integration_order);
        const double* get_xis(const unsigned int &integration_order) { return xis[integration_order - 1]; }
        const double* get_weights(const unsigned int &integration_order) { return weights[integration_order - 1]; }

	protected:
		static const unsigned int MAX_NC_ORDER = 5;
		double * xis[MAX_NC_ORDER];
		double * weights[MAX_NC_ORDER];

        double get_coord(const unsigned int &k, const unsigned int &integration_order) const;
        double get_weight(const unsigned int &k, const unsigned int &integration_order) const;
	};
}