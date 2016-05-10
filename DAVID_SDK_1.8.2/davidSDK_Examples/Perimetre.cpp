#include "davidSDK/david.h"
#include <vector>
#include <time.h>
#include "Perimetre.h"

namespace examples {

	void main_Perimetre() {
		
	}

	std::list<Point3D> getpts(const std::string& filename) {

	}

	std::list<Point3D> getpts_obj(const std::string & filename)
	{
		return std::list<Point3D>();
	}

	std::list<Point3D> getpts_asc(const std::string & filename)
	{
		return std::list<Point3D>();
	}

	double dist_ptplane(Point3D pt, Plane pl)
	{
		return 0.0;
	}

	std::list<Point3D> section(const std::list<Point3D>& cloud, const Plane & pl, const double & eps)
	{
		return std::list<Point3D>();
	}

	double scalar_product(Point3D a, Point3D b)
	{
		return 0.0;
	}

	double norm(Point3D a)
	{
		return 0.0;
	}

	double dist(Point3D a, Point3D b)
	{
		return 0.0;
	}

	double dist(Point2D a, Point2D b)
	{
		return 0.0;
	}

	double dist_line(Point2D a, Point3D ln)
	{
		return 0.0;
	}

	Point3D bisector(Point2D a, Point2D b)
	{
		return Point3D();
	}

	Point3D tangent(Point2D a, Point2D b)
	{
		return Point3D();
	}

	double det(Point2D a, Point2D b, Point2D c)
	{
		return 0.0;
	}

	bool is_rightturn(Point2D a, Point2D b, Point2D c)
	{
		return false;
	}

	std::list<Point2D> convex_hull(std::list<Point2D> cloud)
	{
		return std::list<Point2D>();
	}

	std::list<Point2D> seg_tighten(Point2D u, Point2D v, std::list<Point2D> cloud, double eta, double mu)
	{
		return std::list<Point2D>();
	}

	std::list<Point2D> tighten(std::list<Point2D> hull, std::list<Point2D> cloud, double eps, double eta, double mu)
	{
		return std::list<Point2D>();
	}

	double perimiter(std::list<Point2D> cloud)
	{
		return 0.0;
	}


} // namespace examples
