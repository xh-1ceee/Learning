#pragma once
#include<string>
#include<iostream>
using namespace std;
class Error {
private:
	string err[26] = {
		"标识符长度大于最大长度",
		"常量定义结尾缺少分号",
		"变量定义结尾缺少分号",
		"过程定义结尾缺少分号",
		"常量定义非标识符",
		"常量定义应使用=",
		"变量定义非标识符",
		"过程定义非标识符",
		"未定义标识符",
		"为非变量赋值",
		"无效的赋值号",
		"if没有对应的then",
		"while没有对应的do",
		"复合语句的语句间分号丢失",
		"begin为找到对应的end",
		"call调用非法内容",
		"call调用非过程标识符",
		"读写未定义标识符",
		"读非变量标识符",
		"读操作左括号缺少",
		"读操作右括号缺少",
		"写操作右括号缺少",
		"写操作左括号缺少",
		"变量赋值涉及过程标识符",
		"变量赋值右括号缺少",
		"非法运算符"
	};
public:
	void printErr(int i)
	{
		cout << err[i] << endl;
	}
};