#include "otherHelps.h"
//git

std::string* getDest(std::string destPath, char splitchar)
{
	std::string* returnStr;
	returnStr = new std::string[3];
	std::string workStr = destPath;
	int starts[3]; starts[0] = 0; int stp = 1;
	int end[3]; int endp = 0;
	for (int i = 0; i < (int)workStr.length(); i++)
	{
		if (workStr[i] == splitchar) {
			end[endp] = (i)-starts[stp - 1];
			starts[stp] = i + 1;
			stp++;
			endp++;

		}
	}
	end[endp] = end[2] = (int)workStr.length() - starts[2];
	for (int i = 0; i < 3; i++) {
		if (end[i] == 0) {
			returnStr[i] = "1";
		}
		else {
			returnStr[i] = workStr.substr(starts[i], end[i]);
		}
	}
	return returnStr;
}

void swap(std::vector<vertex>& a)
{
	vertex temp = a[a.size() - 1];
	a[a.size() - 1] = a[a.size() - 2];
	a[a.size() - 2] = temp;
}
