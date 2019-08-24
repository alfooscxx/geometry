#include "geo.h"

_geoData::line::line(const point& first, const point& second)
{
	const double x1 = first.x;
	const double x2 = second.x;
	const double y1 = first.y;
	const double y2 = second.y;
	k = (y2 - y1) / (x2 - x1);
	b = x1 * (x2 - x1) * (y1 - y2) + y1;
}

_geoData::line::line(const pair& pair)
{
	const double x1 = pair.A.x;
	const double x2 = pair.B.x;
	const double y1 = pair.A.y;
	const double y2 = pair.B.y;
	k = (y2 - y1) / (x2 - x1);
	b = x1 * (x2 - x1) * (y1 - y2) + y1;
}

const double _geoData::line::operator()(double x) const
{
	return x * k + b;
}

const _geoData::point intersect(const _geoData::line& first, const _geoData::line& second)
{
	const double x = (second.b - first.b) / (first.k - second.k);
	const double y = first(x);
	return _geoData::point(x, y);
}

const _geoData::line perpendicular(const _geoData::point& point, const _geoData::line& line)
{
	_geoData::line result;
	result.k = -1 / line.k;
	result.b = point.y - result.k * point.x;
	return result;
}

double getAngle( _geoData::line first, const _geoData::line& second)
{
	//FIXME:
	return 0.0;
}

bool areBelongOneLine(const _geoData::point& first, const _geoData::point& second, const _geoData::point& third)
{
	const _geoData::line AB(first, second);
	return (AB(third.x) == third.y);
}

bool areOnTheOneSide(const _geoData::point& first, const _geoData::point& second, const _geoData::line& line)
{
	const double first_diff = first.y - line(first.x);
	const double second_diff = second.y - line(second.x);
	return (first_diff >= 0) == (second_diff >= 0);
}

bool isInTriangle(const _geoData::point& point, const _geoData::triangle& triangle)
{
	bool A = areOnTheOneSide(triangle.A, point, _geoData::line(triangle.B, triangle.C));
	bool B = areOnTheOneSide(triangle.B, point, _geoData::line(triangle.A, triangle.C));
	bool C = areOnTheOneSide(triangle.C, point, _geoData::line(triangle.A, triangle.B));
	return A && B && C;
}

bool isInCircle(const _geoData::point& point, const _geoData::circle& circle)
{
	const double _distance = distance(point, circle.center);
	return _distance <= circle.radius;
}

double distance(const _geoData::point& A, const _geoData::point& B)
{
	const double X = A.x - B.x;
	const double Y = A.y - B.y;
	return sqrt(X * X + Y * Y);
}

double area(const _geoData::triangle& triangle)
{
	const double a = distance(triangle.B, triangle.C);
	const double b = distance(triangle.A, triangle.C);
	const double c = distance(triangle.A, triangle.B);
	const double p = (a + b + c) / 2;
	return sqrt(p * (p - a) * (p - b) * (p - c));
}

_geoData::circle::circle(const triangle& triangle)
{
	const double a = distance(triangle.B, triangle.C);
	const double b = distance(triangle.A, triangle.C);
	const double c = distance(triangle.A, triangle.B);
	radius = a * b * c / (area(triangle) * 4);
}

const _geoData::point circumcenter(const _geoData::triangle& triangle)
{
	return _geoData::circle(triangle).center;
}

#include <iostream>
int main()
{
	_geoData::point f(0, 1);
	_geoData::point s(1, 2);

	_geoData::line l(f, s);

	std::cout << l.k << " " << l.b;
	system("pause");
}