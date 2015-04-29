#ifndef VECTOR_H
#define VECTOR_H

#include <cmath>
#include <windows.h>

#pragma pack(push, 1)
class Vector
{
public:
	float x;
	DWORD _paddingx;
	float y;
	DWORD _paddingy;
	float z;
	DWORD _paddingz;
public:
	Vector(float x = 0, float y = 0, float z = 0);
	~Vector();
	
	float length() const;
	Vector normalized() const;

	float dot(const Vector& other) const;
	Vector cross(const Vector& other) const;

	float getX() const;
	float getY() const;
	float getZ() const;

	Vector& operator+=(const Vector& other);
	Vector& operator-=(const Vector& other);

	Vector& operator=(const Vector& other);

	friend Vector operator+(const Vector& vec1, const Vector& vec2);
	friend Vector operator-(const Vector& vec1, const Vector& vec2);

	friend Vector operator/(const Vector& vector, float scalar);
	friend Vector operator*(const Vector& vector, float scalar);
	friend Vector operator*(float scalar, const Vector& vector);

	
};
#pragma pack(pop)
#endif