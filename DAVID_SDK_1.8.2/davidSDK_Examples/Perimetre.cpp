#include "davidSDK/david.h"
#include <vector>
#include <time.h>
#include <fstream>
#include "Perimetre.h"

namespace examples {

	void main_Perimetre() {
		std::vector<Point3D *> points = getpts("C:\\Users\\Jay\\Documents\\Scans\\3d.asc", 1000, 159781);

		Plane pl = Plane(1, 1, 0.001, 100);

		double eps1 = 0.5;
		double eps2 = 2;
		double eta = 2;
		double mu = 8.5;

		double perimeter = calculate_perimeter(points, pl, eps1, eps2, eta, mu);

		std::cout << "Perimeter" << perimeter << "\n";
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
			}
			else {
				// Badly formatted line, throw some error
			}

			++count;
		}

		return points;
	}

	// Return the shortest distance from a point to a plane
	double dist_ptplane(Point3D pt, Plane pl) {
		double r1 = abs(pl.a * pt.x + pl.b * pt.y + pl.c * pt.z + pl.d);
		double r2 = sqrt(pl.a * pl.a + pl.b * pl.b + pl.c * pl.c);

		return(r1 / r2);
	}

	/* Goes through a vector list of points and returns only those points within a certain 
		distance of a plane. Returns a copy not a view into the original list
	*/
	std::vector<Point3D *> section(std::vector<Point3D *> *cloud, const Plane & pl, const double & eps) {
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
	double scalar_product(Point3D a, Point3D b) {
		return a.x * b.x + a.y * b.y + a.z * b.z;
	}

	// Gets the scalar product of two 2-vectors
	double scalar_product(Point2D a, Point2D b) {
		return a.x * b.x + a.y * b.y;
	}

	// Gets the norm of a 3-vector
	double norm(Point3D a) {
		return scalar_product(a, a);
	}

	// Gets the norm of a 2-vector
	double norm(Point2D a) {
		return scalar_product(a, a);
	}

	// Gets the distance between two 3D points a and b
	double dist(Point3D a, Point3D b) {
		// Compute the magnitude of a - b (or b - a)
		Point3D c = Point3D(a.x - b.x, a.y - b.y, a.z - b.z);
		return norm(c);
	}

	// Gets the distance between two 2D points a and b
	double dist(Point2D a, Point2D b) {
		Point2D c = Point2D(a.x - b.x, a.y - b.y);
		return norm(c);
	}

	/* Takes a point cloud and a plane represented by a Point3D and projects the 
		points in the cloud onto the plane
	*/
	std::vector<Point2D *> projection(std::vector<Point3D *> cloud, Plane plane) {
		Point3D u = Point3D(-1 * plane.b, plane.a, 0);
		Point3D v = Point3D(-1 * plane.a * plane.c, 
			-1 * plane.b * plane.c, 
			plane.a * plane.a + plane.b * plane.b);

		double unorm = norm(u);
		double vnorm = norm(v);

		u = Point3D(u.x / unorm, u.y / unorm, u.z / unorm);
		v = Point3D(v.x / vnorm, v.y / vnorm, v.z / vnorm);

		std::vector<Point2D *> proj;

		int len = (int)(cloud.size());
		for (int i = 0; i < len; i++) {
			proj.push_back(new Point2D(scalar_product(*(cloud.at(i)), u),
				scalar_product(*(cloud.at(i)), v)));
		}
		
		return proj;
	}

	// Gets the distance between a point a and a line ln represented by a Point3D
	double dist_line(Point2D a, Point3D ln) {
		double r1 = abs(ln.x * a.x + ln.y * a.y + ln.z);
		double r2 = sqrt(ln.x * ln.x + ln.y * ln.y);

		return(r1 / r2);
	}

	// Returns a representation of the cartesian equation of the bisector of ab
	Point3D bisector(Point2D a, Point2D b) {
		Point2D middle = Point2D((a.x + b.x) / 2, (a.y + b.y) / 2);
		double c = a.x - b.x;
		double d = a.y - b.y;
		double e = -1 * (c * middle.x + d * middle.y);

		return Point3D(c, d, e);
	}

	// Gets the triple of the line between u and v
	Point3D tangent(Point2D u, Point2D v) {
		double a;
		double b;
		double c;

		a = -(u.y - v.y);
		b = u.x - v.x;
		c = -(a * u.x + b * u.y);
		
		return Point3D(a, b, c);
	}

	/* Calculating the determinant of a special matrix with three 2D points
		This is like a cross product pretty much
	*/
	double det(Point2D p, Point2D q, Point2D r) {
		double sum1 = p.x * q.y + q.x * r.y + r.x * p.y;
		double sum2 = p.x * r.y + q.x * p.y + r.x * q.y;
		
		return sum1 - sum2;
	}

	// Determine if the vectors pq and qr form a right turn on the line they define
	bool is_rightturn(Point2D p, Point2D q, Point2D r) {
		if (eq(p, q) || eq(q, r) || eq(p, r)) {
			// throw error
		}
		else {
			return det(p, q, r) < 0;
		}
		return true;
	}

	// Utility method so that we don't have to worry about pulling out points inside a loop
	bool is_rightturn(std::vector<Point2D *> pts) {
		if ((int)(pts.size()) == 3) {
			return is_rightturn(*(pts.at(0)), *(pts.at(1)), *(pts.at(2)));
		}
		else {
			// And we could even throw some error
			return false;
		}
	}

	/* Gets the convex hull of a group of points. The convex hull is the smallest set of
		points that fully contains the full cloud. The algorithm computes a bottom and
		top part of the hull by wrapping around in strict left turns
	*/
	std::vector<Point2D *> convex_hull(std::vector<Point2D *> cloud) {
		lexical_sort(cloud);

		if ((int)(cloud.size()) < 3) {
			return std::vector<Point2D *>();
		}

		std::vector<Point2D *> upper = std::vector<Point2D *>();
		std::vector<Point2D *> lower = std::vector<Point2D *>();

		upper.push_back(copy(cloud.at(0)));
		upper.push_back(copy(cloud.at(1)));

		int len = (int)(cloud.size());
		for (int i = 2; i < len; i++) {
			upper.push_back(copy(cloud.at(i)));

			while ((int)(upper.size()) > 2 && !is_rightturn(get_last_n(upper, 3))) {
				upper.erase(upper.end() - 2);
			}
		}

		std::reverse(cloud.begin(), cloud.end());

		lower.push_back(copy(cloud.at(0)));
		lower.push_back(copy(cloud.at(1)));
		for (int i = 2; i < len; i++) {
			lower.push_back(copy(cloud.at(i)));

			while ((int)(lower.size()) > 2 && !is_rightturn(get_last_n(lower, 3))) {
				lower.erase(lower.end() - 2);
			}
		}

		// Remove duplicates
		lower.erase(lower.begin());
		lower.erase(lower.end());

		upper.insert(upper.end(), lower.begin(), lower.end());

		return upper;
	}

	std::vector<Point2D *> seg_tighten(Point2D u, Point2D v, std::vector<Point2D *> cloud, 
		double eps, double eta, double mu) {

		double d = dist(u, v);
		if (d < eta) {
			return std::vector<Point2D *>();
		}

		double eps2 = d / 4;
		std::vector<pt_dist *> closer = std::vector<pt_dist *>();
		Point3D bis = bisector(u, v);
		Point2D mid = Point2D((u.x + v.x) / 2, (u.y + v.y) / 2);
		Point3D tan = tangent(u, v);

		int len = (int)(cloud.size());
		for (int i = 0; i < len; i++) {
			Point2D x = *(copy(cloud.at(i)));
			double dbis = dist_line(x, bis);
			double dtan = dist(x, mid);

			if (dbis < (eps < eps2 ? eps : eps2)) {
				closer.push_back(new pt_dist(dtan, x));
			}
		}

		if (!closer.empty()) {
			pt_dist ptd = get_min(closer);

			if (ptd.dist < mu) {
				std::vector<Point2D *> t1 = seg_tighten(u, ptd.point, cloud, eps, eta, mu);
				std::vector<Point2D *> t2 = seg_tighten(ptd.point, v, cloud, eps, eta, mu);

				t1.push_back(&(ptd.point));
				t1.insert(t1.end(), t2.begin(), t2.end());

				return t1;
			}
			else {
				return std::vector<Point2D *>();
			}
		}
		else {
			return std::vector<Point2D *>();
		}
	}

	/* Tightens a convex hull to get it closer to the perimiter which is not necessarily
		the convex hull
	*/
	std::vector<Point2D *> tighten(std::vector<Point2D *> hull, std::vector<Point2D *> cloud, 
		double eps, double eta, double mu) {

		int n = (int)(hull.size());
		std::vector<Point2D *> hull_cpy;

		for (int i = 0; i < n; i++) {
			hull_cpy.push_back(copy(hull.at(i)));
		}

		Point2D *u = hull.at((int)(hull_cpy.size()) - 1);
		
		for (int i = 1; i < n; i++) {
			if (i != 0) {
				Point2D *v = hull.at(i - 1);
			}
			Point2D *v = hull.at(i);
			int k = 0;
			bool flag = false;
			for (int j = 0; j < (int)(hull_cpy.size()) && !flag; j++) {
				if (v->x == hull_cpy.at(j)->x && v->y == hull_cpy.at(j)->y) {
					k = j;
					flag = true;
				}
			}

			std::vector<Point2D *> tightened = seg_tighten(*u, *v, cloud, eps, eta, mu);
			hull_cpy.insert(hull_cpy.begin() + k, tightened.begin(), tightened.end());
		}

		return hull_cpy;
	}

	// Loops through a convex hull and calculates the length of the total hull
	double perimeter(std::vector<Point2D *> hull) {
		int n = (int)(hull.size());

		double p = dist(*(hull.at(n - 1)), *(hull.at(0)));
		for (int i = 1; i < n; i++) {
			p += dist(*(hull.at(i - 1)), *(hull.at(i)));
		}

		return p;
	}

	double calculate_perimeter(std::vector<Point3D *> cloud, Plane pl, double eps1,
		double eps2, double eta, double mu) {

		std::cout << "1: Entered Function\n";
		std::vector<Point3D *> cloud2 = section(&cloud, pl, eps1);
		std::cout << "2: Sectioned Cloud\n";
		int len = (int)(cloud2.size());
		Point3D first = len > 0 ? *(cloud2.at(0)) : Point3D();
		printf("len is %d, first point is x: %f, y: %f, z: %f\n", len, first.x, first.y, first.z);

		std::vector<Point2D *> cloud3 = projection(cloud2, pl);
		std::cout << "3: Projected Cloud\n";

		std::vector<Point2D *> cloud4 = convex_hull(cloud3);
		std::cout << "4: Got convex hull\n";

		std::vector<Point2D *> cloud5 = tighten(cloud4, cloud3, eps2, eta, mu);
		std::cout << "5: Tightened convex hull\n";

		return perimeter(cloud5);
	}


} // namespace examples
