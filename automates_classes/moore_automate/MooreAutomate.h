#pragma once
#include "../Automate.h"

class MooreAutomate : public Automate
{
public:
	MooreAutomate(ifstream& input);
	void setData(ifstream& input) override;
	void deleteUnattainableStatements() override;
	vector<vector<string>> getMinimizedAutomate() const override;
	int binarySearchInAttainableStatements(string statementName) const override;
	int binarySearchInData(string statementName) const override;
private:
	vector<vector<string>> data;
	vector<Statement> attainableStatements;
};