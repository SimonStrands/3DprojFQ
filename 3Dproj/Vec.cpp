#include "Vec.h"
void arrayToVec(std::array<float, 3> arr[3], vec3 vec[3])
{
	for (int i = 0; i < 3; i++) {
		vec[i].x = arr[i][0];
		vec[i].y = arr[i][1];
		vec[i].z = arr[i][2];
	}
}
void arrayToVec(float arr[3], vec3 &vec)
{
	vec.x = arr[0];
	vec.y = arr[1];
	vec.z = arr[2];
}
void arrayToVec(float arr[2], vec2 &vec)
{
	vec.x = arr[0];
	vec.y = arr[1];
}
void arrayToVec(std::array<float, 2> arr[3], vec2 vec[3])
{
	for (int i = 0; i < 3; i++) {
		vec[i].x = arr[i][0];
		vec[i].y = arr[i][1];
	}
}
//git
void vec3::Normalize()
{
	double i = sqrt(
		(double)x * (double)x +
		(double)y * (double)y +
		(double)z * (double)z);

	this->x = (float)(x / i);
	this->y = (float)(y / i);
	this->z = (float)(z / i);
}

vec3 vec3::X(const vec3& other)
{
	float x = (this->y * other.z) - (this->z * other.y);
	float y = (this->z * other.x) - (this->x * other.z);
	float z = (this->x * other.y) - (this->y * other.x);
	return vec3(x, y, z);
}

float vec3::operator*(vec3& other)
{
	return(this->x * other.x + this->y * other.y + this->z * other.z);
}

vec3 vec3::operator+(vec3& other)
{
	return vec3(this->x + other.x, this->y + other.y, this->z + other.z);
}

vec3 vec3::operator-(vec3& other)
{
	return vec3(this->x - other.x, this->y - other.y, this->z - other.z);
}

bool vec3::operator==(vec3& other)
{
	return (this->x == other.x && this->y == other.y && this->z == other.z);
}

void vec3::operator=(vec3 other)
{
	this->x = other.x;
	this->y = other.y;
	this->z = other.z;
}

float vec3::length()
{
	return (float)sqrt((double)(x * x + y * y + z * z));
}

void vec4::Normalize()
{
	double i = sqrt(
		(double)xyz.x * (double)xyz.x +
		(double)xyz.y * (double)xyz.y +
		(double)xyz.z * (double)xyz.z +
		(double)w * (double)w);

	this->xyz.x = (float)(xyz.x / i);
	this->xyz.y = (float)(xyz.y / i);
	this->xyz.z = (float)(xyz.z / i);
	this->w = (float)(w / i);
}

float vec4::operator*(vec4& other)
{
	return(this->xyz.x * other.xyz.x + this->xyz.y * other.xyz.y + this->xyz.z * other.xyz.z + this->w * other.w);
}

vec4 vec4::operator+(vec4& other)
{
	return vec4(this->xyz.x + other.xyz.x, this->xyz.y + other.xyz.y, this->xyz.z + other.xyz.z, this->w + other.w);
}

vec4 vec4::operator-(vec4& other)
{
	return vec4(this->xyz.x - other.xyz.x, this->xyz.y - other.xyz.y, this->xyz.z - other.xyz.z, this->w - other.w);
}

bool vec4::operator==(vec4& other)
{
	return (this->xyz.x == other.xyz.x && this->xyz.y == other.xyz.y && this->xyz.z == other.xyz.z && this->w == other.w);
}

void vec4::operator=(vec4& other)
{
	this->xyz.x = other.xyz.x;
	this->xyz.y = other.xyz.y;
	this->xyz.z = other.xyz.z;
	this->w = other.w;
}

float vec4::length()
{
	return (float)sqrt((double)(xyz.x * xyz.x + xyz.y * xyz.y + xyz.z * xyz.z + w * w));
}