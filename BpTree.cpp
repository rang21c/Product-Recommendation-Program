#include "BpTree.h"

bool BpTree::Insert(int key, set<string> set) 
{
	if (BpTreeNode* temp = this->searchDataNode(key))
	{//Insert same Datanode
		if(temp->getDataMap()->count(key) != 0)//Insert same FrequentPatternNode
			temp->getDataMap()->find(key)->second->InsertList(set);
		else if(!this->excessDataNode(temp))
		{//if not overflow
			FrequentPatternNode* newnode = new FrequentPatternNode;
			newnode->setFrequency(set.size());
			newnode->InsertList(set);
			temp->insertDataMap(key, newnode);
		}
	}
	else if(this->excessDataNode(temp))//overflow
	{

	}
	else//normal
	{
		FrequentPatternNode* newnode = new FrequentPatternNode;
		newnode->setFrequency(set.size());
		newnode->InsertList(set);
		if (root == NULL)
		{//First insert
			BpTreeDataNode* newdatanode = new BpTreeDataNode;
			newdatanode->insertDataMap(key, newnode);
			root = newdatanode;
		}
	}
	return true;
}

BpTreeNode* BpTree::searchDataNode(int n) 
{
	BpTreeNode* pCur = root;
	if (root == NULL)//if the tree is empty
		return NULL;
	while (1)
	{
		if (!pCur->getDataMap())
		{
			auto it = pCur->getIndexMap();
			for (auto i = it->begin(); i != it->end(); i++)
			{
				if (n <= (*i).first)
				{
					if (i == it->begin())
					{
						pCur = pCur->getMostLeftChild();
						break;
					}
					pCur = --(*i).second;
					break;
				}
				if (i == it->end())
				{
					pCur = (*i).second;
					break;
				}
			}
		}
		else
		{
			return pCur;
		}
	}
	return NULL;
}

void BpTree::splitDataNode(BpTreeNode* pDataNode) 
{
	
}

void BpTree::splitIndexNode(BpTreeNode* pIndexNode) 
{
	
}

bool BpTree::excessDataNode(BpTreeNode* pDataNode) 
{
	if (pDataNode == NULL)
		return false;
	else if (pDataNode->getDataMap()->size() >= order - 1) 
		return true;//order is equal to the number of elements 
	else return false;
}

bool BpTree::excessIndexNode(BpTreeNode* pIndexNode) 
{
	if (pIndexNode == NULL)
		return false;
	else if (pIndexNode->getIndexMap()->size() >= order - 1)
		return true;//order is equal to the number of elements 
	else return false;
}


bool BpTree::printConfidence(string item, double item_frequency, int min_frequency)
{

	return true;
}
bool BpTree::printFrequency(string item, int min_frequency)//print winratio in ascending order
{

	return true;
}
bool BpTree::printRange(string item, int min, int max) 
{

	return true;
}
void BpTree::printFrequentPatterns(set<string> pFrequentPattern, string item)
{
	*fout << "{";
	set<string> curPattern = pFrequentPattern;
	curPattern.erase(item);
	for (set<string>::iterator it = curPattern.begin(); it != curPattern.end();) {
		string temp = *it++;
		if (temp != item) *fout << temp;
		if (it == curPattern.end()) {
			*fout << "} ";
			break;
		}
		*fout << ", ";
	}
}
