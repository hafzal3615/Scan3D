#include <list>
#include <string>
#include "Point.h"

namespace examples {

	void main_Perimetre();

	std::list<Point> getpts(const std::string& filename);
	std::list<Point> getpts_obj(const std::string& filename);
	std::list<Point> getpts_asc(const std::string& filename);

	double dist_ptplane(Point pt, Plane pl)

} // namespace examples
#pragma once
