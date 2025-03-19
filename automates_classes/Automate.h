#pragma once
#include <vector>
#include <string>
#include <unordered_set>
#include <set>
#include <queue>
#include <fstream>
#include <algorithm>
#include "../functions/Functions.h"

using namespace std;

struct Statement
{
	string name;
	bool isAttainable;
};

class Automate 
{
public:
	virtual void setData(std::ifstream& input) = 0;
	virtual void deleteUnattainableStatements() = 0;
	virtual vector<vector<string>> getMinimizedAutomate() const = 0;
	virtual int binarySearchInAttainableStatements(string statementName) const = 0;
	virtual int binarySearchInData(string statementName) const = 0;
};

struct EquivalenceClass
{
	int classNum = 1;
	int parentClassNum = 1;
	vector<string> outputSignals;
	set<string> equivalentStatements;
};