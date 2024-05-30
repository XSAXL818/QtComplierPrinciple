#define _CRT_SECURE_NO_WARNINGS
#include "LRUtil.h"

//#include <iostream>
//#include <fstream>
//#include <string>
//using std::string;
//using std::map;
//using std::vector;
//using std::cout;
//using std::endl;
using namespace std;

map<LRKey, string> LRUtil::loadLRTable(vector<char>& terms, vector<char>& nons){
    map<LRKey, string> table;
    vector<char> all;
    FILE* fp;
    fopen_s(&fp, "LRTable.txt", "r");
    if (fp == NULL) {
        cout << "文件未找到！\n";
        return table;
    }
    char line[256];
    // 第一行读取个数
    fgets(line, sizeof(line), fp);
    int num = std::stoi(line);
    cout << "有" << num << endl;
    // 第二行读取字符
    for (int i = 1; i < num; i++) {
        cout << "i:" << i << endl;
        char item[2];
        int n = fscanf(fp, "%s", &item);
        if (item[0] >= 'A' && item[0] <= 'Z') {
            nons.push_back(item[0]);
            all.push_back(item[0]);
        }
        else {
            terms.push_back(item[0]);
            all.push_back(item[0]);
        }
    }
    
    cout << endl;
    for (char c : terms) {
        cout << c;
    }
    cout << endl;
   
    for (char c : nons) {
        cout << c;
    }
    cout << endl;
    cout << "all.size=" << all.size() << endl;
    for (int i = 0; i < 12; i++) {
        char item[10];
        int n = fscanf(fp, "%s", item);
        for (int j = 0; j < 9; j++) {
            cout << "i:" << i << " j:" << j << endl;
            fscanf(fp, "%s", item);
            cout << "item:" << item << endl;
            
            LRKey key( i,all[j] );
            cout << "inset\n";
            table.insert({ key,string(item) });
            cout << "1\n";
        }
        cout << "2\n";
      
    }

    fclose(fp);
    return table;
}
