#include "geo.h"

_geoData::line::line(const point& first, const point& second)
{
	a = second.y - first.y;
	b = first.x - second.x;
	c = -a * first.x - b * first.y;
}

_geoData::line::line(const pair& pair)
{
	(*this) = line(pair.A, pair.B);
}

const _geoData::point symmetry(const _geoData::point& point, const _geoData::point& center)
{
	return center * 2 - point;
}

const _geoData::point symmetry(const _geoData::point& point, const _geoData::line& axis)
{
	return symmetry(point, proection(point, axis));
}

const _geoData::point homothety(const _geoData::point& point, const _geoData::point& center, const double k)
{
	return center + (point - center) * k;
}

const _geoData::point proection(const _geoData::point& point, const _geoData::line& line)
{
	return intersect(line, height(point, line));
}

const _geoData::point rotate90(_geoData::point Z)
{
	return _geoData::point(-Z.y, Z.x);
}

const _geoData::point intersect(const _geoData::line& first, const _geoData::line& second)
{
	double denominator = first.a * second.b - second.a * first.b;
	if (denominator == 0)
		return _geoData::point(INFINITY, first.b / first.a);
	double nominatorX = second.c * first.b - first.c * second.b;
	double nominatorY = second.c * first.a - first.c * second.a;
	return _geoData::point(nominatorX / denominator, nominatorY / denominator);
}

const _geoData::pair intersect(const _geoData::line& line, const _geoData::circle& circle)
{
	const _geoData::point center_symm = symmetry(circle.center, line);
	const _geoData::circle circle_symm(center_symm, circle.radius);
	return intersect(circle, circle_symm);
}

const _geoData::pair intersect(const _geoData::circle& circle, const _geoData::line& line)
{
	const _geoData::point center_symm = symmetry(circle.center, line);
	const _geoData::circle circle_symm(center_symm, circle.radius);
	return intersect(circle, circle_symm);
}

const _geoData::pair intersect(const _geoData::circle& first, const _geoData::circle& second)
{
	const _geoData::point radicalPoint = ::radicalPoint(first, second);
	const double firstR2 = first.radius * first.radius;
	const double secondR2 = second.radius * second.radius;
	double distance = ::distance(first.center, second.center);
	if (first.radius + second.radius < distance)
		return _geoData::pair(_geoData::point(INFINITY, INFINITY), _geoData::point(INFINITY, INFINITY));
	const _geoData::point centersVector{ (second.center - first.center) / distance };
	const double height = sqrt((firstR2 + secondR2) / 2 - (firstR2 - secondR2) * (firstR2 - secondR2) / (distance * distance * 4) - distance * distance / 4);
	const _geoData::point radicalVector{ rotate90(centersVector) * height};
	const _geoData::point firstIntersection = radicalPoint + radicalVector;
	const _geoData::point secondIntersection = radicalPoint - radicalVector;
	return _geoData::pair(firstIntersection, secondIntersection);
}

const _geoData::point radicalPoint(const _geoData::circle & first, const _geoData::circle & second)
{
	const double distance = ::distance(first.center, second.center);
	const double mFirst = distance * distance - first.radius * first.radius + second.radius * second.radius;
	const double mSecond = distance * distance + first.radius * first.radius - second.radius * second.radius;
	const _geoData::pair centeres(first.center, second.center);
	return barycenter(centeres, mFirst, mSecond);
}

const _geoData::line radicalAxis(const _geoData::circle& first, const _geoData::circle& second)
{
	const _geoData::pair centeres(first.center, second.center);
	return height(radicalPoint(first, second), _geoData::line(centeres));
}

const _geoData::line height(const _geoData::point& point, const _geoData::line& line)
{
	_geoData::line result;
	result.a = -line.b;
	result.b = line.a;
	result.c = -line.a * point.y + line.b * point.x;
	return result;
}

_geoData::angle::angle(const line& _first, const line & _second)
	: first{_first}, second{_second}, value{ getAngle(_first, _second)}
{}

double getAngle( _geoData::line first, const _geoData::line& second)
{
	double result = atan(- first.a / first.b) - atan(- second.a / second.b);
	return result;
}

const _geoData::angle tangent(const _geoData::point& point, const _geoData::circle& circle)
{
	const _geoData::pair tanPoints(tangentPoints(point, circle));
	const _geoData::line firstTangent(point, tanPoints.A);
	const _geoData::line secondTangent(point, tanPoints.B);
	return _geoData::angle(firstTangent, secondTangent);
}

const _geoData::pair tangentPoints(const _geoData::point& point, const _geoData::circle& circle)
{
	const double radius = distance(point, circle.center) / 2;
	const _geoData::point center = (point + circle.center) / 2;
	return(intersect(_geoData::circle(center, radius), circle));
}

bool areBelongOneLine(const _geoData::point& first, const _geoData::point& second, const _geoData::point& third)
{
	const _geoData::line AB(first, second);
	return !distance(third, AB);
}

bool areOnTheOneSide(const _geoData::point& first, const _geoData::point& second, const _geoData::line& line)
{
	return (distance(first, line) * distance(second, line) >= 0);
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

double distance(const _geoData::point& point, const _geoData::line& line)
{
	return (line.a * point.x + line.b * point.y + line.c) / sqrt(line.a * line.a + line.b * line.b);
}

double area(const _geoData::triangle& triangle)
{
	const double a = distance(triangle.B, triangle.C);
	const double b = distance(triangle.A, triangle.C);
	const double c = distance(triangle.A, triangle.B);
	const double p = (a + b + c) / 2;
	return sqrt(p * (p - a) * (p - b) * (p - c));
}

const _geoData::point barycenter(const _geoData::triangle & triangle, double mA, double mB, double mC)
{
	return _geoData::point((triangle.A * mA + triangle.B * mB + triangle.C * mC) / (mA + mB + mC));
}

const _geoData::point barycenter(const _geoData::pair& pair, double mA, double mB)
{
	return _geoData::point((pair.A * mA + pair.B * mB) / (mA + mB));
}

const _geoData::point barycenter(const _geoData::polygon& polygon, std::vector<double> weights)
{
	const size_t size = weights.size();
	assert(polygon.size() == size);
	_geoData::point result;
	double summ = 0;
	for (size_t i = 0; i < size; ++i)
	{
		result = result + polygon[i] * weights[i];
		summ += weights[i];
	}
	assert(summ);
	return result / summ;
}

const _geoData::point orthocenter(const _geoData::triangle& triangle)
{
	const _geoData::line AB(triangle.A, triangle.B);
	const _geoData::line BC(triangle.B, triangle.C);
	_geoData::line heightA = height(triangle.A, BC);
	_geoData::line heightC = height(triangle.C, AB);
	return intersect(heightA, heightC);
}

const _geoData::point incenter(const _geoData::triangle& triangle)
{
	const double a = distance(triangle.B, triangle.C);
	const double b = distance(triangle.A, triangle.C);
	const double c = distance(triangle.A, triangle.B);
	return barycenter(triangle, a, b, c);
}

const _geoData::triangle excenters(const _geoData::triangle& triangle)
{
	const double a = distance(triangle.B, triangle.C);
	const double b = distance(triangle.A, triangle.C);
	const double c = distance(triangle.A, triangle.B);
	_geoData::point exA(barycenter(triangle, -a, b, c));
	_geoData::point exB(barycenter(triangle, a, -b, c));
	_geoData::point exC(barycenter(triangle, a, b, -c));
	return _geoData::triangle(exA, exB, exC);
}

const _geoData::point center(const _geoData::triangle& triangle)
{
	return _geoData::point((triangle.A + triangle.B + triangle.C) / 3);
}

const _geoData::point center(const _geoData::pair& pair)
{
	return _geoData::point((pair.A + pair.B) / 2);
}

const _geoData::point center(const _geoData::polygon & polygon)
{
	_geoData::point result;
	for (size_t i = 0; i < polygon.size(); ++i)
		result = result + polygon[i];
	result = result / polygon.size();
	return result;
}

_geoData::circle::circle(const triangle& triangle)
{
	const double a = distance(triangle.B, triangle.C);
	const double b = distance(triangle.A, triangle.C);
	const double c = distance(triangle.A, triangle.B);
	radius = a * b * c / (area(triangle) * 4);
	center = homothety(orthocenter(triangle), ::center(triangle), -1 / 2);
}

const _geoData::point circumcenter(const _geoData::triangle& triangle)
{
	return _geoData::circle(triangle).center;
}

#include <iostream>

void print(_geoData::point Z)
{
	if (Z.x < 0.0000000001)
		Z.x = 0;
	if (Z.y < 0.0000000001)
		Z.y = 0;
	std::cout << "(" << Z.x << "; " << Z.y << ") ";
}

int main()
{
	_geoData::circle circle(_geoData::point(2, 1), sqrt(2));
	_geoData::line line(_geoData::point(0, 0), _geoData::point(1, 0));
	print(intersect(line, circle).A);
	print(intersect(circle, line).B);
	system("pause");
}