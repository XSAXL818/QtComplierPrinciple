#include "CPPUtil.h"

CPPFile CPPUtil::RDGrammarToCPPFile(RDGrammar& rdg, vector<GlobalVariableDeclaration*> gvds, vector<string>& funDec, vector<Statement*>& states)
{
	CPPFile cpp;
	// 头文件
	IncludeSegment* includes = new IncludeSegment();
	initIncludes(*includes);
	// 函数声明
	FunctionDeclarationSegment* fds = new FunctionDeclarationSegment();
	for (set<string>::iterator it = rdg.nons.begin(); it != rdg.nons.end(); it++) {
		fds->functionName.push_back(*it);
	}
	for (vector<string>::iterator it = funDec.begin(); it != funDec.end(); it++) {
		fds->functionName.push_back(*it);
	}
	// 全局变量声明
	// main函数
	MainFunction* main = new MainFunction();
	initManFunction(*main, rdg);
	// 函数定义
	vector<FunctionDefinition*> funcDefs;
	initFunctionDefinition(funcDefs, rdg);
	// 头文件
	cpp.statements.push_back(includes);
	// 函数声明
	cpp.statements.push_back(fds);
	// 全局变量声明
	for (GlobalVariableDeclaration* gvd : gvds) {
		cpp.statements.push_back(gvd);
	}
	for (Statement* fd : states) {
		cpp.statements.push_back(fd);
	}
	// main函数
	cpp.statements.push_back(main);
	// 函数定义
	for (FunctionDefinition* fd : funcDefs) {
		cpp.statements.push_back(fd);
	}
	return cpp;
}

void CPPUtil::initFunctionDefinition(vector<FunctionDefinition*>& funcDefs, RDGrammar& rdg) {
	//rdg.printRDGrammar();
	// 遍历每个非终结符
	for (map<string, vector< vector<SignType> > >::iterator it1 = rdg.regulars.begin(); it1 != rdg.regulars.end(); it1++) {
		FunctionDefinition* funcDef = new FunctionDefinition();
		funcDef->tabSize = 0;
		funcDef->functionName = it1->first;
		//规律: 当产生式为非终结符开头，则该非终结符只有一个产生式,如果有多个则具有回溯性
		// 只需判断第一个产生是否为非终结符开头即可分开两类
		// 开头为非终结符
		if (rdg.nons.find(it1->second.begin()->begin()->value) != rdg.nons.end()) {
			vector<SignType>::iterator begin = it1->second.begin()->begin();
			vector<SignType>::iterator end = it1->second.begin()->end();
			while (begin != end) {
				if (begin->type == NON) {
					FunctionCall* fc = new FunctionCall();
					fc->tabSize = 1;
					fc->functionName = begin->value;
					funcDef->statements.push_back(fc);
				}
				else {
					break;
				}
				begin++;
			}
			if (begin != end) {
				IFSegment* ifSeg = new IFSegment();
				ifSeg->expression = begin->value;
				ifSeg->statements.push_back(CPPUtil::getIFSegment(1, ++begin, end));
				IFSegment Else;
				Else.expression = "";
				FunctionCall* fc = new FunctionCall();
				fc->functionName = "Error";
				fc->tabSize = 2;
				Else.statements.push_back(fc);
				funcDef->statements.push_back(ifSeg);
			}
		}//开头为终结符
		else {
			vector< vector<SignType> >::iterator begin1 = it1->second.begin();
			vector< vector<SignType> >::iterator end1 = it1->second.end();
			vector<SignType>::iterator begin = begin1->begin();
			vector<SignType>::iterator end = begin1->end();
			// 第一个IF
			IFSegment* IF = new IFSegment();
			IF->expression = begin->value;
			IF->tabSize = 1;

			FunctionCall* fc = new FunctionCall();
			fc->functionName = "GetSymbol";
			fc->tabSize = 2;
			IF->statements.push_back(fc);
			begin++;
			while(begin != end) {
				if (begin->type == NON) {
					fc = new FunctionCall();
					fc->functionName = begin->value;
					fc->tabSize = 2;
					IF->statements.push_back(fc);
				}
				else {
					break;
				}
				begin++;
			}
			if (begin != end) {
				IF->statements.push_back(CPPUtil::getIFSegment(1, begin, end));
			}
			// 后续的ELSE IF
			begin1++;
			while (begin1 != end1){
				begin = begin1->begin();
				end = begin1->end();
				IFSegment ELSE;
				ELSE.tabSize = 1;
				ELSE.expression = begin->value;
				begin++;

				FunctionCall* fc1 = new FunctionCall();
				fc1->functionName = "GetSymbol";
				fc1->tabSize = 2;
				ELSE.statements.push_back(fc1);
				while (begin != end) {
					if (begin->type == NON) {
						fc = new FunctionCall();
						fc->functionName = begin->value;
						fc->tabSize = 2;
						ELSE.statements.push_back(fc);
					}
					else {
						break;
					}
					begin++;
				}
				if (begin != end) {
					ELSE.statements.push_back(CPPUtil::getIFSegment(1, begin, end));
				}

				IF->elseIF.push_back(ELSE);
				begin1++;
			}
			// 判断是否有空产生式
			if (rdg.null_pro.find(it1->first) == rdg.null_pro.end()) {//无空产生式
				IFSegment NELSE;
				NELSE.expression = "";
				NELSE.tabSize = 1;
				FunctionCall* fc = new FunctionCall();
				fc->tabSize = 2;
				fc->functionName = "Error";
				NELSE.statements.push_back(fc);
				IF->elseIF.push_back(NELSE);
			}
			funcDef->statements.push_back(IF);
		}
		funcDefs.push_back(funcDef);
	}
}


void CPPUtil::initManFunction(MainFunction& main, RDGrammar& rdg) {
	FunctionCall* gs = new FunctionCall();
	gs->functionName = "GetSymbol";
	gs->tabSize = 1;
	FunctionCall* start = new FunctionCall();
	start->functionName = rdg.start;
	start->tabSize = 1;
	
	main.vs.push_back(gs);
	main.vs.push_back(start);
}

void CPPUtil::initIncludes(IncludeSegment& includes)
{
	vector<string> libName{
		"iostream",
		"vector",
		"string"
	};
	includes.libName = libName;
	includes._namespace = "std;";
}

void CPPFile::fwriteCPPFile(FILE* fp)
{
	for (int i = 0; i < statements.size(); i++) {
		statements[i]->fwriteStatement(fp);
	}
}

void CPPFile::deleteCPPFile()
{
	for (Statement* st : statements) {
		delete st;
	}
}
/// <summary>
/// 递归获取if
/// </summary>
/// <param name="tabSize"></param>
/// <param name="father"></param>
/// <param name="begin"></param>
/// <param name="end"></param>
IFSegment* CPPUtil::getIFSegment(int tabSize, vector<SignType>::iterator begin, vector<SignType>::iterator end) {
	if (begin == end) {
		return NULL;
	}
	//条件表达
	IFSegment* ifs = new IFSegment();
	ifs->expression = begin->value;
	ifs->tabSize = tabSize + 1;
	FunctionCall* fc = new FunctionCall();
	fc->tabSize = tabSize + 2;
	fc->functionName = "GetSymbol";
	ifs->statements.push_back(fc);

	begin++;
	while (begin != end) {
		if (begin->type == NON) {

			fc = new FunctionCall();
			fc->tabSize = tabSize + 2;
			fc->functionName = begin->value;
			ifs->statements.push_back(fc);
		}
		else {
			break;
		}
		begin++;
	}
	// if为true的执行语句
	if (begin != end) {
		ifs->statements.push_back(getIFSegment(tabSize + 1, begin, end));
	}

	// 执行Error
	IFSegment ELSE;
	ELSE.expression = "";
	fc = new FunctionCall();
	fc->tabSize = tabSize + 2;
	fc->functionName = "Error";
	ELSE.statements.push_back(fc);
	ifs->elseIF.push_back(ELSE);
	return ifs;
}

vector<string> CPPUtil::getToken(string filePath)
{
	vector<string> vs;
	FILE* fp;
	fopen_s(&fp,filePath.c_str(), "r");
	if (fp == 0) {
		cout << filePath << "文件打开失败！" << endl;
		return vector<string>();
	}
	char line[100];
	while (fgets(line,sizeof(line),fp)) {
		int i = 0;
		string word = "";
		while (line[i] != ' ') {
			word += line[i];
			i++;
		}
		if (word != "") {
			vs.push_back(word);
		}

	}
	fclose(fp);
	return vs;
}

void CPPUtil::RDAnalysis(Grammar& g, vector<string>& words)
{
	// 全局变量初始化，words，sym，currIndex
	string statement = "{";
	for (string s : words) {
		statement += "\"" + s + "\",";
	}
	statement += "\"$\"}";
	GlobalVariableDeclaration* gvd1 = new GlobalVariableDeclaration();
	gvd1->type = "vector<string>";
	gvd1->name = "symbols";
	gvd1->initVale = statement;
	gvd1->tabSize = 0;
	GlobalVariableDeclaration* gvd2 = new GlobalVariableDeclaration();
	gvd2->type = "int";
	gvd2->name = "currIndex";
	gvd2->initVale = "-1";
	gvd2->tabSize = 0;
	GlobalVariableDeclaration* gvd3 = new GlobalVariableDeclaration();
	gvd3->type = "string";
	gvd3->name = "sym";
	gvd3->initVale = "\"\"";
	gvd3->tabSize = 0;
	vector<GlobalVariableDeclaration*> gvds{ gvd1,gvd2,gvd3 };
	// 函数声明
	vector<string> funDec{ "GetSymbol","Error" };
	// 函数定义
	UserStatement* us = new UserStatement();
	us->getFromFile();
	us->tabSize = 0;
	vector<Statement*> states{ us };
	// 生成cpp文件
	RDGrammar rdg = RDGUtil::grammarToRDGrammar(g);
	//rdg.printRDGrammar();
	CPPFile cpp = CPPUtil::RDGrammarToCPPFile(rdg, gvds, funDec, states);
	
	string filePath = "D:\\Code\\CCode\\RDA\\RDanalysis.cpp";
	FILE* fp;
	fopen_s(&fp, filePath.c_str(), "w");
	if (fp == 0) {
		cout << "文件" << filePath << "打开失败" << endl;
		return;
	}
	cpp.fwriteCPPFile(fp);
	// 释放内存和资源
	cpp.deleteCPPFile();
	fclose(fp);

}
