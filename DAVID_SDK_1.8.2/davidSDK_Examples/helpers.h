#include <vector>

namespace examples {
	std::vector<std::string> &split(const std::string &s, char delim, std::vector<std::string> &elems);
	std::string myreplace(std::string &s, const std::string &toReplace, const std::string &replaceWith);

	bool lexical_lt(Point2D *lhs, Point2D *rhs);
	std::vector<Point2D *> lexical_sort(std::vector<Point2D *> cloud);

	std::vector<Point2D *> get_last_n(std::vector<Point2D *>, int n);
}