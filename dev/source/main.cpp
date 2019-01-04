#include <chrono>
#include <functional>
#include <iostream>
#include <iomanip>

#include "simple_math/Utility.h"

using namespace simple_math;

int main()
{
	Point a = { 0, 0, 0 };
	Point b = { 1, 0, 0 };
	bool result = compare_points(a, b, 1e-12);

	//system("pause");
	return 0;
}