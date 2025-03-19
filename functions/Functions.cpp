#include "Functions.h"

std::vector<std::string> split(std::string str, char delimiter)
{
	bool endOfStr = false;
	std::vector<std::string> splitVector;
	size_t currPos = 0;
	if (str == "-")
	{
		splitVector.insert(splitVector.end(), 2, "-");
		return splitVector;
	}
	while (!endOfStr)
	{
		size_t nextPos = str.find(delimiter, currPos);
		if (nextPos == str.npos)
			endOfStr = true;
		if (nextPos - currPos != 0)
		{
			std::string value = str.substr(currPos, nextPos - currPos);
			splitVector.push_back(value);
			currPos = nextPos + 1;
		}
		else
		{
			currPos++;
			splitVector.push_back("-");
		}
	}
	return splitVector;
}