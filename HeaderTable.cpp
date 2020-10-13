#include "HeaderTable.h"
#include <cstring>
#include <iostream>
using namespace std;

HeaderTable::~HeaderTable() 
{

}

void HeaderTable::insertTable(char* item, int frequency) 
{
	FPNode* newnode = new FPNode;
	newnode->setItem(item);
	if (indexTable.empty())
	{//indextable is empty
		newnode->setFrequency(1);
		indexTable.push_back(make_pair(frequency, item));
		dataTable.insert(pair<string, FPNode*>(item, newnode));
	}
	else
	{//indextable is not empty
		if (find_frequency(item) == 0)
		{//item no exist
			newnode->setFrequency(1);
			indexTable.push_back(make_pair(frequency, item));
			dataTable.insert(pair<string, FPNode*>(item, newnode));
		}
		else
		{//item exist
			//datatable FPNode frequency + 1
			(*dataTable.find(item)).second->setFrequency((*dataTable.find(item)).second->getFrequency() + 1);
			for (auto it = indexTable.begin(); it != indexTable.end(); it++)
			{//indextable frequency + 1
				if ((*it).second == item)
					(*it).first++;
			}
		}
	}
}

int HeaderTable::find_frequency(string item)
{//find item and return frequency
	int frequency = 0;
	for (auto it = indexTable.begin(); it != indexTable.end(); it++)
	{
		if((*it).second == item)
			return (*it).first;//return frequency
	}
	return 0;
}
