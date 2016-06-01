namespace examples {
	
	class Point3D {
	public:
		double x;
		double y;
		double z;
		Point3D(double a, double b, double c);
		Point3D();
	};

	class Point2D {
	public:
		double x;
		double y;
		Point2D(double a, double b);
		Point2D();
	};

	bool eq(Point3D lhs, Point3D rhs);
	bool neq(Point3D lhs, Point3D rhs);

	bool eq(Point2D lhs, Point2D rhs);
	bool neq(Point2D lhs, Point2D rhs);
	/*
	bool operator==(const Point3D & lhs, const Point3D & rhs);
	bool operator!=(const Point3D & lhs, const Point3D & rhs);

	bool operator==(const Point2D & lhs, const Point2D & rhs);
	bool operator!=(const Point2D & lhs, const Point2D & rhs);
	*/
}