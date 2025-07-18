#include<iostream>
#include<fstream>
#include<string>
#include"Mess_detection.h"


using namespace std;

void mess_detection::init_voc() {
	ifstream ifs;
	ifs.open("VocFile.txt", ios::in);
	if (!ifs.is_open()) {
		cout << "�ļ���ʧ�ܣ����ȴ����ļ�" << '\n';
		return;
	}
	
	string buf;
	while (getline(ifs, buf)) {
		Sensitive_lexicon.push_back(buf);
		table_len++;
		if (table_len == Max_len) {
			cout << "�ʻ��������δ�ܶ�ȡ������" << endl;
			return;
		}
	}
		ifs.close();
		cout << "----------------------------\n";
		cout << "�����дʻ���м�����ϣ������� " << Sensitive_lexicon.size() << " �����дʡ�" << '\n';
		cout << "----------------------------\n";
	}


vector<int> mess_detection::cal_next(const string& pattern) {
	int k =0;
	int plen = pattern.size();
	vector<int> next(plen, 0);//��ʼ��

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

//�ҳ�ƥ����ִ�����ʼλ��
vector<int> mess_detection::Kmpsearch(const string& text, const string& pattern) {
	vector<int>next = cal_next(pattern);
	vector<int> matches;
	
	int n = text.size();//��¼text�ĳ���
	int m = pattern.size();//��¼pattern�ĳ���

	int k = 0;
	for (int i = 0; i < n; i++) {
		while (k > 0 && text[i] != pattern[k]) {
			k = next[k - 1];
		}
		if (text[i] == pattern[k]) {
			k++;
		}
		if (k == m) {
			matches.push_back(i - m + 1);//���ִ��Ŀ�ͷ����match������
			k = next[k - 1];//���ݣ�������ǰƥ��
		}

	}
	return matches;
}

//���㺺���ַ���
int countUTF8Chars(const string& str) {
	int count = 0;
	for (size_t i = 0; i < str.length(); ) {
		if ((str[i] & 0x80) == 0) {
			// ASCII�ַ� (0xxxxxxx)
			i++;
			count++;
		}
		else if ((str[i] & 0xE0) == 0xC0) {
			// 2�ֽ�UTF-8�ַ�
			i += 2;
			count++;
		}
		else if ((str[i] & 0xF0) == 0xE0) {
			// 3�ֽ�UTF-8�ַ� (���ֳ���)
			i += 3;
			count++;
		}
		else if ((str[i] & 0xF8) == 0xF0) {
			// 4�ֽ�UTF-8�ַ�
			i += 4;
			count++;
		}
		else {
			// �����δ֪�ֽڣ�ֻ������������
			i++;
		}
	}
	return count;
}


string mess_detection::maskSensitiveWords(const string& text) {
	init_voc();
	string maskedText = text;
	for (const auto& word : Sensitive_lexicon) {
		vector<int> matches = Kmpsearch(maskedText, word); 
		// �Ӻ���ǰ�滻������λ��ƫ������
		for (int i = matches.size() - 1; i >= 0; i--) {
			int pos = matches[i];
			// �������дʵ��ַ������������ֽ�������
			int charCount = countUTF8Chars(word);
			cout << charCount << endl;
			
			// ����Ӧ������*�滻
			string replacement(charCount, '*');
			maskedText.erase(pos, word.length());
			maskedText.insert(pos, string(charCount, '*'));
		}
	}
	return maskedText;
}



int main() {
	//test();
	string pattern;
	string maskedText;
	mess_detection m;
	cout << "������һ���������ַ���\n";
	getline(cin, pattern);
	maskedText = m.maskSensitiveWords(pattern);
	cout << "���˽��Ϊ��\n";
	cout << maskedText << '\n';
}