#include "davidSDK/david.h"
#include <vector>
#include <time.h>
#include <fstream>
#include "Perimetre.h"

namespace examples {

	void main_Perimetre() {
		std::vector<Point3D> points = getpts("C:\\Users\\Jay\\Documents\\Scans\\front_1_3.obj");
	}

	/* Gets a cloud of points from the file at the specified filename
	*	Can handle both .asc and .obj files by using the appropriate method
	*/
	std::vector<Point3D> getpts(const std::string& filename) {
		std::size_t pos = filename.find('.');
		if (pos != std::string::npos) {
			std::string ext = filename.substr(pos, 4);

			if (ext == ".asc") {
				return getpts_asc(filename);
			} 
			else if (ext == ".obj") {
				return getpts_obj(filename);
			}
			else {
				// throw error
				return std::vector<Point3D>();
			}
		} 
		else {
			// Change this to throw an error
			return std::vector<Point3D>();
		}
	}

	std::vector<Point3D> getpts_obj(const std::string & filename) {
		std::ifstream input("filename");
		std::string line_data;

		std::vector<Point3D> points = std::vector<Point3D>();

		for (int count = 0; std::getline(input, line_data, '#'); ++count) {
			std::vector<std::string> elems;

			split(line_data, ' ', elems);

			if (elems.at(0) == "v") {
				// The line is valid (it starts with a v)
				// These are pretty bad, they don't do any checks on whats in the line
				points[count].x = std::stod("foo", NULL);
				std::cout << points[count].x;
				points[count].y = std::stod(elems.at(2), NULL);
				points[count].z = std::stod(elems.at(3), NULL);
			} 
			else {
				// Invalid line, throw some error
			}
		}

		return points;
	}

	std::vector<Point3D> getpts_asc(const std::string & filename) {
		std::ifstream input("filename");
		std::string line_data;

		while (getline(input, line_data, '#')) {

		}

		return std::vector<Point3D>();
	}

	double dist_ptplane(Point3D pt, Plane pl)
	{
		return 0.0;
	}

	std::list<Point3D> section(const std::list<Point3D>& cloud, const Plane & pl, const double & eps)
	{
		return std::list<Point3D>();
	}

	// Gets the scalar product of two 3-vectors
	double scalar_product(Point3D a, Point3D b)
	{
		return a.x * b.x + a.y * b.y + a.z * b.z;
	}

	// Gets the scalar product of two 2-vectors
	double scalar_product(Point2D a, Point2D b)
	{
		return a.x * b.x + a.y * b.y;
	}

	// Gets the norm of a 3-vector
	double norm(Point3D a)
	{
		return scalar_product(a, a);
	}

	// Gets the norm of a 2-vector
	double norm(Point2D a)
	{
		return scalar_product(a, a);
	}

	// Gets the distance between two 3D points a and b
	double dist(Point3D a, Point3D b)
	{
		// Compute the magnitude of a - b (or b - a)
		Point3D c = Point3D(a.x - b.x, a.y - b.y, a.z - b.z);
		return norm(c);
	}

	// Gets the distance between two 2D points a and b
	double dist(Point2D a, Point2D b)
	{
		Point2D c = Point2D(a.x - b.x, a.y - b.y);
		return norm(c);
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
