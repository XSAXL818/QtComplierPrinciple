#pragma once
#include <string>
#include <vector>
#include<algorithm>
#include<set>

using namespace std;



#define CHAR_NULL '@' // 空字符，用@表示
#define STRING_NULL "@" // 空字串
#define EOS 36 //输入语句的结束标志,用于语法分析
#define DYH 39 // 单引号
constexpr auto CHAR_EOS = '$';
constexpr auto STRING_EOS = "$";



class Production {// 某个非终结符的所有产生式 
public:
	int id;// 用于蕴含表 
	string left;
	vector<string> right;
	int have_null; // 是否有空产生式 
	Production();
	bool operator==(const Production& p) const {
		return left == p.left;
	}
	bool operator<(const Production& p) const {
		if (left != p.left) {
			return left < p.left;
		}
		return right.size() < p.right.size();
	}
};
// 文法，四元组表示，<开始符，规则集（产生式），终结符集，非终结符集>
class Grammar {
public:
	string start;//开始符
	vector<Production> productions;// P：产生式
	set<char> terms;//终结符集合
	set<string> nons;//非终结符集合
	
	Grammar() {
		start = "";
		productions.clear();
		terms.clear();
		nons.clear();
	}
	Grammar(string _start, vector<Production> _productions, set<string> _nons, set<char> _terms ) {
		start = _start;
		terms = _terms;
		productions = _productions;
		nons = _nons;
	}
};
// 文法plus，四元组表示，<开始符，规则集（产生式），终结符集，非终结符集>
///优化：非终结符
class GrammarPlus {
public:
	string start;//开始符
	vector<Production> productions;// P：产生式
	set<string> terms;//终结符集合
	set<string> nons;//非终结符集合

	GrammarPlus() {
		start = "";
		productions.clear();
		terms.clear();
		nons.clear();
	}
	GrammarPlus(string _start, vector<Production> _productions, set<string> _nons, set<string> _terms) {
		start = _start;
		terms = _terms;
		productions = _productions;
		nons = _nons;
	}
};