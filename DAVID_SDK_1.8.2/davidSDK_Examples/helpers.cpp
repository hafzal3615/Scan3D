#include <sstream>
#include "helpers.h"

namespace examples {

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
}