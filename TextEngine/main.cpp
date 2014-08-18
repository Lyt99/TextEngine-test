#include <iostream>
#include <fstream>
#include <string>
#include "textEngine.cpp"
using namespace std;

/*
*头文件函数转移失败，乖乖回来重写前端 
*/ 
inline bool ifText(string str){
	return (str.substr(0,1) == "-") ? false : true;//保险就这么写了 
}
int main(int argc, char *argv[])
{
	textEngine te;
	string scriptname = "script.te" , outputname = "a.out";
	if(argc == 1){
		cout << "Lyt99's TextEngine\n"
			 << "Author: Lyt99 & mkpoli\n"
			 << "Version:" << textEngine::getVersion() <<"\n"
			 << "Type " << argv[0] << " --help for more info" << endl;
			 return 0;
	}
	if(argc == 2 && static_cast<string>(argv[1]) == "--help"){ //static_cast黑科技 
		cout << "TextEngine's Help\n"
			 << argv[0] << " <options> [arguments]\n"
			 << "Options: -script -output -override\n"
			 << "E.g: textengine -script script.te -output a.out -override\n"
			 << "Notice:Default scriptname: script.te.  Default outputname: a.out."
			 << endl;
			 return 0;
	}
	for(int a = 1;a < argc + 1;++a){
		if(a == argc)
			break;
		string b = argv[a]; 
		string c = "--";
		if(a != argc - 1)
			c = argv[a + 1];
			
		if(b == "-script")
			if(ifText(c)){
				scriptname = c;
				++a;
				continue;
			}
			else{
				cout << "Missing argument for command " + b << endl;
				return 0;
			}		
				
		if(b == "-debug"){
			te.setItDebug();
			continue;
		}
			
		if(b == "-output")
			if(ifText(c)){
				outputname = c;
				++a;
				continue;
			}
			else{
				cout << "Missing argument for command " + b << endl;
				return 0;
			}
			
		if(b == "-override"){
			te.setOverride(true);
			continue;
		}

		cout << "Unknown command " + b << endl;
		return 0;
	}
	te.setFilename(scriptname);
	te.setOutputFile(outputname);
	if(te.init()){
		int result = te.doIt();
		if(!result)
			cout << "Succeed." << endl;
		else
			cout << "Failed." << endl;
}
	else 
		return 0;
 

	return 0;
}
