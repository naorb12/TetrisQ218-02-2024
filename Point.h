#ifndef _Point
#define _Point

class Point
{
private:
	int x;
	int y;

public:

	Point();
	Point(const int newX,const int newY);

	// Getter functions
	int getX() const;
	int getY() const;

	// Setter functions
	void setXAndY(const int newX,const int newY);
	void setX(const int newX);
	void setY(const int newY);

	// add functions
	void addToX(const int addX);
	void addToY(const int addY);
	void addToXAndY(const int addX, const int addY);
};

#endif
