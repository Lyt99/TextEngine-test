#include <iostream>
#include <fstream>
#include <string>
#include "textEngine.h"
using namespace std;

/*
* 胡妹用VS2012 我用DevC++系列 
*/ 
int main(int argc, char *argv[])
{
	
	//这段要重写
	//Lyt99:懒得写了QAQ 
	//还是写点儿吧< <
		//脚本名，默认为 script.te
		string filename = "script.te";
		if (argc >= 2)
			filename = argv[1]; //文件名后的第一个参数可以配置为脚本名 
		else
			cout << "Using default script file:" << filename << endl;
		//创建textEngine类
		textEngine te(filename);
		te.setItDebug(); //设置调试模式，不喜欢可以(手动)关掉 
		te.init();
		te.setOverride(true); //覆写< < 
		if(te.doIt() == 0)
			cout << "Succeed" << endl;
		else
			cout << "Failed" << endl;
	return 0;
}
/**
 * 用这种注释格式是一个Java程序员的悲哀
 * 好了，这个函数式用来替换制定行的文字的
 *
 * Lyt99:不应该是textEngine::replaceString并且貌似还不是放在这个文件里的...
 * 自行创建一个textEngine.cpp √
 * 算了帮你转移了
 * 卧槽这文件就叫Textengine.cpp 
 */
//其实这么写注释你把鼠标移到函数上就会有显示的23333(VS2013亲测可用) 
int textEngine::replaceString(doline dl, text find, text replacement)
{
	// TODO: GET TEXT
	// TODO: REPLACE
	// TODO: RETURN TEXT
}
