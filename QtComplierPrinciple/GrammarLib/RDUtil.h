#pragma once
#include<iostream>
#include<vector>
#include<set>
#include<string>
#include<stack>
#include<queue>
#include<map>
#include<algorithm>
#include"Grammar.h"

using namespace std;
constexpr auto NON = 0;
constexpr auto TERM = 1;

// 符号，包括其值和类型（非终结符和终结符）
class SignType {
public:
	int type;// 类型
	string value;// 值
	SignType(){
		type = 0;
		value = "";
	}
	SignType(int _type, string _value){
		type = _type;
		value = _value;
	}
	bool operator<( SignType s ) const {
		if (type != s.type) {
			return type < s.type;
		}
		return value < s.value;
	}
};



class RDGrammar {
public:
	string start;//开始符
	set<string> nons;// 非终结符集合
	set<string> terms;// 终结符集合
	set<string> str_terms;// 可输入的单词
	set<string> null_pro;// 含有空产生式的非终结符集合
	map<string, vector< vector<SignType> > > regulars;// 某个非终结符的规则集
	RDGrammar() {
		start = "";
		nons.clear();
		terms.clear();
		regulars.clear();
		str_terms.clear();
		null_pro.clear();
	}
	RDGrammar(string _start, set<string> _nons, set<string> _terms, set<string> _str_terms, set<string> _null_pro, map<string, vector< vector<SignType> > > _regulars) {
		start = _start;
		nons = _nons;
		terms = _terms;
		regulars = _regulars;
		str_terms = _str_terms;
		null_pro = _null_pro;
	}


	/// <summary>
	/// 打印文法
	/// </summary>
	void printRDGrammar();
};


class RDGUtil{
public:
	static pair<int, bool> stringToInt(string str);
	static vector<SignType> getSignTypeFromProduction(string production);
	static bool isDelimiter(char c);
	static bool isOperator();
	static RDGrammar grammarToRDGrammar(Grammar& g);



private:
	static string switchString(string src);
};

