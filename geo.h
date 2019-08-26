#pragma once

#ifndef _GEO_H_
#define _GEO_H_

#include <vector>
#include <cmath>

#include <cassert>

struct _geoData
{
	struct point
	{
		double x;
		double y;
		point(double _x = 0, double _y = 0)
			: x{_x}, y{_y}
		{}
		point(point const& point)
			: x{point.x}, y{point.y}
		{}
		point operator+(const point& second) const
		{
			return point(x + second.x, y + second.y);
		};
		point operator/(const double k) const
		{
			if (k == 0)
				return point(INFINITY, INFINITY);
			else
				return point(x / k, y / k);
		}
		point operator*(const double k) const
		{
			return point(x * k, y * k);
		}
		point operator-(const point& second) const
		{
			return point(x - second.x, y - second.y);
		}
	};
	using polygon = std::vector<point>;
	struct pair
	{
		point A;
		point B;
		pair(const point& _A, const point& _B)
			: A{_A}, B{_B}
		{}
	};
	struct line
	{
		// a * x + b * y + c = 0
		double a;
		double b;
		double c;
		line(double _a = 0, double _b = 0, double _c = 0)
			: a{_a}, b{_b}, c{_c}
		{}
		line(const line& line)
			: a{line.a}, b{line.b}, c{line.c}
		{}
		line(point const& first, point const& second);
		line(const pair& pair);
	};
	struct triangle
	{
		point A;
		point B;
		point C;
		triangle(point const& _A, point const& _B, point const& _C)
			: A{_A}, B{_B}, C{_C}
		{}
	};
	struct circle
	{
		point center;
		double radius;
		circle(const point& _center, double _radius)
			: center {_center}, radius {_radius}
		{}
		circle(const triangle& triangle);
	};
	struct angle
	{
		line first;
		line second;
		double value;
		angle(const line& _first, const line& _second);
	};
};

const _geoData::point symmetry(const _geoData::point& point, const _geoData::point& center);
const _geoData::point symmetry(const _geoData::point& point, const _geoData::line& axis);
const _geoData::point homothety(const _geoData::point& point, const _geoData::point& center, const double k);
const _geoData::point proection(const _geoData::point& point, const _geoData::line& line);
const _geoData::point rotate90(_geoData::point Z);

const _geoData::point intersect(const _geoData::line& first, const _geoData::line& second);
const _geoData::pair intersect(const _geoData::line& line, const _geoData::circle& circle);
const _geoData::pair intersect(const _geoData::circle& circle, const _geoData::line& line);
const _geoData::pair intersect(const _geoData::circle& first, const _geoData::circle& second);

const _geoData::point radicalPoint(const _geoData::circle& first, const _geoData::circle& second);
const _geoData::line radicalAxis(const _geoData::circle& first, const _geoData::circle& second);

const _geoData::line height(const _geoData::point& point, const _geoData::line& line);

double getAngle(_geoData::line first, const _geoData::line& second);
const _geoData::angle tangent(const _geoData::point& point, const _geoData::circle& circle);
const _geoData::pair tangentPoints(const _geoData::point& point, const _geoData::circle& circle);

bool areBelongOneLine(const _geoData::point& first, const _geoData::point& second, const _geoData::point& third);
bool areOnTheOneSide(const _geoData::point& first, const _geoData::point& second, const _geoData::line& line);
bool isInTriangle(const _geoData::point& point, const _geoData::triangle& triangle);
bool isInCircle(const _geoData::point& point, const _geoData::circle& circle);

double distance(const _geoData::point& A, const _geoData::point& B);
double distance(const _geoData::point& point, const _geoData::line& line); // oriented
double area(const _geoData::triangle& triangle);

const _geoData::point barycenter(const _geoData::triangle& triangle, double mA, double mB, double mC);
const _geoData::point barycenter(const _geoData::pair& pair, double mA, double mB);
const _geoData::point barycenter(const _geoData::polygon& polygon, std::vector<double> weights);

const _geoData::point orthocenter(const _geoData::triangle& triangle);
const _geoData::point incenter(const _geoData::triangle& triangle);
const _geoData::triangle excenters(const _geoData::triangle& triangle);
const _geoData::point circumcenter(const _geoData::triangle& triangle);
const _geoData::point center(const _geoData::triangle& triangle);
const _geoData::point center(const _geoData::pair& pair);
const _geoData::point center(const _geoData::polygon& polygon);

// ifndef _GEO_H_
#endif