#include "BpTree.h"
#include <cmath>

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
	int split = ceil((order - 1) / 2.0) + 1;//spit data node index
	int count = 0;
	map<int, FrequentPatternNode*>::iterator mleft;
	map<int, FrequentPatternNode*>::iterator center;
	map<int, FrequentPatternNode*>::iterator temp;
	mleft = pDataNode->getDataMap()->begin();

	for (int i = 1; i < order; i++)
	{//find split point datanode - center
		mleft++;
		if (i == split - 1)
		{
			center = mleft;
			temp = center;
		}
	}
	BpTreeNode* newdatanode = new BpTreeDataNode;//right datanode of a division node
	while (1)
	{//make a right datanode of a division node
		newdatanode->insertDataMap(center->first, center->second);
		count++;//count right datanode
		if (count > (order - split))
		{
			center = temp;//set center node
			break;
		}
		center++;
	}

	//delete data to the right of the split point of an existing data node
	pDataNode->getDataMap()->erase(center, pDataNode->getDataMap()->end());

	//set next and prev
	newdatanode->setNext(pDataNode->getNext());
	if (pDataNode->getNext() != NULL)
		pDataNode->getNext()->setPrev(newdatanode);
	pDataNode->setNext(newdatanode);
	newdatanode->setPrev(pDataNode);

	//indexnode setting
	if (pDataNode->getParent() == NULL)
	{//first index node insert
		BpTreeNode* indexParentnode = new BpTreeIndexNode;
		indexParentnode->insertIndexMap(newdatanode->getDataMap()->begin()->first, newdatanode);
		pDataNode->setParent(indexParentnode);
		newdatanode->setParent(indexParentnode);
		indexParentnode->setMostLeftChild(pDataNode);
		root = indexParentnode;
	}
	else
	{//If not the first insertion of index node
		pDataNode->getParent()->insertIndexMap(newdatanode->getDataMap()->begin()->first, newdatanode);
		newdatanode->setParent(pDataNode->getParent());
		if (excessIndexNode(pDataNode->getParent()) == true)
			splitIndexNode(pDataNode->getParent());
	}
	return;
}

void BpTree::splitIndexNode(BpTreeNode* pIndexNode) 
{
	int split = ceil((order - 1) / 2.0) + 1;//spit index node point
	BpTreeNode* newindexnode = new BpTreeIndexNode;
	map <int, BpTreeNode*>::iterator mleft;
	map <int, BpTreeNode*>::iterator center;
	map <int, BpTreeNode*>::iterator temp;
	mleft = pIndexNode->getIndexMap()->begin();
	for (int i = 1; i <= order; i++)
	{//find split point indexnode - center
		if (split < i + 1)
		{
			if (split == i)
			{
				center = mleft;
				temp = center;
			}
			else
				newindexnode->insertIndexMap(mleft->first, mleft->second);
		}
		mleft++;
	}
	//set existing
	newindexnode->setMostLeftChild(center->second);
	center->second->setParent(newindexnode);

	//set parent of children 
	for (int i = 1; i <= order - split; i++)
		newindexnode->getIndexMap()->begin()++->second->setParent(newindexnode);

	//delete data to the right of the split point of an existing index node
	pIndexNode->getIndexMap()->erase(++center, mleft);

	//set center
	center = temp;

	//indexnode setting
	if (pIndexNode->getParent() == NULL)
	{//pIndexNode don't have parent
		BpTreeNode* newroot = new BpTreeIndexNode;
		pIndexNode->setParent(newroot);
		newindexnode->setParent(newroot);
		newroot->insertIndexMap(center->first, newindexnode);
		newroot->setMostLeftChild(pIndexNode);
		pIndexNode->getIndexMap()->erase(center);
		root = newroot;
	}
	else
	{//pIndexNode have parent
		newindexnode->setParent(pIndexNode->getParent());
		pIndexNode->getParent()->insertIndexMap(center->first, newindexnode);
		pIndexNode->getIndexMap()->erase(center);
		if (excessIndexNode(pIndexNode->getParent()) == true)
			splitIndexNode(pIndexNode->getParent());
	}
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


bool BpTree::printConfidence(string item, double item_frequency, int min_frequency)//double min_frequency
{

	return true;
}
bool BpTree::printFrequency(string item, int min_frequency)
{
	bool check = false;
	BpTreeNode* pCur = root;
	map<int, FrequentPatternNode*>::iterator it;
	while (pCur->getMostLeftChild() != NULL)//find mostleft data node
		pCur = pCur->getMostLeftChild();//pCur is datanode begin()
	while(pCur != NULL)
	{	
		map<int, FrequentPatternNode*> *cur = pCur->getDataMap();//get datanode
		it = cur->begin();//begin iterator
		while(it != cur->end())
		{
			if (min_frequency <= it->first)//find higher than min_frequency
			{
				multimap<int, set<string>>::iterator ia;//setting iterator
				multimap<int, set<string>> temp = it->second->getList();//get multimap
				ia = temp.begin();
				while (ia != temp.end())
				{
					if (ia->second.find(item) != ia->second.end())//if item exist
					{
						if (check == false)
						{
							*fout << "StandardItem" << "    " << "FrequentPattern" << "    " << "Frequency" << endl;
							cout << "StandardItem" << "    " << "FrequentPatternFrequency" << "    " << "Frequency" << endl;
						}
						*fout << item << " -> ";
						cout << item << " -> ";
						printFrequentPatterns(ia->second, item);//print frequentpatterns
						*fout << " " << it->first << endl;
						cout << " " << it->first << endl;
						check = true;//Frequent Pattern exist
					}
					ia++;
				}
			}
			it++;
		}
		pCur = pCur->getNext();// move next datanode
	}
	if (check == false)////Frequent Pattern not exist
		return false;
	*fout << "================================" << endl << endl;
	cout << "================================" << endl << endl;
	return true;//print success
}
bool BpTree::printRange(string item, int min, int max) 
{
	bool check = false;
	BpTreeNode* pCur = root;
	map<int, FrequentPatternNode*>::iterator it;
	while (pCur->getMostLeftChild() != NULL)//find mostleft data node
		pCur = pCur->getMostLeftChild();//pCur is datanode begin()
	while (pCur != NULL)
	{
		map<int, FrequentPatternNode*>* cur = pCur->getDataMap();//get datanode
		it = cur->begin();//begin iterator
		while (it != cur->end())
		{
			if (min <= it->first && it->first <= max)//find higher than min and lower than max
			{
				multimap<int, set<string>>::iterator ia;//setting iterator
				multimap<int, set<string>> temp = it->second->getList();//get multimap
				ia = temp.begin();
				while (ia != temp.end())
				{
					if (ia->second.find(item) != ia->second.end())//if item exist
					{
						if (check == false)
						{
							*fout << "StandardItem" << "    " << "FrequentPattern" << "    " << "Frequency" << endl;
							cout << "StandardItem" << "    " << "FrequentPatternFrequency" << "    " << "Frequency" << endl;
						}
						*fout << item << " -> ";
						cout << item << " -> ";
						printFrequentPatterns(ia->second, item);//print frequentpatterns
						*fout << " " << it->first << endl;
						cout << " " << it->first << endl;
						check = true;//Frequent Pattern exist
					}
					ia++;
				}
			}
			it++;
		}
		pCur = pCur->getNext();// move next datanode
	}
	if (check == false)////Frequent Pattern not exist
		return false;
	*fout << "================================" << endl << endl;
	cout << "================================" << endl << endl;
	return true;
}
void BpTree::printFrequentPatterns(set<string> pFrequentPattern, string item)
{
	*fout << "{";
	cout << "{";
	set<string> curPattern = pFrequentPattern;
	curPattern.erase(item);
	for (set<string>::iterator it = curPattern.begin(); it != curPattern.end();) 
	{
		string temp = *it++;
		if (temp != item)
		{
			*fout << temp;
			cout << temp;
		}
		if (it == curPattern.end()) 
		{
			*fout << "} ";
			cout << "} ";
			break;
		}
		*fout << ", ";
		cout << ", ";
	}
}
