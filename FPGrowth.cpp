#include "FPGrowth.h"


FPGrowth::~FPGrowth() {

}

void FPGrowth::createFPtree(FPNode* root, HeaderTable* table, list<string> item_array, int frequency) 
{
	FPNode* curNode = root;
	for (auto i = item_array.begin(); i != item_array.end(); i++)
	{
		if (this->item_frequency((*i)) >= this->threshold)//threshold check
		{
			if (curNode->getChildrenNode((*i)))//item exist
			{
				curNode->getChildrenNode((*i))->updateFrequency(1);//frequency++
				curNode = curNode->getChildrenNode((*i));
			}
			else//item no exist
			{
				FPNode* newnode = new FPNode;
				newnode->setItem((char*)(*i).c_str());//set item name
				newnode->updateFrequency(frequency);//set frequency
				newnode->setParent(curNode);//set parent
				curNode->pushchildren((*i), newnode);
				connectNode(table, (*i), newnode);
				curNode = newnode;
			}
		}
	}
}

void FPGrowth::connectNode(HeaderTable* table, string item, FPNode* node) 
{
	if (table->getNode(item)->getNext() == NULL)//First insert of nextnode
	{
		table->getNode(item)->setNext(node);
	}
	else//node connect if not First
	{
		FPNode* temp = table->getNode(item);
		while (1)
		{
			if (temp->getNext() == NULL)
				break;
			temp = temp->getNext();
		}
		temp->setNext(node);
	}
}

//bool FPGrowth::contains_single_path(FPNode* pNode) 
//{
//	if (pNode->getChildren().size() == 0) return true;
//	else if (pNode->getChildren().size() > 1) return false;
//	return contains_single_path(pNode->getChildren().begin()->second);
//}
//
//map<set<string>, int> FPGrowth::getFrequentPatterns(HeaderTable* pTable, FPNode* pTree) {
//
//	return {};
//}

//void FPGrowth::powerSet(map<set<string>, int>* FrequentPattern, vector<string> data, string item, int frequency, int* ptr, int depth) {
//	if (data.size() == depth) {
//		set<string> set; set.insert(item);
//		for (int i = 0; i < data.size(); i++) { if (ptr[i] == 1) set.insert(data[i]); }
//		FrequentPattern->insert(make_pair(set, frequency)); return;
//	}
//	ptr[depth] = 1;
//	powerSet(FrequentPattern, data, item, frequency, ptr, depth + 1);
//	ptr[depth] = 0;
//	powerSet(FrequentPattern, data, item, frequency, ptr, depth + 1);
//}

bool FPGrowth::printList() 
{//PRINT_ITEMLIST
	if (this->table->getindexTable().empty())
		return false;
	auto temp = this->table->getindexTable();
	*fout << "Item	Frequency" << endl;
	cout << "Item	Frequency" << endl;
	for (auto it = temp.begin(); it != temp.end(); it++)
	{
		*fout << (*it).second << " " << (*it).first << endl;
		cout << (*it).second << " " << (*it).first << endl;
	}
	*fout << "===============================" << endl << endl;
	cout << "===============================" << endl << endl;
	return true;
}

bool FPGrowth::printTree() 
{//PRINT_FPTREE
	if (this->getTree()->getChildren().empty())
		return false;//Tree is empty
	*fout << "{StandardItem,Frequency} (Path_Item,Frequency)" << endl;
	cout << "{StandardItem,Frequency} (Path_Item,Frequency)" << endl;
	this->table->ascendingIndexTable();//IndexTable ascending order
	auto temp = this->table->getindexTable();
	for (auto it = temp.begin(); it != temp.end(); it++)
	{
		if (this->table->getNode((*it).second)->getFrequency() >= this->threshold)
		{//Print if more than threshold
			*fout << "{" << (*it).second << "," << (*it).first << "}" << endl;
			cout << "{" << (*it).second << "," << (*it).first << "}" << endl;
			FPNode* searchnode = this->table->getNode((*it).second)->getNext();
			FPNode* searchparent;
			while (searchnode != NULL)
			{//Next node search and print
				searchparent = searchnode;
				while (searchparent->getParent() != NULL)
				{//Parent node search and print
					*fout << "(" << searchparent->getItem() << "," << searchparent->getFrequency() << ")" << " ";
					cout << "(" << searchparent->getItem() << "," << searchparent->getFrequency() << ")" << " ";
					searchparent = searchparent->getParent();
				}
				*fout << endl;
				cout << endl;
				searchnode = searchnode->getNext();
			}
		}
	}
	*fout << "===============================" << endl << endl;
	cout << "===============================" << endl << endl;
	return true;
}

//void FPGrowth::saveFrequentPatterns()
//{
//
//}