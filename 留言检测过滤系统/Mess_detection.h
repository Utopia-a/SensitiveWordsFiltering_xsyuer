#pragma once
#include<string>
#include<vector>

using namespace std;

#define Max_len 1000 //�ַ������ĳ���

class mess_detection {
public:
	void init_voc();//��ʼ���ʻ��--�ļ���ȡ
	vector<int> cal_next(const string& pattern);//ƥ���㷨--����next����
	vector<int> Kmpsearch(const string& text, const string& pattern);//kmp�㷨���壬�����ִ�����ʼλ��
	string maskSensitiveWords(const string& text);//�������ı��������д�
public:
 int table_len = 0;
 vector<string> Sensitive_lexicon;
};
