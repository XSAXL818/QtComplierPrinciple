#pragma once
#include<iostream>
#include<map>
#include<vector>
#include<string>
#include <stack>
using namespace std;

class LRKey {
public:
	int state;
	char input;
	LRKey() :state(-1), input('$') {}
	//LRKey( int _state, char _input ):state(_state),input(_input) {}
	LRKey(int _state, char _input) {
		state = _state;
		input = _input;
	}
	bool operator<( const LRKey& key ) const {
		if (state != key.state) {
			return state < key.state;
		}
		return input < key.input;
	}
};

class Pro {
public:
	char left;
	string right;

	
};

class LRUtil{
public:
	// 从文件中读取ACTION和GOTO表,同时读取终结符和非终结符
	static map<LRKey,string> loadLRTable( vector<char>& terms, vector<char>& nons );

	static void printLRTable(vector<char>& terms, vector<char>& nons, map<LRKey, string>& table, int stateNum) {
		for (int i = 0; i < stateNum; i++) {
			for (char c : terms) {
				LRKey key( i, c );
				cout << "\t";
				string s = table[key];
				cout << s;
			}
			for (char c : nons) {
				LRKey key( i, c );
				cout << "\t";
				string s = table[key];
				cout << s;
			}
			cout << endl;
		}
	}
	static bool isJuzi(string juzi, map<LRKey, string>& table,vector<Pro>& pros) {
		stack<int> state;
		stack<char> symbol;
		int curr = 0;
		state.push('#');
		symbol.push('#');
		while (1) {
			int sta = state.top();
			LRKey key{ sta,juzi[curr] };
			auto it = table.find(key);
			if (it != table.end()) {
				if (it->second[0] >= 'R') {
					int num = stoi(it->second);
					Pro p = pros[num];
					for (int i = 0; i < p.right.size(); i++) {

					}

				}
				else if (it->second[0] == 'S') {
					symbol.push(juzi[curr]);
					curr++;
				}
				else if(it->second == "ACC"){
					cout << "这是句子\n";
				}
			}
			else {
				cout << "出错\n";
			}
		}
	}
};

