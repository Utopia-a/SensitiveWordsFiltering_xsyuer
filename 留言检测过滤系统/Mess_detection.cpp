#include<iostream>
#include<fstream>
#include<string>
#include"Mess_detection.h"


using namespace std;

void mess_detection::init_voc() {
	ifstream ifs;
	ifs.open("VocFile.txt", ios::in);
	if (!ifs.is_open()) {
		cout << "文件打开失败，请先创建文件" << '\n';
		return;
	}
	
	string buf;
	while (getline(ifs, buf)) {
		Sensitive_lexicon.push_back(buf);
		table_len++;
		if (table_len == Max_len) {
			cout << "词汇库已满，未能读取更多行" << endl;
			return;
		}
	}
		ifs.close();
		cout << "----------------------------\n";
		cout << "从敏感词汇库中加载完毕，共加载 " << Sensitive_lexicon.size() << " 个敏感词。" << '\n';
		cout << "----------------------------\n";
	}


vector<int> mess_detection::cal_next(const string& pattern) {
	int k =0;
	int plen = pattern.size();
	vector<int> next(plen, 0);//初始化

	//cout << plen << endl;

	for (int i = 1; i < plen; i++) {
		while (k > 0 && pattern[k] != pattern[i]) {
			k = next[k - 1];
		}

		if (pattern[i] == pattern[k]) {
			k = k + 1;
		}
		next[i] = k;
	}
	return next;
}

//找出匹配的字串的起始位置
vector<int> mess_detection::Kmpsearch(const string& text, const string& pattern) {
	vector<int>next = cal_next(pattern);
	vector<int> matches;
	
	int n = text.size();//记录text的长度
	int m = pattern.size();//记录pattern的长度

	int k = 0;
	for (int i = 0; i < n; i++) {
		while (k > 0 && text[i] != pattern[k]) {
			k = next[k - 1];
		}
		if (text[i] == pattern[k]) {
			k++;
		}
		if (k == m) {
			matches.push_back(i - m + 1);//将字串的开头存入match容器中
			k = next[k - 1];//回溯，继续向前匹配
		}

	}
	return matches;
}

string mess_detection::maskSensitiveWords(const string& text) {
	init_voc();
	string maskedText = text;
	//int charlen = 0;
	for (const auto& word : Sensitive_lexicon) {
		vector<int> matches = Kmpsearch(maskedText, word); 
		for (int pos : matches) {//这个循环是获取matches中的敏感词汇 起始位置 将词汇 用*代替
			//size_t charLen = 0;
			for (int i = 0; i < word.size(); ++i) {
				maskedText[pos + i] = '*';
				/*if (maskedText[pos + i] >= 0x81 && maskedText[pos + i] <= 0xFE) {
					charLen = 2;
				}
				else charlen = 1;

				maskedText.replace(pos + i, charlen, "*");
				i += charlen - 1;*/
			}

		}
	}
	return maskedText;
}



int main() {
	//test();
	string pattern;
	string maskedText;
	mess_detection m;
	cout << "请输入一个待检测的字符串\n";
	cin >> pattern;
	maskedText = m.maskSensitiveWords(pattern);
	cout << "过滤结果为：\n";
	cout << maskedText << '\n';
}


