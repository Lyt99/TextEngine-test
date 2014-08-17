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
		//脚本名，默认为 script.te
		string filename = "script.te";
		if (argc >= 2)
			filename = argv[1];
		else
			cout << "Using default script file:" << filename << endl;
		//创建textEngine类
		textEngine te(filename);
		te.setItDebug();
		te.init();
		te.doIt();
	return 0;
}
