#include <list>
#include <string>
#include <vector>
#include <sstream>
#include "Point.h"
#include "Plane.h"

namespace examples {

	void main_Perimetre();

	std::list<Point3D> getpts(const std::string& filename);
	std::list<Point3D> getpts_obj(const std::string& filename);
	std::list<Point3D> getpts_asc(const std::string& filename);

	std::vector<std::string> split(const std::string &s, char delim);

	double dist_ptplane(Point3D pt, Plane pl);
	std::list<Point3D> section(const std::list<Point3D>& cloud, 
		const Plane& pl, const double& eps);
	double scalar_product(Point3D a, Point3D b);
	double scalar_product(Point2D a, Point2D b);
	double norm(Point3D a);
	double norm(Point2D a);
	double dist(Point3D a, Point3D b);
	double dist(Point2D a, Point2D b);
	double dist_line(Point2D a, Point3D ln);
	Point3D bisector(Point2D a, Point2D b);
	Point3D tangent(Point2D a, Point2D b);

	double det(Point2D a, Point2D b, Point2D c);
	bool is_rightturn(Point2D a, Point2D b, Point2D c);
	std::list<Point2D> convex_hull(std::list<Point2D> cloud);

	std::list<Point2D> seg_tighten(Point2D u, Point2D v,
		std::list<Point2D> cloud, double eta, double mu);
	std::list<Point2D> tighten(std::list<Point2D> hull,
		std::list<Point2D> cloud, double eps, double eta, double mu);
	double perimiter(std::list<Point2D> cloud);

} // namespace examples
#pragma once
