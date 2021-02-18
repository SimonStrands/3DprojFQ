#pragma once
#include <array>
//git
struct vertex {
	float pos[3];
	float uv[2];
	float norm[4];
	float tang[4];
	vertex(const std::array<float, 3>& position, const std::array<float, 2>& uvC, const std::array<float, 4>& normal) {
		for (int i = 0; i < 3; i++) {
			this->pos[i] = position[i];
			this->norm[i] = normal[i];
			this->tang[i] = 0;
		}
		this->norm[3] = normal[3];
		this->tang[3] = 0;
		uv[0] = uvC[0];
		uv[1] = uvC[1];
	}
	void fixtang(vec4 tang) {
		this->tang[0] = tang.xyz.x;
		this->tang[1] = tang.xyz.y;
		this->tang[2] = tang.xyz.z;
		this->tang[3] = tang.w;
	}
};