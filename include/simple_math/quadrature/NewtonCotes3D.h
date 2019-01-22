#pragma once

#include "NewtonCotes2D.h"

namespace simple_math
{
	class NewtonCotes3D : public NewtonCotes2D
	{
	public:
		NewtonCotes3D() : NewtonCotes2D() {
			for (unsigned int i = 0; i < MAX_NC_ORDER; i++) {
				zetas[i] = 0;
			}
		};

		virtual ~NewtonCotes3D() {
			for (unsigned int i = 0; i < MAX_NC_ORDER; i++) {
				if (zetas[i]) delete[] zetas[i];
			}
		}

		const double* getZetas(const unsigned int &integrationOrder)
		{
			unsigned int orderIndex = integrationOrder - 1;
			if (!calculated[orderIndex])
				calculate(integrationOrder);
			return zetas[orderIndex];
		}

	protected:
		double *zetas[MAX_NC_ORDER];

		virtual void calculate(const unsigned int &integrationOrder)
		{
			unsigned int orderIndex = integrationOrder - 1;
			if (calculated[orderIndex]) return;
			calculated[orderIndex] = true;

			// Setup some variables
			unsigned int length = integrationOrder * integrationOrder;
			// Allocate the vectors
			xis[orderIndex] = new double[length];
			etas[orderIndex] = new double[length];
			zetas[orderIndex] = new double[length];
			weights[orderIndex] = new double[length];
			unsigned int quadIndex = 0;
			for (unsigned int i = 0; i < integrationOrder; i++) {
				for (unsigned int j = 0; j < integrationOrder; j++) {
					for (unsigned int k = 0; k < integrationOrder; k++) {
						xis[orderIndex][i] = getCoord(i, integrationOrder);
						etas[orderIndex][i] = getCoord(j, integrationOrder);
						zetas[orderIndex][i] = getCoord(k, integrationOrder);
						weights[orderIndex][i] = getWeight(i, integrationOrder)*getWeight(j, integrationOrder)*getWeight(k, integrationOrder);
						quadIndex++;
					}
				}
			}
		}
	};
}