#pragma once
#include <array>
//git
struct vertex {
	float pos[3];
	float uv[2];
	float norm[4];
	vertex(const std::array<float, 3>& position, const std::array<float, 2>& uvC, const std::array<float, 4>& normal) {
		for (int i = 0; i < 3; i++) {
			this->pos[i] = position[i];
			this->norm[i] = normal[i];
		}
		this->norm[3] = normal[3];
		uv[0] = uvC[0];
		uv[1] = uvC[1];
	}
};