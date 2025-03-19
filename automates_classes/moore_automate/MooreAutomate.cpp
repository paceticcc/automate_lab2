#include "MooreAutomate.h"

MooreAutomate::MooreAutomate(ifstream& input)
{
	MooreAutomate::setData(input);
}

void MooreAutomate::setData(std::ifstream& input)
{
	string str = "";
	getline(input, str);
	vector<string> strSplit = split(str, ';');
	for (int i = 0; i < strSplit.size(); i++)
	{
		vector<string> statement{ strSplit[i] };
		MooreAutomate::data.push_back(statement);
	}
	getline(input, str);
	strSplit = split(str, ';');
	for (int i = 0; i < strSplit.size(); i++)
	{
		MooreAutomate::data[i].push_back(strSplit[i]);
	}
	getline(input, str);
	strSplit = split(str, ';');
	while (true)
	{
		for (int i = 0; i < strSplit.size(); i++)
		{
			string statement = strSplit[i];
			MooreAutomate::data[i].push_back(strSplit[i]);
		}
		if (getline(input, str))
			strSplit = split(str, ';');
		else
			break;
	}
}

int MooreAutomate::binarySearchInAttainableStatements(string statementName) const
{
	int left = 0;
	int right = MooreAutomate::attainableStatements.size() - 1;
	while (left < right)
	{
		int middle = left + (right - left) / 2;
		if (MooreAutomate::attainableStatements[middle].name == statementName)
		{
			return middle;
		}
		else if (statementName > MooreAutomate::attainableStatements[middle].name)
		{
			left = middle + 1;
		}
		else
		{
			right = middle - 1;
		}
	}
}

int MooreAutomate::binarySearchInData(string statementName) const
{ 
	int left = 0;
	int right = MooreAutomate::data.size() - 1;
	while (left < right)
	{
		int middle = left + (right - left) / 2;
		if (MooreAutomate::data[middle][1] == statementName)
		{
			return middle;
		}
		else if (statementName > MooreAutomate::data[middle][1])
		{
			left = middle + 1;
		}
		else
		{
			right = middle - 1;
		}
	}
}

void MooreAutomate::deleteUnattainableStatements()
{
	queue<string> BFSQueue;
	for (int i = 2; i < MooreAutomate::data[1].size(); i++)
	{
		string statement = split(MooreAutomate::data[1][i], '/')[0];
		if (statement != "-")
			BFSQueue.push(statement);
	}
	while (!BFSQueue.empty())
	{
		string searchStatement = BFSQueue.front();
		BFSQueue.pop();
		//auto it = find_if(MooreAutomate::attainableStatements.begin(), MooreAutomate::attainableStatements.end(),
			//isSearchStructStatement);
		auto it = MooreAutomate::attainableStatements.begin();
		while (it->name != searchStatement)
		{
			it++;
		}
		if (!it->isAttainable)
		{
			it->isAttainable = true;
			int statementIndex = distance(MooreAutomate::attainableStatements.begin(), it);
			for (int i = 2; i < MooreAutomate::data[statementIndex].size(); i++)
			{
				string statement = split(MooreAutomate::data[statementIndex][i], '/')[0];
				if (statement != "-")
					BFSQueue.push(statement);
			}
		}
	}
	int delta = 0;
	for (auto it = MooreAutomate::attainableStatements.begin(); it != MooreAutomate::attainableStatements.end(); it++)
	{
		if (!it->isAttainable)
		{
			auto dataIt = MooreAutomate::data.begin() + (distance(MooreAutomate::attainableStatements.begin(), it) + 1 - delta);
			delta++;
			MooreAutomate::data.erase(dataIt);
		}
	}
}

vector<vector<string>> MooreAutomate::getMinimizedAutomate() const
{
	bool isMinimized = false;
	vector<EquivalenceClass> prevEquivalenceClasses;
	vector<EquivalenceClass> currEquivalenceClasses;
	int classesCount = 1;
	for (auto statement = MooreAutomate::data.begin() + 1; statement != MooreAutomate::data.end(); statement++)
	{
		vector<string> outputSignal{ *statement->begin() };
		if (prevEquivalenceClasses.empty())
		{
			EquivalenceClass equivalenceClass{ 
				classesCount++,
				1,
				outputSignal,
				*(new set<string>{ *(statement->begin() + 1)}) 
			};
			prevEquivalenceClasses.push_back(equivalenceClass);
		}
		else
		{
			bool isFound = false;
			for (auto vecIt = prevEquivalenceClasses.begin(); vecIt != prevEquivalenceClasses.end(); vecIt++)
			{
				if (outputSignal == vecIt->outputSignals)
				{
					vecIt->equivalentStatements.insert(*(statement->begin() + 1));
					isFound = true;
					break;
				}
			}
			if (!isFound)
			{
				EquivalenceClass equivalenceClass{ 
					classesCount++,
					1,
					outputSignal,
					*(new set<string>{ *(statement->begin() + 1)}) 
				};
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
				auto it = MooreAutomate::data.begin();
				while (it->at(1) != statement)
				{
					it++;
				}
				vector<string> transitions;
				for (auto transition = it->begin() + 2; transition != it->end(); 
					transition++)
				{
					string targetStatement = *transition;
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
					EquivalenceClass newEquivalenceClass{ 
						classesCount++,
						equivalenceClass.classNum,
						transitions,
						*(new set<string>{ statement }) 
					};
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
						EquivalenceClass newEquivalenceClass{ 
							classesCount++,
							equivalenceClass.classNum,
							transitions,
							*(new set<string>{ statement }) 
						};
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
	result.push_back(MooreAutomate::data[0]);
	for (EquivalenceClass equivalenceClass : currEquivalenceClasses)
	{
		vector<string> statement;
		int i = 1;
		bool isFound = false;
		while (!isFound)
		{
			if (MooreAutomate::data[i][1] == *equivalenceClass.equivalentStatements.begin())
			{
				isFound = true;
			}
			else
			{
				i++;
			}
		}
		statement.push_back(MooreAutomate::data[i][0]);
		statement.push_back("X" + to_string(equivalenceClass.classNum));
		for (int j = 2; j < MooreAutomate::data[i].size(); j++)
		{
			string resultTransition = "X" + equivalenceClass.outputSignals[j - 2].erase(0, 1);
			statement.push_back(resultTransition);
		}
		result.push_back(statement);
	}
	return result;
}