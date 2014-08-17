#pragma once
#include <string>
#include <vector>
#include <fstream>
#include <sstream> 
#include <iostream>
#include <map>
#include <cctype>

/* 
Lyt99's TextEngine
My First C++ Application
目的是用来进行自动化的
文本处理操作
话说代码全写头文件里了没问题吧QAQ
~ 有问题啊
*/


/**
 * @author Lyt99(*), mkpoli(~)
 * 喂有这种写法么 
 * 
 ~ 好像没有
 ~
 ~ 还有，你tm就不能在标点符号后面加个空格么
 * ...下次加 
 * PS:DevC++简直良(keng)心(die) 
*/

using namespace std;

class textEngine
{
public:
	/*
	需要完成的命令列表
	File Done 
	New Done
	Line Done
	Empty Done
	Text Done
	
	底下这些一看就效率拙计留着第二版写
	算了就都开坑了< < 
	Replace
	Insert
	Delete 
	*/

	//DEBUG FUNCTION
	// 用大写字母逼格高一点 - mkpoli
	// 已吓尿 -Lyt99 
	void setItDebug(){
		printMessage("DEBUG MODE ON.");
		debug = true;
	}

	//加一个是否能够覆写文件的函数
	void setOverride(bool bo){
		override = bo;
	} 
	//获取当前textEngine的版本
	string static const getVersion(){
		return "0.0.1";
	}

	//构造函数
	textEngine(string sname) :scriptname(sname){}
	/*析构函数
	*其实我一直想问这个析构函数没问题？
	*/ 
	~textEngine(){
		if (scriptstream)
			scriptstream.close();
		if (filestream)
			filestream.close();
	}

	//除了构造时输入名称外，还可以由此设定打开文件(算了还是不用了会坑爹的)
	//void setScriptName(string sname){ scriptname = sname; }
	//初始化函数，在使用功能前必须要运行此函数
	bool init(){
		DEBUG_PRINT("init()" + scriptname);
		if (scriptname.empty()) return false; // 脚本文件名为空则不打开
		scriptstream.open(scriptname); // 打开脚本文件
		if (scriptstream){
			slines = getAllScriptline(); // 读取全部脚本，返回一个行数 
			#pragma region 若脚本为空则打印错误
			if(!slines) {
				printError(0,"Script File's Empty!");
				return false;
			}
			#pragma endregion
			printMessage("Loaded scriptfile" + scriptname, false); //printMessage ><,false为不显示前缀TextEngine: (默认true)
			if (readModFile()) //读取需要被修改的文件
				return false;
			printMessage("Loaded textfile: " + modFile , false);
			flines = getAllFileLines();//和上面那个getAllScriptline()一样(别问我问啥单复数和大小写有问题) 
			if(!flines){
				printError(0,"File "+modFile+" is Empty!");
				return false;
			} 
			printMessage("Using output file name:"+outputfilename,false);
			DEBUG_PRINT("Loaded "+modFile);
			DEBUG_PRINT("Load Commandlist");
			defCommand();//添加命令列表到(map<string,int>)commandlist里 
			DEBUG_PRINT("Loaded Successful");
			//override = true; //调试用的 
			if (!canFileWritten()) return false;
			DEBUG_PRINT("Setted File Written");
			ifinit = true;//设置init为true，证明已经加载 
			DEBUG_PRINT("init() return true");
			DEBUG_PRINT(slines);
			DEBUG_PRINT(flines);
			return true;
		}
		printError(0, "Script " + scriptname + " not found.");
		return false;
	}
	
	//正式执行函数了 
	int doIt(){
		if(!ifinit){
			printError(0,"The TextEngine haven't inited now!");//第一个参数为0就是不显示行号
			return 1; 
		}
		vector<cmdwithline> ins;
		vector<cmdwithline> del;
		nums i = 2;
		for(auto a = scriptlines.begin() + 1;a != scriptlines.end();++i,++a){
		//Insert和Delete放到最后面执行 
		if(getLineType(*a) != 0) 
		{
		DEBUG_PRINT("Skip");
		DEBUG_PRINT(i);
		continue;
	    }
		command cmd = getCommand(*a);
        DEBUG_PRINT("Command " + cmd);
		text txt = getText(*a,cmd);
		DEBUG_PRINT("Text "+txt);
		if(cmd == "Insert"){
			DEBUG_PRINT("Insert");
			ins.push_back(cmdwithline(i,*a));
			continue;
		}
		if(cmd == "Delete"){
			DEBUG_PRINT("Delete");
			del.push_back(cmdwithline(i,*a));
			continue;
		}
		DEBUG_PRINT("Before doCommand()");
		int value = doCommand(i,cmd,txt);
		
		switch(value){
		
		case 0: 
		{
			//正常执行
			//NOPE
			continue;
		}
		case 1:
		{
			//错误
			//NOPE
			return 1; 
		}
		case 2:{
			//记得没错的话是CommandWithText?
			if(doCommandWithText(getVectorWithText(i),i) != 0)
			    return 1;
			a = getIt(scriptlines, i);
			continue;
		}
		case 3:{
			//CommandReplace
			continue;
		}
		case 4:{
			//CommandInsert
			continue;
		}
		case 5:{
			//CommandDelete
			continue;
		}
                     }
	}
	    DEBUG_PRINT("writeToFile");
	    //写到输出文件并返回值 
		DEBUG_PRINT(writeToFile(outputstream) ? 0 : 1);
		return 0;
	
}
private:
	//typedef区

	typedef string text;
	typedef string aline;
	typedef string command;
	typedef unsigned int nums;
	typedef pair<int,int> doline; //<-想起来干啥用了
	typedef pair<nums,aline> cmdwithline;  

	//定义变量区

	ifstream scriptstream;//脚本
	ifstream filestream;//文件
	ofstream outputstream;
	string scriptname;//脚本名
	string modFile;//需要修改的文件名
	string outputfilename = "a.out";//输出文件名 
	bool ifinit = false;//是否加载成功
	nums slines;//脚本行数 
	nums flines;//文件行数 
	//nums load = 2;//已加载的……没用了吧喂……果断启用√  <-最后还是没用了 
	bool written = false;//是否可写 
	bool debug = false;//IF DEBUG MODE 
	bool override = false;//是否覆写输出文件
	
	
	//两个数据，方便看就分开记了
	vector<aline> scriptlines;//脚本数据
	vector<aline> filelines;//文件数据
	 
	//第一位是命令，第二位是ID 
	map<command,int> commandlist;
	//函数区块

	//在这里定义一下命令列表和获取命令的函数吧 
	void defCommand(){
		addCommand(1,"File");
		addCommand(2,"New");
		addCommand(3,"Line");
		addCommand(4,"Empty");
		addCommand(5,"Text");
		addCommand(6,"Insert");
		addCommand(7,"Delete");
		addCommand(8,"Replace");
		DEBUG_PRINT("Command Defined");
		DEBUG_PRINT(commandlist.size()); 
	} 
	inline void addCommand(int id,command cmd)
	{
		commandlist.insert(pair<command,int>(cmd,id));
	}
	//返回CommandId，若为-1则不存在 
	int getCommandId(command cmd){
			auto it = commandlist.find(cmd);
			if(it == commandlist.end())
				return -1;
			else
			return it -> second; 
	}
	
	//读取需要修改的文件,返回值0为读取成功，1为没有检测到File命令，2为打开文件失败
	int readModFile(){
		aline File = getaline(scriptlines , 1);//拿到第一行
		if (getCommand(File) != "File"){
			printError(1, "Couldn't get File command.");
			return 1;
		}
		else{
			modFile = getText(File,"File");
			filestream.open(modFile);
			if (!filestream){
				printError(1, "Couldn't open file " + modFile);
				return 2;
			}
			return 0;
		}
	}
	//读取脚本并将全部数据存入vector<aline> lines里，返回脚本的行数大小
	nums getAllScriptline(){
		aline str;
		while (getline(scriptstream, str)){
			scriptlines.push_back(str);
		}
		return scriptlines.size();
	}
	//读取被修改的文件，数据存入vector<aline> flines，返回文件的行数大小
	nums getAllFileLines(){
		aline str;
		while (getline(filestream, str)){
			filelines.push_back(str);
		}
		return filelines.size();
	}

	//从vector中读取一行，line为读取的行数，默认为0，按顺序读取(读取记录保存在nums load里)
	inline aline getaline(vector<aline> file,nums line = 0){
		if (!line)
			//return file[load++];
			return 0;//先不搞这个 
		else
			return file[line - 1];
	}

	//从aline类型中中获取命令，例如Line 50会得到Line
	command getCommand(aline a){
		command i = "";
		for (auto b : a){
			if (b == ' ')  break;
			i += b;
		}
		return i;
	}
	//获取命令后的内容，例如Line 50会得到50
	inline text getText(aline a, command command){
		return a.substr(command.size() + 1);
	}

	//判断该行类型返回数值,0为一般命令行(hang),1为空白行,2为注释行(#开头)
	//一般用法if(!getLineType(a)) 成立就是命令行 
	int getLineType(aline a){
		DEBUG_PRINT("getLineType");
		if (a.empty())
			return 1;
		else if (a.substr(0, 1) == "#")
			return 2;
		else
			return 0;
	}

	//设置文件可写，为改写文件做准备
	//开始坑了 
	bool canFileWritten(){
		ifstream file(outputfilename);//检测文件是否存在的 
		if (file){
			if (!override){
				printError(0, "Output file is exist.Please use override command or change the outfile name");
				return false;
			}
			}
		file.close();//用完就丢2333
		return true;
	}
	
	//把修改好的行写入文本(全都重写一遍吧喂)
	bool writeToFile(ofstream &stream){
		DEBUG_PRINT(filelines.size());
		stream.open(outputfilename);
		if (!stream) return false;
		for (auto a = filelines.begin(); a != filelines.end(); ++a){
			if(a != filelines.begin()) stream << endl;
			stream << *a << flush;
		}
		DEBUG_PRINT("File Writed");
		return true;
	}

//最后一个坑了(大概)
//返回doline(	 pair(int,int) doline)
//doline的用途是提供一个存有两个int的pair数据
//用于表示行号参数的前值和末值
//参数带入aline
doline getTextLines(aline textline){
		bool tline = false;
		int a = 0,b = 0;
		int *ptr = &a;
		doline rev;
		rev = make_pair(-1,-1);
		string tmp;
		if(textline[0] == '0' || textline[0] == '-'){
			return rev;
		}
		for(char c: textline){
			if(!isdigit(c)){
			if(c == '-') continue;
				return rev;
			}
		}
		for(char c : textline){
			if(c == '-' && !tline){
				*ptr = atoi(tmp.c_str());
				tline = true;
				ptr = &b;
				tmp = "";
				continue;
			}
				tmp += c;
		}
		*ptr = atoi(tmp.c_str());
		if(tline){
			if(a < b){
				rev = make_pair(a,b);
				return rev;
			}
			else
				return rev;
		}
		rev = make_pair(a,a);
		return rev;
	}  
	//各种打印信息的函数，果断inline内联节省资源 
	
	//程序向用户发送普通信息 
	inline void printMessage(string message , bool prefix = true){
		if(prefix)
			cout <<"TextEngine: "<< message << endl;
		else
			cout << message << endl;
	}
	
	//程序向用户发送错误信息 
	inline void printError(nums line, string error){
		cout << "TextEngine Error" << flush;
		if(line == 0)
			cout << ": "+ error <<endl;
		else
			cout << " at line "<< line << ": " << error << endl;
	}

	//DEBUG MESSAGE 
	inline void DEBUG_PRINT(string str){
		if (debug){
			cout << "[DEBUG] " << str << endl;
		}
	}
	
	//DEBUG MESSAGE PRINT INT
	inline void DEBUG_PRINT(int i){ //+1重载 
	if (debug){
			cout << "[DEBUG] " << i << endl;
		}
	}
	
	//正事儿来了
	//返回值表示执行状态,0为正确执行,1为未成功执行,2为CommandWithText,3为CommandReplace,4为CommandInsert,5为CommandDelete 
	//没坑完？好像这个函数完成了 
	int doCommand(nums line,command cmd,text txt){
		DEBUG_PRINT("Now Working on");
		DEBUG_PRINT(line);
		int id = getCommandId(cmd);
		switch(id){
		default:{
			//找不到命令 
			printError(line,"Command \"" + cmd + "\" couldn't be found.");
			return 1;
		}
		case 1:{
			//File
			printError(line,"Unexpect \"File\"");
			return 0;
		}
		case 2:{
			//New
			//只允许往下添加一行
			//↑不行还是良心点吧 ，两种工作方式，和Line一样 
			/*
			New 123
			Text
			
			New 123-124
			Text
			Text
			*/ 
			//为啥会出现莫名其妙多两行的问题....
			//这边一行解决掉了 
			 
			//先创建一下行
			doline dl = getTextLines(txt);
			if(dl.first <= flines){//防止new覆盖掉原有的行 
				printError(line,"Unable to crate new line,the line(s) is(are) exist.");
			} 
			if(oneLine(dl)){
				if(dl.first > flines + 1){
					int opt = dl.first - flines;
					for(;opt >0;--opt){
						filelines.push_back("");
					}
				}
				else
					filelines.push_back("");
			}
			else{
				int opt = dl.second - flines;
				for(;opt >0;--opt){
					filelines.push_back("");
				}
			}
			flines = filelines.size();
			DEBUG_PRINT("New line created");
			DEBUG_PRINT(filelines.size());
			return 2;
		}
		case 3:{
			//Line
			return 2;
		}
		case 4:{
			//Empty
			doline lines = getTextLines(txt);
			if(oneLine(lines)){
				filelines[lines.first - 1] = ""; //单行就直接清空，多行底下搞定
				return 0;
			}
			for(int i = lines.first;i <= lines.second;++i)
			{
				filelines[i - 1] = ""; //for循环解决掉 
			}
			return 0;
		}
		case 5:{
			//Text
			printError(line,"Unexpect \"Text\"");//Text不允许单独出现 
			return 1;
		}
		case 6:{
			//Insert
			//用CommandInsert函数 
			//*Insert最后(Delete前)统一处理 
			return 4;
		}
		case 7:{
			//Delete
			//用CommandDelete函数
			//*Delete最最后处理 
			return 5;
		}
		case 8:{
			//Replace
			//好像还要再加不只一个函数的样子 
			//CommandReplace(doline lines,text old,text new) 
			return 3;
		}
		}
	} 
	//写一个接受vector的，方便点(大概)
	//typedef pair<int,aline> cmdwithline 
	int doCommand(vector<cmdwithline> v){
		for(auto a = v.begin();a != v.end();++a){
			doCommand(a -> first,getCommand(a -> second),getText(a -> second,getCommand(a -> second)));
		}
	}
	
	//老样子0成功，1失败
	//处理需要Text函数的命令 
	//TodoList:对\n等转义符的操作(放到getText里把)
	int doCommandWithText(vector<aline> line , nums li){
		DEBUG_PRINT("Do command With Text");
		nums opt;
		doline dl;
		DEBUG_PRINT(getCommand(line[0]));
		text l = getText(line[0],getCommand(line[0]));
		DEBUG_PRINT("GettedText");
		dl = getTextLines(l);
		DEBUG_PRINT("GettedTextLines");
		if(dl.first == -1)
		{
			//getTextLines()出错时的返回是pair<int,int> (-1,-1) 
			printError(li,"Argument " + l +" is wrong.");
			return 1;
		}
		if(dl.first > flines || dl.second > flines){//假如设置的值过大以致于超过文件行数 
			printError(li,"Line(s) not found!");
			DEBUG_PRINT(dl.first);
			DEBUG_PRINT(dl.second);
			return 1;
		}
		if(oneLine(dl))
			opt = 1; //get操作行数 
		else
			opt = dl.second - dl.first + 1; //get操作行数，别问我为啥会想到起opt这个名，我也不知道 
		if(line.size() != opt + 1){
		printError(li,"Argument Text is missing.");
		return 1;
		}
		//开始对文本进行操作了 
		DEBUG_PRINT("for");
		DEBUG_PRINT(dl.first);
		DEBUG_PRINT(dl.second);
		DEBUG_PRINT(line.size());
		for(nums i = dl.first;i <= dl.second;++i){
			DEBUG_PRINT("for()");
			filelines[i - 1] = getText(line[i - dl.first + 1],"Text");
			DEBUG_PRINT("=");
		}
		return 0;
	}
	
	//判断doline的first和second是否相同 
	inline bool oneLine(doline a){
		return (a.first == a.second);
	}
	
	//得到该行的迭代器 
	inline vector<aline>::iterator getIt(vector<aline> &vec,nums line){
		return vec.begin() + line - 1;
	}
	
	/*得到含有Text函数的Vector，大体感觉是这样
	Line 1-3
	Text 1
	Text 2
	Text 3
	*/ 
	vector<aline> getVectorWithText(nums &line){//*之前那个坑爹了，推倒重写 
		vector<aline> vec;
		auto it = getIt(scriptlines,line);
		vec.push_back(*it);
		++line;
		++it;
		for(;getCommand(*it) == "Text";++line,++it){
			vec.push_back(*it);
			if(it == scriptlines.end() - 1) //杜绝各种未响应 
				break;
		}
		return vec;
	}
	
	int replaceString(doline dl, text find, text replacement);
	/*
	*可能用到的变量或函数 
	*(vector<aline>)flines - 暂存文件修改数据的vector，行数i在其中对应关系为flines[i-1] 
	*getIt(vector<aline> vec,nums line) - 内联函数，获取某vector<aline>中某行对应的迭代器 
    */
};
