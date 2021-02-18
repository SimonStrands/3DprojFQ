#pragma once
#include <math.h>
#include <array>
//git


struct vec2 {
	vec2() {
		this->x = 0;
		this->y = 0;
	}
	vec2(float x, float y) {
		this->x = x;
		this->y = y;
	}
	float legth() {
		return (float)sqrt(x * x + y * y);
	}
	void Normalize() {
		float le = this->legth();
		this->x /= le;
		this->y /= le;
	}
	float x, y;
};
struct vec3 {
	vec3(float a, float b, float c) {
		this->x = a;
		this->y = b;
		this->z = c;
	}
	vec3() {
		this->x = 0;
		this->y = 0;
		this->z = 0;
	}
	void Normalize();
	vec3 X(const vec3& other);
	float operator*(vec3& other);
	vec3 operator+(vec3& other);
	vec3 operator-(vec3& other);
	bool operator==(vec3& other);
	void operator=(vec3 other);
	void operator=(std::array<float, 3> other);
	float length();

	float x;
	float y;
	float z;
};
struct vec4 {
	vec4(float a, float b, float c, float d) {
		xyz.x = a;
		xyz.y = a;
		xyz.z = a;
		w = d;
	}
	void Normalize();
	float operator*(vec4& other);
	vec4 operator+(vec4& other);
	vec4 operator-(vec4& other);
	bool operator==(vec4& other);
	void operator=(vec4 other);
	float length();

	vec3 xyz;
	float w;
};

//void arrayToVec(std::array<float, 3> arr[3], vec3 vec[3])
//{
//	for (int i = 0; i < 3; i++) {
//		vec[i].x = arr[i][0];
//		vec[i].y = arr[i][1];
//		vec[i].z = arr[i][2];
//	}
//}
//void arrayToVec(std::array<float, 2> arr[3], vec2 vec[3])
//{
//	for (int i = 0; i < 3; i++) {
//		vec[i].x = arr[i][0];
//		vec[i].y = arr[i][1];
//	}
//}