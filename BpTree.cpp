#include "BpTree.h"
#include <cmath>
#include <iomanip>

bool BpTree::Insert(int key, set<string> set) 
{
	FrequentPatternNode* newnode = new FrequentPatternNode;
	newnode->setFrequency(set.size());
	newnode->InsertList(set);
	BpTreeNode* insertdatanode;
	if (root == NULL)//First insert
	{
		BpTreeNode* newdatanode = new BpTreeDataNode;
		newdatanode->insertDataMap(key, newnode);
		root = newdatanode;//set root
		return true;
	}
	else//if root is not NULL // find insert point
	{
		insertdatanode = searchDataNode(key);//search data node
		if (insertdatanode->getDataMap()->find(key) == insertdatanode->getDataMap()->end())//can not find same data
		{
			insertdatanode->insertDataMap(key, newnode);
			if (excessDataNode(insertdatanode) == true)
				splitDataNode(insertdatanode);//splitdatanode
		}
		else//find same data
			insertdatanode->getDataMap()->find(key)->second->InsertList(set);
	}
	return true;//insert success
}

BpTreeNode* BpTree::searchDataNode(int n) 
{
	BpTreeNode* pCur = root;
	map<int, BpTreeNode*>::iterator point;
	while (pCur->getMostLeftChild() != NULL)
	{//search
		point = pCur->getIndexMap()->begin();
		if (point->first <= n)
		{//if n is bigger than point or same // so point++ and find continue
			while (1)
			{
				point++;
				if (point == pCur->getIndexMap()->end() || n < point->first)
				{//if iterator visit end or find point 
					point--;
					pCur = point->second;//move child
					break;
				}
			}
		}
		else//if n is smaller than point
			pCur = pCur->getMostLeftChild();//move leftchild
	}
	return pCur;//return find node
}

void BpTree::splitDataNode(BpTreeNode* pDataNode) 
{
	int split = ceil((order - 1) / 2.0) + 1;//spit data node index
	int count = 0;
	BpTreeNode* rightdatanode = new BpTreeDataNode;//right datanode of a division node
	auto center = pDataNode->getDataMap()->begin();//centernode iterator
	map<int, FrequentPatternNode*>::iterator temp;

	//find split node point
	for (int i = 1; i < split; i++)
		center++;
	temp = center;

	while (1)
	{//make a right datanode
		rightdatanode->insertDataMap(center->first, center->second);
		count++;//count right datanode
		if ((order - split) < count)
		{
			center = temp;//set center node
			//delete data to the right of the split point of an existing data node
			pDataNode->getDataMap()->erase(center, pDataNode->getDataMap()->end());
			break;
		}
		center++;
	}

	//set next and prev
	if (pDataNode->getNext() != NULL)
		pDataNode->getNext()->setPrev(rightdatanode);
	rightdatanode->setNext(pDataNode->getNext());
	rightdatanode->setPrev(pDataNode);
	pDataNode->setNext(rightdatanode);

	//indexnode setting
	if (pDataNode->getParent() != NULL)
	{//If not the first insertion of index node
		pDataNode->getParent()->insertIndexMap(rightdatanode->getDataMap()->begin()->first, rightdatanode);
		rightdatanode->setParent(pDataNode->getParent());
		if (excessIndexNode(pDataNode->getParent()) == true)
			splitIndexNode(pDataNode->getParent());
	}
	else
	{//first index node insert
		BpTreeNode* indexParentnode = new BpTreeIndexNode;
		indexParentnode->insertIndexMap(rightdatanode->getDataMap()->begin()->first, rightdatanode);
		root = indexParentnode;//set root
		pDataNode->setParent(indexParentnode);
		rightdatanode->setParent(indexParentnode);
		indexParentnode->setMostLeftChild(pDataNode);
	}
	return;
}

void BpTree::splitIndexNode(BpTreeNode* pIndexNode) 
{
	int split = ceil((order - 1) / 2.0) + 1;//split index
	auto point = pIndexNode->getIndexMap()->begin();
	auto temp = point;
	
	//find split node point
	for (int i = 1; i < split; i++)
		point++;
	temp = point;

	//make parent node	
	BpTreeNode* tempparentnode = new BpTreeIndexNode;//node to be parent
	tempparentnode->insertIndexMap(point->first, point->second);
	point->second->setParent(tempparentnode);

	//make new indexnode
	BpTreeNode* newindexnode = new BpTreeIndexNode;//node to be new indexnode
	point++;//iterator++ because of make new indexnode
	while (1)
	{
		newindexnode->insertIndexMap(point->first, point->second);
		point->second->setParent(newindexnode);
		point++;
		if (point == pIndexNode->getIndexMap()->end())
		{//delete existing node key
			pIndexNode->getIndexMap()->erase(temp, pIndexNode->getIndexMap()->end());
			break;
		}
	}

	//set parent
	newindexnode->setMostLeftChild(tempparentnode->getIndexMap()->begin()->second);
	newindexnode->setParent(tempparentnode);
	tempparentnode->getIndexMap()->begin()->second->setParent(newindexnode);
	tempparentnode->getIndexMap()->begin()->second = newindexnode;

	if (root == pIndexNode)//if existing node is root
	{
		pIndexNode->setParent(tempparentnode);//set parent
		tempparentnode->setMostLeftChild(pIndexNode);//set Leftchild
		root = tempparentnode;//change root node
	}
	else//if existing node is not root
	{
		//search pIndexNode insert point
		BpTreeNode* parentnode = pIndexNode->getParent();//real parentnode
		parentnode->insertIndexMap(tempparentnode->getIndexMap()->begin()->first, tempparentnode->getIndexMap()->begin()->second);
		auto point = parentnode->getIndexMap()->begin();
		for (auto i = point; i->first != tempparentnode->getIndexMap()->begin()->first; i++)
			point++;

		//set real parentnode
		newindexnode->setParent(parentnode);//change parent node

		//set pIndexNode point
		if (point->first == parentnode->getIndexMap()->begin()->first)
		{
			parentnode->setMostLeftChild(pIndexNode);//set child
		}
		else//if point->first != parentnode->getIndexMap()->begin()->first
		{
			--point;
			point->second = pIndexNode;//set child
		}

		//split recursive
		pIndexNode->setParent(parentnode);//set parent
		if (excessIndexNode(parentnode) == true)
			splitIndexNode(parentnode);
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


bool BpTree::printConfidence(string item, double item_frequency, double min_frequency)//double min_frequency
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
			multimap<int, set<string>>::iterator ia;//setting iterator
			multimap<int, set<string>> temp = it->second->getList();//get multimap
			ia = temp.begin();
			while (ia != temp.end())
			{
				double num = it->first / min_frequency;//confidence
				if (num >= item_frequency && ia->second.find(item) != ia->second.end())//if item exist, item confidence check
				{
					if (check == false)
					{
						*fout << "StandardItem" << " " << "FrequentPattern" << " " << "Frequency" << " " << "Confidence" << endl;
						cout << "StandardItem" << " " << "FrequentPattern" << " " << "Frequency" << " " << "Confidence" << endl;
					}
					*fout << item << " -> ";
					cout << item << " -> ";
					printFrequentPatterns(ia->second, item);//print frequentpatterns
					*fout << fixed;
					cout << fixed;
					*fout << " " << it->first << " "  << setprecision(2) << num << endl;
					cout << " " << it->first << " "  << setprecision(2) << num << endl;
					check = true;//Frequent Pattern exist
				}
				ia++;
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
bool BpTree::printFrequency(string item, int min_frequency)
{
	bool check = false;
	BpTreeNode* pCur = root;
	map<int, FrequentPatternNode*>::iterator it;
	pCur = searchDataNode(min_frequency);
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
							*fout << "StandardItem" << " " << "FrequentPattern" << " " << "Frequency" << endl;
							cout << "StandardItem" << " " << "FrequentPattern" << " " << "Frequency" << endl;
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
	if (check == false)//Frequent Pattern not exist
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
	pCur = searchDataNode(min);//find min data node
	while (pCur != NULL)
	{
		map<int, FrequentPatternNode*>* cur = pCur->getDataMap();//get datanode
		it = cur->begin();//begin iterator
		while (it != cur->end())
		{
			if (it->first <= max)//find higher than min and lower than max
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
							*fout << "StandardItem" << " " << "FrequentPattern" << " " << "Frequency" << endl;
							cout << "StandardItem" << " " << "FrequentPattern" << " " << "Frequency" << endl;
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
	curPattern.erase(item);//remove same item parameter
	for (set<string>::iterator it = curPattern.begin(); it != curPattern.end();) 
	{
		string temp = *it++;
		if (temp != item)
		{//print item
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
