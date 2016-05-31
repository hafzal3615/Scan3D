#include <list>
#include <string>
#include <vector>
#include <sstream>
#include "Point.h"
#include "Plane.h"
#include "helpers.h"

namespace examples {

	void main_Perimetre();

	std::vector<Point3D *> getpts(const std::string& filename, int num_pts);
	std::vector<Point3D *> getpts_obj(const std::string& filename, int num_pts);
	std::vector<Point3D *> getpts_asc(const std::string& filename, int num_pts);

	int count_lines(const std::string & filename);

	double dist_ptplane(Point3D pt, Plane pl);
	std::vector<Point3D *> section(std::vector<Point3D *> *cloud, 
		const Plane& pl, const double& eps);
	double scalar_product(Point3D a, Point3D b);
	double scalar_product(Point2D a, Point2D b);
	double norm(Point3D a);
	double norm(Point2D a);
	double dist(Point3D a, Point3D b);
	double dist(Point2D a, Point2D b);
	// proj
	double dist_line(Point2D a, Point3D ln);
	Point3D bisector(Point2D a, Point2D b);
	Point3D tangent(Point2D a, Point2D b);

	double det(Point2D a, Point2D b, Point2D c);
	bool is_rightturn(Point2D a, Point2D b, Point2D c);
	std::list<Point2D> convex_hull(std::list<Point2D> cloud);
	// convex_2

	std::list<Point2D> seg_tighten(Point2D u, Point2D v,
		std::list<Point2D> cloud, double eta, double mu);
	std::list<Point2D> tighten(std::list<Point2D> hull,
		std::list<Point2D> cloud, double eps, double eta, double mu);
	double perimeter(std::list<Point2D> cloud);

} // namespace examples
#pragma once
