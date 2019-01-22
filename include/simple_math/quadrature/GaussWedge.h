#pragma once

#include "GaussTetrahedron.h"

namespace simple_math
{
	class GaussWedge : public GaussTetrahedron
	{
	public:
		GaussWedge() : GaussTetrahedron() {};
		virtual ~GaussWedge() {};

	protected:
		virtual void calculate(const unsigned int &integrationOrder)
		{
			unsigned int orderIndex = integrationOrder - 1;
			if (calculated[orderIndex]) return;
			calculated[orderIndex] = true;

			// Set the lengths
			switch (integrationOrder) {
			case 1:
				length[orderIndex] = 2;
				break;
			case 2:
				length[orderIndex] = 9;
				break;
			case 3:
				length[orderIndex] = 18;
				break;
			default:
				throw _UTIL_MSG_EXCEPT_C("Invalid integration order.");
			}

			xi[orderIndex] = new double[length[orderIndex]];
			eta[orderIndex] = new double[length[orderIndex]];
			zeta[orderIndex] = new double[length[orderIndex]];
			weight[orderIndex] = new double[length[orderIndex]];

			double val1, val2, val3, weightVal;
			switch (integrationOrder) {
			case 1:
				xi[orderIndex][0] = 1.0 / 3;
				eta[orderIndex][0] = 1.0 / 3;
				zeta[orderIndex][0] = 1.0 / sqrt(3.0);
				weight[orderIndex][0] = 1.0 / 2;

				xi[orderIndex][1] = 1.0 / 3;
				eta[orderIndex][1] = 1.0 / 3;
				zeta[orderIndex][1] = -1.0 / sqrt(3.0);
				weight[orderIndex][1] = 1.0 / 2;
				break;
			case 2:
				val1 = 1.0 / 6;
				val2 = 4.0 / 6;
				val3 = sqrt(3.0 / 5);
				weightVal = 5.0 / 54;

				xi[orderIndex][0] = val1;
				eta[orderIndex][0] = val1;
				zeta[orderIndex][0] = val3;
				weight[orderIndex][0] = weightVal;

				xi[orderIndex][1] = val1;
				eta[orderIndex][1] = val2;
				zeta[orderIndex][1] = val3;
				weight[orderIndex][1] = weightVal;

				xi[orderIndex][2] = val2;
				eta[orderIndex][2] = val1;
				zeta[orderIndex][2] = val3;
				weight[orderIndex][2] = weightVal;

				xi[orderIndex][3] = val1;
				eta[orderIndex][3] = val1;
				zeta[orderIndex][3] = -val3;
				weight[orderIndex][3] = weightVal;

				xi[orderIndex][4] = val1;
				eta[orderIndex][4] = val2;
				zeta[orderIndex][4] = -val3;
				weight[orderIndex][4] = weightVal;

				xi[orderIndex][5] = val2;
				eta[orderIndex][5] = val1;
				zeta[orderIndex][5] = -val3;
				weight[orderIndex][5] = weightVal;

				val3 = 0.0;
				weightVal = 8.0 / 54;

				xi[orderIndex][6] = val1;
				eta[orderIndex][6] = val1;
				zeta[orderIndex][6] = val3;
				weight[orderIndex][6] = weightVal;

				xi[orderIndex][7] = val1;
				eta[orderIndex][7] = val2;
				zeta[orderIndex][7] = val3;
				weight[orderIndex][7] = weightVal;

				xi[orderIndex][8] = val2;
				eta[orderIndex][8] = val1;
				zeta[orderIndex][8] = val3;
				weight[orderIndex][8] = weightVal;
				break;
			case 3:
				val1 = 1.0 / 6;
				val2 = 4.0 / 6;
				val3 = sqrt(3.0 / 5);
				weightVal = 1.0 / 12;

				xi[orderIndex][0] = val1;
				eta[orderIndex][0] = val1;
				zeta[orderIndex][0] = val3;
				weight[orderIndex][0] = weightVal;

				xi[orderIndex][1] = val1;
				eta[orderIndex][1] = val2;
				zeta[orderIndex][1] = val3;
				weight[orderIndex][1] = weightVal;

				xi[orderIndex][2] = val2;
				eta[orderIndex][2] = val1;
				zeta[orderIndex][2] = val3;
				weight[orderIndex][2] = weightVal;

				xi[orderIndex][3] = val1;
				eta[orderIndex][3] = val1;
				zeta[orderIndex][3] = -val3;
				weight[orderIndex][3] = weightVal;

				xi[orderIndex][4] = val1;
				eta[orderIndex][4] = val2;
				zeta[orderIndex][4] = -val3;
				weight[orderIndex][4] = weightVal;

				xi[orderIndex][5] = val2;
				eta[orderIndex][5] = val1;
				zeta[orderIndex][5] = -val3;
				weight[orderIndex][5] = weightVal;

				val3 = 0.0;
				weightVal = 2.0 / 15;

				xi[orderIndex][6] = val1;
				eta[orderIndex][6] = val1;
				zeta[orderIndex][6] = val3;
				weight[orderIndex][6] = weightVal;

				xi[orderIndex][7] = val1;
				eta[orderIndex][7] = val2;
				zeta[orderIndex][7] = val3;
				weight[orderIndex][7] = weightVal;

				xi[orderIndex][8] = val2;
				eta[orderIndex][8] = val1;
				zeta[orderIndex][8] = val3;
				weight[orderIndex][8] = weightVal;

				val1 = 1.0 / 2;
				val2 = 0.0;
				val3 = sqrt(3.0 / 5);
				weightVal = 1.0 / 108;

				xi[orderIndex][9] = val1;
				eta[orderIndex][9] = val1;
				zeta[orderIndex][9] = val3;
				weight[orderIndex][9] = weightVal;

				xi[orderIndex][10] = val1;
				eta[orderIndex][10] = val2;
				zeta[orderIndex][10] = val3;
				weight[orderIndex][10] = weightVal;

				xi[orderIndex][11] = val2;
				eta[orderIndex][11] = val1;
				zeta[orderIndex][11] = val3;
				weight[orderIndex][11] = weightVal;

				xi[orderIndex][12] = val1;
				eta[orderIndex][12] = val1;
				zeta[orderIndex][12] = -val3;
				weight[orderIndex][12] = weightVal;

				xi[orderIndex][13] = val1;
				eta[orderIndex][13] = val2;
				zeta[orderIndex][13] = -val3;
				weight[orderIndex][13] = weightVal;

				xi[orderIndex][14] = val2;
				eta[orderIndex][14] = val1;
				zeta[orderIndex][14] = -val3;
				weight[orderIndex][14] = weightVal;

				val3 = 0.0;
				weightVal = 2.0 / 135;

				xi[orderIndex][15] = val1;
				eta[orderIndex][15] = val1;
				zeta[orderIndex][15] = val3;
				weight[orderIndex][15] = weightVal;

				xi[orderIndex][16] = val1;
				eta[orderIndex][16] = val2;
				zeta[orderIndex][16] = val3;
				weight[orderIndex][16] = weightVal;

				xi[orderIndex][17] = val2;
				eta[orderIndex][17] = val1;
				zeta[orderIndex][17] = val3;
				weight[orderIndex][17] = weightVal;
				break;
			}
		}
	};
}