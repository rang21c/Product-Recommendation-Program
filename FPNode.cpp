#include "FPNode.h"

FPNode::FPNode()	//constructor
{
	parent = NULL;
	next = NULL;
	item = new char[20]; memset(item, 0, 20);
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
	if (it == children.end()) return NULL;
	FPNode* findNode = it->second;
	return findNode;
}


