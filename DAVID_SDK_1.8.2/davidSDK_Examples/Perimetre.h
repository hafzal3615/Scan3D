#include <list>
#include <string>
#include <vector>
#include <sstream>
#include <math.h>
#include "Point.h"
#include "Plane.h"
#include "helpers.h"

namespace examples {

	void main_Perimetre();

	std::vector<Point3D *> getpts(const std::string& filename, int num_pts, int total_pts, bool verbose);
	std::vector<Point3D *> getpts_obj(const std::string& filename, int step_len, bool verbose);
	std::vector<Point3D *> getpts_asc(const std::string& filename, int step_len, bool verbose);

	double dist_ptplane(Point3D pt, Plane pl);
	std::vector<Point3D *> section(std::vector<Point3D *> *cloud, 
		const Plane& pl, const double& eps);
	double scalar_product(Point3D a, Point3D b);
	double scalar_product(Point2D a, Point2D b);
	double norm(Point3D a);
	double norm(Point2D a);
	double dist(Point3D a, Point3D b);
	double dist(Point2D a, Point2D b);
	std::vector<Point2D *> projection(std::vector<Point3D *> cloud, Plane plane);
	double dist_line(Point2D a, Point3D ln);
	Point3D bisector(Point2D a, Point2D b);
	Point3D tangent(Point2D a, Point2D b);

	double det(Point2D a, Point2D b, Point2D c);
	bool is_rightturn(Point2D a, Point2D b, Point2D c);
	bool is_rightturn(std::vector<Point2D *> pts);
	std::vector<Point2D *> convex_hull(std::vector<Point2D *> cloud);
	// convex_2

	std::vector<Point2D *> seg_tighten(Point2D u, Point2D v,
		std::vector<Point2D *> cloud, double eps, double eta, double mu);
	std::vector<Point2D *> tighten(std::vector<Point2D *> hull,
		std::vector<Point2D *> cloud, double eps, double eta, double mu);
	double perimeter(std::vector<Point2D *> cloud);
	double calculate_perimeter(std::string filename, Plane plane, double eps1,
		double eps2, double eta, double mu, int num_points, int filter_points, bool verbose);

} // namespace examples
#pragma once
