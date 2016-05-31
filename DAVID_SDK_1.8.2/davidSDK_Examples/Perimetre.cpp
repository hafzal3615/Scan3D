#include "davidSDK/david.h"
#include <vector>
#include <time.h>
#include <fstream>
#include "Perimetre.h"

namespace examples {

	void main_Perimetre() {
		//std::vector<Point3D *> points = getpts("C:\\Users\\Jay\\Documents\\Scans\\3d.asc", 1000, 159781);
	}

	/* Gets a cloud of points from the file at the specified filename
	*	Can handle both .asc and .obj files by using the appropriate method
	*/
	std::vector<Point3D *> getpts(const std::string& filename, int num_pts, int total_pts) {
		std::size_t pos = filename.find('.');
		if (pos != std::string::npos) {
			std::string ext = filename.substr(pos, 4);

			if (ext == ".asc") {
				return getpts_asc(filename, (int)std::round(total_pts / num_pts));
			} 
			else if (ext == ".obj") {
				return getpts_obj(filename, (int)std::round(total_pts / num_pts));
			}
			else {
				// throw error
				return std::vector<Point3D *>();
			}
		} 
		else {
			// Change this to throw an error
			return std::vector<Point3D *>();
		}
	}

	/* Reads in points from a .obj file and converts them into a vector of 3D points
		Points must be in the format 
		v <x> <y> <z> 
		Lines starting with any character other than v are ignored, x, y and z indicate 
		a floating point number, with . as a separator
	*/
	std::vector<Point3D *> getpts_obj(const std::string & filename, int step_len) {
		std::ifstream input(filename);
		std::string line_data;

		std::vector<Point3D *> points = std::vector<Point3D *>();

		bool flag = false;
		int count = 0;

		while (std::getline(input, line_data) && !flag) {
			std::vector<std::string> elems;
			split(line_data, ' ', elems);

			if (count % step_len == 0 && elems.size() != 0 && elems.at(0) != "#") {
				// Is not a comment or an empty line

				if (elems.at(0) == "v") {
					// The line is valid (it starts with a v)
					// These are pretty bad, they don't do any checks on whats in the line
					double x = std::stod(elems.at(1), NULL);
					double y = std::stod(elems.at(2), NULL);
					double z = std::stod(elems.at(3), NULL);

					Point3D *tmp_point = new Point3D(x, y, z);
					points.push_back(tmp_point);

					count++;
				}
				else if (elems.at(0) == "vt") {
					// Done with the normal nodes, leave the loop
					flag = true;
				}
				else {
					// Invalid line, throw some error
				}
				count++;
			}
			else {
				// Comment or empty line, do nothing
			}
		}

		return points;
	}

	/* Reads in a .asc file and converts the contents of the file into a vector of 3D points
		Points must be in the format
		X <x> Y <y> Z <z>
		Lines starting with anything else may crash the program. Points that use a comma instead
		of a period as a decimal separator will be converted
	*/
	std::vector<Point3D *> getpts_asc(const std::string & filename, int step_len) {
		std::ifstream input(filename);
		std::string line_data;

		std::vector<Point3D *> points = std::vector<Point3D *>();
		int count = 0;

		while (getline(input, line_data)) {
			std::vector<std::string> elems;

			split(line_data, ' ', elems);
			
			if (count % step_len == 0 && elems.size() == 6) {
				double x, y, z;
				std::string xstr, ystr, zstr;

				// Again, we don't do checks because we're stupid and lazy
				if (elems.at(0) == "X") {
					xstr = examples::myreplace(elems.at(1), ",", ".");
					x = std::stod(elems.at(1), NULL);
				}
				if (elems.at(2) == "Y") {
					ystr = examples::myreplace(elems.at(3), ",", ".");
					y = std::stod(elems.at(3), NULL);
				}
				if (elems.at(4) == "Z") {
					zstr = examples::myreplace(elems.at(5), ",", ".");
					z = std::stod(elems.at(5), NULL);
				}

				Point3D *tmp_point = new Point3D(x, y, z);
				points.push_back(tmp_point);
				std::cout << points.size() << "\n";
			}
			else {
				// Badly formatted line, throw some error
			}

			++count;
		}

		return points;
	}

	// Return the shortest distance from a point to a plane
	double dist_ptplane(Point3D pt, Plane pl)
	{
		double r1 = abs(pl.a * pt.x + pl.b * pt.y + pl.c * pt.z + pl.d);
		double r2 = sqrt(pl.a * pl.a + pl.b * pl.b + pl.c * pl.c);

		return(r1 / r2);
	}

	/* Goes through a vector list of points and returns only those points within a certain 
		distance of a plane. Returns a copy not a view into the original list
	*/
	std::vector<Point3D *> section(std::vector<Point3D *> *cloud, const Plane & pl, const double & eps)
	{
		std::vector<Point3D *> new_cloud = std::vector<Point3D *>();
		int size = (int)(cloud->size());

		for (int count = 0; count < size; ++count) {
			Point3D pt = *(cloud->at(count));

			if (dist_ptplane(pt, pl) < eps) {
				// copy point into new vector
				// This is probably pretty memory inneficient, if we knew the original cloud won't
				//		change then we could just have your sectioned cloud be a 'view' into the original
				new_cloud.push_back(new Point3D(pt.x, pt.y, pt.z));
			}
			else {
				// discard the point
			}
		}

		return new_cloud;
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

	// Gets the distance between a point a and a line ln represented by a Point3D
	double dist_line(Point2D a, Point3D ln)
	{
		double r1 = abs(ln.x * a.x + ln.y * a.y + ln.z);
		double r2 = sqrt(ln.x * ln.x + ln.y * ln.y);

		return(r1 / r2);
	}

	// Returns a representation of the cartesian equation of the bisector of ab
	Point3D bisector(Point2D a, Point2D b)
	{
		Point2D middle = Point2D((a.x + b.x) / 2, (a.y + b.y) / 2);
		double c = a.x - b.x;
		double d = a.y - b.y;
		double e = -1 * (c * middle.x + d * middle.y);

		return Point3D(c, d, e);
	}

	Point3D tangent(Point2D u, Point2D v)
	{
		//    "Triplet de l'équation cartésienne du plan de la droite (UV)"

		double a;
		double b;
		double c;

		a = -(u.y - v.y);
		b = u.x - v.x;
		c = -(a*u.x + b*u.y);
		
		return Point3D(a, b, c);
	}

	double det(Point2D p, Point2D q, Point2D r)
	{
		//Calculating the determinant of a special matrix with three 2D points

		double sum1;
		double sum2;

		sum1 = p.x*q.y + q.x*r.y + r.x*p.y;
		sum2 = p.x*r.y + q.x*p.y + r.x*q.y;
		
		return sum1-sum2;

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

	double perimeter(std::list<Point2D> cloud)
	{
		return 0.0;
	}


} // namespace examples
