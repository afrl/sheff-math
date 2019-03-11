#pragma once

#include "utility/Exceptions.h"

namespace simple_math
{
	class NewtonCotes1D
	{
	public:
		NewtonCotes1D() {
			for (unsigned int i = 0; i < MAX_NC_ORDER; i++) {
				xis[i] = nullptr;
				weights[i] = nullptr;
				calculated[i] = false;
			}
		};

		virtual ~NewtonCotes1D() {
			for (unsigned int i = 0; i < MAX_NC_ORDER; i++) {
				if (xis[i]) delete[] xis[i];
				if (weights[i]) delete[] weights[i];
			}
		}

		const double* getXis(const unsigned int &integrationOrder)
		{
			unsigned int orderIndex = integrationOrder - 1;
			if (!calculated[orderIndex])
				calculate(integrationOrder);
			return xis[orderIndex];
		}

		const double* getWeights(const unsigned int &integrationOrder)
		{
			unsigned int orderIndex = integrationOrder - 1;
			if (!calculated[orderIndex])
				calculate(integrationOrder);
			return weights[orderIndex];
		}

	protected:
		static const unsigned int MAX_NC_ORDER = 5;
		double *xis[MAX_NC_ORDER];
		double *weights[MAX_NC_ORDER];
		bool calculated[MAX_NC_ORDER];

		double getCoord(const unsigned int &k, const unsigned int &integrationOrder) const
		{
			if (k > integrationOrder) throw _UTIL_MSG_EXCEPT_C("Invalid integration order.");
			if (integrationOrder == 1)return 0.0; //Single point, midpoint.
			return -1.0 + k * (2.0 / (integrationOrder - 1)); //Return the kth point of N=Order points evenly spaced on interval [-1,1]
		}

		double getWeight(const unsigned int &k, const unsigned int &integrationOrder) const
		{
			switch (integrationOrder) {
			case 1:
				return 2.0;
				break;
			case 2:
				return 1.0;
				break;
			case 3:
				if (k == 1) return 4.0 / 3.0;
				else return 1.0 / 3.0;
				break;
			case 4:
				if (k == 1 || k == 2) return 0.75;
				else return 0.25;
				break;
			case 5:
				if (k == 2) return 4.0 / 15.0;
				else if (k == 1 || k == 3) return 32.0 / 45.0;
				else return 7.0 / 45.0;
				break;
			default:
				throw _UTIL_MSG_EXCEPT_C("Invalid integration order.");
			}
		}

		virtual void calculate(const unsigned int &integrationOrder)
		{
			unsigned int orderIndex = integrationOrder - 1;
			if (calculated[orderIndex]) return;
			calculated[orderIndex] = true;

			// Setup some variables
			unsigned int length = integrationOrder;
			// Allocate the vectors
			xis[orderIndex] = new double[length];
			weights[orderIndex] = new double[length];
			unsigned int quadIndex = 0;
			for (unsigned int i = 0; i < integrationOrder; i++) {
				xis[orderIndex][i] = getCoord(i, integrationOrder);
				weights[orderIndex][i] = getWeight(i, integrationOrder);
				quadIndex++;
			}
		}
	};
}