#include "MealyAutomate.h"
#include <iostream>

MealyAutomate::MealyAutomate(ifstream& input)
{
	setData(input);
}
void MealyAutomate::setData(ifstream& input)
{
	string str = "";
	getline(input, str);
	vector<string> strSplit = split(str, ';');
	if (strSplit.size() > 1)
	{
		MealyAutomate::data.push_back(vector<string>{ strSplit[0] });
		for (int i = 1; i < strSplit.size(); i++)
		{
			MealyAutomate::attainableStatements.push_back({ strSplit[i], false });
			vector<string> statement{ strSplit[i] };
			MealyAutomate::data.push_back(statement);
		}
	}
	MealyAutomate::attainableStatements[0].isAttainable = true;
	getline(input, str);
	strSplit = split(str, ';');
	while (true)
	{
		for (int i = 0; i < strSplit.size(); i++)
		{
			MealyAutomate::data[i].push_back(strSplit[i]);
		}
		if (getline(input, str))
			strSplit = split(str, ';');
		else
			break;
	}
}

/*string searchStatement = "";

bool isSearchVectorStatement(vector<string> statement)
{
	if (statement[0] == searchStatement)
		return true;
	return false;
}

bool isSearchStructStatement(Statement statement)
{
	if (statement.name == searchStatement)
		return true;
	return false;
}*/

int MealyAutomate::binarySearchInAttainableStatements(string statementName) const
{
	int left = 0;
	int right = MealyAutomate::attainableStatements.size() - 1;
	while (left <= right)
	{
		int middle = left + (right - left) / 2;
		if (MealyAutomate::attainableStatements[middle].name == statementName)
		{
			return middle;
		}
		else if (statementName > MealyAutomate::attainableStatements[middle].name)
		{
			left = middle + 1;
		}
		else
		{
			right = middle - 1;
		}
	}
}

int MealyAutomate::binarySearchInData(string statementName) const
{
	int left = 0;
	int right = MealyAutomate::data.size() - 1;
	while (left <= right)
	{
		int middle = left + (right - left) / 2;
		if (MealyAutomate::data[middle][0] == statementName)
		{
			return middle;
		}
		else if (statementName > MealyAutomate::data[middle][0])
		{
			left = middle + 1;
		}
		else
		{
			right = middle - 1;
		}
	}
}

void MealyAutomate::deleteUnattainableStatements()
{
	queue<string> BFSQueue;
	for (int i = 1; i < MealyAutomate::data[1].size(); i++)
	{
		string statement = split(MealyAutomate::data[1][i], '/')[0];
		if (statement != "-")
			BFSQueue.push(statement);
	}
	while (!BFSQueue.empty())
	{
		string targetStatement = BFSQueue.front();
		BFSQueue.pop();
		//auto it = find_if(MealyAutomate::attainableStatements.begin(), MealyAutomate::attainableStatements.end(),
			//isSearchStructStatement);
		auto it = MealyAutomate::attainableStatements.begin();
		while (it->name != targetStatement && it != MealyAutomate::attainableStatements.end())
		{
			it++;
		}
		if (!it->isAttainable)
		{
			it->isAttainable = true;
			int statementIndex = distance(MealyAutomate::attainableStatements.begin(), it);
			for (int i = 1; i < MealyAutomate::data[statementIndex + 1].size(); i++)
			{
				string statement = split(MealyAutomate::data[statementIndex + 1][i], '/')[0];
				if (statement != "-")
					BFSQueue.push(statement);
			}
		}
	}
	int delta = 0;
	for (auto it = MealyAutomate::attainableStatements.begin(); it != MealyAutomate::attainableStatements.end(); it++)
	{
		if (!it->isAttainable)
		{
			auto dataIt = MealyAutomate::data.begin() + (distance(MealyAutomate::attainableStatements.begin(), it) + 1 - delta);
			delta++;
			MealyAutomate::data.erase(dataIt);
		}
	}
}

vector<vector<string>> MealyAutomate::getMinimizedAutomate() const
{
	bool isMinimized = false;
	vector<EquivalenceClass> prevEquivalenceClasses;
	vector<EquivalenceClass> currEquivalenceClasses;
	int classesCount = 0;
	for (auto statement = MealyAutomate::data.begin() + 1; statement != MealyAutomate::data.end(); statement++)
	{
		vector<string> outputSignals;
		for (auto transition = statement->begin() + 1; transition != statement->end(); transition++)
		{
			string outputSignal = split(*transition, '/')[1];
			outputSignals.push_back(outputSignal);
		}
		if (prevEquivalenceClasses.empty())
		{
			EquivalenceClass equivalenceClass{ ++classesCount, 1, outputSignals, *(new set<string>{*statement->begin()})};
			prevEquivalenceClasses.push_back(equivalenceClass);
		}
		else
		{
			bool isFound = false;
			for (auto vecIt = prevEquivalenceClasses.begin(); vecIt != prevEquivalenceClasses.end(); vecIt++)
			{
				if (outputSignals == vecIt->outputSignals)
				{
					vecIt->equivalentStatements.insert(*statement->begin());
					isFound = true;
					break;
				}
			}
			if (!isFound)
			{
				EquivalenceClass equivalenceClass{ ++classesCount, 1, outputSignals, *(new set<string>{*statement->begin()}) };
				prevEquivalenceClasses.push_back(equivalenceClass);
			}
		}
	}
	while (!isMinimized)
	{
		classesCount = 0;
		currEquivalenceClasses.clear();
		for (EquivalenceClass equivalenceClass : prevEquivalenceClasses)
		{
			for (string statement : equivalenceClass.equivalentStatements)
			{
				if (statement == "-")
					continue;
				//auto it = find_if(MealyAutomate::data.begin(), MealyAutomate::data.end(), isSearchVectorStatement);
				auto it = MealyAutomate::data.begin();
				while (it->front() != statement && it != MealyAutomate::data.end())
				{
					it++;
				}
				vector<string> transitions;
				for (auto transition = it->begin() + 1; transition != it->end(); transition++)
				{
					string targetStatement = split(*transition, '/')[0];
					if (targetStatement != "-")
					{
						for (int j = 0; j < prevEquivalenceClasses.size(); j++)
						{
							if (prevEquivalenceClasses[j].equivalentStatements.count(targetStatement) == 1)
							{
								transitions.push_back("A" + to_string(prevEquivalenceClasses[j].classNum));
								break;
							}
						}
					}
					else
					{
						transitions.push_back("-");
					}
				}
				if (currEquivalenceClasses.empty())
				{
					EquivalenceClass newEquivalenceClass{ ++classesCount, equivalenceClass.classNum, transitions,
														  *(new set<string>{ statement }) };
					currEquivalenceClasses.push_back(newEquivalenceClass);
				}
				else
				{
					bool isFound = false;
					for (auto vecIt = currEquivalenceClasses.begin(); vecIt != currEquivalenceClasses.end(); vecIt++)
					{
						if ((transitions == vecIt->outputSignals) && (equivalenceClass.classNum == vecIt->parentClassNum))
						{
							vecIt->equivalentStatements.insert(statement);
							isFound = true;
							break;
						}
					}
					if (!isFound)
					{
						EquivalenceClass newEquivalenceClass{ ++classesCount, equivalenceClass.classNum, transitions,
															  *(new set<string>{ statement }) };
						currEquivalenceClasses.push_back(newEquivalenceClass);
					}
				}
			}
		}
		if (prevEquivalenceClasses.size() == currEquivalenceClasses.size())
			isMinimized = true;
		prevEquivalenceClasses = currEquivalenceClasses;
	}
	vector<vector<string>> result;
	result.push_back(MealyAutomate::data[0]);
	for (EquivalenceClass equivalenceClass : currEquivalenceClasses)
	{
		vector<string> statement;
		statement.push_back("X" + to_string(equivalenceClass.classNum));
		int i = 1;
		bool isFound = false;
		while (!isFound)
		{
			if (MealyAutomate::data[i][0] == *equivalenceClass.equivalentStatements.begin())
			{
				isFound = true;
			}
			else
			{
				i++;
			}
		}
		for (int j = 1; j < MealyAutomate::data[i].size(); j++)
		{
			vector<string> splitedTransition = split(MealyAutomate::data[i][j], '/');
			string resultTransition = "X" + (equivalenceClass.outputSignals[j - 1].erase(0, 1)) + "/" + splitedTransition[1];
			statement.push_back(resultTransition);
		}
		result.push_back(statement);
	}
	return result;
}