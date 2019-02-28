#pragma once

#include "utility/Exceptions.h"

namespace simple_math
{
	class GaussTriangle
	{
	public:
		/// Just precalculate all of these since there are only so few supported
		GaussTriangle() {
			for (unsigned int i = 0; i < MAX_GAUSS_TRI_ORDER; i++) {
				length[i] = 0;
				xi[i] = nullptr;
				eta[i] = nullptr;
				weight[i] = nullptr;
				calculated[i] = false;
			}
		};

		virtual ~GaussTriangle() {
			for (unsigned int i = 0; i < MAX_GAUSS_TRI_ORDER; i++) {
				if (xi[i]) delete[] xi[i];
				if (eta[i]) delete[] eta[i];
				if (weight[i]) delete[] weight[i];
			}
		};

		const double* getXis(const unsigned int &integrationOrder)
		{
			unsigned int orderIndex = integrationOrder - 1;
			if (!calculated[orderIndex])
				calculate(integrationOrder);
			return xi[orderIndex];
		}

		const double* getEtas(const unsigned int &integrationOrder)
		{
			unsigned int orderIndex = integrationOrder - 1;
			if (!calculated[orderIndex])
				calculate(integrationOrder);
			return eta[orderIndex];
		}

		const double* getWeights(const unsigned int &integrationOrder)
		{
			unsigned int orderIndex = integrationOrder - 1;
			if (!calculated[orderIndex])
				calculate(integrationOrder);
			return weight[orderIndex];
		}

		unsigned int getNumQuads(const unsigned int &integrationOrder)
		{
			unsigned int orderIndex = integrationOrder - 1;
			if (!calculated[orderIndex])
				calculate(integrationOrder);
			return length[orderIndex];
		}

	protected:
		static const int MAX_GAUSS_TRI_ORDER = 4;

		unsigned int length[MAX_GAUSS_TRI_ORDER];
		double *xi[MAX_GAUSS_TRI_ORDER];
		double *eta[MAX_GAUSS_TRI_ORDER];
		double *weight[MAX_GAUSS_TRI_ORDER];
		bool calculated[MAX_GAUSS_TRI_ORDER];

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
				length[orderIndex] = 3;
				break;
			case 3:
				length[orderIndex] = 4;
				break;
			case 4:
				length[orderIndex] = 7;
				break;
			default:
				throw _UTIL_MSG_EXCEPT_C("Invalid integration order.");
			}

			xi[orderIndex] = new double[length[orderIndex]];
			eta[orderIndex] = new double[length[orderIndex]];
			weight[orderIndex] = new double[length[orderIndex]];

			double val1, val2, weightVal;
			switch (integrationOrder) {
			case 1:
				xi[orderIndex][0] = 1.0 / 3;
				eta[orderIndex][0] = 1.0 / 3;
				weight[orderIndex][0] = 0.5;
				break;

			case 2:
				val1 = 0.5;
				val2 = 0.0;
				weightVal = 1.0 / 6;

				xi[orderIndex][0] = val1;
				eta[orderIndex][0] = val1;
				weight[orderIndex][0] = weightVal;

				xi[orderIndex][1] = val1;
				eta[orderIndex][1] = val2;
				weight[orderIndex][1] = weightVal;

				xi[orderIndex][2] = val2;
				eta[orderIndex][2] = val1;
				weight[orderIndex][2] = weightVal;
				break;

			case 3:
				val1 = 0.2;
				val2 = 0.6;
				weightVal = 25.0 / 96;

				xi[orderIndex][0] = val1;
				eta[orderIndex][0] = val1;
				weight[orderIndex][0] = weightVal;

				xi[orderIndex][1] = val1;
				eta[orderIndex][1] = val2;
				weight[orderIndex][1] = weightVal;

				xi[orderIndex][2] = val2;
				eta[orderIndex][2] = val1;
				weight[orderIndex][2] = weightVal;


				xi[orderIndex][3] = 1.0 / 3;
				eta[orderIndex][3] = 1.0 / 3;
				weight[orderIndex][3] = -27.0 / 96;
				break;

			case 4:
				xi[orderIndex][0] = 1.0 / 3;
				eta[orderIndex][0] = 1.0 / 3;
				weight[orderIndex][0] = 0.225*0.5;


				val1 = 0.101286507323;
				val2 = 0.797426985353;
				weightVal = 0.125939180544 * 0.5;

				xi[orderIndex][1] = val1;
				eta[orderIndex][1] = val1;
				weight[orderIndex][1] = weightVal;

				xi[orderIndex][2] = val1;
				eta[orderIndex][2] = val2;
				weight[orderIndex][2] = weightVal;

				xi[orderIndex][3] = val2;
				eta[orderIndex][3] = val1;
				weight[orderIndex][3] = weightVal;


				val1 = 0.470142064105;
				val2 = 0.059715871789;
				weightVal = 0.132394152788 * 0.5;

				xi[orderIndex][4] = val1;
				eta[orderIndex][4] = val1;
				weight[orderIndex][4] = weightVal;

				xi[orderIndex][5] = val1;
				eta[orderIndex][5] = val2;
				weight[orderIndex][5] = weightVal;

				xi[orderIndex][6] = val2;
				eta[orderIndex][6] = val1;
				weight[orderIndex][6] = weightVal;
				break;
			}
		};
	};
}