#include <sstream>
#include <string>
#include <algorithm>
#include "Point.h"
#include "Plane.h"
#include "helpers.h"

namespace examples {

	pt_dist::pt_dist(double d, Point2D p) {
		dist = d;
		point = p;
	}

	pt_dist::pt_dist() {
		dist = 0;
		point = Point2D();
	}

	// Split results into a pre-constructed vector
	// Taken from http://stackoverflow.com/a/236803
	std::vector<std::string> &split(const std::string &s, char delim, std::vector<std::string> &elems) {
		std::stringstream ss(s);
		std::string item;
		while (std::getline(ss, item, delim))
		{
			elems.push_back(item);
		}

		return elems;
	}

	// Replace string inside of another string
	// Taken from http://stackoverflow.com/a/5878802
	std::string myreplace(std::string &s, const std::string &toReplace, const std::string &replaceWith) {
		return(s.replace(s.find(toReplace), toReplace.length(), replaceWith));
	}

	// Sorts a vector of 2D points into lexical order, depending on their x value
	std::vector<Point2D *> lexical_sort(std::vector<Point2D *> cloud) {
		std::sort(cloud.begin(), cloud.end(), lexical_lt);

		return cloud;
	}

	// Lexically compares two points, comparing only their x values
	bool lexical_lt(Point2D *lhs, Point2D *rhs) {
		return lhs->x < rhs->x;
	}

	// Gets the last n elements from a list of 2D points. Should be done with templates
	std::vector<Point2D *> get_last_n(std::vector<Point2D *> array, int n) {
		return std::vector<Point2D *> (array.end() - std::min((int)(array.size()), n), array.end());
	}

	// Loops through a list of pt_dist and finds the one with the smallest distance
	pt_dist get_min(std::vector<pt_dist *> v) {
		int len = (int)(v.size());

		pt_dist pd_min;
		if (len == 0) {
			return pt_dist();
		}
		else {
			pd_min = *(v.at(0));
		}

		for (int i = 1; i < len; i++) {
			pd_min = pd_min.dist < v.at(i)->dist ? pd_min : *(v.at(i));
		}

		return pd_min;
	}

	void print_point(Point2D *pt) {
		printf("[%f, %f]\n", pt->x, pt->y);
	}
	
	void print_point(Point3D *pt) {
		printf("[%f, %f, %f]\n", pt->x, pt->y, pt->z);
	}

	void print_plane(Plane *pl) {
		printf("Plane: [%f, %f, %f, %f]\n", pl->a, pl->b, pl->c, pl->d);
	}

	void print_vector(std::vector<Point2D *> v) {
		int len = (int)(v.size());

		printf("[");
		for (int i = 0; i < len; i++) {
			if (i != 0) {
				printf(",");
			}
			print_point(v.at(i));
		}
		printf("]\n");
	}

	void print_vector(std::vector<Point3D *> v) {
		int len = (int)(v.size());

		printf("[");
		for (int i = 0; i < len; i++) {
			print_point(v.at(i));
		}
		printf("]\n");
	}
}