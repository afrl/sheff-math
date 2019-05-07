#pragma once

#include "GaussTetrahedron.h"

namespace simple_math
{
	class GaussWedge : public GaussTetrahedron
	{
	public:
		GaussWedge() : GaussTetrahedron() {};
		virtual ~GaussWedge() {};

        virtual void setup(const unsigned int integration_order) override;
	};
}