#pragma once
#include<string>
#include<vector>
#include<set>
#include<algorithm>
using namespace std;

/// @brief 记录一个产生式的信息
class RecordFirst {
public:
	int proID;// 产生式ID
	string left;// 产生式左部
	string dependent;// 被蕴含的非终结符
	set<string> firstSet;// 包含的终结符
	RecordFirst();
	RecordFirst(int _proID);
	/// @brief 创建一个虚产生式的记录
	/// @param _proID 
	/// @param emp 
	RecordFirst(int _proID, string _left);
};

/// @brief 非终结符的FIRST值
class SymbolFirst {
public:
	string left;
	set<string> firstSet;
	SymbolFirst();
};


/// @brief 一个产生式的FIRST
class ProductionFirst {
public:
	int proID;
	string left;
	set<string> firstSet;
	ProductionFirst();
};