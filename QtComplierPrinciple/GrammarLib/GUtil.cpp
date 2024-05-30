#include "GUtil.h"


void PUTIL::printGrammar(Grammar& grammar) {
	cout << "规则集：" << endl;
	for (int i = 0; i < grammar.productions.size(); i++) {
		cout << grammar.productions[i].id << " " << grammar.productions[i].left << "::=";
		for (vector<string>::iterator it = grammar.productions[i].right.begin(); it != grammar.productions[i].right.end(); it++) {
			cout << *it;
			if (it + 1 != grammar.productions[i].right.end()) {
				cout << "|";
			}
		}
		if (grammar.productions[i].have_null == 1) {
			if (grammar.productions[i].right.size() >= 1) {
				cout << "|";
			}
			cout << STRING_NULL;
		}
		cout << endl;
	}
	cout << "开始符：{" << grammar.start << ",}" << endl;
	cout << "非终结符集合：{";
	for (set<string>::iterator it = grammar.nons.begin(); it != grammar.nons.end(); it++) {
		cout << *it << ",";
	}
	cout << "}\n";
	cout << "终结符集合：{";
	for (set<char>::iterator it = grammar.terms.begin(); it != grammar.terms.end(); it++) {
		cout << *it << ",";
	}
	cout << "}" << endl;
}
// 覆盖式写入文件中 
void PUTIL::fwriteGrammar(vector<Production>& vp,string filePath) {
	FILE* fp;
	errno_t err;
	err = fopen_s(&fp, filePath.c_str(), "a");
	if (fp == 0) {
		cout << "文件打开失败!" << endl;
		return;
	}
	for (int i = 0; i < vp.size(); i++) {
		fprintf(fp, "%s::=", vp[i].left.c_str());
		for (vector<string>::iterator it = vp[i].right.begin(); it != vp[i].right.end(); it++) {
			fprintf(fp, "%s", it->c_str());
			if (it + 1 != vp[i].right.end()) {
				fprintf(fp, "|");
			}
		}
		fprintf(fp, "\n");
	}
	fprintf(fp, "\n");
	fclose(fp);
}
// 从文件中读取产生式 
//void PUTIL::readGrammar( vector< Grammar >& grammars, string fileName ){
//
//	char line[200];
//	FILE* fp;
//	errno_t err = fopen_s( &fp, fileName.c_str(), "r" );
//	int row = -1;
//	//vector<Production> vp;
//	Grammar g;
//	string start;//开始符
//	vector<Production> productions;// P：产生式
//	set<char> terms;//终结符集合
//	set<string> nons;//非终结符集合
//	if (fp == 0) {
//		cout << "文件打开失败!" << endl;
//		return;
//	}
//	while( fgets( line, sizeof(line),fp ) ){// 每次读取一行 
//		Production pro;// 某个终结符的产生式 
//		row++;
//		pro.id = row;
//		string str = line;
//		if( str.find_last_of('\n') != -1 )	{// 去除换行符 
//			str.erase( --str.end() );
//		}
//		string left = "";
//		int pos = str.find( "::=" );
//		// 获取左边,可获取E' 
//		if( pos == -1 ){ // 说明是新的文法
//			if( !productions.empty() ) {
//				g = Grammar( productions[0].left, productions, nons, terms);
//				start = "";
//				productions = vector<Production>();
//				terms = set<char>();
//				nons = set<string>();
//				grammars.push_back(g);
//				row = -1; 
//			}
//			continue;
//		} else {
//			for( int i = 0; i < pos; i++ ){
//				left += str[i];
//			}
//		}
//		pro.left = left;	
//		nons.insert(left);
//		// 获取右边 
//		string right = "";
//		for( int i = pos+3; i < str.size(); i++ ){
//			if( str[i] == '|' ){
//				pro.right.push_back( right );
//				right = "";
//			} else if( str[i] == CHAR_NULL ){
//				pro.have_null = 1;
//			} else {
//				right += str[i];
//				if (!((str[i] >= 'A' && str[i] <= 'Z') || str[i - 1] >= 'A' && str[i - 1] <= 'Z' && str[i] == DYH)) {
//					terms.insert(str[i]);
//				}
//			}
//		}
//		if( right != "" ){
//			pro.right.push_back( right );
//		}
//		vector<Production>::iterator repeat = findProductionByLeft( productions, pro.left );
//		if( repeat != productions.end() ){// 重复的左边合并
//			for( vector<string>::iterator sit = pro.right.begin(); sit != pro.right.end(); sit++ ){
//				repeat->right.push_back( *sit );
//			}
//			if( pro.have_null == 1 ){
//				repeat->have_null = 1;
//			}
//		} else {
//			productions.push_back( pro );
//		}
//	}
//	g = Grammar(productions[0].left, productions, nons, terms);
//	if( !g.productions.empty() ) {
//		grammars.push_back(g);
//	}
//}

// 从文件中读取产生式 
void PUTIL::readGrammar(vector< Grammar >& grammars, string fileName) {

	char line[200];
	FILE* fp;
	errno_t err = fopen_s(&fp, fileName.c_str(), "r");
	int row = -1;
	//vector<Production> vp;
	Grammar g;
	string start;//开始符
	vector<Production> productions;// P：产生式
	set<char> terms;//终结符集合
	set<string> nons;//非终结符集合
	if (fp == 0) {
		cout << "文件打开失败!" << endl;
		return;
	}
	while (fgets(line, sizeof(line), fp)) {// 每次读取一行 
		Production pro;// 某个终结符的产生式 
		row++;
		pro.id = row;
		string str = line;
		if (str.find_last_of('\n') != -1) {// 去除换行符 
			str.erase(--str.end());
		}
		string left = "";
		int pos = (int)str.find("::=");
		// 获取左边,可获取E' 
		if (pos == -1) { // 说明是新的文法
			if (!productions.empty()) {
				g = Grammar(productions[0].left, productions, nons, terms);
				start = "";
				productions = vector<Production>();
				terms = set<char>();
				nons = set<string>();
				grammars.push_back(g);
				row = -1;
			}
			continue;
		}
		else {
			for (int i = 0; i < pos; i++) {
				left += str[i];
			}
		}
		pro.left = left;
		nons.insert(left);
		// 获取右边 
		string right = "";
		for (int i = pos + 3; i < str.size(); i++) {
			if (str[i] == '|') {
				pro.right.push_back(right);
				right = "";
			}
			else if (str[i] == CHAR_NULL) {
				pro.have_null = 1;
			}
			else {
				right += str[i];
				if (!((str[i] >= 'A' && str[i] <= 'Z') || str[i - 1] >= 'A' && str[i - 1] <= 'Z' && str[i] == DYH)) {
					terms.insert(str[i]);
				}
			}
		}
		if (right != "") {
			pro.right.push_back(right);
		}
		vector<Production>::iterator repeat = findProductionByLeft(productions, pro.left);
		if (repeat != productions.end()) {// 重复的左边合并
			for (vector<string>::iterator sit = pro.right.begin(); sit != pro.right.end(); sit++) {
				repeat->right.push_back(*sit);
			}
			if (pro.have_null == 1) {
				repeat->have_null = 1;
			}
		}
		else {
			productions.push_back(pro);
		}
	}
	terms.erase(' ');
	g = Grammar(productions[0].left, productions, nons, terms);
	if (!g.productions.empty()) {
		grammars.push_back(g);
	}

	fclose(fp);
}

// 打印含有空产生式的非终结符Non
void PUTIL::printNonHaveNull(vector<Production>& vp) {
	cout << "含有空产生式的非终结符如下：" << endl;
	for (int i = 0; i < vp.size(); i++) {
		if (vp[i].have_null == 1) {
			cout << vp[i].left << " ";
		}
	}
	cout << endl;
}
/// @brief 建立非终结符的预测表，纵坐标为terms中的顺序，横坐标为vp中的left次序
	/// @param vp 
	/// @param non 
map<LL1Key,string>  PUTIL::getLL1Table(vector<ProductionFirst>& vpf,vector<Production>& vp, vector<string>& terms) {
	vector<RecordFirst> vrf;
	vector<SymbolFirst> vsf;
	//vector<ProductionFirst> vpf;
	cout << "********************获取FRIST表开始**************" << endl;
	FirstUtil::getFirst(vp, vrf, vsf, vpf);
	cout << "********************获取FRIST表结束**************" << endl;
	cout << "********************获取FOLLOW表开始**************" << endl;
	vector<SymbolFollow> vsfo = FollowUtil::getFollow(vp, vsf, vpf);
	cout << "********************获取FOLLOW表结束**************" << endl;
	cout << "非终结符的FIRST如下：" << endl;
	FirstUtil::printSymbolFirst(vsf);
	
	cout << "非终结符的FOLLOW如下：" << endl;
	FollowUtil::printSymbolFollow(vsfo);
	// 初始化语法预测表
	vector< vector<string> > LL1Table(vp.size(), vector<string>(terms.size()));
	// 构造产生式数组
	vector<string> productions = initProductions(vp);
	cout << "产生式：" << endl;
	for (int i = 0; i < productions.size(); i++) {
		cout << i << " " << productions[i] << endl;
	}


	///新增
	/*for (int i = 0; i < vpf.size() - 1; i++) {
		set<string> newFirst{};
		string pro = productions[vpf[i].proID];
		vector<string> vs = getRightsFromPro(pro);
		bool allNon = true;
		for (int j = 0; j < vs.size(); j++) {
			if (isNon(vs[j])) {
				int smIndex = findSymbolFirstByLeft(vsf, vs[j]);
				newFirst.insert(vsf[smIndex].firstSet.begin(), vsf[smIndex].firstSet.end());
				if (newFirst.find("@") == newFirst.end()) {
					allNon = false;
				}
			}
			else {
				allNon = false;
			}
			if (!allNon) {
				break;
			}
		}
		vpf[i].firstSet = newFirst;
	}*/


	cout << "产生式的FIRST：" << endl;
	FirstUtil::printGrammarFirst(vpf);

	
	///
	
	// 获取LL1Table分析预测表
	return initLL1Table( vp, productions, terms, vsf, vpf, vsfo);
}




map<LL1Key, string> PUTIL::getLL1TablePlus(GrammarPlus& gp){
	vector<RecordFirst> vrf;
	vector<SymbolFirst> vsf;
	vector<ProductionFirst> vpf;
	// 构造产生式数组
	FirstUtil::getFirst(gp.productions, vrf, vsf, vpf);
	cout << "********************获取FRIST表**************" << endl;
	// 构造产生式数组
	vector<SymbolFollow> vsfo = FollowUtil::getFollow(gp.productions, vsf, vpf);
	// 构造产生式数组
	cout << "非终结符的FIRST如下：" << endl;
	FirstUtil::printSymbolFirst(vsf);
	cout << "非终结符的FOLLOW如下：" << endl;
	FollowUtil::printSymbolFollow(vsfo);
	// 初始化语法预测表
	vector< vector<string> > LL1Table(gp.productions.size(), vector<string>(gp.terms.size()));
	// 构造产生式数组
	vector<string> productions = initProductions(gp.productions);
	cout << "产生式：" << endl;
	for (int i = 0; i < productions.size(); i++) {
		cout << i << " " << productions[i] << endl;
	}
	cout << "产生式的FIRST：" << endl;
	FirstUtil::printGrammarFirst(vpf);
	
	vector<string> terms;
	for (string s : gp.terms) {
		terms.push_back(s);
	}
	// 获取LL1Table分析预测表
	return initLL1Table(gp.productions, productions, terms, vsf, vpf, vsfo);
}
// 加标记法:需要去除的有如下 
// 1.直接左递归（E=E） 
// 2.无意义产生式（E=E+T,T=i,P=i）,去除P 
// 3.无法从右部含有全是终结符（S=i）反推到的产生式（E=E+T,T=i,P=G,G=P）,去除P,G 
// 所需数据结构
// Production：包含非终结符及其所有右部（E=E+T|T） 
// 队列：从开始符S推导出的非终结符全部加入队列，用于标记。
// 蕴含（映射）表：table[E][T]=1 表示E的右部有T。使用二维数组，也可multimap实现。
//	queue:  
// 所需功能模块：
// void eraseRepeatLeft( Production& p )
// 消除直接p的直接左递归 (E=E)
// bool addLabelForRight( Production p,queue q,map<string,bool>& vis )
// 将右部出现的非终结符加标记 
// 返回值：右部出现未标记的返回true,否则为false。 目前没有用途 
// 给出某个非终结符的产生式p(Z=E+T),将右部出现的产生式加入队列q,并标记为可推导出vis[E]=true
// vector<string> findLeftToTerm( vector<production>& vp )
// 找出vp中所有右部可以直接推出全为非终结符的left
// 找出 left -> term{term} 的所有left 

// bool addLabelForLeft( string left,queue q,map<string,bool> rvis, int** table )
// bool addLabelForLeft( string left,queue q,map<string,bool> rvis, int** table )
// 返回值：当有新的标记加入返回true即rvis[left][new]由0赋值为1出现 
// left的右部可推导出全为终结符，对于table[left][non]=1的，标记rvis[non]=true 

// vector<string> findLeftNotInVis( vector<production>& vp, map<string,bool> vis )
// 找出所有未被标记的lef并返回 

// void eraseLeftNotInVis( vector<production>& vp, map<string,bool> vis )

// 加标记法：
// 步骤：
// 1.初始化：将开始符S加入队列 q,标记S(vis[S]=1),
void PUTIL::labelMethod(vector<Production>& vp) {
	// 初始化数据结构 
	queue<string> q;
	map<string, bool> vis;
	initMap(vp, vis);
	// 第二步（删除不能推导出全为终结符的Non、由于Non被删除，导致某些产生式被删除） 
	// 采用的是一次扫描，不回溯，所以无法后面处理导致前面受影响 
	bool isLoop = false;
	// 如E=E+T|S+F|T,T=S,S=i,F=P。处理循序：E、T、S、F。第二步删除了F=P,此时导致前面的E=S+F需要删除。 
	// 第一步，从开始符推导出所有可以出现的
	// 1、初始化：将开始符加入队列并标记 
	q.push(vp[0].left);// 将开始符加入到队列 
	vis[vp[0].left] = true;
	// 从开始符推导 
	while (!q.empty()) {
		// 2、取出一个左边的非终结符 
		string left = q.front();
		q.pop();
		vector<Production>::iterator pro = findProductionByLeft(vp, left);
		// 3、消除自身的直接左递归
		eraseRepeatLeft(*pro);
		// 4、将该非终结符右边出现的<未标记过的非终结符>标记，并加入队列
		addLabelForRight(*pro, q, vis);
	}
	// 5、删除不能从开始推出的非终结符及其产生式
	eraseLeftNotInVis(vp, vis);
	// 6、删除由于删除直接左递归导致无产生式的非终结符
	eraserNullProduction(vp);
	// 第二步，从可以推出全为终结符的非终结符开始
	// 1、初始化：将直接能推导出全为终结符的left加入到队列,并标记 
	q = queue<string>();
	initMap(vp, vis);
	findLeftToTerm(vp, q, vis);// 可推导出全为终结符的加入队列 
	// 初始化蕴含表（E=E+T）则有table[T][E] = 1,且为非终结符重新编号。为了方便蕴含表的建立
	vector< vector<int> > table(vp.size(), vector<int>(vp.size()));
	initTable(vp, table);
	while (!q.empty()) {
		// 2、取出一个左边的非终结符left 
		string left = q.front();
		q.pop();
		Production pro = *findProductionByLeft(vp, left);
		// 3、将蕴含该非终结符left，且因此<可以推导出全终结符的非终结符>标记
		addLabelForLeft(vp, left, q, vis, table);
	}
	// 4、删除无法从推导出全为终结符的非终结符left
	eraseLeftNotInVis(vp, vis);// 该方法具有回溯的特性 
	// 5、删除右部存在已删除非终结符的产生式 
	isLoop = eraseNotExsitNon(vp);
	if (isLoop) {
		labelMethod(vp);
	}
	else {
		cout << "***********加标记法处理结束***************" << endl;
	}
}
// findProduction:按照left查找 
vector<Production>::iterator PUTIL::findProductionByLeft(vector<Production>& vp, Production& p) {
	return find(vp.begin(), vp.end(), p);
}
vector<Production>::iterator PUTIL::findProductionByLeft(vector<Production>& vp, string left) {
	Production p;
	p.left = left;
	return find(vp.begin(), vp.end(), p);
}

// 去除某个非终结符的 重复定义：E = E|EE 
void PUTIL::eraseRepeatLeft(Production& p) {
	for (int i = 0; i < p.right.size(); i++ ) {
		bool erase = false;
		if (p.right[i] == p.left) { // 左边标识符和右边产生式相同，无意义, 删除 
			erase = true;
		}
		else {
			int len = (int)p.left.size();
			int first = (int)p.right[i].find(p.left);// 相邻两个 left的第一个 
			int second;// 第二个 
			while (1) {
				if (first + len == p.right[i].size()) {// 字符串遍历完了，不存在第二个了，说明右部为 left重复，该项删除 
					erase = true;
					break;
				}
				second = (int)p.right[i].find(p.left, first + len);//第二个的位置
				if (second == -1) {// 第二个没找到,即不存在重复定义left， 该项不删除 
					break;
				}
				else {
					if (first + len == second) {// 第一个和第二个相邻，继续找下一个对
						first = second;
					} else {// 不是重复定义left，该项不删除 
						break;
					}
				}
			}
		}
		if (erase) {
			p.right.erase(i+p.right.begin());
			i--;
		}
	}
}
// 利用已有的left去替换right中的left 
bool PUTIL::swapLeftToRright(string& right, string& left, string& leftProduction) {
	bool isSwap = false;
	int pos = (int)right.find(left);
	while (pos != -1) {
		right.erase(pos, left.size());
		right.insert(pos, leftProduction);
		pos = (int)right.find(left, pos + left.size());
		isSwap = true;
	}
	return isSwap;
}
//  将右部出现的非终结符加标记 ，并加入队列 
void PUTIL::addLabelForRight(Production& p, queue<string>& q, map<string, bool>& vis) {
	for (vector<string>::iterator it = p.right.begin(); it != p.right.end(); it++) {
		vector<string> rights = getRightsFromPro(*it);
		for (int i = 0; i < rights.size(); i++) {
			if (isNon(rights[i])) {
				if (!vis[rights[i]]) {
					q.push(rights[i]);
					vis[rights[i]] = true;
				}
			}
			
		}
		/*string str = *it;
		for (int i = 0; i < str.size(); i++) {
			if (str[i] >= 'A' && str[i] <= 'Z' && str[i] != p.left[0]) {
				string t = "";
				t += str[i];
				if (!vis[t]) {
					q.push(t);
					vis[t] = true;
				}

			}
		}*/
	}
}
//	找出vp中所有右部可以直接推出全为非终结符的left
//	找出 left -> term{term} 的所有left 	
bool PUTIL::findLeftToTerm(vector<Production>& vp, queue<string>& q, map<string, bool>& rvis) {
	for (int i = 0; i < vp.size(); i++) {
		bool terms=true;
		for (int j = 0; j < vp[i].right.size(); j++) {
			terms = true;
			string right = vp[i].right[j];
			for (int k = 0; k < right.size(); k++) {
				if (right[k] >= 'A' && right[k] <= 'Z') {
					terms = false;
					break;
				}
			}
			if (terms) {
				break;
			}
		}
		if (terms) {
			q.push(vp[i].left);
			rvis[vp[i].left] = true;
		}
	}
	return true;
}
//	返回值：当有新的标记加入返回true即rvis[left][new]由0赋值为1出现 
//  left的右部可推导出全为终结符，对于table[left][non]=1的，标记rvis[non]=true 	
bool PUTIL::addLabelForLeft(vector<Production>& vp, string termRight, queue<string>& q, map<string, bool>& rvis, vector< vector<int> >& table) {
	int pos = -1;
	while (vp[++pos].left != termRight) {}// 找到termRight在vp中的位置
	for (int i = 0; i < vp.size(); i++) {
		if (table[pos][i] == 1 && !rvis[vp[i].left]) {// 由left可以反推到的非终结符 
			Production pro = vp[i];
			bool can = true;
			for (int j = 0; j < pro.right.size(); j++) {
				string right = pro.right[j];
				can = true;
				if (right.find(termRight) != -1) {// right中含有termRight 
					for (int k = 0; k < right.size(); k++) {
						string t = "";
						t += right[k];
						if (right[k] >= 'A' && right[k] <= 'Z' && rvis[t] == false) {// 出现了非终结符，且该非终结符不推出全为终结符 
							can = false;
							break;
						}
					}
					if (can) {
						break;
					}
				}
				else {
					can = false;
				}
			}
			if (can) {
				rvis[pro.left] = true;
				q.push(pro.left);
			}
		}
	}
	return true;
}
//0 Z::=E+T
//1 E::=S+F|T
//2 F::=FP|P
//3 P::=G
//4 G::=F
//5 T::=T*i|i
//6 S::=i
// 标记：S、T 
// 队列： 
// 第一步 把右部所有的S带入S=i。此时遍历存在能推导出S的非终结符 
// 找出所有未被标记的left并返回 	
vector<string> PUTIL::findLeftNotInVis(vector<Production>& vp, map<string, bool>& vis) {
	return vector<string>();
}


// 删除未被标记的产生式 
void PUTIL::eraseLeftNotInVis(vector<Production>& vp, map<string, bool>& vis) {
	for (int i = 0; i < vp.size(); i++) {
		if (!vis[vp[i].left]) {
			vp.erase(vp.begin() + i);
			i--;
		}
	}
}
// 初始化map标记 
void PUTIL::initMap(vector<Production>& vp, map<string, bool>& vis) {
	vis.clear();
	for (int i = 0; i < vp.size(); i++) {
		vis.insert(make_pair(vp[i].left, false));
	}

}
// 删除空产生式 
void PUTIL::eraserNullProduction(vector<Production>& vp) {
	for (int i = 0; i < vp.size(); i++) {
		if (vp[i].right.size() == 0) {
			vp.erase(vp.begin() + i);
			i--;
		}
	}
}
// 删除右部出现不存在的非终结符的产生式(E=E+P，不存在P删除E),删除了返回true 
// 有删除的返回true 
bool PUTIL::eraseNotExsitNon(vector<Production>& vp) {

	map<string, bool> m;
	for (int i = 0; i < vp.size(); i++) {// 标记所有出现在左边的非终结符 
		m.insert(make_pair(vp[i].left, true));
	}
	bool isLoop = false;//需要循环:某个非终结符的右部全被删除,该非终结符可能影响已扫描的 
	for (int i = 0; i < vp.size(); i++) {
		//			cout << i << endl;
		for (int j = 0; j < vp[i].right.size(); j++) {
			string pro = vp[i].right[j];
			for (int k = 0; k < pro.size(); k++) {
				string t = "";
				t += pro[k];
				if (pro[k] >= 'A' && pro[k] <= 'Z' && m[t] == false) {// 删除某个产生式 
					vp[i].right.erase(j + vp[i].right.begin());
					j--;
					isLoop = true;
					break;
				}
			}
		}
		if (vp[i].right.size() == 0) {// 当前非终结符需要删除
			vp.erase(vp.begin() + i);
			i--;
			//				 isLoop = true;// 再次循环 
		}
	}
	if (isLoop) {// 如果后面删除的影响前面，则重复执行 
		eraseNotExsitNon(vp);
	}

	return isLoop;
}
/// <summary>
/// 初始化标记表,表中每项为<非终结符，所在下标>
/// </summary>
/// <param name="vp">文法的所有产生式</param>
/// <param name="table">标记表</param>
void PUTIL::initTable(vector<Production>& vp, vector< vector<int> >& table) {
	for (int i = 0; i < vp.size(); i++) {
		vp[i].id = i;
	}
	for (int i = 0; i < vp.size(); i++) {
		string left = vp[i].left;
		for (int j = 0; j < vp[i].right.size(); j++) {
			string right = vp[i].right[j];
			for (int k = 0; k < right.size(); k++) {
				string t = "";
				if (right[k] >= 'A' && right[k] <= 'Z') {
					t += right[k];
					int id = (*findProductionByLeft(vp, t)).id;
					table[id][i] = 1;
				}
			}
		}
	}
}
/// <summary>
/// 打印标记表
/// </summary>
/// <param name="vp"></param>
/// <param name="vis"></param>
void PUTIL::print(vector<Production>& vp, map<string, bool>& vis) {
	for (int i = 0; i < vp.size(); i++) {
		cout << vp[i].left;
		if (vis[vp[i].left]) {
			cout << " true" << endl;
		}
		else {
			cout << " false" << endl;
		}
	}
}
/// <summary>
/// 打印标记表
/// </summary>
/// <param name="vp"></param>
/// <param name="table"></param>
void PUTIL::printTable(vector<Production>& vp, vector< vector<int> >& table) {
	cout << "\t";
	int len = (int)vp.size();
	for (int i = 0; i < len; i++) {
		cout << vp[i].left << "\t";
	}
	cout << "\n";
	for (int i = 0; i < len; i++) {
		cout << vp[i].left << "\t";
		for (int j = 0; j < len; j++) {
			cout << table[i][j] << "\t";
		}
		cout << endl;
	}
}
/// <summary>
/// 打印字符串队列
/// </summary>
/// <param name="q"></param>
void PUTIL::printQueue(queue<string> q) {
	while (!q.empty()) {
		cout << q.front() << endl;
		q.pop();
	}
}
/// <summary>
/// 判断是否为非终结符
/// </summary>
/// <param name="str">字符</param>
/// <returns>非终结符返回true，否则false</returns>
bool PUTIL::isNon(string str) {
	if (str[0] >= 'A' && str[0] <= 'Z') {
		return true;
	}
	return false;
}
bool PUTIL::isNon(char c) {
	if (c >= 'A' && c <= 'Z') {
		return true;
	}
	return false;
}
/// <summary>
/// 自顶向下分析
/// </summary>
/// <param name="src">待分析的字符串</param>
/// <param name="LL1Table">分析预测表</param>
/// <param name="vp">产生式表</param>
/// <param name="terms">终结符表</param>
void PUTIL::topToBottom(string src, map<LL1Key,string>& LL1, vector<Production>& vp, vector<string>& terms) {
	// 输入的字串以$结尾，且输入字母表中无$。
	src += "$";
	stack<string> s;
	int currentIndex = 0;
	// 首先加入开始符和字符串结束符EOS
	s.push("$");
	s.push(vp[0].left);
	cout << "输入句子为：" << src << endl << "\t\t";
	printStack(s);
	LL1Key key;
	string getStr = "";
	while (!s.empty() && currentIndex < src.size() ) {
		while (src[currentIndex] == '@') {
			currentIndex++;
		}
		// 取出最左的符号
		string top = s.top();
		s.pop();
		if (top == "$") {
			currentIndex++;
			break;
		}
		else if (top == "@") {
			
		}
		else if (isNon(top)) {// 非终结符
			string pro;
			key.input = src[currentIndex];
			key.non = top;
			// 查表
			if (LL1.find(key) != LL1.end()) {
				pro = LL1[key];
			}
			else {// 未查到，退出
				break;
			}
			// 将产生式加入栈
			vector<string> rights = getRightsFromPro(pro);
			for (int i = (int)rights.size() - 1; i >= 0; i--) {
				s.push(rights[i]);
			}
			// 打印中间信息
			cout << "分析表匹配：<" << top << "," << key.input << "> = " << pro << endl;
			cout << getStr << "\t\t";
			printStack(s);
		}
		else {
			getStr += src[currentIndex];
			cout << "字串匹配: " << src[currentIndex] << "\n" << getStr << "\t\t";
			if (top[0] == src[currentIndex]) {
				currentIndex++;
			}
			else {
				break;
			}
			printStack(s);
		}
	}
	if (currentIndex != src.size()) {
		cout << "推导出错,这不是一个句子!" << endl;
	}
	else {
		cout << "这是一个句子。" << endl;
	}
}

void PUTIL::topToBottomNoProcess(string src, map<LL1Key, string>& LL1, vector<Production>& vp, vector<string>& terms) {
	// 输入的字串以$结尾，且输入字母表中无$。
	src += "$";
	stack<string> s;
	int currentIndex = 0;
	// 首先加入开始符和字符串结束符EOS
	s.push("$");
	s.push(vp[0].left);
	cout << "输入句子为：" << src << endl;
	LL1Key key;
	string getStr = "";
	while (!s.empty() && currentIndex < src.size()) {
		while (src[currentIndex] == '@') {
			currentIndex++;
		}
		// 取出最左的符号
		string top = s.top();
		s.pop();
		if (top == "$") {
			currentIndex++;
			break;
		}
		else if (top == "@") {

		}
		else if (isNon(top)) {// 非终结符
			string pro;
			key.input = src[currentIndex];
			key.non = top;
			// 查表
			if (LL1.find(key) != LL1.end()) {
				pro = LL1[key];
			}
			else {// 未查到，退出
				break;
			}
			// 将产生式加入栈
			vector<string> rights = getRightsFromPro(pro);
			for (int i = (int)rights.size() - 1; i >= 0; i--) {
				s.push(rights[i]);
			}
		}
		else {
			getStr += src[currentIndex];
			if (top[0] == src[currentIndex]) {
				currentIndex++;
			}
			else {
				break;
			}
		}
	}
	if (currentIndex != src.size()) {
		cout << "推导出错,这不是一个句子!" << endl;
	}
	else {
		cout << "这是一个句子。" << endl;
	}
}
/// <summary>
/// 将某个产生式处理，返回按出现顺序排序的非终结符和终结符的vector
/// </summary>
/// <param name="pro">产生式</param>
/// <returns>字符数组</returns>
vector<string> PUTIL::getRightsFromPro(string pro) {
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
		else if( pro[i]== '$') {
			t += pro[i];
		}
		rights.push_back(t);
	}
	return rights;
}

/// <summary>
/// 初始化产生式数组，按文法中产生式出现的顺序存储
/// </summary>
/// <param name="vp">文法的所有产生式</param>
/// <returns>产生式数组</returns>
vector<string> PUTIL::initProductions(vector<Production>& vp) {
	vector<string> productions;
	for (int i = 0; i < vp.size(); i++) {
		//cout << "left : " << vp[i].left << endl;
		for (vector<string>::iterator it = vp[i].right.begin(); it != vp[i].right.end(); it++) {
			productions.push_back(*it);
		}
		if (vp[i].have_null == 1) {
			productions.push_back(STRING_NULL);
		}
	}
	return productions;
}
/// <summary>
/// 查询LL1表中某个非终结符所处的位置
/// </summary>
/// <param name="left">给定非终结符</param>
/// <param name="vp">用于求解LL1Table的产生式</param>
/// <returns></returns>
int PUTIL::findLeftIndexInLL1(string left, vector<Production>& vp) {
	for (int i = 0; i < vp.size(); i++) {
		if (vp[i].left == left) {
			return i;
		}
	}
	return (int)vp.size();
}
/// <summary>
/// 查询非终结符在SymbolFirst的位置
/// </summary>
/// <param name="vsf">SymbolFirst表</param>
/// <param name="left">给定Non</param>
/// <returns>返回所在下标</returns>
int PUTIL::findSymbolFirstByLeft(vector<SymbolFirst>& vsf, string left) {
	int i = 0;
	while (i < vsf.size()) {
		if (vsf[i].left == left) {
			break;
		}
		i++;
	}
	return i;
}
/// <summary>
/// 查找非终结符在SymbolFollow中的下标
/// </summary>
/// <param name="vsfo"></param>
/// <param name="left">给定Non</param>
/// <returns>返回下标，未找到返回vsfo.size()</returns>
int PUTIL::findSymbolFollowByLeft(vector<SymbolFollow>& vsfo, string left) {
	int i = 0;
	while (i < vsfo.size()) {
		if (vsfo[i].left == left) {
			break;
		}
		i++;
	}
	return i;
}
/// <summary>
/// 查询ProductionFirst中非终结符及First中某个终结符所在的下标
/// </summary>
/// <param name="left">给定非终结符</param>
/// <param name="term">给定终结符</param>
/// <param name="vpf"></param>
/// <returns>所在产生式的下标,与vector<string> productions,中下标相同</returns>
int PUTIL::findProID(string left, string term, vector<ProductionFirst>& vpf) {
	int i = 0;
	while (i < vpf.size()) {
		if (vpf[i].left == left && vpf[i].firstSet.find(term) != vpf[i].firstSet.end()) {// 匹配到FIRST
			break;
		}
		i++;
	}
	return vpf[i].proID;
}
/// <summary>
/// 初始化LL1Table表
/// </summary>
/// <param name="LL1Tabl">待生成的预测分析表，要求size大于等于vp.size()</param>
/// <param name="vp">文法的所有产生式,每个元素是一个二元组<非终结符,对应的所有产生式></param>
/// <param name="productions">文法的产生式，按文法中产生式的出现顺序排序</param>
/// <param name="terms">文法中出现的非终结符表</param>
/// <param name="vsf">SymbolFirst,非终结符的FIRST表</param>
/// <param name="vpf">产生式的First表</param>
/// <param name="vsfo">非终结符的Follow表</param>
map<LL1Key, string> PUTIL::initLL1Table( vector<Production>& vp, vector<string>& productions, vector<string>& terms, vector<SymbolFirst>& vsf, vector<ProductionFirst>& vpf, vector<SymbolFollow>& vsfo) {
	string strNull = "";
	string strEnd = "";
	strNull += CHAR_NULL;
	strEnd += EOS;

	map<LL1Key, string> LL1;
	for (int i = 0; i < vp.size(); i++) {
		string left = vp[i].left;
		//找到非终结符的FIRST
		int symbolFirstIndex = findSymbolFirstByLeft(vsf, left);
		for (string term : vsf[symbolFirstIndex].firstSet) {
			if (term == "@") {
				// FIRST存在虚产生式				
				// 找到FOLLOW
				int symbolFollowIndex = findSymbolFollowByLeft(vsfo, left);
				for (string item : vsfo[symbolFollowIndex].followSet) {
					LL1.insert({ LL1Key(left,item[0]),"@"});
				}
			} else {
				// FIRST匹配
				int proID = findProID(left, term, vpf);
				LL1.insert({ LL1Key(left,term[0]),productions[proID]});
			}
		}
	}
	/*for (int i = 0; i < vpf.size(); i++) {
		string head = vpf[i].left;
		string pro = productions[vpf[i].proID];
		for (string c : vpf[i].firstSet) {			
			if (c != "@") {
				LL1Key key;
				key.non = head;
				key.input = c[0];
				if (LL1.find(key) == LL1.end()) {
					LL1.insert({ key, pro });
				}
			}
			else {
				int foIndex = findSymbolFollowByLeft(vsfo, head);
				for (string cc : vsfo[foIndex].followSet) {
					LL1Key key;
					key.non = head;
					key.input = cc[0];
					if (LL1.find(key) == LL1.end()) {
						LL1.insert({ key, pro });
					}
				}
			}
		}
	}*/
	return LL1;
}
/// <summary>
/// 打印栈中的元素
/// </summary>
/// <param name="s"></param>
void PUTIL::printStack(stack<string> s) {
	while (!s.empty()) {
		cout << s.top();
		s.pop();
	}
	cout << endl;
}
/// <summary>
/// 打印预测分析表
/// </summary>
/// <param name="LL1Table"></param>
/// <param name="vp"></param>
/// <param name="terms"></param>
void PUTIL::printLL1Table(map<LL1Key, string>& LL1, vector<Production>& vp, vector<string>& terms) {
	for (string item : terms) {
		cout << "\t" << item[0];
	}
	cout << endl;
	for (int i = 0; i < vp.size(); i++) {
		cout << vp[i].left;
		for (int j = 0; j < terms.size(); j++) {
			map<LL1Key, string>::iterator it = LL1.find(LL1Key(vp[i].left, terms[j][0]));
			if (it != LL1.end()) {
				cout << "\t" << it->second;
			}
			else {
				cout << "\t";
			}
		}
		cout << endl;
	}
}
/// <summary>
/// 获取文法中的所有终结符
/// </summary>
/// <param name="vp"></param>
/// <returns></returns>
vector<string> PUTIL::getTerms(vector<Production>&vp) {
	set<string> s;
	for (int i = 0; i < vp.size(); i++) {
		for (int j = 0; j < vp[i].right.size(); j++) {
			string pro = vp[i].right[j];
			for (int k = 0; k < pro.size(); k++) {
				if ( !( pro[k]>='A' && pro[k]<='Z') && pro[k] != CHAR_NULL && pro[k]!=DYH) {
					string t = "";
					t += pro[k];
					s.insert(t);
				}
			}
		}
	}
	vector<string> vs;
	for (set<string>::iterator it = s.begin(); it != s.end(); it++) {
		vs.push_back(*it);
	}
	vs.push_back("$");
	return vs;
}
// 消去左递归
void PUTIL::EliminateLeftRecursion(vector<Production>& vp) {
	string start = vp[0].left;
	// 第一步：将非终结符排序，且开始符号为排序中的最后一个
	//预处理，将开始符号与非终结符“Z”（存在）交换，不存在"Z"则把开始符设置为Z，按非终结符的升序排序
	map<string, int> m;// 记录非终结符在vp中的下标
	preprocessing(vp, m);
	// 第二步：消除左递归，产生 Non(i) ::= Non(i+1)Term | Non(i+2)Term | Non(i+3)Term | Non(i+4)Term .....
	for (int i = 0; i < vp.size(); i++) {// 循环某个非终结符及其产生式
		// Non:非终结符，Term:终结符，Non(i):文法的第i个非终结符
		// 消去间接左递归,确保所有非终结符有:Non(i) ::= Non(i)Term | Non(i+1)Term | Non(i+2)Term | Non(i+3)Term .....
		EliminateIndirectLeftRecursion( vp,i,m);
		// 消去直接左递归,将Non(i) ::= Non(i)Term处理掉
		EliminateDirectLeftRecursion(vp,i);
	}
	// 使开始符位于数组的第一个元素
	vector<Production>::iterator it = findProductionByLeft(vp, start);
	Production p = *it;
	*it = vp[0];
	vp[0] = p;
	// 第三步：去除不可从开始符推导的产生式
	eraseNonNotDerivation(vp);
}
/// <summary>
/// 删除无法从开始符推导出的非终结符
/// </summary>
void PUTIL::eraseNonNotDerivation(vector<Production>& vp) {
	// 初始化数据结构 
	queue<string> q;
	map<string, bool> vis;
	initMap(vp, vis);
	// 第二步（删除不能推导出全为终结符的Non、由于Non被删除，导致某些产生式被删除） 
	// 采用的是一次扫描，不回溯，所以无法后面处理导致前面受影响 
	bool isLoop = false;
	// 如E=E+T|S+F|T,T=S,S=i,F=P。处理循序：E、T、S、F。第二步删除了F=P,此时导致前面的E=S+F需要删除。 
	// 第一步，从开始符推导出所有可以出现的
	// 1、初始化：将开始符加入队列并标记 
	q.push(vp[0].left);// 将开始符加入到队列 
	vis[vp[0].left] = true;
	// 从开始符推导 
	while (!q.empty()) {
		// 2、取出一个左边的非终结符 
		string left = q.front();
		q.pop();
		vector<Production>::iterator pro = findProductionByLeft(vp, left);
		// 3、将该非终结符右边出现的<未标记过的非终结符>标记，并加入队列
		addLabelForRight(*pro, q, vis);
	}
	// 4、删除不能从开始推出的非终结符及其产生式
	eraseLeftNotInVis(vp, vis);
	// 5、删除由于删除直接左递归导致无产生式的非终结符
	eraserNullProduction(vp);
}

//预处理
void PUTIL::preprocessing(vector<Production>& vp, map<string, int>& m) {
	//将开始符放在下数组最后
	Production p = vp[0];
	vp[0] = vp[vp.size()-1];
	vp[vp.size() - 1] = p;
	for (int i = 0; i < vp.size(); i++) {
		m.insert(make_pair(vp[i].left, i));
	}
}
// 消去间接左递归,确保所有非终结符有:Non(i) ::= Non(i)Term | Non(i+1)Term | Non(i+2)Term | Non(i+3)Term .....
void PUTIL::EliminateIndirectLeftRecursion(vector<Production>& vp, int pos, map<string, int>& m) {
	vector<string> newRights;
	for (int j = 0; j < vp[pos].right.size(); j++) {
		string pro = vp[pos].right[j];
		// 获取产生式的第一个非终结符
		string non = "";
		int nonPos;
		if (pro[0] >= 'A' && pro[0] <= 'Z') {
			non += pro[0];
			nonPos = m[non];
		}
		else {
			nonPos = pos;
		}
		if (nonPos < pos) {// 首非终结符出现在该字符次序前面,处理掉
			vector<string> rights = getRightsFromPro(pro);
			for (vector<string>::iterator it = vp[nonPos].right.begin(); it != vp[nonPos].right.end(); it++) {
				rights[0] = *it;
				string newPro = getProFromRights(rights);
				newRights.push_back(newPro);
			}
		}
		else {// 不出现，直接加入到新的产生式数组
			newRights.push_back(pro);
		}
	}	
	vp[pos].right = newRights;
}
/// <summary>
/// 消去直接左递归,将Non(i) ::= Non(i)Term处理掉.E=E+T|T => E = TE' ;  E' = +TE'|@
/// </summary>
/// <param name="vp"></param>
/// <param name="pos"></param>
void PUTIL::EliminateDirectLeftRecursion(vector<Production>& vp, int pos) {
	// 将直接可左递归的产生式与不可的分开，即为[first,second),[second,size-1)
	vector<string>::iterator range = sortForEliminateDirectLeftRecursion(vp, pos);// 排序
	vector<string>::iterator begin = vp[pos].right.begin();
	vector<string>::iterator end = vp[pos].right.end();
	if (range != begin) {// 存在直接左递归
		Production newPro;
		string newLeft = vp[pos].left + "'";
		newPro.left = newLeft;
		newPro.have_null = 1;
		vector<string>::iterator it = begin;
		while (it != range) {// 处理E'的新产生式即 类似 E' = aE'|@
			string pro = *it;
			pro.erase(0, 1);
			pro += newLeft;
			newPro.right.push_back(pro) ;
			it++;
		}
		int size = (int)(range - begin);
		while (size--) {// 删除源非终结符的左递归产生式
			vp[pos].right.erase(vp[pos].right.begin());
		}
		it = vp[pos].right.begin();
		end = vp[pos].right.end();
		while (it != end) {// 处理源非终结符的产生式 类似 E = tE'
			string newRight = *it;
			newRight += newLeft;
			*it = newRight;
			it++;
		}
		vp.push_back(newPro);
	}	
}
/// <summary>
/// 用于处理直接左递归的排序，处理后：产生式前几个是当前非终结符开头的.
/// </summary>
/// <param name="vp"></param>
/// <param name="pos"></param>
/// <returns>区间[ 0, pos )为直接左递归所在的区间</returns>
vector<string>::iterator PUTIL::sortForEliminateDirectLeftRecursion(vector<Production>& vp, int pos) {
	vector<string>::iterator left = vp[pos].right.begin();
	vector<string>::iterator right = vp[pos].right.begin();
	vector<string>::iterator end = vp[pos].right.end();
	string non = vp[pos].left;
	while (right != end) {
		if (right->find(non) == 0) {
			string t = *left;
			*left = *right;
			*right = t;
			left++;
		}
		right++;
	}
	return left;
}

void PUTIL::switchNon(vector<Production>& vp, string src1, string src2) {
	for (int i = 0; i < vp.size(); i++) {
		// 存在替换的字符
		if (vp[i].left == src1) {
			vp[i].left = src2;
		}
		else if (vp[i].left == src2) {
			vp[i].left = src1;
		}
		for (int j = 0; j < vp[i].right.size(); j++) {
			string pro = vp[i].right[j];
			if (pro.find(src1) != pro.size() || pro.find(src2) != pro.size() ) {
				vector<string> vs = getRightsFromPro(pro);
				for (vector<string>::iterator it = vs.begin(); it != vs.end(); it++) {
					if (*it == src1) {
						*it = src2;
					}
					else if (*it == src2) {
						*it = src1;
					}
				}
				pro = getProFromRights(vs);
				vp[i].right[j] = pro;
			}
		}
	}
}

string PUTIL::getProFromRights(vector<string> vs) {
	string pro = "";
	for (int i = 0; i < vs.size(); i++) {
		pro += vs[i];
	}
	return pro;
}

void PUTIL::fixNumber(vector<Production>& vp) {
	for (int i = 0; i < vp.size(); i++) {
		vp[i].id = i;
	}
}
