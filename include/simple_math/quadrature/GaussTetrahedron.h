#pragma once

#include "GaussTriangle.h"

namespace simple_math
{
	class GaussTetrahedron : public GaussTriangle
	{
	public:
		GaussTetrahedron() : GaussTriangle() {
			for (unsigned int i = 0; i < MAX_GAUSS_TRI_ORDER; i++) {
				zeta[i] = nullptr;
			}
		}

		virtual ~GaussTetrahedron() {
			for (unsigned int i = 0; i < MAX_GAUSS_TRI_ORDER; i++) {
				if (zeta[i]) delete[] zeta[i];
			}
		};

		const double* getZetas(const unsigned int &integrationOrder)
		{
			unsigned int orderIndex = integrationOrder - 1;
			if (!calculated[orderIndex])
				calculate(integrationOrder);
			return zeta[orderIndex];
		}

	protected:

		double* zeta[MAX_GAUSS_TRI_ORDER];

		virtual void calculate(const unsigned int &integrationOrder)
		{
			unsigned int orderIndex = integrationOrder - 1;
			if (calculated[orderIndex]) return;
			calculated[orderIndex] = true;

			// Set the lengths
			switch (integrationOrder) {
			case 1:
				length[orderIndex] = 1;
				break;
			case 2:
				length[orderIndex] = 4;
				break;
			case 3:
				length[orderIndex] = 15;
				break;
			default:
				throw _UTIL_MSG_EXCEPT_C("Invalid integration order.");
			}

			xi[orderIndex] = new double[length[orderIndex]];
			eta[orderIndex] = new double[length[orderIndex]];
			zeta[orderIndex] = new double[length[orderIndex]];
			weight[orderIndex] = new double[length[orderIndex]];

			double val1, val2, weightVal;
			switch (integrationOrder) {
			case 1:
				xi[orderIndex][0] = 0.25;
				eta[orderIndex][0] = 0.25;
				zeta[orderIndex][0] = 0.25;
				weight[orderIndex][0] = 1.0 / 6;
				break;
			case 2:
				val1 = (5.0 - sqrt(5.0)) / 20;
				val2 = (5.0 + 3 * sqrt(5.0)) / 20;
				xi[orderIndex][0] = val1;
				eta[orderIndex][0] = val1;
				zeta[orderIndex][0] = val1;
				weight[orderIndex][0] = 1.0 / 24;

				xi[orderIndex][1] = val2;
				eta[orderIndex][1] = val1;
				zeta[orderIndex][1] = val1;
				weight[orderIndex][1] = 1.0 / 24;

				xi[orderIndex][2] = val1;
				eta[orderIndex][2] = val2;
				zeta[orderIndex][2] = val1;
				weight[orderIndex][2] = 1.0 / 24;

				xi[orderIndex][3] = val1;
				eta[orderIndex][3] = val1;
				zeta[orderIndex][3] = val2;
				weight[orderIndex][3] = 1.0 / 24;
				break;
			case 3:
				xi[orderIndex][0] = 0.25;
				eta[orderIndex][0] = 0.25;
				zeta[orderIndex][0] = 0.25;
				weight[orderIndex][0] = 16.0 / 810;

				val1 = (7.0 - sqrt(15.0)) / 34;
				val2 = (13.0 + 3.0*sqrt(15.0)) / 34;
				weightVal = (2665.0 + 14.0*sqrt(15.0)) / 226800;

				xi[orderIndex][1] = val1;
				eta[orderIndex][1] = val1;
				zeta[orderIndex][1] = val1;
				weight[orderIndex][1] = weightVal;

				xi[orderIndex][2] = val2;
				eta[orderIndex][2] = val1;
				zeta[orderIndex][2] = val1;
				weight[orderIndex][2] = weightVal;

				xi[orderIndex][3] = val1;
				eta[orderIndex][3] = val2;
				zeta[orderIndex][3] = val1;
				weight[orderIndex][3] = weightVal;

				xi[orderIndex][4] = val1;
				eta[orderIndex][4] = val1;
				zeta[orderIndex][4] = val2;
				weight[orderIndex][4] = weightVal;

				val1 = (7.0 + sqrt(15.0)) / 34;
				val2 = (13.0 - 3.0*sqrt(15.0)) / 34;
				weightVal = (2665.0 - 14.0*sqrt(15.0)) / 226800;

				xi[orderIndex][5] = val1;
				eta[orderIndex][5] = val1;
				zeta[orderIndex][5] = val1;
				weight[orderIndex][5] = weightVal;

				xi[orderIndex][6] = val2;
				eta[orderIndex][6] = val1;
				zeta[orderIndex][6] = val1;
				weight[orderIndex][6] = weightVal;

				xi[orderIndex][7] = val1;
				eta[orderIndex][7] = val2;
				zeta[orderIndex][7] = val1;
				weight[orderIndex][7] = weightVal;

				xi[orderIndex][8] = val1;
				eta[orderIndex][8] = val1;
				zeta[orderIndex][8] = val2;
				weight[orderIndex][8] = weightVal;

				val1 = (10.0 - 2.0*sqrt(15.0)) / 40;
				val2 = (10.0 + 2.0*sqrt(15.0)) / 40;
				weightVal = 20.0 / 2268;

				xi[orderIndex][9] = val2;
				eta[orderIndex][9] = val1;
				zeta[orderIndex][9] = val1;
				weight[orderIndex][9] = weightVal;

				xi[orderIndex][10] = val1;
				eta[orderIndex][10] = val2;
				zeta[orderIndex][10] = val1;
				weight[orderIndex][10] = weightVal;

				xi[orderIndex][11] = val1;
				eta[orderIndex][11] = val1;
				zeta[orderIndex][11] = val2;
				weight[orderIndex][11] = weightVal;

				xi[orderIndex][12] = val1;
				eta[orderIndex][12] = val2;
				zeta[orderIndex][12] = val2;
				weight[orderIndex][12] = weightVal;

				xi[orderIndex][13] = val2;
				eta[orderIndex][13] = val1;
				zeta[orderIndex][13] = val2;
				weight[orderIndex][13] = weightVal;

				xi[orderIndex][14] = val2;
				eta[orderIndex][14] = val2;
				zeta[orderIndex][14] = val1;
				weight[orderIndex][14] = weightVal;
				break;
			}
		}
	};
}