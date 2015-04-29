#include "Vector.h"

Vector::Vector(float x, float y, float z)
{
	this->x = x;
	this->y = y;
	this->z = z;
}

Vector::Vector(Vector3 other)
{
	this->x = other.x;
	this->y = other.y;
	this->z = other.z;
}

Vector::~Vector()
{

}

float Vector::length() const
{
	return sqrt(x * x + y * y + z * z);
}

Vector Vector::normalized() const
{
	float l = length();
	return Vector(x / l, y / l, z / l);
}

float Vector::dot(const Vector& other) const
{
	return (x * other.x) + (y * other.y) + (z * other.z);
}

Vector Vector::cross(const Vector& other) const
{
	return Vector(
		y * other.z - z * other.y,
		z * other.x - x * other.z,
		x * other.y - y * other.x);
}

float Vector::getX() const
{
	return x;
}

float Vector::getY() const
{
	return y;
}

float Vector::getZ() const
{
	return z;
}

Vector& Vector::operator+=(const Vector& other)
{
	this->x += other.x;
	this->y += other.y;
	this->z += other.z;
	return *this;
}

Vector& Vector::operator-=(const Vector& other)
{
	this->x -= other.x;
	this->y -= other.y;
	this->z -= other.z;
	return *this;
}

Vector& Vector::operator=(const Vector& other)
{
	this->x = other.x;
	this->y = other.y;
	this->z = other.z;
	return *this;
}
Vector& Vector::operator=(const Vector3& other)
{
	this->x = other.x;
	this->y = other.y;
	this->z = other.z;
	return *this;
}

Vector operator+(const Vector& vec1, const Vector& vec2)
{
	return Vector(vec1.x + vec2.x, vec1.y + vec2.y, vec1.z + vec2.z);
}

Vector operator-(const Vector& vec1, const Vector& vec2)
{
	return Vector(vec1.x - vec2.x, vec1.y - vec2.y, vec1.z - vec2.z);
}

Vector operator/(const Vector& vector, float scalar)
{
	return Vector(vector.x / scalar, vector.y /scalar, vector.z /scalar);
}

Vector operator*(const Vector& vector, float scalar)
{
	return Vector(vector.x * scalar, vector.y * scalar, vector.z * scalar);
}

Vector operator*(float scalar, const Vector& vector)
{
	return Vector(vector.x * scalar, vector.y * scalar, vector.z * scalar);
}
