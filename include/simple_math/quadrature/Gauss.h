#pragma once

#include "utility/Exceptions.h"

#ifndef _USE_MATH_DEFINES
#define _USE_MATH_DEFINES
#endif
#include <math.h>

namespace simple_math
{
	static const unsigned int MAX_GAUSS_ORDER = 20;

	class Gauss
	{
	public:
		/// Default constructor just iniailizes the arrays of pointers to be null
		Gauss() {
			for (unsigned int i = 0; i < MAX_GAUSS_ORDER; i++) {
				abscissae[i] = 0;
				weights[i] = 0;
				calculated[i] = false;
			}
		};
		/// Destructor cleans up any memory allocated
		~Gauss() {
			for (unsigned int i = 0; i < MAX_GAUSS_ORDER; i++) {
				if (abscissae[i]) delete[] abscissae[i];
				if (weights[i]) delete[] weights[i];
			}
		};

		/// Get a pointer to the abscissae.  Make a copy of it or use, but
		/// don't try to modify it outside this class.
		const double * getAbscissae(unsigned int integrationOrder) {
			unsigned int orderIndex = integrationOrder - 1;
			if (!calculated[orderIndex])
				calculate(integrationOrder);
			return abscissae[orderIndex];
		};

		/// Get a pointer to the weights.  Make a copy of it or use, but
		/// don't try to modify it outside this class.
		const double * getWeights(unsigned int integrationOrder) {
			unsigned int orderIndex = integrationOrder - 1;
			if (!calculated[orderIndex])
				calculate(integrationOrder);
			return weights[orderIndex];
		};

	private:
		double *abscissae[MAX_GAUSS_ORDER];
		double *weights[MAX_GAUSS_ORDER];
		bool calculated[MAX_GAUSS_ORDER];

		/// A helper method to calculat the Gauss-Legrendre weights and abcissae
		void calculate(unsigned int desiredOrder) {
			if (desiredOrder > MAX_GAUSS_ORDER || desiredOrder < 1)
				throw _UTIL_MSG_EXCEPT("Gauss was asked to calculate either too small or too large of an integration order: " + std::to_string(desiredOrder));

			unsigned int orderIndex = desiredOrder - 1;
			double EPS = 3e-15;

			if (calculated[orderIndex]) return;
			calculated[orderIndex] = true;

			abscissae[orderIndex] = new double[desiredOrder];
			weights[orderIndex] = new double[desiredOrder];
			int m = (desiredOrder + 1) / 2;
			double xm = 0;
			double xl = 1;
			for (int i = 0; i < m; i++)
			{
				double z = cos(M_PI*((double)i + 1 - .25) / ((double)desiredOrder + .5));
				double z1;
				double PP;
				do {
					double P1 = 1;
					double P2 = 0;
					for (unsigned int j = 0; j < desiredOrder; j++)
					{
						double P3 = P2;
						P2 = P1;
						P1 = ((2 * j + 1)*z*P2 - (j)*P3) / (j + 1);
					}
					PP = desiredOrder * (z*P1 - P2) / (z*z - 1);
					z1 = z;
					z = z1 - P1 / PP;
				} while (fabs(z - z1) > EPS);
				abscissae[orderIndex][i] = xm - xl * z;
				abscissae[orderIndex][desiredOrder - i - 1] = xm + xl * z;
				weights[orderIndex][i] = 2 * xl / ((1 - z * z)*PP*PP);
				weights[orderIndex][desiredOrder - i - 1] = weights[orderIndex][i];
			}
		};
	};

}