#include "FOLLOW.h"

RecordFollow::RecordFollow() {
	left = "";
	dependent = "";
	followSet.clear();
}
RecordFollow::RecordFollow(string _left, set<string> _followSet) {
	left = _left;
	dependent = "";
	followSet = _followSet;
}
RecordFollow::RecordFollow(string _left) {
	left = _left;
	dependent = "";
	followSet.clear();
}
bool RecordFollow::operator<(RecordFollow rf) {
	if (left != rf.left) {// 先比lefr
		return left > rf.left;
	}
	else if (dependent != rf.dependent) {// 在比dependent
		return dependent > rf.dependent;
	}
	else {//最后比followSet的个数
		return followSet.size() > rf.followSet.size();
	}
}
bool RecordFollow::operator==( const RecordFollow rf) const {
	if (left == rf.left && dependent == rf.dependent && followSet == rf.followSet) {
		return true;
	}
	return false;
}



SymbolFollow::SymbolFollow() {
	left = "";
	followSet.clear();
}
SymbolFollow::SymbolFollow(string _left) {
	left = _left;
	followSet.clear();
}