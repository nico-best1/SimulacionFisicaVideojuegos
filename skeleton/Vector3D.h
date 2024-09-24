#pragma once
#include <cmath> 
class Vector3D
{
public:
	float x, y, z;
	Vector3D(float x_, float y_, float z_) : x(x_), y(y_), z(z_) {}
	float modulo(){ return sqrt(pow(x, 2) + pow(y, 2) + pow(z, 2)); }

	Vector3D normalizar() {
		float x_, y_, z_;
		float m = modulo();
		x_ = x / m;
		y_ = y / m;
		z_ = z / m;
		return Vector3D(x_, y_, z_);
	}

	float escalar(Vector3D vec) { return x * vec.x + y * vec.y + x * vec.z; }

	bool operator==(const Vector3D& v) const
	{
		return x == v.x && y == v.y && z == v.z;
	}

	Vector3D& operator=(const Vector3D& v) {
		x = v.x;
		y = v.y;
		z = v.z;
		return *this;
	}

	Vector3D& operator*(float n)  {
		x *= n;
		y *= n;
		z *= n;
		return *this;
	}

	Vector3D operator*(const Vector3D& v){
		return Vector3D(y * v.z - z * v.y, z * v.x - x * v.z, x * v.y - y * v.x);
	}

	Vector3D operator+(const Vector3D& v) const
	{
		return Vector3D(x + v.x, y + v.y, z + v.z);
	}

	Vector3D operator-(const Vector3D& v) const
	{
		return Vector3D(x - v.x, y - v.y, z - v.z);
	}
};

