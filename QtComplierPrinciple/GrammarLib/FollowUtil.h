#pragma once
#include<iostream>
#include<vector>
#include<set>
#include<map>
#include<algorithm>
#include<stack>
#include<string>
#include"Grammar.h"
#include"FOLLOW.h"
#include"FIRST.h"
#include"FirstUtil.h"

using namespace std;

/// @brief 求解FOLLOW的工具类
class FollowUtil {
public:
	/// @brief 求解非终结符的 FOLLOW
	/// @param vp 文法的产生式
	/// @param vsf 非终结符的FIRST
	/// @param vpf 某个产生式的FIRST
	/// @return 
	static vector<SymbolFollow> getFollow(vector<Production>& vp, vector<SymbolFirst>& vsf, vector<ProductionFirst>& vpf);
	static void printSymbolFollow(vector<SymbolFollow>& vsfo);
private:
	static void replaceDependent(vector<RecordFollow>& vrfo, vector<SymbolFollow>& vsfo, map<string, int>& sig);
	static void initMap(vector<Production>& vp, map<string, int>& sig);
	/// @brief 从开始pos位置开始，找到第一个可以直接求出FIRST的非终结符，返回所在区间
		/// @param vrf 产生式蕴含信息表
		/// @param pos 开始位置
		/// @return 找到的下标区间[left,right],left==-1说明未找到
	static pair<int, int> findNoDependent(vector<RecordFollow>& vrfo, int pos);
	/// @brief 将vr中不依赖其他非终结符即可求出FIRST的删除
	/// @param vr 产生式蕴含信息表
	/// @return 返回vr中所有可求出的SymbolFirst和ProductionFirst
	static vector<SymbolFollow> moveNoDependent(vector<RecordFollow>& vrfo);
	/// @brief 通过vsf和vp求解所有RecordFollow信息表
	/// @param vp 文法的所有产生式
	/// @param vsf 非终结符的FIRST
	/// @return 返回RecordFollow表
	static vector<RecordFollow> getRecordFollow(vector<Production>& vp, vector<SymbolFirst>& vsf);
	/// @brief 删除重复出现 或者 自蕴含的
	/// @param vrfo 
	static void eraseRepeatFollow(vector<RecordFollow>& vrfo);
	/// @brief 求出一个产生式 left=pro 的RecordFollow蕴含信息
	/// @param vrfo 
	/// @param vsf 
	/// @param left 
	/// @param pro 
	static void getFollowFromProduction(vector<RecordFollow>& vrfo, vector<SymbolFirst>& vsf, string left, string pro);
	/// @brief 判断str是否为非终结符
	/// @param str 
	/// @return 非终结符返回true
	static bool isNon(string str);
	/// @brief 将产生式字串分解为字符(非终结符或终结符)数组
	/// @param pro 
	/// @return 
	static vector<string> getRightsFromPro(string pro);
	static void printRecordFollow(vector<RecordFollow>& vrfo);
	static void printGraph();

	static map< string, int > visisted;
	static map<string, string> parent;
	static vector<string> circle;
	static set<string> vertex;
	static map<string, set<string> > g;

	/// <summary>
	/// 递归思路：现将当前结点置为正在访问，遍历可到达的下一个结点，
	/// 判断该节点是否正在被访问，正在被访问说明出现环。
	/// 遍历所有节点后未发现环，则将该结点置为已访问。
	/// 程序结束后，能遍历到的路径都会置为2，所以外层调用时
	/// 不必对所有结点，进行查询环的操作。
	/// </summary>
	/// <param name="ver"></param>
	/// <param name="g"></param>
	/// <returns></returns>
	static bool DFS(string ver, map<string, set<string> >& g) {
		// 设置为正在访问
		visisted[ver] = 1;
		// 遍历该节点可以到达的结点
		for (string next : g[ver]) {
			if (visisted[next] == 0) {// 该节点未遍历，则继续深度优先搜索
				// 该路径下next的父节点是ver
				parent[next] = ver;
				// 如果该结点的遍历找到了环
				if (DFS(next, g)) {
					visisted[ver] = 2;
					return true;
				}
				// 为找到环，则进行下一轮循环，看其他结点是否可以找到环
			}
			else if (visisted[next] == 1) {// 该节点正在被访问，找到一个环
				//找到一个next [-> ... ->] next 的环
				string cur = ver;
				circle.push_back(next);
				// 将 上面[]之间的结点加入
				while (cur != next) {
					circle.push_back(cur);
					cur = parent[cur];
				}
				//加入起始结点
				circle.push_back(next);
				reverse(circle.begin(), circle.end());
				visisted[ver] = 2;
				return true;
			}
		}
		// 设置为已访问
		visisted[ver] = 2;
		return false;
	};
	static void initDFS(vector<RecordFollow>& vrfo) {
		// 清空脏数据
		visisted.clear();
		vertex.clear();
		parent.clear();
		circle.clear();
		g.clear();
		//初始化图和结点
		for (RecordFollow rf : vrfo) {
			if (rf.dependent != "") {// 存在蕴含关系
				g[rf.left].insert(rf.dependent);
				vertex.insert(rf.left);
				vertex.insert(rf.dependent);
			}
		}
		// 初始化访问表
		for (string s : vertex) {
			visisted.insert({ s,0 });
		}
	}
	/// <summary>
	/// 返回找到的所有环
	/// </summary>
	/// <returns></returns>
	static vector<vector<string> > getCircles(vector<RecordFollow>& vrfo);
	static void printCircles(vector<vector<string> >& circles) {
		for (int i = 0; i < circles.size(); i++) {
			cout << i << "\t";
			for (string s : circles[i]) {
				cout << s << " ";
			}
			cout << endl;
		}
	}
	/// <summary>
	/// 将换转换到集合中存储
	/// </summary>
	/// <param name="circles"></param>
	/// <returns></returns>
	static vector<set<string> > circlesVecToSet( vector<vector<string> >& circles ) {
		vector<set<string> > newCircles;
		for (int i = 0; i < circles.size(); i++) {
			set<string> cir;
			for (string s : circles[i]) {
				cir.insert(s);
			}
			newCircles.push_back(cir);
		}
		return newCircles;
	}
	static vector<int> findSelfDependent(vector<RecordFollow>& vrfo, vector<set<string>>& circlesSet) {
		vector<int> vi;
		for (int i = 0; i < circlesSet.size(); i++ ) {
			bool isSelf = false;// 默认为false，即下面循环未走，则说明无该符号，
			for (int j = 0; j < vrfo.size(); j++) {
				// 依赖符不为空，且是环内符
				if (vrfo[j].dependent != "" && circlesSet[i].find(vrfo[j].left) != circlesSet[i].end()) {
					// 环内符的依赖符不在环内
					if (circlesSet[i].find(vrfo[j].dependent) == circlesSet[i].end()) {
						isSelf = false;
						break;
					}
					else {// 
						isSelf = true;
					}
				}
			}
			if (isSelf) {
				vi.push_back(i);
			}
		}
		return vi;
	}
	static set<string> getCircleDependentAndEraseCircle(set<string >& circleSet, vector<SymbolFollow>& vsfo,vector<RecordFollow>& vrfo) {
		set<string> followSet;
		for (int i = (int)vrfo.size() - 1; i >= 0; i--) {
			// 找到环内符
			if (circleSet.find(vrfo[i].left) != circleSet.end()) {
				if (vrfo[i].dependent == "") {// 有followSet
					followSet.insert(vrfo[i].followSet.begin(), vrfo[i].followSet.end());
				}
				vrfo.erase(i + vrfo.begin());
			}
		}
		return followSet;
	}
	static void addCircleToSymbolFollowTable(set<string>& circleSet, vector<SymbolFollow>& vsfo, set<string>& followSet) {
		for (string item : circleSet) {
			SymbolFollow sfo(item, followSet);
			vsfo.push_back(sfo);
		}
	}
	static void eraseCircle(vector<RecordFollow>& vrfo,vector<set<string>>& circlesSet,vector<SymbolFollow>& vsfo) {
		// 查找环内符只依赖环内符的环
		vector<int> selfDependent = findSelfDependent(vrfo, circlesSet);
		cout << "环内符只依赖环内符的有: ";
		for (int i = 0; i < selfDependent.size(); i++) {
			for (string item : circlesSet[selfDependent[i]]) {
				cout << item << " ";
			}
		}
		cout << endl;
		for (int index : selfDependent) {
			// 获取该环的followSet,并将vrfo中的所有的left是环内符的删除
			set<string> newFollowSet = getCircleDependentAndEraseCircle(circlesSet[index], vsfo, vrfo);
			cout << "待删除的环 : ";
			for (string item : circlesSet[index]) {
				cout << item << " ";
			}
			cout << endl;
			cout << "获得该环followSet: ";
			for (string item : newFollowSet) {
				cout << item << " ";
			}
			cout << endl;
			// 将环内符与followSet加入到SymbolFollow表中
			addCircleToSymbolFollowTable(circlesSet[index], vsfo, newFollowSet);
			// 将该符号表中，蕴含该符号的followSet更改
			for (int i = 0; i < vrfo.size(); i++) {
				if (circlesSet[index].find(vrfo[i].dependent) != circlesSet[index].end()) {
					vrfo[i].dependent = "";
					vrfo[i] .followSet = newFollowSet;
				}
			}
		}
	}
	static int findRecordFollowBySymbolAndDependent( vector<RecordFollow>& vrfo, string symbol, string dependent ) {
		for (int i = 0; i < vrfo.size(); i++) {
			if (vrfo[i].left == symbol && vrfo[i].dependent == dependent) {
				return i;
			}
		}
		return -1;
	}
};
