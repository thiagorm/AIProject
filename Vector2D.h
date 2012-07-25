#ifndef _Vector2D_
#define _Vector2D_

class Vector2D
{
public:
	float x; 
	float y;

	Vector2D();
	Vector2D(float x, float y);
	~Vector2D();

	Vector2D operator+(Vector2D vec);
	Vector2D operator-(Vector2D vec);
	Vector2D operator*(float factor);
	Vector2D operator/(float div);
	Vector2D operator+=(float b);
	Vector2D operator/=(float c);

	float length();
	Vector2D rotate(float angle);
	float getSize();
	inline void Normalize();
};


#endif