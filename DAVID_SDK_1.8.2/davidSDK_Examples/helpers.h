#include <vector>
#include <string>

namespace examples {

	std::vector<std::string> &split(const std::string &s, char delim, std::vector<std::string> &elems);
	std::string myreplace(std::string &s, const std::string &toReplace, const std::string &replaceWith);
}