#include "RDUtil.h"


pair<int, bool> RDGUtil::stringToInt(string str) {
	int i = 0;
	int sum = 0;
	bool isDig = true;
	while ( str[i] >= '0' && str[i] <= '9' && i < str.size() ) {
		sum *= 10;
		sum += str[i]-'0';
		i++;
	}
	if (i != str.size()) {
		isDig = false;
	}
	return { sum,isDig };
}
/// <summary>
/// 给定一个产生式，将其分解为符号数组，每个元素是该符号的值和类型
/// </summary>
/// <param name="production"></param>
/// <returns></returns>
vector<SignType> RDGUtil::getSignTypeFromProduction(string production){
	vector<SignType> vst;
	//每次遇到 | 说明遇到一个单词
	production += "|";
	SignType s;
	int i = 0;
	while (i < production.size()) {
		string word = "";
		// 非终结符,每次添加一个字符时，指针停在该字符的最后一个字母
		if (production[i] >= 'A' && production[i] <= 'Z') {
			word += production[i];
			if (production[i+1] == DYH) {
				word += production[++i];
			}
			s.value = word;
			s.type = NON;
			vst.push_back(s);
		}// 终结符的单词
		else if (production[i] >= 'a' && production[i] <= 'z' || production[i] >= '0' && production[i] <= '9') {
			word += production[i];
			while (production[i+1] >= 'a' && production[i+1] <= 'z' || production[i+1] >= '0' && production[i+1] <= '9') {
				word += production[++i];
			}
			s.value = word;
			s.type = TERM;
			vst.push_back(s);
		}// 终结符的界限符
		else if ( isDelimiter(production[i])) {
			word += production[i];
			s.value = word;
			s.type = TERM;
			vst.push_back(s);
		}/// 终结符的运算符
		else if (isOperator()) {

		}
		i++;
	}
	return vst;
}

bool RDGUtil::isDelimiter(char c)
{
	set<char> delimiter{ DYH, ';', ',', '.', '{', '}', '(', ')', '*', '/', '+', '-'};
	if (delimiter.find(c) != delimiter.end()) {
		return true;
	}
	return false;
}

bool RDGUtil::isOperator()
{
	return false;
}



/// <summary>
	/// 打印文法
	/// </summary>
void RDGrammar::printRDGrammar() {
	cout << "------------------------------------------\n文法的五元组如下：\n";
	cout << "非终结符集: {";
	for (set<string>::iterator it = nons.begin(); it != nons.end(); it++) {
		cout << *it << ",";
	}
	cout << "}\n";
	cout << "终结符集: {";
	for (set<string>::iterator it = terms.begin(); it != terms.end(); it++) {
		cout << *it << ",";
	}
	cout << "}\n";
	cout << "含有空产生式的非终结符集: {";
	for (set<string>::iterator it = null_pro.begin(); it != null_pro.end(); it++) {
		cout << *it << ",";
	}
	cout << "}\n";
	cout << "可输入的终结符串（单词）如下：\n{";
	for (set<string>::iterator it = str_terms.begin(); it != str_terms.end(); it++) {
		cout << *it << ",";
	}
	cout << "}\n";
	cout << "规则集: \n";
	for (map<string, vector< vector<SignType> > >::iterator it = regulars.begin(); it != regulars.end(); it++) {
		cout << it->first << " ::= ";
		int blankSize = it->first.size() + 5;
		string blank(blankSize, ' ');
		for (vector<vector<SignType> >::iterator it1 = it->second.begin(); it1 != it->second.end(); it1++) {
			for (vector<SignType>::iterator it2 = it1->begin(); it2 != it1->end(); it2++) {
				cout << it2->value;
				if ( str_terms.find(it2->value ) != str_terms.end() && !RDGUtil::isDelimiter(it2->value[0])) {
					cout << " ";
				}
			}
			vector<vector<SignType> >::iterator itb = it1;
			itb++;
			if (itb != it->second.end()) {
				cout << "|\n" << blank;
			}
		}
		if (null_pro.find(it->first) != null_pro.end()) {
			cout << "|\n" << blank << STRING_NULL << endl;
		}
		else {
			cout << endl;
		}
	}
	cout << "\n带符号类型的规则集打印: \n";
	for (map<string, vector< vector<SignType> > >::iterator it = regulars.begin(); it != regulars.end(); it++) {
		cout << it->first << " ::= ";
		int blankSize = it->first.size()+5;
		string blank(blankSize, ' ');
		for (vector<vector<SignType> >::iterator it1 = it->second.begin(); it1 != it->second.end(); it1++) {
			for (vector<SignType>::iterator it2 = it1->begin(); it2 != it1->end(); it2++) {
				cout << it2->value;
				if (it2->type == NON) {
					cout << "(non) ";
				}
				else {
					cout << "(term) ";
				}
			}
			vector<vector<SignType> >::iterator itb = it1;
			itb++;
			if (itb != it->second.end()) {
				cout << "|\n" << blank;
			}
		}
		if (null_pro.find(it->first) != null_pro.end()) {
			cout << "|\n" << blank << STRING_NULL << endl;
		}
		else {
			cout << endl;
		}
	}
	cout << "------------------------------------------\n";
}

RDGrammar RDGUtil::grammarToRDGrammar(Grammar& g){
	set<string> nons;// 非终结符集合
	set<string> terms;// 终结符集合
	set<string> str_terms;// 可输入的单词
	set<string> null_pro;// 含有空产生式的非终结符集合

	map<string, vector< vector<SignType> > > regulars;// 某个非终结符的规则集
	for (set<string>::iterator it = g.nons.begin(); it != g.nons.end(); it++) {
		nons.insert(switchString(*it));
	}
	for (set<char>::iterator it = g.terms.begin(); it != g.terms.end(); it++) {
		string t = "";
		t += *it;
		terms.insert(t);
	}
	for (vector<Production>::iterator it = g.productions.begin(); it != g.productions.end(); it++) {
		vector< vector<SignType> > vvst;
		// 将单引号转换为1
		string left = switchString(it->left);
		for (vector<string>::iterator it1 = it->right.begin(); it1 != it->right.end(); it1++) {
			vector<SignType> vst = getSignTypeFromProduction(*it1);
			for (vector<SignType>::iterator itt = vst.begin(); itt != vst.end(); itt++) {
				itt->value = switchString(itt->value);
			}
			vvst.push_back(vst);
			for (vector<SignType>::iterator it2 = vst.begin(); it2 != vst.end(); it2++) {
				if (it2->type == TERM) {
					str_terms.insert(it2->value);
				}
			}
		}
		
		regulars.insert({ left,vvst });
		if (it->have_null == 1) {
			null_pro.insert(left);
		}
	}

	return RDGrammar(g.start, nons, terms, str_terms, null_pro, regulars);
}

string RDGUtil::switchString(string src)
{
	int i = 0;
	while (i < src.size()) {
		if (src[i] == DYH) {
			src[i] = '1';
		}
		i++;
	}
	return src;
}
