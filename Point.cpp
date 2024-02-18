#include "Point.h"

Point::Point() : x(0), y(0) {} // default ctor
Point::Point(const int newX,const int newY) : x(newX), y(newY) {} // copy ctor

// Getter functions
int Point::getX() const{
	return x;
}

int Point::getY() const{
	return y;
}

// Setter functions
void Point::setXAndY(const int newX, const int newY) {
	x = newX;
	y = newY;
}
void Point::setX(const int newX) {
	x = newX;
}
void Point::setY(const int newY) {
	y = newY;
}

// add functions
void Point::addToX(const int addX)
{
	x += addX;
}
void Point::addToY(const int addY)
{
	y += addY;
}
void Point::addToXAndY(const int addX, const int addY)
{
	x += addX;
	y += addY;
}
