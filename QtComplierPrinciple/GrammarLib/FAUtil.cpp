#include "FAUtil.h"

void FAUtil::readDFA( vector<DFA>& dfas, string filePath ) {
	char line[200];
	FILE* fp;
	errno_t err = fopen_s(&fp, filePath.c_str(), "r");
	if (fp == 0) {
		cout << "文件打开失败!" << endl;
		return;
	}
	fgets(line, sizeof(line), fp);// 读取第一行的说明文本
	while (fgets(line, sizeof(line), fp)) {
		set<string> statusSet = set<string>();
		set<char> inputLetter = set<char>();
		string start = "";
		map<DFAKey, string> mapTable = map<DFAKey, string>();
		set<string> finalSet = set<string>();

		string str = line;
		if (str.find_last_of('\n') != -1) {// 去除换行符 
			str.erase(--str.end());
		}
		// 读取DFA的边
		if (allNum(str)) {//全是数字
			int cnt = stringToNum(str);
			while (cnt--) {
				fgets(line, sizeof(line), fp);
				str = line;
				string ver1 = "";
				char edge = str[2];
				string ver2 = "";
				ver1 += str[0];
				//edge += str[2]
				ver2 += str[4];
				// 状态集合
				statusSet.insert(ver1);
				statusSet.insert(ver2);
				// 映射集合
				DFAKey in(ver1, edge);
				mapTable.insert(make_pair(in, ver2));
				// 输入字符集合
				inputLetter.insert(edge);
			}
		}
		//读取开始状态
		fgets(line, sizeof(line), fp);
		str = line;
		if (str.find_last_of('\n') != -1) {// 去除换行符 
			str.erase(--str.end());
		}
		if (allNum(str)) {//全是数字
			int cnt = stringToNum(str);
			while (cnt--) {
				fgets(line, sizeof(line), fp);
				str = line;
				if (str.find_last_of('\n') != -1) {// 去除换行符 
					str.erase(--str.end());
				}
				start = str;
			}
		}
		//读取终止状态
		fgets(line, sizeof(line), fp);
		str = line;
		if (str.find_last_of('\n') != -1) {// 去除换行符 
			str.erase(--str.end());
		}
		if (allNum(str)) {//全是数字
			int cnt = stringToNum(str);
			while (cnt--) {
				fgets(line, sizeof(line), fp);
				str = line;
				if (str.find_last_of('\n') != -1) {// 去除换行符 
					str.erase(--str.end());
				}
				finalSet.insert(str);
			}
		}
		DFA dfa( statusSet, inputLetter, mapTable, start, finalSet );
		dfas.push_back(dfa);
	}
	fclose(fp);
	cout << filePath << "文件读取结束" << endl;
}


void FAUtil::printMatDFA(DFA& dfa) {
	for (set<char>::iterator it = dfa.inputLetter.begin(); it != dfa.inputLetter.end(); it++) {
		cout << "\t|" << *it;
	}
	cout << endl;
	for (set<string>::iterator it1 = dfa.statusSet.begin(); it1 != dfa.statusSet.end(); it1++) {
		cout << "________________________________________________________________" << endl;
		cout << *it1 << "\t|";
		for (set<char>::iterator it2 = dfa.inputLetter.begin(); it2 != dfa.inputLetter.end(); it2++) {
			DFAKey in(*it1, *it2);
			if (dfa.mapTable.find( in )!= dfa.mapTable.end()) {
				cout << dfa.mapTable[in];
			}
			else {
				cout << "*";
			}
			cout << "\t|";
		}
		cout << endl;
	}
}

void DFA::printDFA() {
	cout << "状态集：{";
	for (set<string>::iterator it = statusSet.begin(); it != statusSet.end(); it++) {
		cout << *it << ",";
	}
	cout << "}\n";
	cout << "状态转移矩阵打印：" << endl;
	FAUtil::printMatDFA(*this);
	cout << "映射集(" << mapTable.size() <<  "):" << endl;
	for (map<DFAKey, string>::iterator it = this->mapTable.begin(); it != this->mapTable.end(); it++) {
		cout << it->first.ver << " " << it->first.edge << " " << it->second << endl;
	}
	cout << "可输入字符(" << inputLetter.size() << "): {";
	for (set<char>::iterator it = this->inputLetter.begin(); it != this->inputLetter.end(); it++) {
		cout << *it << ",";
	}
	cout <<"}" << endl;
	cout << "开始状态为：" << this->start << endl;
	cout << "终止状态为(" << finalSet.size() <<  ")：{";
	for (set<string>::iterator it = this->finalSet.begin(); it != this->finalSet.end(); it++) {
		cout << *it << ",";
	}
	cout << "}" << endl;
};

void DFA::printMatDFA() {
	for (set<char>::iterator it = this->inputLetter.begin(); it != this->inputLetter.end(); it++) {
		cout << "\t|" << *it;
	}
	cout << endl;
	for (set<string>::iterator it1 = this->statusSet.begin(); it1 != this->statusSet.end(); it1++) {
		cout << "________________________________________________________________" << endl;
		cout << *it1 << "\t|";
		for (set<char>::iterator it2 = this->inputLetter.begin(); it2 != this->inputLetter.end(); it2++) {
			DFAKey in(*it1, *it2);
			if (this->mapTable.find(in) != this->mapTable.end()) {
				cout << this->mapTable[in];
			}
			else {
				cout << "*";
			}
			cout << "\t|";
		}
		cout << endl;
	}

}

void FAUtil::printDFAs(vector<DFA>& dfas) {
	for (int i = 0; i < dfas.size(); i++) {
		cout << "第" << i << "个DFA: " << endl;
		dfas[i].printDFA();
	}
	cout << endl;
}

bool FAUtil::allNum(string str) {
	for (int i = 0; i < str.size(); i++) {
		if (str[i] < '0' || str[i]>'9') {
			return false;
		}
	}
	return true;
}
int FAUtil::stringToNum(string str) {
	int num = 0;
	for (int i = 0; i < str.size(); i++) {
		num *= 10;
		num += str[i] - '0';
	}
	return num;
}

void FAUtil::DFADerivedSentence(DFA& dfa,string str) {
	string head = dfa.start;
	DFAKey in;
	cout << "M(" << head << "," << str << ") = " << endl;
	while (str.size() != 0) {// 循环接受每一个字符
		char inputChar = str[0];
		in = DFAKey(head, inputChar);
		str.erase(0, 1);
		if (str.size() != 0) {
			cout << "\t=M(" << "M(" << in.ver << "," << in.edge << ")," << str << ")" << endl;
		}
		if ( dfa.mapTable.find(in) == dfa.mapTable.end() ) {
			cout << "未匹配到映射！这不是一个句子" << endl;
			return;
		}
		else {
			head = dfa.mapTable[in];// 获取 M(A,a) = B
		}
	}
	cout << "\t=M(" << in.ver << "," << in.edge << ")" << endl << "\t=";
	if (dfa.finalSet.find(head) != dfa.finalSet.end()) {
		cout << head << endl;
		cout << "这是一个句子！\n\n";
	}
	else {
		cout << head << endl << "\t推导结果不是终止态，不是一个句子！" << endl;
	}
}


void FAUtil::readNFA(vector<NFA>& nfas, string filePath) {
	char line[200];
	FILE* fp;
	errno_t err = fopen_s(&fp, filePath.c_str(), "r");
	if (fp == 0) {
		cout << "文件打开失败!" << endl;
		return;
	}
	fgets(line, sizeof(line), fp);// 读取第一行的说明文本
	while (fgets(line, sizeof(line), fp)) {
		set<string> statusSet;//状态集合
		set<char> inputLetter;//输入字符集合
		map< DFAKey, set<string> > mapTable;//映射集合
		set<string> start;//开始状态集合
		set<string> finalSet;//终止状态集合

		string str = line;
		if (str.find_last_of('\n') != -1) {// 去除换行符 
			str.erase(--str.end());
		}
		// 读取DFA的边
		if (allNum(str)) {//全是数字
			int cnt = stringToNum(str);
			while (cnt--) {
				fgets(line, sizeof(line), fp);
				str = line;
				string ver1 = "";
				char edge = str[2];
				string ver2 = "";
				ver1 += str[0];
				//edge += str[2]
				ver2 += str[4];
				// 状态集合
				statusSet.insert(ver1);
				statusSet.insert(ver2);
				// 映射集合
				DFAKey in(ver1, edge);
				if (mapTable.find(in) != mapTable.end() ) {
					mapTable[in].insert(ver2);
				}
				else {
					mapTable.insert(make_pair(in, set<string>{ver2}));
				}
				// 输入字符集合
				inputLetter.insert(edge);
			}
		}
		//读取开始状态
		fgets(line, sizeof(line), fp);
		str = line;
		if (str.find_last_of('\n') != -1) {// 去除换行符 
			str.erase(--str.end());
		}
		if (allNum(str)) {//全是数字
			int cnt = stringToNum(str);
			while (cnt--) {
				fgets(line, sizeof(line), fp);
				str = line;
				if (str.find_last_of('\n') != -1) {// 去除换行符 
					str.erase(--str.end());
				}
				start.insert(str);
			}
		}
		//读取终止状态
		fgets(line, sizeof(line), fp);
		str = line;
		if (str.find_last_of('\n') != -1) {// 去除换行符 
			str.erase(--str.end());
		}
		if (allNum(str)) {//全是数字
			int cnt = stringToNum(str);
			while (cnt--) {
				fgets(line, sizeof(line), fp);
				str = line;
				if (str.find_last_of('\n') != -1) {// 去除换行符 
					str.erase(--str.end());
				}
				finalSet.insert(str);
			}
		}
		NFA nfa(statusSet, inputLetter, mapTable, start, finalSet);
		nfas.push_back(nfa);
	}
	fclose(fp);
	cout << filePath << "文件读取结束" << endl;
}

void FAUtil::printMatNFA(NFA& nfa) {
	for (set<char>::iterator it = nfa.inputLetter.begin(); it != nfa.inputLetter.end(); it++) {
		cout << "\t|\t" << *it;
	}
	cout << endl;
	for (set<string>::iterator it1 = nfa.statusSet.begin(); it1 != nfa.statusSet.end(); it1++) {
		cout << "________________________________________________________________" << endl;
		cout << *it1 << "\t|";
		for (set<char>::iterator it2 = nfa.inputLetter.begin(); it2 != nfa.inputLetter.end(); it2++) {
			DFAKey in(*it1, *it2);
			if (nfa.mapTable.find(in) != nfa.mapTable.end()) {
				for (set<string>::iterator sit = nfa.mapTable[in].begin(); sit != nfa.mapTable[in].end(); sit++) {
					cout << *sit << ",";
				}
			}
			else {
				cout << "*";
			}
			cout << "\t\t|";
		}
		cout << endl;
	}
}
void FAUtil::printNFAs(vector<NFA>& nfas) {
	for (int i = 0; i < nfas.size(); i++) {
		cout << "<<<<<<<<<<<<<<<<<<<<<<<<<第" << i << "个NFA: >>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>" << endl;
		nfas[i].printNFA();
	}
	cout << endl;
}

void NFA::printNFA() {
	cout << "状态转移矩阵打印：" << endl;
	FAUtil::printMatNFA(*this);
	cout << "映射集(" << mapTable.size() << "):" << endl;
	for (map<DFAKey, set<string> >::iterator it = mapTable.begin(); it != mapTable.end(); it++) {
		cout << it->first.ver << " " << it->first.edge << " {";
		for (set<string>::iterator itt = it->second.begin(); itt != it->second.end(); itt++) {
			cout << *itt << ",";
		}
		cout << "}" << endl;
	}
	cout << "可输入字符(" << inputLetter.size() << "): {";
	for (set<char>::iterator it = inputLetter.begin(); it != inputLetter.end(); it++) {
		cout << *it << ",";
	}
	cout << "}" << endl;
	cout << "开始状态为(" << start.size() << ")：{";
	for (set<string>::iterator it = start.begin(); it != start.end(); it++) {
		cout << *it << ",";
	}
	cout << "}" << endl;
	cout << "终止状态为(" << finalSet.size() << ")：{";
	for (set<string>::iterator it = finalSet.begin(); it != finalSet.end(); it++) {
		cout << *it << ",";
	}
	cout << "}" << endl;
}

/// <summary>
/// 初始化@_closure记录蕴含表
/// 规则：<A,@>=B则A_closure += B_closure + { A } ,不存在<A,@>的映射，则A_closure={ A }
/// </summary>
/// <param name="vrc"></param>
void NFA::initRecordClosure(vector<RecordClosure>& vrc) {
	for (set<string>::iterator it = statusSet.begin(); it != statusSet.end(); it++) {
		DFAKey key(*it, CHAR_NULL);// <ver,@>
		set<string> dependent = set<string>{};// 蕴含的符号
		set<string> closure = set<string>{};// 闭包
		if (mapTable.find(key) != mapTable.end()) {// 存在<ver,@>的映射，遍历映射
			for (set<string>::iterator sit = mapTable[key].begin(); sit != mapTable[key].end(); sit++) {
				dependent.insert(*sit);
			}
		}
		closure.insert(*it);//将自身加入闭包
		vrc.push_back(RecordClosure(*it, dependent, closure));

	}
}
void NFA::printRecordClosure(vector<RecordClosure>& vrc) {
	cout << "状态的@_closure如下：" << endl;
	cout << "symbol\tdependent\tclosure\n";
	for (int i = 0; i < vrc.size(); i++) {
		cout << vrc[i].symbol << "\t|";
		for (set<string>::iterator it = vrc[i].dependent.begin(); it != vrc[i].dependent.end(); it++) {
			cout << *it << " ";
		}
		if (vrc[i].dependent.size() == 0) {
			cout << "*";
		}
		cout << "\t\t|";
		for (set<string>::iterator it = vrc[i].closure.begin(); it != vrc[i].closure.end(); it++) {
			cout << *it << " ";
		}
		cout << "\t\t|" << endl;
	}
}
void NFA::initNullClosure(vector<RecordClosure>& vrc) {
	while (vrc.size()) {
		eraseNullDependent(vrc);
		replaceDependent(vrc);
	}
}


void NFA::initClosure() {
	//@_closure蕴含表
	vector<RecordClosure> vrc;
	// 初始化@_closure蕴含表
	initRecordClosure(vrc);
	cout << "@_closure蕴含表如下：" << endl;
	printRecordClosure(vrc);
	// 初始化@_closure
	cout << "@_closure表如下：" << endl;
	initNullClosure(vrc);
	printNullClosure();
	inputLetter.erase(CHAR_NULL);
	//初始化letter_closure蕴含表:即状态A的 a_closure = <A,a>的所有映射的状态的@_closure
	initLetterClosure();
	cout << "letter_closure表如下：" << endl;
	printLetterClosure();
}

DFA FAUtil::NFAToDFA(NFA& nfa) {
	NFA old = nfa;
	// 未初始化状态的闭包
	cout << "<<<<<<<<<<<<<<<<<<<<<<<闭包运算" << endl<<endl;
	if (nfa.nullClosure.empty()||nfa.letterClosure.empty()) {
		nfa.initClosure();
		// 重新设置开始符集合，等于原集合的@_closure的并
		set<string> newStart;
		for (set<string>::iterator it = nfa.start.begin(); it != nfa.start.end(); it++) {
			newStart.insert(nfa.nullClosure[*it].begin(), nfa.nullClosure[*it].end());
		}
		nfa.start = newStart;
		cout << "开始符的闭包为：{";
		for (set<string>::iterator it = nfa.start.begin(); it != nfa.start.end(); it++) {
			cout << *it << ",";
		}
		cout << "}" << endl;
	}
	string file = "NFAOutput.txt";
	cout << "NFA输出到" << file << "中" << endl;
	nfa.fwriteNFA(file);
	cout << "\n<<<<<<<<<<<<<<<<<<<<<<<<<NFA的确定化<<<<<<<<<<<<<<<<<<<<<<<<<<<<\n" << endl;
	//	NFA等价构造DFA,转换为中间结果表示，便于转换
	NFAtoDFA ntd = equivalentReplacement( nfa );
	// 打印中间状态结果
	cout << "NFA转化为DFA的中间状态如下：" << endl;
	ntd.printNFAtoDFA();
	DFA dfa = ntd.toDFA();
	cout << "\n<<<<<<<<<<<<<<<<<<重新编号后<<<<<<<<<<<<<<<<<<<<<<" << endl;
	dfa.printDFA();
	file = "NoSimplificationDFA.txt";
	cout << "未化简文件保存在"<<file<< "中"<<endl;
	dfa.fwriteDFA(file.c_str());
	// DFA的化简
	cout << "\n<<<<<<<<<<<<<<<<<<<<<<<<<DFA的化简<<<<<<<<<<<<<<<<<<<<<<<<<<<\n\n";
	dfa.simplification();
	nfa = old;
	return dfa;
}

void DFA::simplification() {
	// 第一次划分为终止状态组和非终止状态组
	set<string> s1;//非终止状态组
	set_difference(statusSet.begin(), statusSet.end(), finalSet.begin(), finalSet.end(), inserter(s1, s1.begin()));
	// 初始化栈和用于保存已划分组的map,栈可以便于分组的首状态升序
	stack< set<string> > st;
	map< set<string>, int > m;//用于编号
	set< set<string> > allGroup;//保存每个阶段的所有分组
	st.push(finalSet);
	st.push(s1);
	allGroup.insert(finalSet);
	allGroup.insert(s1);
	int id = 0;
	int loopTimes = 0;
	cout << "初始未划分的组为：" << endl;
	FAUtil::printGroupStack(st);

	while (!st.empty()) {
		cout << "***************************第" << loopTimes++ << "次迭代*******************************" << endl;
		// 取出一个分组
		set<string> group = st.top();
		st.pop();
		cout << "进行划分的组为: ";
		for (set<string>::iterator sit = group.begin(); sit != group.end(); sit++) {
			cout << *sit;
		}
		cout << endl;
		//组内只有一个元素,已划分好
		if (group.size() == 1) {
			cout << "组内只有一个元素，不可划分" << endl;
			allGroup.insert(group);
			m.insert(make_pair(group, id++));
		}
		else {
			// 划分子组，当前每次划分2组，后期可替换函数，实现每次划分N组
			vector< set<string> > groups = divideTwoGroup(group, allGroup);
			if (groups.size() == 1) {// 未分组,即当前组不可划分
				cout << "当前组不可划分" << endl;
				m.insert(make_pair(group, id++));// 直接加入
			}
			else {// 可划分,重新加入栈中，等待下次划分
				cout << "当前组可划分，新划分的组为如下：大小为" << groups.size() << endl;
				// 现有分组集合：删除当前分组，加入新分组
				allGroup.erase(group);
				for (vector< set<string> >::iterator it = groups.begin(); it != groups.end(); it++) {
					st.push(*it);
					allGroup.insert(*it);
				}
				for (vector< set<string> >::iterator it = groups.begin(); it != groups.end(); it++) {
					for (set<string>::iterator sit = it->begin(); sit != it->end(); sit++) {
						cout << *sit;
					}
					cout << " ";
				}
				cout << endl;
			}
		}
		
		 cout << "未划分的组: " << endl;
		 FAUtil::printGroupStack(st);
		 cout << "已划分的组:" << endl;
		 for (map< set<string>, int >::iterator it = m.begin(); it != m.end(); it++) {
			 cout << "{";
			 for (set<string>::iterator sit = it->first.begin(); sit != it->first.end(); sit++) {
				 cout << *sit << ",";
			 }
			 cout <<"}" << endl;
		 }
	}
	id = 0;
	cout << "划分后的所有分组:" << endl;
	for (set< set<string> >::iterator it = allGroup.begin(); it != allGroup.end(); it++) {
		m[*it] = id++;
	}
	cout << "状态\t\tID" << endl;
	for (map< set<string>, int >::iterator it = m.begin(); it != m.end(); it++) {
		cout << "{";
		for (set<string>::iterator sit = it->first.begin(); sit != it->first.end(); sit++) {
			cout << *sit << ",";
		}
		cout << "}\t\t" << it->second << endl;
	}
	// 重新编号,化简DFA
	map<DFAKey, string> newMapTable;
	// 遍历字符
	for (set<char>::iterator it1 = inputLetter.begin(); it1 != inputLetter.end(); it1++) {
		// 遍历新生成的状态集
		for (set< set<string> >::iterator it2 = allGroup.begin(); it2 != allGroup.end(); it2++) {
			set<string> newStatusSet;
			// 获取该状态集可转换的状态的集合
			for (set<string>::iterator it3 = it2->begin(); it3 != it2->end(); it3++) {
				map<DFAKey, string>::iterator ret = mapTable.find({ *it3,*it1 });
				if (ret != mapTable.end()) {
					newStatusSet.insert(ret->second);
				}
			}
			if (!newStatusSet.empty()) {// 获取的状态集合不为空，即有可能终结符不接受字符，则不考虑
				DFAKey key( to_string(m[*it2]), *it1);
				string newState;
				for (set<set<string> >::iterator it = allGroup.begin(); it != allGroup.end(); it++) {
					if (FAUtil::isInclude(*it, newStatusSet)) {
						newState = to_string(m[*it]);
					}
				}
				newMapTable.insert({ key, newState });
			}
		}
	}
	this->mapTable = newMapTable;
	//重新获取开始符,终结符集合，状态集合
	bool isFind = false;
	set<string> newFinalSet;
	set<string> newStatusSet;
	for (set<set<string> >::iterator it = allGroup.begin(); it != allGroup.end(); it++) {
		if (!isFind) {//开始符只有一个
			if (FAUtil::isInclude(*it, set<string>{start})) {
				start = to_string(m[*it]);
			}
		}
		for (set<string>::iterator it2 = it->begin(); it2 != it->end(); it2++) {
			if (finalSet.find(*it2 )!= finalSet.end()) {//该状态集合包含终止状态
				newFinalSet.insert(to_string(m[*it]));
			}
		}
		newStatusSet.insert(to_string(m[*it]));
	}
	finalSet = newFinalSet;
	statusSet = newStatusSet;
	cout << "***************************简化后的DFA如下：**********************" << endl;
	printDFA();
}
void DFA::fwriteDFA(string filePath) {
	FILE* fp;
	errno_t err = fopen_s(&fp, filePath.c_str(), "w");
	if (fp == 0) {
		cout << "文件打开失败!" << endl;
		return;
	}
	fprintf(fp, "%d\n", (int)mapTable.size());
	for (map<DFAKey, string>::iterator it = mapTable.begin(); it != mapTable.end(); it++) {
		fprintf(fp, "%s %c %s\n",it->first.ver.c_str(), it->first.edge, it->second.c_str());
	}
	fprintf(fp, "%s\n", start.c_str());
	fprintf(fp, "%d\n", (int)finalSet.size());
	for (set<string>::iterator it = finalSet.begin(); it != finalSet.end(); it++) {
		fprintf(fp, "%s\n", it->c_str());
	}
	fclose(fp);
}
/// <summary>
/// 判断s1是否包含s2,利用s2-s1的差集是否为空
/// </summary>
/// <param name="s1"></param>
/// <param name="s2"></param>
/// <returns></returns>
bool FAUtil::isInclude(set<string> s1, set<string> s2) {
	set<string> s3;
	set_difference(s2.begin(), s2.end(), s1.begin(), s1.end(), inserter(s3, s3.begin()));
	if (s3.size() == 0) {
		return true;
	}
	return false;

}
void FAUtil::printGroupStack(stack<set<string>> st){
	while (!st.empty()) {
		set<string> top = st.top();
		st.pop();
		cout << "{";
		for (set<string>::iterator it = top.begin(); it != top.end(); it++) {
			cout << *it << ",";
		}
		cout << "}" << endl;
	}
}

/// <summary>
/// 每次划分两组，原理：对当前组Group1的两个状态S1,S2，存在 <S1,a>=T1, <S2,a>=T2,且T1,T2状态处于不同的组中
/// 则说明S1,S2不等价，即可以将划分成两组（与S1等价，与S1不等价）
/// 如果T1处于组Group2中,则有下面划分
/// 组1（与S1等价）：Group1中的所有状态遇到a后转换为的状态在Group2中
/// 组2（与S1不等价）：
/// </summary>
/// <param name="group"></param>
/// <returns>size==1,则不可分，否则可分</returns>
vector< set<string> > DFA::divideTwoGroup(set<string> group, set< set<string> >& allGroup) {
	vector< set<string> > groups;// 返回的组
	bool isDiv = false;//最开始默认不可分，即如果该状态集合不接受任何字符时，状态不可分
	char c;
	set<string> divSet;// 保存S1的组
	for (set<char>::iterator cit = inputLetter.begin(); cit != inputLetter.end(); cit++) {
		// 获取当前组遇到某个字符所转换的状态的集合
		set<string> sslc = getDFAStatusSetLetterClosure(group, *cit);
		if (sslc.size() == 0) {// 集合内不存在能接受字符的状态,说明不可分
			isDiv = false;
			continue;
		}
		// 判断是否可分，默认可分，直到遇到一个现有的状态集包括当前 转换得到的状态集 时才不可分
		isDiv = true;
		c = *cit;
		for (set< set<string> >::iterator it = allGroup.begin(); it != allGroup.end(); it++) {
			if (FAUtil::isInclude(*it, sslc)) {// 存在一个分组，包含当前状态,即当前状态不可分
				isDiv = false;
				break;
			}
		}
		cout << "遇到字符 " << c << " 所转换的状态集合为: {";
		for (set<string>::iterator sit = sslc.begin(); sit != sslc.end(); sit++) {
			cout << *sit << ",";
		}
		cout << "}" << endl;

		if (isDiv) {// 当前字符转换的状态集合可分
			string state = *sslc.begin();//默认第一个状态是S1，则state为T1
			set<string> stateSet{ state };
			for (set< set<string> >::iterator it = allGroup.begin(); it != allGroup.end(); it++) {// 找到T1所在的组
				if (FAUtil::isInclude(*it, stateSet)) {
					divSet = *it;
					break;
				}
			}
			break;
		}
	}
	if (isDiv) {//可分
		set<string> newGroup1;// 划分出当前组的所有状态,与S1等价的
		for (set<string>::iterator it = group.begin(); it != group.end(); it++) {
			if (divSet.find(mapTable[{*it, c}]) != divSet.end()) {//S1等价
				newGroup1.insert(*it);
			}
		}



		set<string> newGroup2;//与S1不等价
		set_difference(group.begin(), group.end(), newGroup1.begin(), newGroup1.end(), inserter(newGroup2, newGroup2.begin()) );
		return vector< set<string> >{ newGroup1, newGroup2 };
	}
	else {
		return vector< set<string> >{group};
	}
}

set<string> DFA::getDFAStatusSetLetterClosure(set<string>& statusSet, char letter) {
	set<string> newStatusSet;
	DFAKey key;
	key.edge = letter;
	for (set<string>::iterator it = statusSet.begin(); it != statusSet.end(); it++) {
		key.ver = *it;
		map<DFAKey, string>::iterator ret = mapTable.find(key);
		if (ret != mapTable.end()) {
			newStatusSet.insert(ret->second);
		}
	}
	return newStatusSet;
}
DFA NFAtoDFA::toDFA( ) {
	DFA dfa;
	dfa.inputLetter = inputLetter;
	//开始
	dfa.start = statusID[start];
	// 状态
	for (set< NFASTATUS >::iterator it = statusSet.begin(); it != statusSet.end(); it++) {
		dfa.statusSet.insert(statusID[*it]);
	}
	//映射
	for (map<NFAtoDFAKey, NFASTATUS>::iterator it = mapTable.begin(); it != mapTable.end(); it++) {
		DFAKey key(statusID[it->first.NFAStatus], it->first.input);
		dfa.mapTable.insert(make_pair(key, statusID[it->second]));
	}
	//终止状态
	for (set< NFASTATUS >::iterator it = finalSet.begin(); it != finalSet.end(); it++) {
		dfa.finalSet.insert(statusID[*it]);
	}
	return dfa;
}

NFAtoDFA FAUtil::equivalentReplacement(NFA& nfa) {
	// 初始化队列,访问标记表
	queue<NFASTATUS> q;
	set<NFASTATUS> vis;
	// 初始化NFAtoDFA的开始状态集和状态集
	int cnt = 0;
	NFAtoDFA ntd;
	ntd.start = nfa.start;
	ntd.statusSet.insert(nfa.start);
	ntd.statusID[nfa.start] = to_string(cnt++);
	ntd.inputLetter = nfa.inputLetter;
	ntd.inputLetter.erase(CHAR_NULL);
	ntd.stateID.push_back(nfa.start);
	q.push(nfa.start);
	vis.insert(nfa.start);
	// 寻找新的状态
	while (!q.empty()) {
		// 取出某个状态集合
		NFASTATUS vertex = q.front();
		q.pop();
		// 遍历当前状态集合遇到 所有输入字符的< 状态集合, 输入字符 >的映射,即得到一行数据
		// 遍历输入字符
		for (set<char>::iterator cit = nfa.inputLetter.begin(); cit != nfa.inputLetter.end(); cit++) {
			set<string> newStatusSet = set<string>();
			// 将状态集合中状态取出，获取所有的 <状态, 输入字符>的映射,即得到一个映射关系
			for (set<string>::iterator sit = vertex.begin(); sit != vertex.end(); sit++) {
				DFAKey key(*sit, *cit);
				newStatusSet.insert(nfa.letterClosure[key].begin(), nfa.letterClosure[key].end());
			}
			if (newStatusSet.size() == 0) {// 该状态集合字符无法到达新的状态集合
				continue;
			}
			NFAtoDFAKey ntdkey(vertex, *cit);
			// 新的<状态集合,输入字符>
			ntd.mapTable.insert(make_pair(ntdkey, newStatusSet));
			if (vis.find(newStatusSet) == vis.end()) {// 新状态
				bool isFinal = false;
				// 判断是否非终止状态
				for (set<string>::iterator fit = nfa.finalSet.begin(); fit != nfa.finalSet.end(); fit++) {
					if (newStatusSet.find(*fit) != newStatusSet.end()) {// 该状态为终止状态
						isFinal = true;
						break;
					}
				}
				if (isFinal) {// 将入终止状态集合
					ntd.finalSet.insert(newStatusSet);
				}
				//加入队列
				q.push(newStatusSet);
				//加入访问标记
				vis.insert(newStatusSet);
				//新状态赋予编号
				ntd.statusID[newStatusSet] = to_string(cnt++);
				ntd.stateID.push_back(newStatusSet);
				//加入到状态的集合
				ntd.statusSet.insert(newStatusSet);
			}
		}
	}
	for (set<NFASTATUS>::iterator it = ntd.statusSet.begin(); it != ntd.statusSet.end(); it++) {
		bool isFinal = false;
		for (set<string>::iterator it2 = it->begin(); it2 != it->end(); it2++) {
			for (set<string>::iterator it3 = nfa.finalSet.begin(); it3 != nfa.finalSet.end(); it3++) {
				if (*it2 == *it3) {
					isFinal = true;
					break;
				}
			}
			if (isFinal) {
				break;
			}
		}
		if (isFinal) {
			ntd.finalSet.insert(*it);
		}
	}
	return ntd;
}

void NFAtoDFA::renumber() {
	
}

void NFA::eraseNullDependent(vector<RecordClosure>&vrc) {
	for (int i = 0; i < vrc.size(); i++) {
		if (vrc[i].dependent.size() == 0) {//可直接求出空闭包
			nullClosure.insert(make_pair(vrc[i].symbol, vrc[i].closure));
			vrc.erase(vrc.begin() + i);
			i--;
		}
	}
};
void NFA::replaceDependent(vector<RecordClosure>& vrc) {
	for (int i = 0; i < vrc.size(); i++) {
		set<string> st = vrc[i].dependent;
		for (set<string>::iterator it = vrc[i].dependent.begin(); it != vrc[i].dependent.end(); it++) {
			map<string, set<string>>::iterator ret = nullClosure.find(*it);
			if (ret != nullClosure.end()) {//替换
				vrc[i].closure.insert(ret->second.begin(), ret->second.end());
				st.erase(*it);
			}
		}
		vrc[i].dependent = st;
	}
}
void NFA::printNullClosure() {
	cout << "symbol\t@_closure\n";
	cout << "_______________________________________" << endl;
	for (map<string, set<string> >::iterator it = nullClosure.begin(); it != nullClosure.end(); it++) {
		cout << it->first << "\t|";
		for (set<string>::iterator sit = it->second.begin(); sit != it->second.end(); sit++) {
			cout << *sit << ",";
		}
		cout <<endl << "_______________________________________" << endl;
	}
};

void NFA::printLetterClosure() {
	for (set<char>::iterator it2 = inputLetter.begin(); it2 != inputLetter.end(); it2++) {
		cout << "\t|" << *it2;
	}
	cout << endl << "____________________________________" << endl;
	for (set<string>::iterator it1 = statusSet.begin(); it1 != statusSet.end(); it1++) {
		cout << *it1 << "\t|";
		for (set<char>::iterator it2 = inputLetter.begin(); it2 != inputLetter.end(); it2++) {
			if (*it2 == CHAR_NULL) {
				continue;
			}
			map<DFAKey, set<string> >::iterator ret = letterClosure.find(DFAKey(*it1, *it2));
			if (ret != letterClosure.end()) {
				for (set<string>::iterator sit = ret->second.begin(); sit != ret->second.end(); sit++) {
					cout << *sit << ",";
				}
			}
			else {
				cout << "*";
			}
			cout << "\t|";
		}
		cout << endl << "____________________________________" << endl;
	}
}

void NFA::initLetterClosure() {
	int cnt = 0;
	for (map< DFAKey, set<string> >::iterator it = mapTable.begin(); it != mapTable.end(); it++) {
		letterClosure.insert(make_pair(it->first, set<string>()));
		for (set<string>::iterator sit = it->second.begin(); sit != it->second.end(); sit++) {
			letterClosure[it->first].insert(nullClosure[*sit].begin(), nullClosure[*sit].end());
		}
	}
}
void NFAtoDFA::printMapTable() {
	for (set<char>::iterator it2 = inputLetter.begin(); it2 != inputLetter.end(); it2++) {
		cout << "\t" << *it2 << "\t";
	}
	cout << endl << "___________________________________" << endl;
	for (vector<NFASTATUS>::iterator it1 = stateID.begin(); it1 != stateID.end(); it1++) {
		printNFAStatus(*it1);
		cout << "\t|";
		for (set<char>::iterator it2 = inputLetter.begin(); it2 != inputLetter.end(); it2++) {
			NFAtoDFAKey key(*it1, *it2);
			map<NFAtoDFAKey, NFASTATUS>::iterator ret = mapTable.find(key);
			if (ret != mapTable.end()) {
				printNFAStatus(ret->second);
			}
			else {
				cout << "*";
			}
			cout << "\t\t|";
		}
		cout << endl;
	}
}
void NFAtoDFA::printNFAtoDFA() {
	cout << "状态集合(" << statusSet.size()<< "): ";
	printStatusSet();

	for (set<NFASTATUS>::iterator sit = statusSet.begin(); sit != statusSet.end(); sit++) {
		if (*sit == NFASTATUS()) {
			cout << "存在空状态" << endl;
			break;
		}
	}

	cout << "开始状态(" << start.size() << "): ";
	printStart();
	cout << "终止状态集合(" << finalSet.size()<< ")：";
	printFinalSet();
	cout << "映射集合：" << endl;
	printMapTable();
}
void NFAtoDFA::printStatusSet() {
	cout << "{";
	for (set<NFASTATUS>::iterator sit = statusSet.begin(); sit != statusSet.end(); sit++) {
		printNFAStatus(*sit);
		cout << ",";
	}
	cout << "}" << endl;
}
void NFA::fwriteNFA(string filePath){
	FILE* fp;
	errno_t err = fopen_s(&fp, filePath.c_str(), "w");
	if (fp == 0) {
		cout << "文件打开失败!" << endl;
		return;
	}
	int size = 0;
	for (map< DFAKey, set<string> >::iterator it1 = mapTable.begin(); it1 != mapTable.end(); it1++) {
		size += (int)it1->second.size();
	}
	fprintf(fp, "%d\n", size);
	for (map< DFAKey, set<string> >::iterator it1 = mapTable.begin(); it1 != mapTable.end(); it1++) {
		for (set<string>::iterator it2 = it1->second.begin(); it2 != it1->second.end(); it2++) {
			fprintf(fp, "%s %c %s\n", it1->first.ver.c_str(), it1->first.edge, it2->c_str());
		}
	}
	fprintf(fp, "%d\n", (int)start.size());
	for (set<string>::iterator it = start.begin(); it != start.end(); it++) {
		fprintf(fp, "%s\n", it->c_str());
	}
	fprintf(fp, "%d\n", (int)finalSet.size());
	for (set<string>::iterator it = finalSet.begin(); it != finalSet.end(); it++) {
		fprintf(fp, "%s\n", it->c_str());
	}
	fclose(fp);
}
void NFAtoDFA::printStart() {
	cout << "{";
	for (NFASTATUS::iterator sit = start.begin(); sit != start.end(); sit++) {
		cout << *sit << ",";
	}
	cout << "}" << endl;
}
void NFA::eraseNullClosure() {
	for (set<string>::iterator it1 = statusSet.begin(); it1 != statusSet.end(); it1++) {
		bool isDel = true;
		for (set<char>::iterator it2 = inputLetter.begin(); it2 != inputLetter.end(); it2++) {
			DFAKey key(*it1, *it2);
			if (letterClosure.find(key) != letterClosure.end()) {
				isDel = false;
				break;
			}
		}
		if (isDel) {

		}
	}
}
void NFAtoDFA::printFinalSet() {
	cout << "{ ";
	for (set<NFASTATUS>::iterator fit = finalSet.begin(); fit != finalSet.end(); fit++) {
		printNFAStatus(*fit) ;
		cout << ",";
	}
	cout << "}" << endl;
}
void NFAtoDFA::printNFAStatus( const NFASTATUS& sta) {
	//状态
	for (set<string>::iterator it = sta.begin(); it != sta.end(); it++) {
		cout << *it;
	}
}

NFA::NFA(set<string> _statusSet, set<char> _inputLetter, map< DFAKey, set<string> > _mapTable, set<string> _start, set<string> _finalSet) {
	statusSet = _statusSet;
	inputLetter = _inputLetter;
	mapTable = _mapTable;
	start = _start;
	finalSet = _finalSet;
	nullClosure.clear();
	letterClosure.clear();
}
DFA::DFA(set<string> _statusSet, set<char> _inputLetter, map<DFAKey, string> _mapTable, string _start, set<string> _finalSet) {
	statusSet = _statusSet;
	inputLetter = _inputLetter;
	mapTable.insert(_mapTable.begin(), _mapTable.end());
	start = _start;
	finalSet = _finalSet;
}

bool DFAKey::operator<(DFAKey in) const {
	if (ver == in.ver) {
		return edge < in.edge;
	}
	return ver < in.ver;
}
bool RecordClosure::operator<(RecordClosure rc) const {
	if (symbol != rc.symbol) {
		return symbol < rc.symbol;
	}
	else if (dependent != rc.dependent) {
		return dependent < rc.dependent;
	}
	return closure < rc.closure;
}
RecordClosure::RecordClosure(string _symbol, set<string> _dependent, set<string> _closure) {
	symbol = _symbol;
	dependent = _dependent;
	closure = _closure;
}



