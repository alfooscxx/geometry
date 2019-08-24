#pragma once

#ifndef _GEO_H_
#define _GEO_H_

#include <vector>
#include <cmath>

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
	};
	struct pair
	{
		point A;
		point B;
	};
	struct line
	{
		double k;
		double b;
		line(double _k = 0, double _b = 0)
			: k{_k}, b{_b}
		{}
		line(const line& line)
		{
			k = line.k;
			b = line.b;
		}
		line(point const& first, point const& second);
		line(const pair& pair);
		const double operator()(double x) const;
	};
	struct triangle
	{
		point A;
		point B;
		point C;
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
		//angle(line _first, const line& _second)
		//	: first{ _first }, second{ _second }, value{ getAngle(_first, _second) }
		//{}
	};
	using polygon = std::vector<point>;
};

const _geoData::point intersect(const _geoData::line& first, const _geoData::line& second);
//const _geoData::point intersect(const _geoData::line& line, const _geoData::circle& circle);
//const _geoData::point intersect(const _geoData::circle& circle, const _geoData::line& line);
//const _geoData::pair intersect(const _geoData::circle& first, const _geoData::circle& second);

const _geoData::line perpendicular(const _geoData::point& point, const _geoData::line& line);


double getAngle( _geoData::line first, const _geoData::line& second);
//const _geoData::angle tangent(const _geoData::point& point, const _geoData::circle& circle);
//const _geoData::pair tangentPoints(const _geoData::point& point, const _geoData::circle& circle);

bool areBelongOneLine(const _geoData::point& first, const _geoData::point& second, const _geoData::point& third);
bool areOnTheOneSide(const _geoData::point& first, const _geoData::point& second, const _geoData::line& line);
bool isInTriangle(const _geoData::point& point, const _geoData::triangle& triangle);
bool isInCircle(const _geoData::point& point, const _geoData::circle& circle);

double distance(const _geoData::point& A, const _geoData::point& B);
double area(const _geoData::triangle& triangle);

//const _geoData::point orhtocenter(const _geoData::triangle& triangle);
//const _geoData::point incenter(const _geoData::triangle& triangle);
const _geoData::point circumcenter(const _geoData::triangle& triangle);

// ifndef _GEO_H_
#endif