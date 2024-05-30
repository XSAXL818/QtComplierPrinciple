#pragma once
#include<string>
#include<set>
#include<algorithm>
using namespace std;

/// @brief 蕴含的Follow信息表
class RecordFollow {
public:
	string left;
	string dependent;
	set<string> followSet;

	RecordFollow();
	RecordFollow(string _left);
	RecordFollow(string _left,set<string> _followSet);
	bool operator<(RecordFollow rf);
	bool operator==(RecordFollow rf) const;
};
class SymbolFollow {
public:
	string left;
	set<string> followSet;

	SymbolFollow();
	SymbolFollow(string _left);
	SymbolFollow(string _left, set<string> _followSet) {
		left = _left;
		followSet = _followSet;
	}

	bool operator<(SymbolFollow sf) const {
		if (left == sf.left) {
			return followSet < sf.followSet;
		}
		return left < sf.left;
	}
	bool operator==( const SymbolFollow sf )const {
		if (left != sf.left && followSet != sf.followSet) {
			return false;
		}
		return true;
	}
};


