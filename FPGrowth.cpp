#include "FPGrowth.h"


FPGrowth::~FPGrowth() {

}

void FPGrowth::createFPtree(FPNode* root, HeaderTable* table, list<string> item_array, int frequency) 
{
	FPNode* curNode = root;
	for (auto i = item_array.begin(); i != item_array.end(); i++)
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

void FPGrowth::connectNode(HeaderTable* table, string item, FPNode* node) 
{

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

//void FPGrowth::powerSet(map<set<string>, int>* FrequenctPattern, vector<string> data, string item, int frequency, int* ptr, int depth) {
//	if (data.size() == depth) {
//		set<string> set; set.insert(item);
//		for (int i = 0; i < data.size(); i++) { if (ptr[i] == 1) set.insert(data[i]); }
//		FrequenctPattern->insert(make_pair(set, frequency)); return;
//	}
//	ptr[depth] = 1;
//	powerSet(FrequenctPattern, data, item, frequency, ptr, depth + 1);
//	ptr[depth] = 0;
//	powerSet(FrequenctPattern, data, item, frequency, ptr, depth + 1);
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
	if (!this->getTree())
		return false;
	*fout << "{StandardItem,Frequency} (Path_Item,Frequency)" << endl;
	cout << "{StandardItem,Frequency} (Path_Item,Frequency)" << endl;


	return true;
}

//void FPGrowth::saveFrequentPatterns()
//{
//
//}