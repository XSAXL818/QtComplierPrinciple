#pragma once
#define _CRT_SECURE_NO_WARNINGS
#include<string>
#include<iostream>
#include<map>
#include<vector>
#include<stdio.h>
#include<stack>
#include<set>
#include<queue>
#include<algorithm>
#include"Grammar.h"

using namespace std;
// NFA的状态，可用于NFA转换为DFA
typedef set<string> NFASTATUS;


/// <summary>
/// 用于表示结点及其某条出边上的字符
/// </summary>
class DFAKey {
public:
	string ver;
	char edge;
	DFAKey(string _ver, char _edge) {
		ver = _ver;
		edge = _edge;
	}
	DFAKey() {
		ver = "";
		edge = 0;
	}
	bool operator<(DFAKey in) const;
};
class NFAtoDFAKey {
public:
	NFASTATUS NFAStatus;
	char input;

	bool operator<(NFAtoDFAKey key) const {
		if (input != key.input) {
			return input < key.input;
		}
		return NFAStatus < key.NFAStatus;
	}
	NFAtoDFAKey(NFASTATUS status, char in) {
		NFAStatus = status;
		input = in;
	}
	NFAtoDFAKey() {};
};


class RecordClosure{
public:
	string symbol;// 符号
	set<string> dependent;// 蕴含的符号
	set<string> closure;// 闭包

	RecordClosure(string _symbol, set<string> _dependent, set<string> _closure);
	bool operator<(RecordClosure rc) const;

};

class NFA {
public:
	set<string> statusSet;//状态集合
	set<char> inputLetter;//输入字符集合
	map< DFAKey, set<string> > mapTable;//映射集合
	set<string> start;//开始状态集合
	set<string> finalSet;//终止状态集合
	// 状态的@_closure,即状态接受空字符能到达的所有状态
	map< string, set<string> > nullClosure;
	// 状态的letter_closure,即状态接受letter字符后能到达的所有状态
	map< DFAKey, set<string> > letterClosure;
	NFA(set<string> _statusSet, set<char> _inputLetter, map< DFAKey, set<string> > _mapTable, set<string> _start, set<string> _finalSet);
	NFA(){}
	/// <summary>
	/// 获取状态的@_closure
	/// </summary>
	void initClosure();
	void printNFA();
	void fwriteNFA(string filePath);
private:
	/// <summary>
	/// 初始化记录表
	/// </summary>
	/// <param name="vrc"></param>
	void initRecordClosure(vector<RecordClosure>& vrc);
	/// <summary>
	/// 初始化空闭包
	/// </summary>
	/// <param name="vrc"></param>
	void initNullClosure(vector<RecordClosure>& vrc);
	/// <summary>
	/// 删除蕴含表中可以直接求出闭包的项，并加入到nullClousure中
	/// </summary>
	/// <param name="vrc"></param>
	void eraseNullDependent(vector<RecordClosure>& vrc);
	/// <summary>
	/// 将已求出@_closure的字符带入带蕴含表的dependent中
	/// </summary>
	/// <param name="vrc"></param>
	void replaceDependent(vector<RecordClosure>& vrc);
	/// <summary>
	/// 打印记录表
	/// </summary>
	/// <param name="vrc"></param>
	void printRecordClosure(vector<RecordClosure>& vrc);
	/// <summary>
	/// 打印@_closure
	/// </summary>
	void printNullClosure();
	/// <summary>
	/// 初始化x_closure
	/// </summary>
	void initLetterClosure();
	void printLetterClosure();
	void eraseNullClosure();
};

class DFA {
public:
	set<string> statusSet;//状态集合
	set<char> inputLetter;//输入字符集合
	map<DFAKey, string> mapTable;//映射集合
	string start;//开始状态
	set<string> finalSet;//终止状态集合
	DFA(){}
	DFA(set<string> _statusSet, set<char> _inputLetter, map<DFAKey, string> _mapTable, string _start, set<string> _finalSet);
	void printDFA();
	void printMatDFA();
	void simplification();
	void fwriteDFA( string filePath );
private:
	vector<set<string> > divideTwoGroup(set<string> group, set< set<string> >& allGroup);
	set<string> getDFAStatusSetLetterClosure(set<string>& statusSet, char letter);

};

/// <summary>
/// NFA转换为DFA的中间表示的五元组
/// </summary>
class NFAtoDFA {
public:
	set< NFASTATUS > statusSet;//状态集合
	set<char> inputLetter;//输入字符集合
	map<NFAtoDFAKey, NFASTATUS> mapTable;//映射集合
	NFASTATUS start;//开始状态集合
	set< NFASTATUS > finalSet;//终止状态集合
	map<NFASTATUS, string> statusID;//对状态集合的重新编号
	vector< NFASTATUS > stateID;//用于状态集合的编号
	void printNFAtoDFA();
	DFA toDFA();
private:
	void printNFAStatus(const NFASTATUS& sta);
	void printFinalSet();
	void renumber();
	void printMapTable();
	void printStart();
	void printStatusSet();
};

class FAUtil
{
public:
	static void readDFA( vector<DFA>& dfas, string filePatg);
	static void readNFA( vector<NFA>& nfas, string filePath );
	static void printDFAs(vector<DFA>& dfas);
	static void printNFAs(vector<NFA>& nfas);
	static void DFADerivedSentence(DFA& dfa,string str);
	static void printMatDFA( DFA& dfa );
	static void printMatNFA(NFA& nfa);
	static DFA NFAToDFA( NFA& nfa );
	static void printGroupStack(stack< set<string> > st);
	static bool isInclude( set<string> s1, set<string> s2 );
private:
	static bool allNum(string str);
	static int stringToNum(string str);
	static NFAtoDFA equivalentReplacement(NFA& nfa);
	
};