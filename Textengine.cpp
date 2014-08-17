#include <iostream>
#include <fstream>
#include <string>
#include "textEngine.h"
using namespace std;

/*
Visual Studio 2013太坑
果断移植出来
舒服多了< <
我TM又移植回来了...
这次双平台操♂作 
*/ 
int main(int argc, char *argv[])
{
	
	//这段要重写
	//Lyt99:懒得写了QAQ 
		//脚本名，默认为 script.te
		string filename = "script.te";
		if (argc >= 2)
			filename = argv[1]; //文件名后的第一个参数可以配置 
		else
			cout << "Using default script file:" << filename << endl;
		//创建textEngine类
		textEngine te(filename);
		te.setDebug() //设置调试模式，不喜欢可以关掉 
		te.init();
		if(te.doIt())
			cout << "Succeed" << endl;
		else
			cout << "Failed" << endl;
	return 0;
}
