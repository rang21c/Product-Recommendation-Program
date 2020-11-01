#include "BpTree.h"

bool BpTree::Insert(int key, set<string> set) 
{
	FrequentPatternNode* newnode = new FrequentPatternNode;
	newnode->setFrequency(set.size());
	newnode->InsertList(set);
	if (root == NULL)//First insert
	{
		BpTreeNode* pDataNode = new BpTreeDataNode;
		pDataNode->insertDataMap(key, newnode);
		root = pDataNode;
		return true;
	}
	BpTreeNode* pDataNode = searchDataNode(key);//search data node

	map<int, FrequentPatternNode*>* m = pDataNode->getDataMap();
	if (m->find(key) == m->end())//can not find same data
	{
		pDataNode->insertDataMap(key, newnode);
		if (excessDataNode(pDataNode) == true)
			splitDataNode(pDataNode);//splitdatanode
	}
	else//find same data
	{
		m->find(key)->second->InsertList(set);
	}
	return true;//insert success
}

BpTreeNode* BpTree::searchDataNode(int n) 
{
	BpTreeNode* pCur = root;
	map<int, BpTreeNode*>::iterator it;

	while (pCur->getMostLeftChild() != NULL)
	{
		it = pCur->getIndexMap()->begin();
		if (n < it->first)
			pCur = pCur->getMostLeftChild();
		else
		{
			while (1)
			{
				it++;
				if (it == pCur->getIndexMap()->end() || n < it->first)
				{
					it--;
					pCur = it->second;
					break;
				}
			}
		}
	}
	return pCur;
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
	else if (pDataNode->getDataMap()->size() > order - 1) 
		return true;//order is equal to the number of elements 
	else return false;
}

bool BpTree::excessIndexNode(BpTreeNode* pIndexNode) 
{
	if (pIndexNode == NULL)
		return false;
	else if (pIndexNode->getIndexMap()->size() > order - 1)
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
