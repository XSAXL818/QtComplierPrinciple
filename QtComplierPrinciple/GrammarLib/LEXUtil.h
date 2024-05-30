#pragma once
#define _CRT_SECURE_NO_WARNINGS
#include<iostream>
#include<vector>
#include<set>
#include<map>
#include<algorithm>
#include<string>
#include<stdio.h>
#include"Grammar.h"


using namespace std;

// 符号，包括其值和类型（非终结符和终结符）
class Word {
public:
	int type;// 类型
	string value;// 值
	Word() {
		type = 0;
		value = "";
	}
	Word(int _type, string _value) {
		type = _type;
		value = _value;
	}
	bool operator<(Word s) const {
		if (type != s.type) {
			return type < s.type;
		}
		return value < s.value;
	}
};

class LEXUtil{
public:
	static void LEX(string argv) {
		string op = "a.exe " + argv;
		system(op.c_str());
	}
	static void load_lex() {
		string regular = "flex regular.l";
		// 生成词法分析器
		cout << "词法分析加载.........................." << endl;
		if (system(regular.c_str()) == 0) {
			cout << "规则文件:" << regular << " 生成中...." << endl;
		}
		else {
			cout << "规则文件: " << regular << " 生成失败...." << endl;
		}
		if (system("gcc lex.yy.c") == 0) {
			cout << "词法分析程序" << regular << "生成中...." << endl;
		}
		else {
			cout << "词法分析程序" << regular << "生成失败...." << endl;
		}
	}

	static vector<Word> getWordsFromProduction( string pro, GrammarPlus& gp ) {
		FILE* fp;
		fopen_s(&fp,"Lexoutput.txt", "r");
		




		fclose(fp);
	}
private:
	
};

