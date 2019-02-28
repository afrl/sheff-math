#pragma once

#include "Gauss.h"

namespace simple_math
{
	/// A helper class that uses the Gauss class to create a list of
	/// locations and weights for each quad point.  In the case of 2D
	/// this would create a list of integrationOrder^2 long.
	class Gauss2D
	{
	public:

		Gauss2D() {
			for (unsigned int i = 0; i < MAX_GAUSS_ORDER; i++) {
				xis[i] = nullptr;
				etas[i] = nullptr;
				weights[i] = nullptr;
				calculated[i] = false;
			}
		};

		virtual ~Gauss2D() {
			for (unsigned int i = 0; i < MAX_GAUSS_ORDER; i++) {
				if (xis[i]) delete[] xis[i];
				if (etas[i]) delete[] etas[i];
				if (weights[i]) delete[] weights[i];
			}
		}

		const double* getXis(const unsigned int &integrationOrder, Gauss &gauss)
		{
			unsigned int orderIndex = integrationOrder - 1;
			if (!calculated[orderIndex])
				calculate(integrationOrder, gauss);
			return xis[orderIndex];
		}

		const double* getEtas(const unsigned int &integrationOrder, Gauss &gauss)
		{
			unsigned int orderIndex = integrationOrder - 1;
			if (!calculated[orderIndex])
				calculate(integrationOrder, gauss);
			return etas[orderIndex];
		}

		const double* getWeights(const unsigned int &integrationOrder, Gauss &gauss)
		{
			unsigned int orderIndex = integrationOrder - 1;
			if (!calculated[orderIndex])
				calculate(integrationOrder, gauss);
			return weights[orderIndex];
		}

	protected:
		double *xis[MAX_GAUSS_ORDER];
		double *etas[MAX_GAUSS_ORDER];
		double *weights[MAX_GAUSS_ORDER];
		bool calculated[MAX_GAUSS_ORDER];

		virtual void calculate(const unsigned int &integrationOrder, Gauss &gauss)
		{
			unsigned int orderIndex = integrationOrder - 1;
			if (calculated[orderIndex]) return;
			calculated[orderIndex] = true;

			// Setup some variables
			unsigned int length = integrationOrder * integrationOrder;
			const double * gaussAbscissae = gauss.getAbscissae(integrationOrder);
			const double * gaussWeights = gauss.getWeights(integrationOrder);
			// Allocate the vectors
			xis[orderIndex] = new double[length];
			etas[orderIndex] = new double[length];
			weights[orderIndex] = new double[length];
			// Set the values
			unsigned int quadIndex = 0;
			for (unsigned int i = 0; i < integrationOrder; i++) {
				for (unsigned int j = 0; j < integrationOrder; j++) {
					xis[orderIndex][quadIndex] = gaussAbscissae[i];
					etas[orderIndex][quadIndex] = gaussAbscissae[j];
					weights[orderIndex][quadIndex] = gaussWeights[i] * gaussWeights[j];
					quadIndex++;
				}
			}
		}
	};
}