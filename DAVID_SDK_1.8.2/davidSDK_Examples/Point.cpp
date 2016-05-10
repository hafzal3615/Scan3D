#include "Point.h"

namespace examples {

	Point3D::Point3D(double a, double b, double c) {
		x = a;
		y = b;
		z = c;
	}

	Point3D::Point3D() {
		x = 0;
		y = 0;
		z = 0;
	}

	Point2D::Point2D(double a, double b) {
		x = a;
		y = b;
	}

	Point2D::Point2D() {
		x = 0;
		y = 0;
	}
}