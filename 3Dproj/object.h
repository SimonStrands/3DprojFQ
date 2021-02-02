#pragma once
#include <vector>
#include "vertex.h"
#include <string>

//#thisNeedFix
//git
class object{
public:
	object();
private:
	std::vector<std::vector<vertex>> vertices;
	std::vector<std::string> material;
};
//vertices