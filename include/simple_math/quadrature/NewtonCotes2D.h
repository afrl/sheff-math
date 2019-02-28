#pragma once

#include "NewtonCotes1D.h"

namespace simple_math
{
	class NewtonCotes2D : public NewtonCotes1D
	{
	public:
		NewtonCotes2D() : NewtonCotes1D() {
			for (unsigned int i = 0; i < MAX_NC_ORDER; i++) {
				etas[i] = nullptr;
			}
		};

		virtual ~NewtonCotes2D() {
			for (unsigned int i = 0; i < MAX_NC_ORDER; i++) {
				if (etas[i]) delete[] etas[i];
			}
		}

		const double* getEtas(const unsigned int &integrationOrder)
		{
			unsigned int orderIndex = integrationOrder - 1;
			if (!calculated[orderIndex])
				calculate(integrationOrder);
			return etas[orderIndex];
		}

	protected:
		double *etas[MAX_NC_ORDER];

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
			weights[orderIndex] = new double[length];
			unsigned int quadIndex = 0;
			for (unsigned int i = 0; i < integrationOrder; i++) {
				for (unsigned int j = 0; j < integrationOrder; j++) {
					xis[orderIndex][i] = getCoord(i, integrationOrder);
					etas[orderIndex][i] = getCoord(j, integrationOrder);
					weights[orderIndex][i] = getWeight(i, integrationOrder)*getWeight(j, integrationOrder);
					quadIndex++;
				}
			}
		}
	};
}