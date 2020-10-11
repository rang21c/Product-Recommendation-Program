#include "FPGrowth.h"


FPGrowth::~FPGrowth() {

}

void FPGrowth::createFPtree(FPNode* root, HeaderTable* table, list<string> item_array, int frequency) {


}

void FPGrowth::connectNode(HeaderTable* table, string item, FPNode* node) {

}

bool FPGrowth::contains_single_path(FPNode* pNode) {
	if (pNode->getChildren().size() == 0) return true;
	else if (pNode->getChildren().size() > 1) return false;
	return contains_single_path(pNode->getChildren().begin()->second);
}

map<set<string>, int> FPGrowth::getFrequentPatterns(HeaderTable* pTable, FPNode* pTree) {

	return {};
}



void FPGrowth::powerSet(map<set<string>, int>* FrequenctPattern, vector<string> data, string item, int frequency, int* ptr, int depth) {
	if (data.size() == depth) {
		set<string> set; set.insert(item);
		for (int i = 0; i < data.size(); i++) { if (ptr[i] == 1) set.insert(data[i]); }
		FrequenctPattern->insert(make_pair(set, frequency)); return;
	}
	ptr[depth] = 1;
	powerSet(FrequenctPattern, data, item, frequency, ptr, depth + 1);
	ptr[depth] = 0;
	powerSet(FrequenctPattern, data, item, frequency, ptr, depth + 1);
}

bool FPGrowth::printList() {

	return true;
}
bool FPGrowth::printTree() {
	return true;
}
void FPGrowth::saveFrequentPatterns(){

}
