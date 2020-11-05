#include "FPNode.h"

FPNode::FPNode()//constructor
{
	parent = NULL;
	next = NULL;
	item = new char[40]; memset(item, 0, 40);
	frequency = 0;
}

FPNode::~FPNode()//destructor
{
	parent = NULL;
	next = NULL;
	delete[] item;
	frequency = 0;
}

FPNode* FPNode::getChildrenNode(string item) 
{
	map<string, FPNode*>::iterator it = children.find(item);
	if (it == children.end()) return NULL;//can not find
	FPNode* findNode = it->second;
	return findNode;//find
}


