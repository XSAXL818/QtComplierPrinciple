#include<iostream>
#include <Windows.h>
#include<vector>
#include<algorithm>
#include<set> 
#include<map>
#include<stdio.h>
#include<queue>
#include<string>
#include <cstdlib>
#include <filesystem>
#include"Grammar.h"
#include"GUtil.h"
#include"UI.h"
#include"FAUtil.h"
#include"RDUtil.h"
#include"CPPUtil.h"
#include"LEXUtil.h"
#include"OPGUtil.h"

using namespace std;

void f0(Grammar& g);
void f1(Grammar& g);
void f2(Grammar& g);
void f3(vector<DFA>& dfas);
void f4(vector<NFA>& nfas, vector<DFA>& dfas);
void f5(Grammar& g);
void f6(Grammar& g);
void f7(GrammarPlus& g);

//int funcID = 5;

int main() {
	/*bool b1 = true;
	bool b2 = true;
	cout << b1 + b2 << endl;*/

	vector<Grammar > grammars;
	vector< string > func;
	vector<NFA> nfas;
	vector<DFA> dfas;
	PUTIL::readGrammar(grammars, "input.txt");
	UI::readFunction(func, "function.txt");
	FAUtil::readDFA(dfas, "DFAInput.txt");
	FAUtil::readNFA(nfas, "NFAInput.txt");

	GrammarPlus gp;
	gp.productions = grammars[8].productions;
	gp.start = grammars[8].start;
	set<string> terms{ "if", "else", "then", ";", "e", "a"};
	gp.terms = terms;
	/*gp.nons = grammars[8].nons;
	for (char c : grammars[8].terms) {
		string t = "";
		t += c;
		gp.terms.insert(t);
	}*/

	while (1) {

		int sel;
		while (1) {
			cout << endl << "--------输入1进入操作界面，0摇滚出界面----------" << endl;
			cin >> sel;
			if (sel == 1) {
				break;
			}
			else {
				return 0;
			}
		}
		
		pair<int, int> op = UI::select(grammars, func);
		if (op.first == -1) {
			return 0;
		}
		if (op.first > grammars.size()) {
			cout << "请选择正确的文法！" << endl;
			continue;
		}
		switch (op.second) {
		case 0:// 加标记法
			f0(grammars[op.first]);
			break;
		case 1:// LL1自顶向下推导
			f1(grammars[op.first]);
			break;
		case 2:// 消除左递归
			f2(grammars[op.first]);
			break;
		case 3:// DFA推导
			f3(dfas);
			break;
		case 4:// NFA转换为DFA
			f4(nfas,dfas);
			break;
		case 5:// 递归下降分析
			f5(grammars[op.first]);
			break;
		case 6:// 算符优先
			f6(grammars[op.first]);
			break;
		case 7:
			f7(gp);
			break;
		default:
			cout << "请输入正确的功能编号！" << endl;
		}

	}
	return 0;
}


void f0(Grammar& g) {
	PUTIL::labelMethod(g.productions);
	cout << "----------加标记法处理结果如下：(输出文件为labelOutPut.txt)" << endl;
	PUTIL::fixNumber(g.productions);
	PUTIL::printGrammar(g);
	PUTIL::fwriteGrammar(g.productions, "labelOutPut.txt");
	cout << "--------------------------------" << endl;
}

void f1(Grammar& g) {

	cout << "所选文法为：\n";
	PUTIL::printGrammar(g);

	vector<string> terms;
	for (set<char>::iterator it = g.terms.begin(); it != g.terms.end(); it++) {
		string t = "";
		t += *it;
		terms.push_back(t);
	}
	terms.push_back("$");
	vector<ProductionFirst> vpf;
	map<LL1Key,string> LL1Table = PUTIL::getLL1Table(vpf,g.productions, terms);

	cout << "LL1语法分析表如下：" << endl;
	PUTIL::printLL1Table(LL1Table, g.productions, terms);
	string juzi;
	char c;
	while (1) {
		cout << "请选择以下功能:\n";
		cout << "1.键盘标准输入\n2.读取文件(每行为一个句)\n3.退出\n";
		int op;
		cin >> op;
		//getchar();
		cin.get(c);
		if(op == 1) {
			while (1) {
				cout << "请输入待推导的的句子：(输入@退出)" << endl;
				getline(cin, juzi);
				if (juzi == "@") {
					break;
				}
				int index = juzi.find(' ', 0);
				while (index != -1) {
					juzi.erase(index, 1);
					juzi.insert(index, "@");
					index++;
					index = juzi.find(' ', index);
				}
				PUTIL::topToBottom(juzi, LL1Table, g.productions, terms);
			}
			
		}
		else if (op == 2) {
			FILE* fp;
			fopen_s(&fp, "LL1input.txt", "r");
			if (fp == 0) {
				cout << "文件打打开失败" << endl;
				break;
			}
			char str[100];
			while (fgets(str, sizeof(str), fp)) {
				string line = str;
				if (line[line.size() - 1] == '\n') {
					line.erase(line.size() - 1);
				}
				PUTIL::topToBottomNoProcess(line, LL1Table, g.productions, terms);

			}


			fclose(fp);
		}
		else if (op == 3) {
			break;
		}
	}
}
void f2(Grammar& g) {
	cout << "消除左递归结果: " << endl;
	PUTIL::EliminateLeftRecursion(g.productions);
	PUTIL::fixNumber(g.productions);
	PUTIL::printGrammar(g);
	PUTIL::fwriteGrammar(g.productions, "output.txt");
}

void f3(vector<DFA>& dfas) {
	while (1) {
		FAUtil::printDFAs(dfas);
		cout << "请输入<DFA编号,字符>进行操作，输入-1退出程序" << endl;
		int opID=-1;
		string str;
		cin >> opID;
		cout << "opID: " << opID;
		if (opID == -1 || opID < 0 || opID > dfas.size() ) {
			break;
		}
		else {
			cin >> str;
			FAUtil::DFADerivedSentence(dfas[opID], str);
		}
		
	}
	
}
void f4(vector<NFA>& nfas, vector<DFA>& dfas) {
	while (1) {
		FAUtil::printNFAs(nfas);
		cout << "请输入<NFA编号>进行操作，输入-1退出程序>" << endl;
		int opID;
		string str;
		cin >> opID;
		if (opID == -1) {
			break;
		}
		DFA dfa = FAUtil::NFAToDFA(nfas[opID]);
		string output = "DFAOutput.txt";
		cout << "DFA已输出到文件" << output <<"中" << endl;
		dfa.fwriteDFA(output.c_str());
		dfas.push_back(dfa);
	}
}
void LEX(string argv) {
	string op = "a.exe " + argv;
	system(op.c_str());
}
void load_lex() {
	string regular = "flex regular.l";
	// 生成词法分析器
	cout << "词法分析加载.........................." << endl;
	if (system(regular.c_str()) == 0) {
		cout << "规则文件" << regular << "生成中...." << endl;
	}
	else {
		cout << "规则文件" << regular << "生成失败...." << endl;
	}
	if (system("gcc lex.yy.c") == 0) {
		cout << "词法分析程序" << regular << "生成中...." << endl;
	}
	else {
		cout << "词法分析程序" << regular << "生成失败...." << endl;
	}
}
/// <summary>
/// 运行递归下降分析程序
/// </summary>
/// <param name="words"></param>
void executionRD(vector<string>& words) {
	// 编译递归下降分析源码，test.cpp文件
	system("g++ \"D:\\Code\\CCode\\RDA\\RDanalysis.cpp\" -o \"D:\\Code\\CCode\\RDA\\RDanalysis.exe\"");
	cout << "读取的单词为: {";
	for (string s : words) {
		cout << s << ",";
	}
	cout << "}\n";
	// 运行递归下降分析程序
	int ret = system("\"D:\\Code\\CCode\\RDA\\RDanalysis.exe\"");
	if (ret == 0) {
		cout << "********这是一个句子！**********" << endl;
	}
	else {
		cout << "********这不是一个句子!********" << endl;
	}
}
/// <summary>
/// 不用再编译递归下降分析
/// </summary>
/// <param name="words"></param>
void executionRDNoCom(vector<string>& words) {
	//// 编译递归下降分析源码，test.cpp文件
	cout << "读取的单词为: {";
	for (string s : words) {
		cout << s << ",";
	}
	cout << "}\n";
	// 运行递归下降分析程序
	int ret = system("\"D:\\Code\\CCode\\RDA\\RDanalysis.exe\"");
	if (ret == 0) {
		cout << "********这是一个句子！**********" << endl;
	}
	else {
		cout << "********这不是一个句子!********" << endl;
	}
}
void f5(Grammar& g) {

	// 加载词法分析器
	LEXUtil::load_lex();
	// 获取读取字串分割的单词数组
	vector<string> words;
	string output = "Lexoutput.txt";// 词法分析程序的结果输出文件
	// 使用词法分析器分析字符串
	while (1) {
		cout << "功能：\n\t1.读取文件\n\t2.输入字串\n\t3.退出\n";
		string op;
		cin >> op;
		string para = "";
		if ( op == "1") {
			// 词法分析器将文件Lexinput.txt中的源码分割成单词,并写入文件Lexoutput.txt中
			LEX(para);
			// 获取词法分析器分割的单词
			words = CPPUtil::getToken(output.c_str());
			// 自动生成递归下降分析源码
			CPPUtil::RDAnalysis(g, words);
			// 执行递归下降分析程序
			executionRD(words);
		}
		else if (op == "2") {
			bool isGcc = false;
			getchar();
			while (1) {
				cout << "请输入要推导的一行字串（输入$退出）\n";
				// 读取一行字符
				getline(cin, para);
				if (para == "$") {
					break;
				}
				// 改造字串，使符合要求
				int index = para.find('*', 0);
				while (index != -1) {
					para.insert(index,"\\\\");
					index = para.find('*', index+3);
				}
				//para = "\"" + para + "\"";
				//cout << "转义后" << para << endl;
				// 词法分析器将输入字串分割成单词,并写入文件Lexoutput.txt中
				LEX(para);
				// 获取词法分析器分割的单词
				words = CPPUtil::getToken(output.c_str());
				// 自动生成递归下降分析源码
				CPPUtil::RDAnalysis(g, words);
				// 执行递归下降分析程序
				if (!isGcc) {// 避免返回编译
					executionRD(words);
					isGcc = !isGcc;
				}
				else {
					executionRDNoCom(words);
				}
			}
		}
		else {
			break;
		}
	}	
}
void f6(Grammar& grammar) {
	cout << "----------------求解FIRSTTERM中------------------" << endl;
	vector<FirstTerm> first = OPGUtil::getFirstTerm(grammar);
	cout << "----------------求解LASTTERM中------------------" << endl;
	vector<LastTerm> last = OPGUtil::getLastTerm(grammar);
	cout << "----------------求解算符优先表中------------------" << endl;
	map<pair<char, char>, char> table = OPGUtil::getTable(grammar,first,last);
	map<pair<char, char>, char> noTable = OPGUtil::getTableNo(grammar, first, last);
	cout << "算符优先矩阵：\n";
	OPGUtil::printOPGTable(grammar, table);
	grammar.terms.erase('#');
	cout << "----------------求解算符优先表中------------------" << endl;
	static map<pair<char, char>, int> fgTable = OPGUtil::getFGTable(noTable, grammar);
	cout << endl<<  "优先函数表如下:" << endl;
	OPGUtil::printFGTable(grammar, fgTable);
	grammar.terms.insert('#');
	map<string, char> proToLeftTable = OPGUtil::getProToLeftTable(grammar);
	while (1) {
		string juzi;
		cout << "请选择功能：1、识别句子\t2、Warshall算法求解\t3、退出\n";
		string op;
		cin >> op;
		if (op == "1") {
			cout << "算符优先矩阵：\n";
			OPGUtil::printOPGTable(grammar, table);
			grammar.terms.insert('#');
			cout << "请输入一个句子：(输入#退出)\n";
			cin >> juzi;
			if (juzi == "#") {
				break;
			}
			if (OPGUtil::isSentence(grammar.start, juzi, table, proToLeftTable) == ISSENTENCE) {
				cout << "这是个句子\n";
				system("\"D:\\Code\\CCode\\VS\\QtProject\\OPG\\x64\\Debug\\OPG.exe\"");
			}
			else {
				cout << "这不是个句子\n";
				system("\"D:\\Code\\CCode\\VS\\QtProject\\OPG\\x64\\Debug\\OPG.exe\"");
			}
		}
		else if (op == "2") {
			grammar.terms.erase('#');
			cout << "算符优先矩阵：\n";
			OPGUtil::printOPGTable(grammar, noTable);
			OPGUtil::warShall(noTable,grammar);
		}
		else {
			break;
		}
		
	}
	/*while (1) {
		cout << "算符优先矩阵：\n";
		OPGUtil::printOPGTable(grammar, table);
		string juzi;
		cout << "请输入一个句子：(输入#退出)\n";
		cin >> juzi;
		if (juzi == "#") {
			break;
		}
		else {
			if (OPGUtil::isJuzi(grammar.start,juzi, table)) {
				cout << "这是个句子\n";
			}
			else {
				cout << "这不是个句子\n";
			}
		}
	}*/
}

void f7(GrammarPlus& gp) {
	
}