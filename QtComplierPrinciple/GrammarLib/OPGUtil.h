#pragma once
#include<iostream>
#include<vector>
#include<set>
#include<map>
#include<string>
#include"Grammar.h"
#include"GUtil.h"
#include<cassert>
#include<string>

#include<stack>

#define RecordLastTerm RecordFirstTerm
#define LastTerm FirstTerm
#define _CRT_SECURE_NO_WARNINGS

using namespace std;
/// @brief 记录一个产生式的信息
class RecordFirstTerm {
public:
	int proID;// 产生式ID
	string left;// 产生式左部
	string dependent;// 被蕴含的非终结符
	set<string> firstSet;// 包含的终结符
	RecordFirstTerm();
	RecordFirstTerm(int _proID);
	/// @brief 创建一个虚产生式的记录
	/// @param _proID 
	/// @param emp 
	RecordFirstTerm(int _proID, string _left);
};
/// @brief 非终结符的FIRST值
class FirstTerm {
public:
	string left;
	set<string> firstSet;
	FirstTerm();
};
enum ERRORTYPE {
	NOTSENTENCE,
	NOTSTARTCHAR,
	NONEXISTENTRELATIONSHIP,
	ISSENTENCE,
};
class OPGUtil
{
public:
	static map<pair<char, char>, int> getFGTable(map<pair<char, char>, char>& table, Grammar& g);
	static vector<FirstTerm> getFirstTerm(Grammar& g);
	static vector<LastTerm> getLastTerm(Grammar& g);
	static map<pair<char, char>, char> getTable(Grammar& g, vector<FirstTerm>& first, vector<LastTerm>& last);
	static map<pair<char, char>, char> getTableNo(Grammar& g, vector<FirstTerm>& first, vector<LastTerm>& last);
	static void printFGTable(Grammar& g, map<pair<char, char>, int>& fgTable);
	static void printOPGTable(Grammar& g, map<pair<char, char>, char>& table);
	static bool isJuzi(string start, string juzi, map<pair<char, char>, char>& table);
	/// <summary>
	///采用算数优先矩阵、自底向上分析是否为句子
	/// </summary>
	/// <param name="start"></param>
	/// <param name="juzi"></param>
	/// <param name="table"></param>
	/// <param name="proToLeftTable"></param>
	/// <returns></returns>
	static ERRORTYPE isSentence(string start, string juzi, map<pair<char, char>, char>& table, map<string, char>& proToLeftTable);
	static ERRORTYPE isSentencePlus(Grammar& g, string start, string juzi, map<pair<char, char>, char>& table, map<string, char>& proToLeftTable);
	static map<string, char> getProToLeftTable(Grammar& g);
	static void warShall(map<pair<char, char>, char>& table, Grammar& g);
private:
	static void solveWarshall(vector<vector<bool>>& m, map<char, int>& termIndex);
	static void printMatrix(vector<vector<bool>>& m, set<char>& terms);
	static void initMatrix(map<pair<char, char>, char>& table, Grammar& g, vector<vector<bool>>& m, map<char, int>& termIndex);
	static void initForFGTable(Grammar& g, map<pair<char, char>, char>& table, map<pair<char, char>, int>& fgTable, map<pair<char, char>, char>& lessTable, map<pair<char, char>, char>& greaterTable, map<pair<char, char>, char>& equalTable);
	/// <summary>
	/// 处理关系
	/// </summary>
	/// <param name="fgTable"></param>
	/// <param name="relaTable"></param>
	/// <returns></returns>
	static bool getRelation(map<pair<char, char>, int>& fgTable, map<pair<char, char>, char>& relaTable);
	static int findLeft(vector<FirstTerm>& vft, string left);
	static void generateRecordLastTerm(vector<Production>& vp, vector<RecordFirstTerm>& vrft);
	static void printFirstTerm(vector<FirstTerm>& vft);
	static void replaceDependent(vector<RecordFirstTerm>& vrft, vector<FirstTerm>& vft, map<string, int>& sig);
	static vector<FirstTerm> moveNoDependent(vector<RecordFirstTerm>& vrft);
	static pair<int, int> findNoDependent(vector<RecordFirstTerm>& vrft, int pos);
	//getFirst();
	static void initMap(vector<Production>& vp, map<string, int>& sig);
	/// <summary>
	/// 生成蕴含记录表
	/// </summary>
	/// <param name="vp"></param>
	/// <param name="vrft"></param>
	static void generateRecordFirstTerm(vector<Production>& vp, vector<RecordFirstTerm>& vrft);
	static void printRecordFirstTerm(vector<RecordFirstTerm>& vrft);
	static string getLeftmostPrimePhrase(vector<char>& symbols, vector<char>& relation);
	static bool includeTerm(string pro);
	static pair<string, char> stringToPro(map<string, char>& table, string regular);
};

