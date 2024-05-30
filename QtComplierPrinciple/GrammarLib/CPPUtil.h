#pragma once
#define _CRT_SECURE_NO_WARNINGS
#include<iostream>
#include<vector>
#include<algorithm>
#include<string>
#include<stdio.h>
#include"RDUtil.h"

using namespace std;

/// <summary>
/// cpp源文件的语句
/// </summary>
class Statement {
public:
	/// <summary>
	/// 记录当前语句需要几个tab
	/// </summary>
	int tabSize;
	/// <summary>
	/// 将语句写入到文件中
	/// </summary>
	/// <param name="fp"></param>
	virtual void fwriteStatement(FILE* fp) = 0;
};

/// <summary>
/// 导包
/// </summary>
class IncludeSegment : public Statement {
public:
	/// <summary>
	/// 库的名称
	/// </summary>
	vector<string> libName;
	string _namespace;
	void fwriteStatement(FILE* fp) override {
		for (int i = 0; i < libName.size(); i++) {
			fprintf(fp, "#include<%s>\n", libName[i].c_str());
		}
		fprintf(fp, "using namespace %s\n\n", _namespace.c_str());
	}
};
/// <summary>
/// 函数声明
/// </summary>
class FunctionDeclarationSegment : public Statement {
public:

	vector<string> functionName;
	void fwriteStatement(FILE* fp) override {
		for (string s : functionName) {
			fprintf(fp, "void %s();\n", s.c_str());
		}
	}
};
/// <summary>
/// 全局变量声明
/// </summary>
class GlobalVariableDeclaration : public Statement {
public:
	string type;
	string name;
	string initVale;

	void fwriteStatement(FILE* fp) override {
		fprintf(fp, "%s %s = %s;\n", type.c_str(), name.c_str(), initVale.c_str());
	}
};


/// <summary>
/// main函数
/// </summary>
class MainFunction : public Statement {
public:
	string returnType;
	string name;
	vector<Statement*> vs;

	string returnValue;
	MainFunction() {
		returnType = "int";
		name = "main";
		returnValue = "0";
	}

	void fwriteStatement(FILE* fp) override {
		fprintf( fp, "int main()\n{\n");
		for (Statement* st : vs) {
			st->fwriteStatement(fp);
		}
		fprintf(fp, "\tif( sym != \"$\" ){\n\t\treturn 1;\n\t}\n");
		fprintf(fp, "\treturn 0;\n");
		fprintf(fp, "}\n");
	}
};
/// <summary>
/// 函数调用
/// </summary>
class FunctionCall : public Statement {
public:
	string functionName;

	void fwriteStatement(FILE* fp) override {
		string tabs(tabSize, '\t');
		fprintf(fp, "%s%s();\n",tabs.c_str(), functionName.c_str());
	}
};
/// <summary>
/// if语句
/// </summary>
class IFSegment : public Statement {
public:
	/// <summary>
	/// 第一个if的条件表达式
	/// </summary>
	string expression;
	/// <summary>
	/// 第一个if为true执行的语句
	/// </summary>
	vector<Statement*> statements;
	/// <summary>
	/// 其余的else if
	/// </summary>
	vector<IFSegment> elseIF;

	IFSegment() {
		expression = "";
	}

	void fwriteStatement(FILE* fp) override {
		string blank(tabSize, '\t');
		fprintf(fp, "%sif(sym==\"%s\")\n% s{% s\n", blank.c_str(), expression.c_str(), blank.c_str(), blank.c_str());
		for (Statement* st : statements) {
			//if (st) {
				st->fwriteStatement(fp);
			//}
		}
		fprintf(fp, "%s}\n", blank.c_str());
		for (IFSegment s : elseIF) {
			if (s.expression != "") {
				fprintf(fp, "%selse if(sym==\"%s\")\n%s{\n",blank.c_str(), s.expression.c_str(), blank.c_str());
				for (Statement* st : s.statements) {
					st->fwriteStatement(fp);
				}
				fprintf(fp, "%s}\n", blank.c_str());
			}
			else {
				fprintf(fp, "%selse {\n",blank.c_str());
				s.statements[0]->fwriteStatement(fp);
				fprintf(fp, "%s}\n", blank.c_str());
			}
		}
	}
};
/// <summary>
/// 函数定义
/// </summary>
class FunctionDefinition : public Statement {
public:
	string functionName;
	vector<Statement*> statements;

	FunctionDefinition() {}

	void fwriteStatement(FILE* fp) override {
		fprintf(fp, "void %s()\n{\n", functionName.c_str());
		for (Statement* st : statements) {
			st->fwriteStatement(fp);
		}
		fprintf(fp,"}\n");
	}
	~FunctionDefinition() {
		for (Statement* st : statements) {
			delete st;
		}
	}
};
class UserStatement : public Statement {
public:
	string statement;
	UserStatement() {
		statement = "";
	}

	void fwriteStatement(FILE* fp) override {
		string blank(tabSize, '\t');
		fprintf(fp, "%s%s\n",blank.c_str(),statement.c_str());
	}
	void getFromFile() {
		FILE* fp;
		fopen_s(&fp, "myLib.txt", "r");
		char str[200];
		if (fp == NULL) {
			return;
		}
		while (fgets(str, sizeof(str), fp)) {
			string t = str;
			statement += t;
		}
	}
};
class CPPFile {
public:
	vector<Statement*> statements;

	void fwriteCPPFile(FILE* fp);
	void deleteCPPFile();
};






class CPPUtil{
public:
	static CPPFile RDGrammarToCPPFile(RDGrammar& rdg, vector<GlobalVariableDeclaration*> gvds,vector<string>& funDec, vector<Statement*>& states);
	static vector<string> getToken(string filePath);
	static void RDAnalysis(Grammar& g, vector<string>& words);
private:
	static void initIncludes(IncludeSegment& includes);
	static void initFunctionDeclarationSegment(FunctionDeclarationSegment& fds, RDGrammar& rdg);
	static void initManFunction(MainFunction& main, RDGrammar& rdg);
	static void initFunctionDefinition( vector<FunctionDefinition*>& funcDefs, RDGrammar& rdg );
	static IFSegment* getIFSegment( int tabSize, vector<SignType>::iterator begin, vector<SignType>::iterator end );
	
};


