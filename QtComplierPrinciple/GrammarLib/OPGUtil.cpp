#include "OPGUtil.h"


RecordFirstTerm::RecordFirstTerm() {
	proID = -1;
	firstSet.clear();
	left = "";
	dependent = "";
}

RecordFirstTerm::RecordFirstTerm(int _proID) {
	proID = _proID;
	left = "";
	dependent = "";
	firstSet.clear();
}

RecordFirstTerm::RecordFirstTerm(int _proID, string _left) {
	proID = _proID;
	left = _left;
	dependent = "";
	firstSet.clear();
}

FirstTerm::FirstTerm() {
	left = "";
	firstSet.clear();
}

map<pair<char, char>, int> OPGUtil::getFGTable(map<pair<char, char>, char>& table, Grammar& g) {
	map<pair<char, char>, int> fgTable;
	// 预处理：找到所有的<,>,=关系并保存;优先函数表初始化全为1
	map<pair<char, char>, char> lessTable;
	map<pair<char, char>, char> greaterTable;
	map<pair<char, char>, char> equalTable;
	initForFGTable(g, table, fgTable, lessTable, greaterTable, equalTable);
	// 迭代处理
	bool isLoop = true;
	int cnt = 0;
	while (isLoop) {
		isLoop = false;
		cout << "迭代次数：" << ++cnt << endl;
		printFGTable(g, fgTable);
		// 处理小于
		if (getRelation(fgTable, lessTable)) {
			isLoop = true;
		}
		// 处理大于
		if (getRelation(fgTable, greaterTable)) {
			isLoop = true;
		}
		// 处理等于
		if (getRelation(fgTable, equalTable)) {
			isLoop = true;
		}
	}
	return fgTable;
}


vector<FirstTerm> OPGUtil::getFirstTerm(Grammar& g) {
	// 蕴含信息表
	vector<RecordFirstTerm> vrft;
	vector<FirstTerm> vft;
	map<string, int> sig;
	// 生成蕴含信息表
	generateRecordFirstTerm(g.productions, vrft);
	initMap(g.productions, sig);
	cout << "FIRSTTERM蕴含记录表\n";
	printRecordFirstTerm(vrft);
	while (!vrft.empty()) {
		// 第一步，删除可以直接求出FIRSTTERM的非终结符
		vector<FirstTerm> newVft = moveNoDependent(vrft);
		cout << "直接求出FIRSTTERM+的有：" << newVft.size() << endl;
		for (FirstTerm ft : newVft) {
			cout << ft.left << "\t";
			for (string s : ft.firstSet) {
				cout << s << ",";
			}
			cout << endl;
		}
		int start = vft.size();// 标记已求出FIRST的非终结符，及其所在vsf中的位置
		for (int i = 0; i < newVft.size(); i++) {
			sig[newVft[i].left] = start + i;
		}
		vft.insert(vft.end(), newVft.begin(), newVft.end());
		// 第二步，将已求出FIRSTTERM的非终结符替换到vrf中
		replaceDependent(vrft, vft, sig);
	}
	cout << "非终结符的FIRSTTERM+为：\n";
	printFirstTerm(vft);

	return vft;
}


vector<LastTerm> OPGUtil::getLastTerm(Grammar& g) {
	// 蕴含信息表
	vector<RecordLastTerm> vrft;
	vector<LastTerm> vft;
	map<string, int> sig;
	// 生成蕴含信息表
	generateRecordLastTerm(g.productions, vrft);
	initMap(g.productions, sig);
	cout << "LASTTERM蕴含记录表\n";
	printRecordFirstTerm(vrft);
	while (!vrft.empty()) {
		// 第一步，删除可以直接求出FIRSTTERM的非终结符
		vector<FirstTerm> newVft = moveNoDependent(vrft);
		cout << "直接求出LASTTERM+的有：" << newVft.size() << endl;
		for (FirstTerm ft : newVft) {
			cout << ft.left << "\t";
			for (string s : ft.firstSet) {
				cout << s << ",";
			}
			cout << endl;
		}
		int start = vft.size();// 标记已求出FIRST的非终结符，及其所在vsf中的位置
		for (int i = 0; i < newVft.size(); i++) {
			sig[newVft[i].left] = start + i;
		}
		vft.insert(vft.end(), newVft.begin(), newVft.end());
		// 第二步，将已求出FIRSTTERM的非终结符替换到vrf中
		replaceDependent(vrft, vft, sig);
	}
	cout << "非终结符的LASTTERM+为：\n";
	printFirstTerm(vft);

	return vft;
}

map<pair<char, char>, char> OPGUtil::getTable(Grammar& g, vector<FirstTerm>& first, vector<LastTerm>& last) {
	map<pair<char, char>, char> table;
	// 遍历某个非终结符及其产生式
	for (int i = 0; i < g.productions.size(); i++) {
		// 遍历每个产生式
		for (string production : g.productions[i].right) {

			// 将产生式分割为终结符和非终结符的数组
			vector<string> rights = PUTIL::getRightsFromPro(production);
			cout << "当前产生式字符数组为: ";
			for (string s : rights) {
				cout << s << " ";
			}
			cout << endl;
			// 寻找关系
			for (int j = 0; j < rights.size() - 1; j++) {
				//cout << "第" << j << "次迭代开始" << endl;
				// ab 或者 aQb
				if (!PUTIL::isNon(rights[j])) {
					if (!PUTIL::isNon(rights[j + 1])) {
						pair<char, char> p = { rights[j][0],rights[j + 1][0] };
						cout << "存在 = 关系: " << rights[j][0] << " " << rights[j + 1][0] << endl;
						table.insert({ p,'=' });
					}
					else if (j != (rights.size() - 2)) {
						if (!PUTIL::isNon(rights[j + 2])) {
							pair<char, char> p = { rights[j][0],rights[j + 2][0] };
							cout << "存在 = 关系: " << rights[j][0] << " " << rights[j + 2][0] << endl;
							table.insert({ p,'=' });
						}
						else {
							//assert("存在产生式：" + right + ",不是算符优先文法!");
							cout << "不是算符优先文法\n";
							return map<pair<char, char>, char>();
						}
					}
				}
				string sym1 = rights[j];
				string sym2 = rights[j + 1];
				// 非终结符和终结符, LAST(Non) > term
				if (PUTIL::isNon(sym1) && !PUTIL::isNon(sym2)) {
					int index = findLeft(last, sym1);
					cout << "存在 > 关系: " << sym1 << " " << sym2 << endl;
					cout << "LAST : " << last[index].left << " ";
					for (string s : last[index].firstSet) {
						cout << s << " ";
						pair<char, char> p = { s[0],sym2[0] };
						table.insert({ p, '>' });
					}
					cout << endl;
				}
				// 终结符和非终结符, term < FIRST(Non)
				else if (!PUTIL::isNon(sym1) && PUTIL::isNon(sym2)) {
					int index = findLeft(first, sym2);
					cout << "存在 < 关系: " << sym1 << " " << sym2 << endl;
					cout << "FIRST: " << first[index].left << " ";
					for (string s : first[index].firstSet) {
						cout << s[0] << " ";
						pair<char, char> p = { sym1[0],s[0] };
						table.insert({ p, '<' });
					}
					cout << endl;
				}
				//cout << "第" << j << "次迭代结束" << endl;
			}
		}
	}
	// 加上句末符，存在关系#E#
	int index1 = findLeft(first, g.productions[0].left);
	int index2 = findLeft(last, g.productions[0].left);
	for (string s : first[index1].firstSet) {
		table.insert({ { '#',s[0]},'<' });
	}
	for (string s : last[index2].firstSet) {
		table.insert({ { s[0],'#'},'>' });
	}
	/*for (char c : g.terms) {
		table.insert({ { c,'#' }, '>' });
		table.insert({ { '#',c }, '<' });
	}*/
	table.insert({ { '#','#'}, '=' });
	g.terms.insert('#');
	return table;
}
map<pair<char, char>, char> OPGUtil::getTableNo(Grammar& g, vector<FirstTerm>& first, vector<LastTerm>& last) {
	map<pair<char, char>, char> table;
	// 遍历某个非终结符及其产生式
	for (int i = 0; i < g.productions.size(); i++) {
		// 遍历每个产生式
		for (string production : g.productions[i].right) {

			// 将产生式分割为终结符和非终结符的数组
			vector<string> rights = PUTIL::getRightsFromPro(production);
			cout << "当前产生式字符数组为: ";
			for (string s : rights) {
				cout << s << " ";
			}
			cout << endl;
			// 寻找关系
			for (int j = 0; j < rights.size() - 1; j++) {
				//cout << "第" << j << "次迭代开始" << endl;
				// ab 或者 aQb
				if (!PUTIL::isNon(rights[j])) {
					if (!PUTIL::isNon(rights[j + 1])) {
						pair<char, char> p = { rights[j][0],rights[j + 1][0] };
						cout << "存在 = 关系: " << rights[j][0] << " " << rights[j + 1][0] << endl;
						table.insert({ p,'=' });
					}
					else if (j != (rights.size() - 2)) {
						if (!PUTIL::isNon(rights[j + 2])) {
							pair<char, char> p = { rights[j][0],rights[j + 2][0] };
							cout << "存在 = 关系: " << rights[j][0] << " " << rights[j + 2][0] << endl;
							table.insert({ p,'=' });
						}
						else {
							//assert("存在产生式：" + right + ",不是算符优先文法!")/*;*/
						}
					}
				}
				string sym1 = rights[j];
				string sym2 = rights[j + 1];
				// 非终结符和终结符, LAST(Non) > term
				if (PUTIL::isNon(sym1) && !PUTIL::isNon(sym2)) {
					int index = findLeft(last, sym1);
					cout << "存在 > 关系: " << sym1 << " " << sym2 << endl;
					cout << "LAST : " << last[index].left << " ";
					for (string s : last[index].firstSet) {
						cout << s << " ";
						pair<char, char> p = { s[0],sym2[0] };
						table.insert({ p, '>' });
					}
					cout << endl;
				}
				// 终结符和非终结符, term < FIRST(Non)
				else if (!PUTIL::isNon(sym1) && PUTIL::isNon(sym2)) {
					int index = findLeft(first, sym2);
					cout << "存在 < 关系: " << sym1 << " " << sym2 << endl;
					cout << "FIRST: " << first[index].left << " ";
					for (string s : first[index].firstSet) {
						cout << s[0] << " ";
						pair<char, char> p = { sym1[0],s[0] };
						table.insert({ p, '<' });
					}
					cout << endl;
				}
			}
		}
	}

	g.terms.insert('#');
	return table;
}
void OPGUtil::printFGTable(Grammar& g, map<pair<char, char>, int>& fgTable) {
	for (char c : g.terms) {
		cout << "\t" << c;
	}
	cout << endl << "f\t";
	for (char c : g.terms) {
		pair<char, char> p = { 'f',c };
		if (fgTable.find(p) != fgTable.end()) {
			cout << fgTable[p];
		}
		cout << "\t";
	}
	cout << endl << "g\t";
	for (char c : g.terms) {
		pair<char, char> p = { 'g',c };
		if (fgTable.find(p) != fgTable.end()) {
			cout << fgTable[p];
		}
		cout << "\t";
	}
	cout << endl;
}
void OPGUtil::initForFGTable(Grammar& g, map<pair<char, char>, char>& table, map<pair<char, char>, int>& fgTable, map<pair<char, char>, char>& lessTable, map<pair<char, char>, char>& greaterTable, map<pair<char, char>, char>& equalTable) {
	for (pair<pair<char, char>, char> p : table) {
		if (p.second == '<') {
			lessTable.insert(p);
		}
		else if (p.second == '>') {
			greaterTable.insert(p);
		}
		else {
			equalTable.insert(p);
		}
	}
	for (char c : g.terms) {
		fgTable.insert({ {'f',c},1 });
		fgTable.insert({ {'g',c},1 });
	}
}
/// <summary>
/// 处理关系
/// </summary>
/// <param name="fgTable"></param>
/// <param name="relaTable"></param>
/// <returns></returns>
bool OPGUtil::getRelation(map<pair<char, char>, int>& fgTable, map<pair<char, char>, char>& relaTable) {
	bool isChange = false;
	// 循环所有关系  c1  c2  rela
	for (pair<pair<char, char>, char> p : relaTable) {
		if (p.second == '<') {// 实现 f(c1) < g(c2)
			if (fgTable[{'f', p.first.first}] >= fgTable[{'g', p.first.second}]) {
				fgTable[{'g', p.first.second}] = fgTable[{'f', p.first.first}] + 1;
				isChange = true;
			}
		}
		else if (p.second == '>') {// 实现 f(c1) > g(c2)
			if (fgTable[{'f', p.first.first}] <= fgTable[{'g', p.first.second}]) {
				fgTable[{'f', p.first.first}] = fgTable[{'g', p.first.second}] + 1;
				isChange = true;
			}

		}
		else {// 实现 f(c1) == g(c2)
			if (fgTable[{'f', p.first.first}] != fgTable[{'g', p.first.second}]) {
				int max = fgTable[{'f', p.first.first}] > fgTable[{'g', p.first.second}] ? fgTable[{'f', p.first.first}] : fgTable[{'g', p.first.second}];
				fgTable[{'f', p.first.first}] = fgTable[{'g', p.first.second}] = max;
				isChange = true;
			}
		}
	}
	return isChange;
}

int OPGUtil::findLeft(vector<FirstTerm>& vft, string left) {
	for (int i = 0; i < vft.size(); i++) {
		if (vft[i].left == left) {
			return i;
		}
	}
	return -1;
}
void OPGUtil::generateRecordLastTerm(vector<Production>& vp, vector<RecordFirstTerm>& vrft) {
	int proID = 0;
	// 遍历某个非终结符及其产生式
	for (int i = 0; i < vp.size(); i++) {
		// 左部
		string left = vp[i].left;
		// 遍历产生式，获取非终结符的LASTTERM与非终结符与非终结符的蕴含关系
		for (int j = 0; j < vp[i].right.size(); j++) {
			RecordFirstTerm rft(proID++, left);
			string right = vp[i].right[j];
			// 将产生式转换为字符数组
			vector<string> rights = PUTIL::getRightsFromPro(right);
			// 开始下标
			int start = rights.size() - 1;
			// 判断后两个字符，将最后一个终结符加入到LASTTERM中，如果最后一个字符时非终结符则加入蕴含关系
			if (PUTIL::isNon(rights[start])) {// 非终结符
				rft.dependent = rights[start];
				if (left != rights[start]) {// 去除自蕴含
					vrft.push_back(rft);
				}
				// 判断是否有两个字符
				if (rights.size() > 1) {
					if (!PUTIL::isNon(rights[start - 1])) {// 终结符
						rft = RecordFirstTerm(proID++, left);
						rft.firstSet.insert(rights[start - 1]);
						vrft.push_back(rft);
					}
					else {// 非终结符，报错
						assert("不是算符优先文法!");
					}
				}
			}
			else {
				rft.firstSet.insert(rights[start]);
				vrft.push_back(rft);
			}
		}
	}
}
void OPGUtil::printFirstTerm(vector<FirstTerm>& vft) {
	cout << "symbol\tFirstTermSet" << endl;
	for (int i = 0; i < vft.size(); i++) {
		cout << vft[i].left << "\t";
		for (set<string>::iterator it = vft[i].firstSet.begin(); it != vft[i].firstSet.end(); it++) {
			cout << *it << " ";
		}
		cout << endl;
	}
}
void OPGUtil::replaceDependent(vector<RecordFirstTerm>& vrft, vector<FirstTerm>& vft, map<string, int>& sig) {
	for (int i = 0; i < vrft.size(); i++) {
		if (vrft[i].dependent != "") {// 找到可能被替换的
			int pos = sig[vrft[i].dependent];
			if (pos >= 0) {//可以替换
				vrft[i].dependent = "";
				vrft[i].firstSet.insert(vft[pos].firstSet.begin(), vft[pos].firstSet.end());
			}
		}
	}
}
vector<FirstTerm> OPGUtil::moveNoDependent(vector<RecordFirstTerm>& vrft) {
	vector<FirstTerm> vft;
	int left = 0, right = 0;
	int pos = 0;
	pair<int, int> p = findNoDependent(vrft, pos);
	while (p.first < vrft.size()) {
		FirstTerm ft;
		ft.left = vrft[p.first].left;
		for (int i = p.first; i < p.second; i++) {
			ft.firstSet.insert(vrft[i].firstSet.begin(), vrft[i].firstSet.end());
		}
		while (p.second != p.first) {
			vrft.erase(vrft.begin() + p.first);
			p.second--;
		}
		vft.push_back(ft);
		p = findNoDependent(vrft, p.first);
	}
	return vft;
}
pair<int, int> OPGUtil::findNoDependent(vector<RecordFirstTerm>& vrft, int pos) {
	int left = pos, right = pos;
	bool is = true;
	while (right < vrft.size()) {
		if (vrft[left].left == vrft[right].left) {
			if (vrft[right].dependent != "") {// 当前非终结符不可，跳过
				while (vrft[right].left == vrft[left].left && right < vrft.size()) {
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
//getFirst();
void OPGUtil::initMap(vector<Production>& vp, map<string, int>& sig) {
	sig.clear();
	for (int i = 0; i < vp.size(); i++) {
		sig.insert({ vp[i].left , -1 });
	}
}
/// <summary>
/// 生成蕴含记录表
/// </summary>
/// <param name="vp"></param>
/// <param name="vrft"></param>
void OPGUtil::generateRecordFirstTerm(vector<Production>& vp, vector<RecordFirstTerm>& vrft) {
	int proID = 0;
	// 循环每个非终结符及其产生式
	for (int i = 0; i < vp.size(); i++) {
		// 左部
		string left = vp[i].left;
		// 循环每个产生式,获取非终结符的FIRSTTERM和非终结符与非终结符的FIRSTTERM蕴含关系
		for (int j = 0; j < vp[i].right.size(); j++) {
			RecordFirstTerm rft(proID++, left);
			string right = vp[i].right[j];
			// 将产生式转为字符数组
			vector<string> rights = PUTIL::getRightsFromPro(right);
			// 判断前两个字符，并将第一个出现的终结符加入firstterm，如果有蕴含关系则加入
			if (PUTIL::isNon(rights[0])) {// 非终结符，有蕴含关系
				rft.dependent = rights[0];
				if (left != rights[0]) {// 去除自蕴含
					vrft.push_back(rft);
				}
				// 判断是否有第二个字符，且是否为终结符，如果为非终结符则报错，不是算符优先文法
				if (rights.size() > 1) {
					if (!PUTIL::isNon(rights[1])) {// 终结符
						rft = RecordFirstTerm(proID++, left);
						rft.firstSet.insert(rights[1]);
						vrft.push_back(rft);
					}
					else {// 出现两个非终结符相连
						assert("不是算符优先文法!");
					}
				}
			}
			else {// 终结符，只需将该加入该非终结符的FIRSTTERM中即可
				rft.firstSet.insert(rights[0]);
				vrft.push_back(rft);
			}
		}
	}
}
void OPGUtil::printRecordFirstTerm(vector<RecordFirstTerm>& vrft) {
	cout << "proID\tleft\tdependent\tfirstSet" << endl;
	for (int i = 0; i < vrft.size(); i++) {
		cout << vrft[i].proID << "\t" << vrft[i].left << "\t" << vrft[i].dependent << "\t\t";
		for (set<string>::iterator it = vrft[i].firstSet.begin(); it != vrft[i].firstSet.end(); it++) {
			cout << *it << " ";
		}
		cout << endl;
	}
}

bool OPGUtil::isJuzi(string start, string juzi, map<pair<char, char>, char>& table){
	// 终结符栈和比较关系栈，比较关系栈第i个表示的关系是终结符栈第i个和第i+1个符号的关系，如果i+1无，则是待识别句子的终结符
	vector<char> sym;
	vector<char> op;
	string t;
	// 初始化状态
	juzi = juzi + "#";
	sym.push_back('#');
	int currentIndex = 0;// 句子正在识别的符号的下标
	int firstTerm = 0;// 栈中第一个终结符的下标
	int cnt = 0;
	cout << "符号栈\t\t待识别的句子\t\t匹配的规则\n";
	while (1) {
		cout << endl << "循环次数: " << cnt++ << endl;
		for (int i = 0; i < sym.size(); i++) {
			cout << sym[i];
		}
		cout << "\t\t";
		for (int i = currentIndex; i < juzi.size(); i++) {
			cout << juzi[i];
		}
		cout << "\t\t";
		


		map<pair<char, char>, char>::iterator it = table.find({ sym[firstTerm],juzi[currentIndex] });
		if (it != table.end()) {// 表中存在关系
			char s1 = it->first.first;
			char s2 = it->first.second;
			char r = it->second;
			if (r == '<' || r == '=') {// 将正在识别的符号和比较关系加入栈中
				sym.push_back(s2);
				op.push_back(r);
				// 设置栈中第一个终结符的下标
				string t;
				if (PUTIL::isNon( s2+t)) {// 不是非终结符
					firstTerm = sym.size() - 2;
				}
				else {
					firstTerm = sym.size() - 1;
				}
				currentIndex++;

				cout << s1 << r << s2;
			}
			else if (it->second == '>') {
				string pattern;
				while (firstTerm < sym.size()) {// 将第一个终结符即其之后的符号加入
					pattern = pattern + sym.back();
					sym.pop_back();
				}
				firstTerm = sym.size() - 2;
				t = "";
				while (op.back() == '=' || PUTIL::isNon(t+sym.back())) {
					pattern = pattern + sym.back();
					sym.pop_back();
					op.pop_back();
				}
				char newNon = start[0];
				sym.push_back(newNon);
				cout << pattern << "->" << newNon;
				firstTerm = sym.size() - 2;
			}
			else {// 表出错
				cout << "存在不属于<=>关系:" << sym[firstTerm] << "->" << juzi[currentIndex] << endl;
				return false;
			}
		}
		else {
			cout << "不存在关系:" << sym[firstTerm] << "->" << juzi[currentIndex] << endl;
			return false;
			return false;
		}
		if ( currentIndex == juzi.size()-1 && sym.size() == 2 ) {
			cout << endl << "分析结束" << endl;
			t = "";
			t += sym[1];
			if (t != start) {
				cout << "start: " << start << " " << t << endl;
				return false;
			}
			break;
		}
	}
	
	cout << endl;
	return true;
}
/// <summary>
/// 采用算数优先矩阵、自底向上分析是否为句子
/// </summary>
/// <param name="start"></param>
/// <param name="juzi"></param>
/// <param name="table"></param>
/// <param name="proToLeftTable"></param>
/// <returns></returns>
ERRORTYPE OPGUtil::isSentence(string start, string juzi, map<pair<char, char>, char>& table, map<string, char>& proToLeftTable)
{
	FILE* fp;
//D:\Code\CCode\VS\QtProject\OPG\x64\Debug
	//fopen_s(&fp,"..\\QtProject\\OPG\\x64\\Debug\\toQt.txt", "w");
	//fopen_s(&fp,"toQt.txt", "w");
	fopen_s(&fp, R"(D:\Code\CCode\VS\QtProject\QtComplierPrinciple\x64\Debug\toQt.txt)", "w");
	start = "N";
	// 初始化
	ERRORTYPE error = ISSENTENCE;
	vector<char> symbols;// 符号栈
	vector<char> relation;// 关系栈，relation[i]表示符号栈symbols[i] <=> symbol[i+1]
	int firstTerm = 0;// 符号栈中第一个终结符的下标
	int current = 0;// 正在识别的句子的符号的下标
	juzi = juzi + "#";
	symbols.push_back('#');
	int cnt = 0;
	cout << "步骤\t栈\t关系\t下一个符号\t其余输入部分\t最左质短语\n";
	fprintf(fp, "步骤\t栈\t关系\t下一个符号\t其余输入部分\t最左质短语\n");
	while (1) {
		fprintf(fp,"%d\t",cnt);
		cout << cnt++ << "\t";
		for (int i = 0; i < symbols.size(); i++) {
			cout << symbols[i];
			fprintf(fp, "%c",symbols[i]);
		}
		cout << "\t";
		fprintf(fp, "\t");
		// 查找符号栈的第一个终结符与当前字符的关系
		map<pair<char, char>, char>::iterator it = table.find({ symbols[firstTerm],juzi[current] });
		if (it != table.end()) {
			cout << it->second << "\t" << juzi[current] << "\t\t";
			fprintf(fp, "%c\t%c\t\t",it->second, juzi[current]);
			if (current == juzi.size() - 1) {
				fprintf(fp, "$");
			}
			for (int i = current + 1; i < juzi.size(); i++) {
				cout << juzi[i];
				fprintf(fp, "%c",juzi[i]);
			}
			cout << "\t\t";
			fprintf(fp, "\t\t");
			// 处理 <=
			if (it->second == '<' || it->second == '=') {
				symbols.push_back(it->first.second);
				relation.push_back(it->second);
				firstTerm = symbols.size() - 1;
				current++;
				fprintf(fp,"$");
			}
			else if (it->second == '>') {
				// 找到最左质短语
				string phase = getLeftmostPrimePhrase(symbols, relation);
				// 将质短语可规约为的非终结符加入符号栈
				char newNon = 'N';
				pair<string, char> p = stringToPro(proToLeftTable, phase);
				//char newNon = p.second;
				symbols.push_back(newNon);
				cout << phase;
				fprintf(fp, "%s",phase.c_str());
				// 找到符号栈的第一个终结符,不可能出现两个相连的非终结符，且最后一个符号是终结符
				firstTerm = symbols.size() - 2;
			}
			else {// 处理错误关系
				error = NONEXISTENTRELATIONSHIP;
				break;
			}
			cout << endl;
			fprintf(fp, "\n");
		}
		else {// 找到不存在的关系
			error = NONEXISTENTRELATIONSHIP;
			break;
		}
		// 识别结束
		if (juzi[current]=='#' && symbols.size() == 2) {// 符号栈中为 #N 且 句子识别到#
			if (PUTIL::isNon(symbols[1])) {
				break;
			}
		
		}
	}
	fprintf(fp, "%d\t",cnt);
	cout << cnt++ << "\t";
	for (char c : symbols){
		cout << c;
		fprintf(fp, "%c",c);
	}
	cout << "\t\t#\t\t\t\t\n";
	fprintf(fp, "\t$\t#\t\t$\t\t$\n");
	if (start[0] != symbols[1]) {
		error = NOTSTARTCHAR;
	}
	if (error == ISSENTENCE) {
		fprintf(fp,"是一个句子!");
	}
	else {
		fprintf(fp, "不是一个句子!");
	}
	fclose(fp);
	return error;
}




void OPGUtil::printOPGTable(Grammar& g, map<pair<char, char>, char>& table) {
	for (char c1 : g.terms) {
		cout << "\t" << c1;
	}
	cout << endl;
	for (char c1 : g.terms) {
		cout << c1 << "\t";
		for (char c2 : g.terms) {
			pair<char, char> p = { c1,c2 };
			if (table.find(p) != table.end()) {
				cout << table[p] << "\t";
			}
			else {
				cout << "\t";
			}
		}
		cout << endl;
	}
}

string OPGUtil::getLeftmostPrimePhrase(vector<char>& symbols, vector<char>& relation) {
	string pattern;
	// 符号栈与关系栈有：n1 < n2 = n3 = n4
	// 将 n1 < n2 = n3 = n4, 出现的第一个 < 后面的字符连接
	while (relation.size() > 0) {
		if (PUTIL::isNon(symbols.back())) {// 非终结符直接加入
			pattern = symbols.back() + pattern;
			symbols.pop_back();
		}
		else {// 终结符判断关系
			if (relation.back() == '=') {
				pattern = symbols.back() + pattern;
				symbols.pop_back();
				relation.pop_back();
			}
			else if (relation.back() == '<') {
				pattern = symbols.back() + pattern;
				symbols.pop_back();
				relation.pop_back();
				break;
			}
		}
	}
	while (PUTIL::isNon(symbols.back())) {
		pattern = symbols.back() + pattern;
		symbols.pop_back();
	}
	return pattern;
}

ERRORTYPE OPGUtil::isSentencePlus(Grammar& g, string start, string juzi, map<pair<char, char>, char>& table, map<string, char>& proToLeftTable)
{

	//start = "N";
	// 初始化
	ERRORTYPE error = ISSENTENCE;
	vector<char> symbols;// 符号栈
	vector<char> relation;// 关系栈，relation[i]表示符号栈symbols[i] <=> symbol[i+1]
	int firstTerm = 0;// 符号栈中第一个终结符的下标
	int current = 0;// 正在识别的句子的符号的下标
	juzi = juzi + "#";
	symbols.push_back('#');
	int cnt = 0;
	cout << "步骤\t栈\t关系\t下一个符号\t其余输入部分\t最左质短语\n";
	while (1) {
		cout << cnt++ << "\t";
		for (int i = 0; i < symbols.size(); i++) {
			cout << symbols[i];
		}
		cout << "\t";
		// 查找符号栈的第一个终结符与当前字符的关系
		map<pair<char, char>, char>::iterator it = table.find({ symbols[firstTerm],juzi[current] });
		if (it != table.end()) {
			cout << it->second << "\t" << juzi[current] << "\t\t";
			for (int i = current + 1; i < juzi.size(); i++) {
				cout << juzi[i];
			}
			cout << "\t\t";

			// 处理 <=
			if (it->second == '<' || it->second == '=') {
				symbols.push_back(it->first.second);
				relation.push_back(it->second);
				firstTerm = symbols.size() - 1;
				current++;
			}
			else if (it->second == '>') {
				// 找到最左质短语

				string phase = getLeftmostPrimePhrase(symbols, relation);
				// 将质短语可规约为的非终结符加入符号栈
				//char newNon = 'N';
				pair<string, char> p = stringToPro(proToLeftTable, phase);
				char newNon = p.second;
				symbols.push_back(newNon);
				cout << phase;
				// 找到符号栈的第一个终结符,不可能出现两个相连的非终结符，且最后一个符号是终结符
				firstTerm = symbols.size() - 2;
			}
			else {// 处理错误关系
				error = NONEXISTENTRELATIONSHIP;
				break;
			}
			cout << endl;
		}
		else {// 找到不存在的关系
			error = NONEXISTENTRELATIONSHIP;
			break;
		}
		// 识别结束
		if (juzi[current] == '#' && symbols.size() == 2) {// 符号栈中为 #N 且 句子识别到#
			break;
		}
	}
	cout << cnt << "\t";
	for (char c : symbols) {
		cout << c;
	}
	cout << "\t\t#\n";
	if (start[0] != symbols[1]) {
		error = NOTSTARTCHAR;
	}

	return error;
}


map<string, char> OPGUtil::getProToLeftTable(Grammar& g) {
	map<string, char> table;
	for (int i = 0; i < g.productions.size(); i++) {
		string left = g.productions[i].left;
		for (int j = 0; j < g.productions[i].right.size(); j++) {
			if (includeTerm(g.productions[i].right[j])) {
				table.insert({ g.productions[i].right[j] ,left[0] });
			}
		}
	}
	return table;
}


void OPGUtil::warShall(map<pair<char, char>, char>& table, Grammar& g) {
	// 初始化2n*2n的矩阵
	vector<vector<bool>> m(g.terms.size() * 2, vector<bool>(g.terms.size() * 2, false));
	map<char, int> termIndex;// 对终结符的排序映射
	initMatrix(table, g, m, termIndex);
	cout << "初始化的矩阵：\n";
	printMatrix(m, g.terms);
	/*cout << "手动排序终结符的顺序的矩阵：\n";
	vector<char> terms = { '+','*','(',')','i'};
	printMatrix1();*/
	solveWarshall(m, termIndex);
	int n = g.terms.size() * 2;
	for (int i = 0; i < n; i++) {
		m[i][i] = true;
	}
	cout << "求解后的矩阵：\n";
	printMatrix(m, g.terms);
	set<char>::iterator it = g.terms.begin();
	bool b = true;
	cout << endl << endl;
	for (char c : g.terms) {
		cout << "\t" << c;
	}
	cout << endl;
	for (int i = 0; i < n; i++) {
		if (b && i < n / 2) {
			cout << "f: ";
			b = false;
		}
		else if (!b && i == n / 2) {
			cout << "g: ";
			b = false;
		}
		int cnt = 0;
		for (int j = 0; j < n; j++) {
			if (m[i][j]) {
				cnt++;
			}
		}
		cout << "\t" << cnt;
		it++;
		if (i == (n / 2) - 1) {
			b = true;

			it = g.terms.begin();
			cout << endl;
			cout << "g: ";
		}
	}
	cout << endl;
}
void OPGUtil::solveWarshall(vector<vector<bool>>& m, map<char, int>& termIndex) {
	int n = termIndex.size() * 2;
	for (int col = 0; col < n; col++) {
		for (int row = 0; row < n; row++) {
			if (m[row][col] == true) {
				for (int k = 0; k < n; k++) {
					m[row][k] = m[col][k] + m[row][k];
				}
			}
		}
	}
}

void OPGUtil::printMatrix(vector<vector<bool>>& m, set<char>& terms) {
	for (char c : terms) {
		cout << "\t" << c;
	}
	cout << "\t|";
	for (char c : terms) {
		cout << "\t" << c;
	}
	cout << endl;
	int i = 0, j = 0;
	for (char c1 : terms) {
		cout << c1 << "\t";
		for (char c2 : terms) {
			cout << m[i][j] << "\t";
			j++;
		}
		cout << "|\t";
		for (char c2 : terms) {
			cout << m[i][j] << "\t";
			j++;
		}
		cout << endl;
		i++;
		j = 0;
	}
	for (char c2 : terms) {
		cout << "--------------------";
	}
	cout << endl;
	for (char c1 : terms) {
		cout << c1 << "\t";
		for (char c2 : terms) {
			cout << m[i][j] << "\t";
			j++;
		}
		cout << "|\t";
		for (char c2 : terms) {
			cout << m[i][j] << "\t";
			j++;
		}
		cout << endl;
		i++;
		j = 0;
	}
}

void OPGUtil::initMatrix(map<pair<char, char>, char>& table, Grammar& g, vector<vector<bool>>& m, map<char, int>& termIndex) {
	int index = 0;
	int n = g.terms.size();
	// 终结符的排序
	cout << "终结符: ";
	for (char c : g.terms) {
		termIndex.insert({ c,index++ });
		cout << c << " ";
	}
	cout << endl;
	int cnt = 0;
	for (pair<pair<char, char>, char> p : table) {
		//cout << p.first.first << " " << p.first.second << " " << p.second << endl;
		// 获取 t1 > t2 中t1,t2在矩阵中的下标
		int index1 = termIndex[p.first.first];
		int index2 = termIndex[p.first.second];
		if (p.second == '>') {
			m[index1][index2 + n] = true;
			//cout << index1 << " " << index2 + n << endl;
		}
		else if (p.second == '=') {
			m[index1][index2 + n] = true;
			//m[index1+n][index2-n] = true;// 赋值时转置
			m[index2 + n][index1] = true;

			//cout << index1 << " " << index2 + n << endl;
			//cout << index2+n << " " << index1 << endl;
		}
		else {
			//m[index1 + n][index2 - n] = true;// 赋值时转置
			m[index2 + n][index1] = true;
			//cout << index2 + n << " " << index1 << endl;
		}
	}
}
bool OPGUtil::includeTerm(string pro) {
	for (int i = 0; i < pro.size(); i++) {
		if (pro[i] >= 'a' && pro[i] <= 'z') {
			return true;
		}
	}
	false;
}
pair<string, char> OPGUtil::stringToPro(map<string, char>& table, string regular) {
	for (pair<string, char> p : table) {
		if (p.first.size() == regular.size()) {
			bool isReturn = true;
			for (int i = 0; i < regular.size(); i++) {
				if ((PUTIL::isNon(p.first[i]) && PUTIL::isNon(regular[i])) || p.first[i] == regular[i]) {

				}
				else {
					isReturn = false;
					break;
				}
			}
			if (isReturn) {
				return p;
			}
		}
	}
	return { "",0 };
}