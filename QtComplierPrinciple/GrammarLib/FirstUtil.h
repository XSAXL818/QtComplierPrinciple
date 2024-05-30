#pragma once
#include<string>
#include<map>
#include<vector>
#include<iostream>
#include<algorithm>
#include"Grammar.h"
#include"FIRST.h"

using namespace std;

/// @brief 求解FIRST的工具类
class FirstUtil {
public:
	static vector<SymbolFirst>::iterator findRecordFirstByLeft(string left, vector<SymbolFirst>& vsf);
	/// @brief 求解非终结符的FIRST和产生式的FIRST
	/// @param vp 文法的所有产生式
	/// @param vrf 产生式蕴含信息表
	/// @param vsf 非终结符FIRST
	/// @param vpf 产生式FIRST
	static void getFirst(vector<Production>& vp, vector<RecordFirst>& vrf, vector<SymbolFirst>& vsf, vector<ProductionFirst>& vpf);
	static void printSymbolFirst(vector<SymbolFirst>& vsf);
	static void printGrammarFirst(vector<ProductionFirst>& vpf);
private:
	/// @brief 将文法信息读取如RecordFirst表中
	/// @param vp 文法的所有产生式
	/// @param vr 产生式蕴含信息记录表
	static void generateRecordFirst(vector<Production>& vp, vector<RecordFirst>& vrf);
	/// @brief 从开始pos位置开始，找到第一个可以直接求出FIRST的非终结符，返回所在区间
	/// @param vrf 产生式蕴含信息表
	/// @param pos 开始位置
	/// @return 找到的下标区间[left,right],left==-1说明未找到
	static pair<int, int> findNoDependent(vector<RecordFirst>& vrf, int pos);
	/// @brief 将vr中不依赖其他非终结符即可求出FIRST的删除
	/// @param vr 产生式蕴含信息表
	/// @return 返回vr中所有可求出的SymbolFirst和ProductionFirst
	static pair< vector<SymbolFirst>, vector<ProductionFirst> > moveNoDependent(vector<RecordFirst>& vrf);
	/// @brief 将vr中出现在vs中的dependent去除，并替换相应的非终结符到vr中firstSet中
	/// @param vrf 产生式蕴含信息表
	/// @param vsf 非终结符的FIRST
	/// @param sig 标记表，记录所有非终结符及出现在vs中的下标，未出现为-1
	static void replaceDependent(vector<RecordFirst>& vrf, vector<SymbolFirst>& vsf, map<string, int>& sig);
	static void initMap(vector<Production>& vp, map<string, int>& sig);
	static void printMap(vector<Production>& vp, map<string, int>& sig);
	static void printRecordFirst(vector<RecordFirst>& vrf);
	static int findProductionFirst(vector<ProductionFirst>& vpf,int proID) {
		for (int i = 0; i < vpf.size(); i++) {
			if (vpf[i].proID == proID) {
				return i;
			}
		}
		return -1;

	}
};