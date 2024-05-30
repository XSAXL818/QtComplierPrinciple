#pragma once

#include<string>
#include"Grammar.h"
#include"GUtil.h"


using namespace std;
class UI
{
public:
	static void readFunction(vector<string>& vs, string fileName);
	static pair<int, int> select(vector< Grammar >& vvp, vector<string>& vs);
	static void fun() {
		cout << "请输入!" << endl;
	}
};

