#pragma once
#include<string>
#include<vector>
#include<queue>
#include<map>
#include<stack>
#include<iostream>
#include<algorithm>
#include "Grammar.h"
#include "FirstUtil.h"
#include "FollowUtil.h"

using namespace std;

class LL1Key {
public:
	string non;
	char input;

	LL1Key() {
		non = "";
		input = 0;
	}
	LL1Key(string _non, char _input) {
		non = _non;
		input = _input;
	}
	bool operator<(LL1Key key) const {
		if (non == key.non) {
			return input < key.input;
		}
		return non < key.non;
	}
};
class PUTIL {
public:
	// 打印产生式
	static void printGrammar(Grammar& grammar);
	// 覆盖式写入文件中 
	static void fwriteGrammar(vector<Production>& vp,string filePath);
	// 从文件中读取产生式 
	static void readGrammar(vector< Grammar >& grammars, string fileName);
	// 加标记法：
	static void labelMethod(vector<Production>& vp);
	// 打印含有空产生式的非终结符Non
	static void printNonHaveNull(vector<Production>& vp);
	/// @brief 建立非终结符的预测表，纵坐标为terms中的顺序，横坐标为vp中的left次序
	/// @param vp 
	/// @param non 
	static map<LL1Key, string> getLL1Table(vector<ProductionFirst>& vpf,vector<Production>& vp, vector<string>& terms);
	static map<LL1Key, string> getLL1TablePlus(GrammarPlus& gp);
	/// <summary>
	/// 判断是否为非终结符
	/// </summary>
	/// <param name="str"></param>
	/// <returns></returns>
	static bool isNon(string str);
	static bool isNon(char c);
	/// <summary>
	/// 自顶向下分析
	/// </summary>
	/// <param name="src"></param>
	/// <param name="LL1Table"></param>
	/// <param name="vp"></param>
	/// <param name="terms"></param>
	static void topToBottom(string src, map<LL1Key, string>& LL1, vector<Production>& vp, vector<string>& terms);
	static void topToBottomNoProcess(string src, map<LL1Key, string>& LL1, vector<Production>& vp, vector<string>& terms);

	/// <summary>
	/// 将产生式转换为字符数组
	/// </summary>
	/// <param name="pro"></param>
	/// <returns></returns>
	
	/// <summary>
	/// 将产生式字符数组转换为产生式字串
	/// </summary>
	/// <param name="pro"></param>
	/// <returns></returns>
	static string getProFromRights(vector<string> vs);
	/// <summary>
	/// 将产生式字串转换为产生式字符数组
	/// </summary>
	/// <param name="pro"></param>
	/// <returns></returns>
	static vector<string> getRightsFromPro(string pro);
	/// <summary>
	/// 打印LL1Table表
	/// </summary>
	/// <param name="LL1Table"></param>
	/// <param name="vp"></param>
	/// <param name="terms"></param>
	static void printLL1Table(map<LL1Key, string>& LL1, vector<Production>& vp, vector<string>& terms );
	static vector<string> getTerms(vector<Production>& vp);
	//消除文法的左递归
	static void EliminateLeftRecursion(vector<Production>& vp);
	static void fixNumber(vector<Production>& vp);
private:
	/// <summary>
	/// 将文法的二元组表示转换成一元表示（按产生式出现顺序排序）：<left,productions> => <productions>
	/// </summary>
	/// <param name="vp"></param>
	/// <returns></returns>
	static vector<string> initProductions(vector<Production>& vp);
	/// <summary>
	/// 将文法中的非终结符src1,src2互换
	/// </summary>
	/// <param name="vp"></param>
	/// <param name="src1"></param>
	/// <param name="src2"></param>
	static void switchNon(vector<Production>& vp,string src1,string src2);
	/// <summary>
	/// 初始化LL1Table预测分析表
	/// </summary>
	/// <param name="LL1Tabl"></param>
	/// <param name="vp"></param>
	/// <param name="productions"></param>
	/// <param name="terms"></param>
	/// <param name="vsf"></param>
	/// <param name="vpf"></param>
	/// <param name="vsfo"></param>
	static map<LL1Key, string> initLL1Table( vector<Production>& vp, vector<string>& productions, vector<string>& terms, vector<SymbolFirst>& vsf, vector<ProductionFirst>& vpf, vector<SymbolFollow>& vsfo);
	static void initLL1TablePlus(vector< vector<string> >& LL1Tabl, vector<Production>& vp, vector<string>& productions, vector<string>& terms, vector<SymbolFirst>& vsf, vector<ProductionFirst>& vpf, vector<SymbolFollow>& vsfo);

	/// <summary>
	/// 给定非终结符，给出其在LL1Table表中的位置
	/// </summary>
	/// <param name="left"></param>
	/// <param name="vp"></param>
	/// <returns></returns>
	static int findLeftIndexInLL1(string left, vector< Production >& vp);
	static int findSymbolFirstByLeft(vector<SymbolFirst>& vsf, string left);
	static int findSymbolFollowByLeft(vector<SymbolFollow>& vsfo, string left);
	static int findProID(string left, string term, vector<ProductionFirst>& vpf);
	// findProduction:按照left查找 
	static vector<Production>::iterator findProductionByLeft(vector<Production>& vp, Production& p);
	static vector<Production>::iterator findProductionByLeft(vector<Production>& vp, string left);
	// 去除某个非终结符的 重复定义：E = E|EE 
	static void eraseRepeatLeft(Production& p);
	// 利用已有的left去替换right中的left 
	static bool swapLeftToRright(string& right, string& left, string& leftProduction);
	//  将右部出现的非终结符加标记 ，并加入队列 
	static void addLabelForRight(Production& p, queue<string>& q, map<string, bool>& vis);
	//	找出vp中所有右部可以直接推出全为非终结符的left
	//	找出 left -> term{term} 的所有left 	
	static bool findLeftToTerm(vector<Production>& vp, queue<string>& q, map<string, bool>& rvis);
	//	返回值：当有新的标记加入返回true即rvis[left][new]由0赋值为1出现 
	//  left的右部可推导出全为终结符，对于table[left][non]=1的，标记rvis[non]=true 	
	static bool addLabelForLeft(vector<Production>& vp, string termRight, queue<string>& q, map<string, bool>& rvis, vector< vector<int> >& table);
	// 找出所有未被标记的left并返回 	
	static vector<string> findLeftNotInVis(vector<Production>& vp, map<string, bool>& vis);
	// 删除未被标记的产生式 
	static void eraseLeftNotInVis(vector<Production>& vp, map<string, bool>& vis);
	// 初始化map标记 
	static void initMap(vector<Production>& vp, map<string, bool>& vis);
	// 删除空产生式 
	static void eraserNullProduction(vector<Production>& vp);
	// 删除右部出现不存在的非终结符的产生式(E=E+P，不存在P删除E),删除了返回true 
	// 有删除的返回true 
	static bool eraseNotExsitNon(vector<Production>& vp);
	static void initTable(vector<Production>& vp, vector< vector<int> >& table);
	static void print(vector<Production>& vp, map<string, bool>& vis);
	static void printTable(vector<Production>& vp, vector< vector<int> >& table);
	static void printQueue(queue<string> q);
	static void printStack(stack<string> s);
	//预处理，将开始符号与非终结符“Z”（存在）交换，不存在"Z"则把开始符设置为Z，按非终结符的升序排序
	static void preprocessing(vector<Production>& vp,map<string,int>& m);
	// 消去间接左递归,确保所有非终结符有:Non(i) ::= Non(i)Term | Non(i+1)Term | Non(i+2)Term | Non(i+3)Term .....
	static void EliminateIndirectLeftRecursion(vector<Production>& vp,int pos, map<string, int>& m);
	// 消去直接左递归,将Non(i) ::= Non(i)Term处理掉
	static void EliminateDirectLeftRecursion(vector<Production>& vp,int pos);
	/// <summary>
	/// 用于处理直接左递归的排序，处理后：产生式前几个是当前非终结符开头的.
	/// </summary>
	/// <param name="vp"></param>
	/// <param name="pos"></param>
	static vector<string>::iterator sortForEliminateDirectLeftRecursion(vector<Production>& vp, int pos);
	/// <summary>
	/// 删除无法从开始符推导出的非终结符
	/// </summary>
	static void eraseNonNotDerivation(vector<Production>& vp);
};

