#include "Plane.h"

namespace examples {
	Plane::Plane(double w, double x, double y, double z) {
		a = w;
		b = x;
		c = y;
		d = z;
	}

	Plane::Plane() {
		a = 0;
		b = 0;
		c = 0;
		d = 0;
	}
}