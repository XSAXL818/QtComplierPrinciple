#include "FIRST.h"


RecordFirst::RecordFirst() {
	left = "";
	dependent = "";
	firstSet.clear();
}
RecordFirst::RecordFirst(int _proID) {
	proID = _proID;
	left = "";
	dependent = "";
	firstSet.clear();
}
/// @brief 创建一个虚产生式的记录
/// @param _proID 
/// @param emp 
RecordFirst::RecordFirst(int _proID, string _left) {
	proID = _proID;
	left = _left;
	dependent = "";
	firstSet.clear();
	firstSet.insert("@");
}

SymbolFirst::SymbolFirst() {
	left = "";
	firstSet.clear();
}

ProductionFirst::ProductionFirst() {
	proID = -1;
	left = "";
	firstSet.clear();
}