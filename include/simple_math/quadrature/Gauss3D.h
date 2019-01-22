#pragma once

#include "Gauss2D.h"

namespace simple_math
{
	/// A helper class that uses the Gauss class to create a list of
	/// locations and weights for each quad point.  In the case of 2D
	/// this would create a list of integrationOrder^2 long.
	class Gauss3D : public Gauss2D
	{
	public:

		Gauss3D() : Gauss2D() {
			for (unsigned int i = 0; i < MAX_GAUSS_ORDER; i++) {
				zetas[i] = 0;
			}
		};

		virtual ~Gauss3D() {
			for (unsigned int i = 0; i < MAX_GAUSS_ORDER; i++) {
				if (zetas[i]) delete[] zetas[i];
			}
		}

		const double* getZetas(const unsigned int &integrationOrder, Gauss &gauss)
		{
			unsigned int orderIndex = integrationOrder - 1;
			if (!calculated[orderIndex])
				calculate(integrationOrder, gauss);
			return zetas[orderIndex];
		}

	protected:
		double *zetas[MAX_GAUSS_ORDER];

		virtual void calculate(const unsigned int &integrationOrder, Gauss &gauss)
		{
			unsigned int orderIndex = integrationOrder - 1;
			if (calculated[orderIndex]) return;
			calculated[orderIndex] = true;

			// Setup some variables
			unsigned int length = integrationOrder * integrationOrder*integrationOrder;
			const double * gaussAbscissae = gauss.getAbscissae(integrationOrder);
			const double * gaussWeights = gauss.getWeights(integrationOrder);
			// Allocate the vectors
			xis[orderIndex] = new double[length];
			etas[orderIndex] = new double[length];
			zetas[orderIndex] = new double[length];
			weights[orderIndex] = new double[length];
			// Set the values
			unsigned int quadIndex = 0;
			for (unsigned int i = 0; i < integrationOrder; i++) {
				for (unsigned int j = 0; j < integrationOrder; j++) {
					for (unsigned int k = 0; k < integrationOrder; k++) {
						xis[orderIndex][quadIndex] = gaussAbscissae[i];
						etas[orderIndex][quadIndex] = gaussAbscissae[j];
						zetas[orderIndex][quadIndex] = gaussAbscissae[k];
						weights[orderIndex][quadIndex] = gaussWeights[i] * gaussWeights[j] * gaussWeights[k];
						quadIndex++;
					}
				}
			}
		}
	};
}