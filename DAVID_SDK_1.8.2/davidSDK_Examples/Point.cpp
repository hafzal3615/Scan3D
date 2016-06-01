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

	bool eq(Point3D lhs, Point3D rhs) {
		return lhs.x == rhs.x && lhs.y == rhs.y && lhs.z == rhs.z;
	}

	bool neq(Point3D lhs, Point3D rhs) {
		return !eq(lhs, rhs);
	}

	bool eq(Point2D lhs, Point2D rhs) {
		return lhs.x == rhs.x && lhs.y == rhs.y;
	}

	bool neq(Point2D lhs, Point2D rhs) {
		return !eq(lhs, rhs);
	}

	Point3D *copy(Point3D *pt) {
		return new Point3D(pt->x, pt->y, pt->z);
	}

	Point2D *copy(Point2D *pt) {
		return new Point2D(pt->x, pt->y);
	}

	/* Ugh, not sure why these stop the program from linking properly, to investigate later
	// Overloaded equality operator for 3D points, only true if all x, y and z are equal
	inline bool operator==(const Point3D& lhs, const Point3D& rhs) {
		return lhs.x == rhs.x && lhs.y == rhs.y && lhs.z == rhs.z;
	}

	// Overloaded inequality operator for 3D points
	inline bool operator!=(const Point3D& lhs, const Point3D& rhs) {
		return !operator==(lhs, rhs);
	}

	// Overloaded equality operator for 2D points, only true if x and y are equal
	inline bool operator==(const Point2D& lhs, const Point2D& rhs) {
		return lhs.x == rhs.x && lhs.y == rhs.y;
	}

	// Overloaded inequality operator for 2D points
	inline bool operator!=(const Point2D& lhs, const Point2D& rhs) {
		return !operator==(lhs, rhs);
	}
	*/
}