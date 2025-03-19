#include <iostream>
#include <vector>
#include <optional>
#include <fstream>
#include <string>
#include <set>
#include <map>
#include "automates_classes/mealy_automate/MealyAutomate.h"
#include "automates_classes/moore_automate/MooreAutomate.h"
using namespace std;

const char DELIMITER = ';';

struct Args {
	string automateType;
	string inputFileName;
	string outputFileName;
};

optional<Args> ParseArgs(int argc, char* argv[])
{
	Args args;
	args.automateType = argv[1];
	args.inputFileName = argv[2];
	args.outputFileName = argv[3];
	return args;
}

int main(int argc, char* argv[])
{
	auto args = ParseArgs(argc, argv);
	ifstream input;
	input.open(args->inputFileName);
	ofstream output;
	output.open(args->outputFileName);
	string automateType = args->automateType;

	if (automateType == "mealy")
	{
		MealyAutomate* mealyAutomate = new MealyAutomate(input);
		mealyAutomate->deleteUnattainableStatements();
		vector<vector<string>> result = mealyAutomate->getMinimizedAutomate();
		for (int i = 0; i < result[0].size(); i++)
		{
			string outputStr = "";
			for (int j = 0; j < result.size(); j++)
			{
				if (result[j][i] == "-")
				{
					outputStr += ";";
					continue;
				}
				outputStr += result[j][i] + ";";
			}
			outputStr.erase(outputStr.end() - 1);
			output << outputStr << "\n";
		}
	}
	else if (automateType == "moore")
	{
		MooreAutomate* mooreAutomate = new MooreAutomate(input);
		mooreAutomate->deleteUnattainableStatements();
		vector<vector<string>> result = mooreAutomate->getMinimizedAutomate();
		for (int i = 0; i < result[0].size(); i++)
		{
			string outputStr = "";
			for (int j = 0; j < result.size(); j++)
			{
				if (result[j][i] == "-")
				{
					outputStr += ";";
					continue;
				}
				outputStr += result[j][i] + ";";
			}
			outputStr.erase(outputStr.end() - 1);
			output << outputStr << "\n";
		}
	}
	return 0;
}