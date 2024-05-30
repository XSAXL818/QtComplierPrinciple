#include "FirstUtil.h"


/// @brief 求解FIRST的工具类

vector<SymbolFirst>::iterator FirstUtil::findRecordFirstByLeft(string left, vector<SymbolFirst>& vsf) {
	vector<SymbolFirst>::iterator it = vsf.begin();
	while (it != vsf.end()) {
		if (it->left == left) {
			break;
		}
		it++;
	}
	return it;
}
/// @brief 求解非终结符的FIRST和产生式的FIRST
/// @param vp 文法的所有产生式
/// @param vrf 产生式蕴含信息表
/// @param vsf 非终结符FIRST
/// @param vpf 产生式FIRST
void FirstUtil::getFirst(vector<Production>& vp, vector<RecordFirst>& vrf, vector<SymbolFirst>& vsf, vector<ProductionFirst>& vpf) {
	//初始化
	map<string, int> sig;// 标记已求出的FIRST的非终结符
	generateRecordFirst(vp, vrf);// 生成产生式蕴含信息表
	// 循环迭代：第一步：找出可以直接求出的FIRST的非终结符；第二步：将第一步求出的带入到为求出的dependent的。
	int cnt = 0;
	initMap(vp, sig);
	cout << "FIRST 蕴含记录表\n";
	printRecordFirst(vrf);
	while (!vrf.empty()) {
		/*cout << "第" << cnt++ << "步\n";
		printRecordFirst(vrf);*/
		// 第一步,删除可直接求出FIRST的非终结符，并将入到vsf和vpf中
		pair< vector<SymbolFirst>, vector<ProductionFirst> > p = moveNoDependent(vrf);
		int start = vsf.size();// 标记已求出FIRST的非终结符，及其所在vsf中的位置
		for (int i = 0; i < p.first.size(); i++) {
			sig[p.first[i].left] = start + i;
		}
		vsf.insert(vsf.end(), p.first.begin(), p.first.end());
	/*	for (int i = 0; i < p.second.size(); i++) {
			int index = findProductionFirst(vpf, p.second[i].proID);
			if (index != -1) {
				vpf[index].firstSet.insert(p.second[i].firstSet.begin(), p.second[i].firstSet.end());
			}
			else {
				vpf.push_back(p.second[i]);
			}
		}*/
		vpf.insert(vpf.end(), p.second.begin(), p.second.end());
		// 第二步，将已求出FIRST的非终结符替换到vrf中
		replaceDependent(vrf, vsf, sig);
		// 第三步，如果存在循环蕴含，则它们的的
	}
	cout << "非终结符的FIRST为：\n";
	printSymbolFirst(vsf);
}
/// <summary>
/// 打印SymbolFirst表
/// </summary>
/// <param name="vsf"></param>
void FirstUtil::printSymbolFirst(vector<SymbolFirst>& vsf) {
	cout << "symbol\tFirstSet" << endl;
	for (int i = 0; i < vsf.size(); i++) {
		cout << vsf[i].left << "\t";
		for (set<string>::iterator it = vsf[i].firstSet.begin(); it != vsf[i].firstSet.end(); it++) {
			cout << *it << " ";
		}
		cout << endl;
	}
}
/// <summary>
/// 打印ProductionFirst表
/// </summary>
/// <param name="vpf"></param>
void FirstUtil::printGrammarFirst(vector<ProductionFirst>& vpf) {
	cout << "proID\tleft\tFirstSET" << endl;
	for (int i = 0; i < vpf.size(); i++) {
		cout << vpf[i].proID << "\t" << vpf[i].left << "\t";
		for (set<string>::iterator it = vpf[i].firstSet.begin(); it != vpf[i].firstSet.end(); it++) {
			cout << *it << " ";
		}
		cout << endl;
	}
}
/// @brief 将文法信息读取入RecordFirst表中
/// @param vp 文法的所有产生式
/// @param vr 产生式蕴含信息记录表
void FirstUtil::generateRecordFirst(vector<Production>& vp, vector<RecordFirst>& vrf) {
	int proID = 0;// 产生式ID,由vp顺序访问产生
	for (int i = 0; i < vp.size(); i++) {
		string left = vp[i].left;
		for (int j = 0; j < vp[i].right.size(); j++) {// 循环每个产生式
			RecordFirst rf(proID++);// 初始化一条记录
			rf.left = left;
			string right = vp[i].right[j];
			// 找到第一个非终结符或终结符
			string t = "";
			t += right[0];
			if (right[0] >= 'A' && right[0] <= 'Z') {// 获取非终结符  E' 
				if (right[1] == DYH) {
					t += right[1];
				}
				rf.dependent = t;
			}
			else {// 终结符加入FirstSet
				rf.firstSet.insert(t);
			}
			vrf.push_back(rf);
		}
		if (vp[i].have_null == 1) {// 由虚产生式，加入记录
			vrf.push_back(RecordFirst(proID++, left));
		}
	}
}

/// @brief 从开始pos位置开始，找到第一个可以直接求出FIRST的非终结符，返回所在区间
/// @param vrf 产生式蕴含信息表
/// @param pos 开始位置
/// @return 找到的下标区间[left,right],left==-1说明未找到
pair<int, int> FirstUtil::findNoDependent(vector<RecordFirst>& vrf, int pos) {
	int left = pos, right = pos;
	bool is = true;
	while (right < vrf.size()) {
		if (vrf[left].left == vrf[right].left) {
			if (vrf[right].dependent != "") {// 当前非终结符不可，跳过
				while (vrf[right].left == vrf[left].left && right < vrf.size()) {
					right++;
				}
				left = right;
			}
			else {
				right++;
			}
		}
		else {// 找到一个可以处理的
			break;
		}
	}
	return make_pair(left, right);

}
/// @brief 将vr中不依赖其他非终结符即可求出FIRST的删除
/// @param vr 产生式蕴含信息表
/// @return 返回vr中所有可求出的SymbolFirst和ProductionFirst
pair< vector<SymbolFirst>, vector<ProductionFirst> > FirstUtil::moveNoDependent(vector<RecordFirst>& vrf) {
	vector<SymbolFirst> vsf;
	vector<ProductionFirst> vpf;
	int left = 0, right = 0;// [ left, right ) 为同一个非终结符的所有产生式
	int pos = 0;
	pair<int, int> p = findNoDependent(vrf, pos);
	while (p.first < vrf.size()) {
		SymbolFirst sf;
		sf.left = vrf[p.first].left;
		for (int i = p.first; i < p.second; i++) {
			ProductionFirst pf;
			pf.left = sf.left;
			pf.proID = vrf[i].proID;
			pf.firstSet.insert(vrf[i].firstSet.begin(), vrf[i].firstSet.end());
			vpf.push_back(pf);
			sf.firstSet.insert(vrf[i].firstSet.begin(), vrf[i].firstSet.end());
		}
		while (p.second != p.first) {
			vrf.erase(vrf.begin() + p.first);
			p.second--;
		}
		vsf.push_back(sf);
		p = findNoDependent(vrf, p.first);
	}
	return 	make_pair(vsf, vpf);
}
/// @brief 将vr中出现在vs中的dependent去除，并替换相应的非终结符到vr中firstSet中
/// @param vrf 产生式蕴含信息表
/// @param vsf 非终结符的FIRST
/// @param sig 标记表，记录所有非终结符及出现在vs中的下标，未出现为-1
void FirstUtil::replaceDependent(vector<RecordFirst>& vrf, vector<SymbolFirst>& vsf, map<string, int>& sig) {
	for (int i = 0; i < vrf.size(); i++) {
		if (vrf[i].dependent != "") {// 找到可能被替换的
			int pos = sig[vrf[i].dependent];
			if (pos >= 0) {//可以替换
				vrf[i].dependent = "";
				vrf[i].firstSet.insert(vsf[pos].firstSet.begin(), vsf[pos].firstSet.end());
			}
		}
	}
}
void FirstUtil::initMap(vector<Production>& vp, map<string, int>& sig) {
	sig.clear();
	for (int i = 0; i < vp.size(); i++) {
		sig.insert(make_pair(vp[i].left, -1));
	}
}
void FirstUtil::printMap(vector<Production>& vp, map<string, int>& sig) {
	for (int i = 0; i < vp.size(); i++) {
		cout << vp[i].left << "\t" << sig[vp[i].left] << endl;
	}
}
void FirstUtil::printRecordFirst(vector<RecordFirst>& vrf) {
	cout << "proID\tleft\tdependent\tfirstSet" << endl;
	for (int i = 0; i < vrf.size(); i++) {
		cout << vrf[i].proID << "\t" << vrf[i].left << "\t" << vrf[i].dependent << "\t\t";
		for (set<string>::iterator it = vrf[i].firstSet.begin(); it != vrf[i].firstSet.end(); it++) {
			cout << *it << " ";
		}
		cout << endl;
	}
}
