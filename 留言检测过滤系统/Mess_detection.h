#pragma once
#include<string>
#include<vector>

using namespace std;

#define Max_len 1000 //字符串最大的长度

class mess_detection {
public:
	void init_voc();//初始化词汇库--文件读取
	vector<int> cal_next(const string& pattern);//匹配算法--计算next数组
	vector<int> Kmpsearch(const string& text, const string& pattern);//kmp算法主体，返回字串的起始位置
	string maskSensitiveWords(const string& text);//对输入文本过滤敏感词
public:
 int table_len = 0;
 vector<string> Sensitive_lexicon;
};
