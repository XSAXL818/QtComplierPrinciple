#include "FollowUtil.h"


map<string, int> FollowUtil::visisted;
vector<string> FollowUtil::circle;
map<string, string> FollowUtil::parent;
set<string> FollowUtil::vertex;
map<string, set<string> > FollowUtil::g;

void FollowUtil::printGraph() {
	for (pair<string, set<string> > p : g) {
		cout << p.first << " : ";
		for (string s : p.second) {
			cout << s << ",";
		}
		cout << endl;
	}
}


/// @brief 求解FOLLOW的工具类
/// @brief 求解非终结符的 FOLLOW
/// @param vp 文法的产生式
/// @param vsf 非终结符的FIRST
/// @param vpf 某个产生式的FIRST
/// @return 
vector<SymbolFollow> FollowUtil::getFollow(vector<Production>& vp, vector<SymbolFirst>& vsf, vector<ProductionFirst>& vpf) {
	// 获取RecordFollow表
	vector<RecordFollow> vrfo = getRecordFollow(vp, vsf);
	vector<SymbolFollow> vsfo;
	map<string, int> sig;// 标记已求出的FIRST的非终结符
	initMap(vp, sig);
	// // 获取可以直接推出FOLLOW的非终结符,并在vrfo中删除
	// 循环迭代：第一步：找出可以直接求出的FIRST的非终结符；第二步：将第一步求出的带入到为求出的dependent的。
	cout << "初始化的FOLLOW蕴含表如下" << endl;
	printRecordFollow(vrfo);
	int cnt = 0;
	// 第0步，找到蕴含关系中的环,环表示它们的FOLLOW相同，可以先去除，然后最后结果合并
	vector<vector<string> > circles = getCircles(vrfo);
	vector<set<string> > circlesSet = circlesVecToSet(circles);
	sort(vrfo.begin(), vrfo.end());// 排序，便于查找同一个left的区间
	cout << "蕴含表中出现的所有的环: " << endl;
	printCircles(circles);
	// 删除重复的表项
	eraseRepeatFollow(vrfo);
	while (!vrfo.empty()) {
		// 第一步,删除可直接求出Follow的非终结符，并将入到vsfo
		vector<SymbolFollow> ret = moveNoDependent(vrfo);
		int start = (int)vsfo.size();// 标记已求出FOLLOW的非终结符，及其所在vsfo中的位置
		for (int i = 0; i < ret.size(); i++) {
			sig[ret[i].left] = start + i;
		}
		vsfo.insert(vsfo.end(), ret.begin(), ret.end());
		// 第二步，将已求出FIRST的非终结符替换到vrf中
		replaceDependent(vrfo, vsfo, sig);

		// 第三步，判断是否有依赖环,有则求出该环，同时该环不需要依赖环外元素即可求出
		if (ret.empty()) {
			eraseCircle(vrfo,circlesSet,vsfo);
		}
		cout << "第" << cnt++ << "步求解Follow" << endl;
		cout << "蕴含表\n"; 
		printRecordFollow(vrfo);
		cout << "符号FOLLOW表\n";
		printSymbolFollow(vsfo);
	}
	return vsfo;
}
vector<vector<string> > FollowUtil::getCircles(vector<RecordFollow>& vrfo) {
	initDFS(vrfo);
	vector<vector<string> > vvs;
	for (string v : vertex) {
		if (visisted[v] == 0) {//未访问，则从该结点查询是否有路径是环
			if (DFS(v, g)) {//存在环
				//vector<vector<string>> vcir{circle};/*
				//set<string> circleSet = *circlesVecToSet(vcir).begin();*/
				// 处理多条路的环
				/*bool is = false;
				for (int i = 0; i < circle.size() - 3; i++) {
					cout << 1 << endl;
					string head = circle[i];
					string medium = circle[i + 1];
					string tail = circle[i + 2];
					for (int j = 0; j < vrfo.size(); j++) {
						cout << 2 << endl;
						if (circleSet.find(vrfo[j].left)==circleSet.end()) {
							int index1 = findRecordFollowBySymbolAndDependent(vrfo, head, vrfo[j].left);
							int index2 = findRecordFollowBySymbolAndDependent(vrfo, vrfo[j].left, tail);
							circle.insert(circle.begin() + i, vrfo[j].left);
							is = true;
							break;
						}
					}
					if (is) {
						break;
					}
				}*/

				vvs.push_back(circle);
				circle.clear();
			}
		}
	}
	return vvs;
}
/// <summary>
/// 打印SymbolFollow表
/// </summary>
/// <param name="vsfo"></param>
void FollowUtil::printSymbolFollow(vector<SymbolFollow>& vsfo) {
	cout << "left\tfollowSet" << endl;
	for (int i = 0; i < vsfo.size(); i++) {
		cout << vsfo[i].left << "\t";
		for (set<string>::iterator it = vsfo[i].followSet.begin(); it != vsfo[i].followSet.end(); it++) {
			cout << *it << " ";
		}
		cout << endl;
	}
}
/// <summary>
/// 将已经求出Follow的非终结符，代替到未求出的非终结符的Dependent中
/// </summary>
/// <param name="vrfo">非终结符的蕴含信息表</param>
/// <param name="vsfo">已求出Follow的表</param>
/// <param name="sig">标记表，sig[非终结符]>=0，则该非终结符已求出且在SymbolFollow的下标为sig[Non]</param>
void FollowUtil::replaceDependent(vector<RecordFollow>& vrfo, vector<SymbolFollow>& vsfo, map<string, int>& sig) {
	for (int i = 0; i < vrfo.size(); i++) {
		if (vrfo[i].dependent != "") {// 找到可能被替换的
			int pos = sig[vrfo[i].dependent];
			if (pos >= 0) {//可以替换
				vrfo[i].dependent = "";
				vrfo[i].followSet.insert(vsfo[pos].followSet.begin(), vsfo[pos].followSet.end());
			}
		}
	}
}
/// <summary>
/// 初始化标记表
/// </summary>
/// <param name="vp"></param>
/// <param name="sig"></param>
void FollowUtil::initMap(vector<Production>& vp, map<string, int>& sig) {
	sig.clear();
	for (int i = 0; i < vp.size(); i++) {
		sig.insert(make_pair(vp[i].left, -1));
	}
}
/// @brief 从开始pos位置开始，找到第一个可以直接求出FIRST的非终结符，返回所在区间
	/// @param vrf 产生式蕴含信息表
	/// @param pos 开始位置
	/// @return 找到的下标区间[left,right],left==-1说明未找到
pair<int, int> FollowUtil::findNoDependent(vector<RecordFollow>& vrfo, int pos) {
	int left = pos, right = pos;
	bool is = true;
	while (right < vrfo.size()) {
		if (vrfo[left].left == vrfo[right].left) {
			if (vrfo[right].dependent != "") {// 当前非终结符不可，跳过
				while (right < vrfo.size()) {
					if (vrfo[right].left != vrfo[left].left) {
						break;
					}
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
vector<SymbolFollow> FollowUtil::moveNoDependent(vector<RecordFollow>& vrfo) {
	vector<SymbolFollow> vsfo;
	int left = 0, right = 0;// [ left, right ) 为同一个非终结符的所有产生式
	bool is = true;// 用于标记当前非终结符是否可以直接求出
	int pos = 0;
	pair<int, int> p = findNoDependent(vrfo, pos);
	while (p.first < vrfo.size()) {
		SymbolFollow sfo;
		sfo.left = vrfo[p.first].left;
		for (int i = p.first; i < p.second; i++) {
			sfo.followSet.insert(vrfo[i].followSet.begin(), vrfo[i].followSet.end());
		}
		while (p.second != p.first) {
			vrfo.erase(vrfo.begin() + p.first);
			p.second--;
		}
		vsfo.push_back(sfo);
		p = findNoDependent(vrfo, p.first);
	}
	return 	vsfo;
}
/// @brief 通过vsf和vp求解所有RecordFollow信息表
/// @param vp 文法的所有产生式
/// @param vsf 非终结符的FIRST
/// @return 返回RecordFollow表
vector<RecordFollow> FollowUtil::getRecordFollow(vector<Production>& vp, vector<SymbolFirst>& vsf) {
	//FirstUtil::printSymbolFirst(vsf);
	vector<RecordFollow> vrfo;
	for (int i = 0; i < vp.size(); i++) {
		for (vector<string>::iterator sit = vp[i].right.begin(); sit != vp[i].right.end(); sit++) {// 遍历所有产生式
			getFollowFromProduction(vrfo, vsf, vp[i].left, *sit);
		}
	}
	// 为后续工作进行预处理：删除表中重复和自蕴含的无意义表项
	eraseRepeatFollow(vrfo);
	RecordFollow rf(vp[0].left);// 为开始加入字符串结束EOF标志
	string end = "";
	end += EOS;
	rf.followSet.insert(end);
	vrfo.push_back(rf);
	return vrfo;
}
/// @brief 删除重复出现 或者 自蕴含的
/// @param vrfo 
void FollowUtil::eraseRepeatFollow(vector<RecordFollow>& vrfo) {
	sort(vrfo.begin(), vrfo.end());
	for (int i = 0; i < vrfo.size(); i++) {
		if (vrfo[i].left == vrfo[i].dependent) {// 删除自身
			vrfo.erase(vrfo.begin() + i);
			i--;
		}
		else if (i != vrfo.size() - 1) {
			if (vrfo[i] == vrfo[i + 1]) {
				vrfo.erase(vrfo.begin() + i + 1);
				i--;
			}
		}
	}
}
/// @brief 求出一个产生式 left=pro 的RecordFollow蕴含信息
/// @param vrfo 
/// @param vsf 
/// @param left 
/// @param pro 
void FollowUtil::getFollowFromProduction(vector<RecordFollow>& vrfo, vector<SymbolFirst>& vsf, string left, string pro) {
	// 将产生式转为终结符和非终结符的数组
	vector<string> rights = getRightsFromPro(pro);
	int i = (int)rights.size() - 2;// 循环从 size-2开始
	int j = i + 1;// 最后一个字符
	bool allNull = true;// 记录 Yi+1 到 Yn-1 全都可以推出虚产生式,用于结论1
	// 非终结符 Y 有空产生式意味着 FIRST[Y]集合中有 空@
	// X -> Y0 Y2 Y3 ... Yn-1:即X推到为Y0Y1Y2...Yn-1
	// 结论1-1： Yn是非终结符 则 FOLLOW[Yn-1] += FOLLOW[X]
	// 解释：有XZ存在时，必然可推导为Y0Y1Y2...Yn-1 Z,但存在 Yn-1Q，所以Yn-1的FOLLOW包含X的
	// 结论1-2：对Yi( 0 <= i < n ),如果Yi+1 到 Yn 都含虚产生式，则FOLLOW[Yi] += FOLLOW[X]
	// 解释：基于结论1可知，当Yi+1到Yn都推为虚产生式则有 X-> Y1 Y2 Y3 ... Yi
	// 结论2-1： 对于Yi( 0 <= i < n-1 ),如果Yi是非终结符，则FOLLOW[Yi] += FIRST[Yi+1]-{ 空@ }
	// 解释：Yi的FOLLOW肯定是包含后一个字符FIRST，如果Yi+1是终结符，则FIRST[Yi+1] = { Yi+1 }
	// 结论2-2：对Yi( 0 <= i < i+1 < j < n ),如果 Yi+1 到 Yj 全为非终结符且全含虚产生式，则 FOLLOW[Yi] += FIRST[j+1]-{ 空@ }
	// 小结：
	// E = + T E'
	//      结论1：Yi [ Yi+1 ... Yn-1 ] ,如果[]内无元素，或全为非终结符且含虚产生式,   则FOLLOW[Yi] += FOLLOW[X]
	//      结论2：Yi [ Yi+1 ... Yj-1 ] Yj,如果[]无元素，或内全为非终结符且含虚产生式，则FOLLOW[Yi] += FIRST[Yj]-{ 空@ }
	// 其中结论2的Yj可以是非终结符
	// 处理：对遍历Yi时，先判断是否为非终结符，非终结符内判断时是否有虚产生式,无产生式则结论1失效，2重新判断鞍；终结符则1失效，2重新判断区间
	if (isNon(rights[j])) {// 最后一个为非终结符，结论1的特殊情况结论1-1
		RecordFollow rf(rights[j]);
		rf.dependent = left;
		vrfo.push_back(rf);
	}
	else {
		allNull = false;
	}
	// i=size-2
	while (i >= 0) {// 遍历所有字符
		if (isNon(rights[i])) {// 非终结符
			// 处理结论2
			RecordFollow rf(rights[i]);
			if (isNon(rights[j])) { // FOLLOW[Yi] += FIRST[Yj]-{ 空@ }
				vector<SymbolFirst>::iterator it = FirstUtil::findRecordFirstByLeft(rights[j], vsf);
				rf.followSet.insert(it->firstSet.begin(), it->firstSet.end());
				rf.followSet.erase("@");
			}
			else { // 终结符的FIRST等于自己
				rf.followSet.insert(rights[j]);
			}
			vrfo.push_back(rf);
			// 判断当前非终结符是否有虚产生式
			vector<SymbolFirst>::iterator itt = FirstUtil::findRecordFirstByLeft(rights[i], vsf);
			if (itt->firstSet.find("@") == itt->firstSet.end()) {// 无虚产生式
				j = i;// 结论2 区间改变
			}
			// 判断结论1的条件,即当前非终结符后的 所有非终结符是否都有虚产生式
			if (allNull) {
				itt = FirstUtil::findRecordFirstByLeft(rights[i + 1], vsf);
				if (itt->firstSet.find("@") == itt->firstSet.end()) {// 无虚产生式
					allNull = false;
				}
			}
			// 结论1的处理
			if (allNull) {// 当前字符后的所有非终结符有虚产生式
				rf = RecordFollow(rights[i]);
				rf.dependent = left;
				vrfo.push_back(rf);
			}
		}
		else {
			allNull = false;// 结论1失效
			j = i; // 结论2 区间后的 字符
		}
		i--;
	}
}
/// @brief 判断str是否为非终结符
/// @param str 
/// @return 非终结符返回true
bool FollowUtil::isNon(string str) {
	if (str[0] >= 'A' && str[0] <= 'Z') {
		return true;
	}
	return false;
}
/// @brief 将产生式字串分解为字符(非终结符或终结符)数组
/// @param pro 
/// @return 
vector<string> FollowUtil::getRightsFromPro(string pro) {
	vector<string> rights;
	string t;
	for (int i = 0; i < pro.size(); i++) {
		t = "";
		t += pro[i];
		if (pro[i] >= 'A' && pro[i] <= 'Z') {// 判断是否为 E'
			if (i != pro.size() - 1) {// 该字符不是最后一个字符
				if (pro[i + 1] == DYH) {
					t += pro[i + 1];
					i++;
				}
			}
		}
		rights.push_back(t);
	}
	return rights;
}
/// <summary>
/// 打印RecordFollow表
/// </summary>
/// <param name="vrfo"></param>
void FollowUtil::printRecordFollow(vector<RecordFollow>& vrfo) {
	cout << "left\tdependent\tfollowSet" << endl;
	for (int i = 0; i < vrfo.size(); i++) {
		cout << vrfo[i].left << "\t" << vrfo[i].dependent << "\t\t";
		for (set<string>::iterator it = vrfo[i].followSet.begin(); it != vrfo[i].followSet.end(); it++) {
			cout << *it << " ";
		}
		cout << endl;
	}
}

