#include <math.h> 
#include <limits>
#include "Vector2D.h"

using namespace std;

Vector2D::Vector2D()
{
}

Vector2D::Vector2D(float x, float y)
{
	this->x = x;
	this->y = y;
}

Vector2D::~Vector2D()
{
}

Vector2D Vector2D::operator+(Vector2D vec)
{
	Vector2D result;
	
	result.x = x + vec.x;
	result.y = y + vec.y;

	return result;
}

Vector2D Vector2D::operator-(Vector2D vec)
{
	Vector2D result;
	
	result.x = x - vec.x;
	result.y = y - vec.y;

	return result;
}

Vector2D Vector2D::operator*(float factor)
{
	Vector2D result;
	
	result.x = x * factor;
	result.y = y * factor;

	return result;
}

Vector2D Vector2D::operator/(float div)
{
	Vector2D result;
	
	result.x = x / div;
	result.y = y / div;

	return result;
}

Vector2D Vector2D::operator += (float b)
{
    x += b;
    y += b;
    return *this;
}

Vector2D Vector2D::operator /= (float c)
{
    x /= c;
    y /= c;
    return *this;
}

float Vector2D::length()
{
	return sqrt(x*x + y*y);
}

Vector2D Vector2D::rotate(float angle)
{
	Vector2D result;

	result.x = x * cos(angle) + y * sin(angle);
	result.y = -1 * x * sin(angle) + y * cos(angle);

	return result;
}

inline void Vector2D::Normalize()
{ 
  float vector_length = this->length();

  if (vector_length > std::numeric_limits<float>::epsilon())
  {
    this->x /= vector_length;
    this->y /= vector_length;
  }
}