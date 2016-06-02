#include <vector>
#include <stdio.h>

namespace examples {

	struct pt_dist {
		double dist;
		Point2D point;
		pt_dist(double d, Point2D p);
		pt_dist();
	};

	std::vector<std::string> &split(const std::string &s, char delim, std::vector<std::string> &elems);
	std::string myreplace(std::string &s, const std::string &toReplace, const std::string &replaceWith);

	bool lexical_lt(Point2D *lhs, Point2D *rhs);
	std::vector<Point2D *> lexical_sort(std::vector<Point2D *> cloud);

	std::vector<Point2D *> get_last_n(std::vector<Point2D *>, int n);

	pt_dist get_min(std::vector<pt_dist *> v);

	void print_point(Point2D *pt);
	void print_point(Point3D *pt);
	void print_vector(std::vector<Point2D *> v);
	void print_vector(std::vector<Point3D *> v);
}