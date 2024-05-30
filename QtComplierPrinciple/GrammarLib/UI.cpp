#include "UI.h"


void UI::readFunction(vector<string>& vs, string fileName) {
	FILE* fp;
	errno_t err;
	err = fopen_s(&fp, fileName.c_str(), "r");
	char line[200];
	while (fgets(line, sizeof(line), fp)) {
		string str = line;
		if (line[0] != '\n') {
			vs.push_back(str);
		}
	}
	fclose(fp);
}
pair<int, int> UI::select(vector< Grammar >& grammar, vector<string>& vs) {
	cout << "读取的所有文法如下：" << endl;
	for (int i = 0; i < grammar.size(); i++) {
		cout << "********** 文法" << i << ": " << " L(" << grammar[i].start << ") 如下： ***********" << endl;
		PUTIL::printGrammar(grammar[i]);
		cout << endl;
	}
	cout << "目前支持的功能如下：" << endl;
	for (int i = 0; i < vs.size(); i++) {
		cout << i << "：" + vs[i];
	}
	cout << endl<<endl;
	cout << "**************请输入需要的操作，其格式为：文法编号 功能编号。***************" << endl;
	cout << "**************              输入 -1 退出程序                 ***************" << endl;
	int number1, number2;
	cin >> number1;
	if (number1 != -1) {
		cin >> number2;
	}
	return make_pair(number1, number2);
}