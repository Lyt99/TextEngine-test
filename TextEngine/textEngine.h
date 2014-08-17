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
Ŀ�������������Զ�����
�ı��������
��˵����ȫдͷ�ļ�����û�����QAQ
~ �����Ⱑ
*/


/**
 * @author Lyt99(*), mkpoli(~)
 * ι������д��ô 
 * 
 ~ ����û��
 ~
 ~ ���У���tm�Ͳ����ڱ����ź���Ӹ��ո�ô
 * ...�´μ� 
 * PS:DevC++��ֱ��(keng)��(die) 
*/

using namespace std;

class textEngine
{
public:
	/*
	��Ҫ��ɵ������б�
	File Done 
	New Done
	Line Done
	Empty Done
	Text Done
	
	������Щһ����Ч��׾�����ŵڶ���д
	���˾Ͷ�������< < 
	Replace
	Insert
	Delete 
	*/

	//DEBUG FUNCTION
	// �ô�д��ĸ�Ƹ��һ�� - mkpoli
	// ������ -Lyt99 
	void setItDebug(){
		printMessage("DEBUG MODE ON.");
		debug = true;
	}

	//��һ���Ƿ��ܹ���д�ļ��ĺ���
	void setOverride(bool bo){
		override = bo;
	} 
	//��ȡ��ǰtextEngine�İ汾
	string static const getVersion(){
		return "0.0.1";
	}

	//���캯��
	textEngine(string sname) :scriptname(sname){}
	/*��������
	*��ʵ��һֱ���������������û���⣿
	*/ 
	~textEngine(){
		if (scriptstream)
			scriptstream.close();
		if (filestream)
			filestream.close();
	}

	//���˹���ʱ���������⣬�������ɴ��趨���ļ�(���˻��ǲ����˻�ӵ���)
	//void setScriptName(string sname){ scriptname = sname; }
	//��ʼ����������ʹ�ù���ǰ����Ҫ���д˺���
	bool init(){
		DEBUG_PRINT("init()" + scriptname);
		if (scriptname.empty()) return false; // �ű��ļ���Ϊ���򲻴�
		scriptstream.open(scriptname); // �򿪽ű��ļ�
		if (scriptstream){
			slines = getAllScriptline(); // ��ȡȫ���ű�������һ������ 
			#pragma region ���ű�Ϊ�����ӡ����
			if(!slines) {
				printError(0,"Script File's Empty!");
				return false;
			}
			#pragma endregion
			printMessage("Loaded scriptfile" + scriptname, false); //printMessage ><,falseΪ����ʾǰ׺TextEngine: (Ĭ��true)
			if (readModFile()) //��ȡ��Ҫ���޸ĵ��ļ�
				return false;
			printMessage("Loaded textfile: " + modFile , false);
			flines = getAllFileLines();//�������Ǹ�getAllScriptline()һ��(��������ɶ�������ʹ�Сд������) 
			if(!flines){
				printError(0,"File "+modFile+" is Empty!");
				return false;
			} 
			printMessage("Using output file name:"+outputfilename,false);
			DEBUG_PRINT("Loaded "+modFile);
			DEBUG_PRINT("Load Commandlist");
			defCommand();//��������б�(map<string,int>)commandlist�� 
			DEBUG_PRINT("Loaded Successful");
			//override = true; //�����õ� 
			if (!canFileWritten()) return false;
			DEBUG_PRINT("Setted File Written");
			ifinit = true;//����initΪtrue��֤���Ѿ����� 
			DEBUG_PRINT("init() return true");
			DEBUG_PRINT(slines);
			DEBUG_PRINT(flines);
			return true;
		}
		printError(0, "Script " + scriptname + " not found.");
		return false;
	}
	
	//��ʽִ�к����� 
	int doIt(){
		if(!ifinit){
			printError(0,"The TextEngine haven't inited now!");//��һ������Ϊ0���ǲ���ʾ�к�
			return 1; 
		}
		vector<cmdwithline> ins;
		vector<cmdwithline> del;
		nums i = 2;
		for(auto a = scriptlines.begin() + 1;a != scriptlines.end();++i,++a){
		//Insert��Delete�ŵ������ִ�� 
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
			//����ִ��
			//NOPE
			continue;
		}
		case 1:
		{
			//����
			//NOPE
			return 1; 
		}
		case 2:{
			//�ǵ�û��Ļ���CommandWithText?
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
	    //д������ļ�������ֵ 
		DEBUG_PRINT(writeToFile(outputstream) ? 0 : 1);
		return 0;
	
}
private:
	//typedef��

	typedef string text;
	typedef string aline;
	typedef string command;
	typedef unsigned int nums;
	typedef pair<int,int> doline; //<-��������ɶ����
	typedef pair<nums,aline> cmdwithline;  

	//���������

	ifstream scriptstream;//�ű�
	ifstream filestream;//�ļ�
	ofstream outputstream;
	string scriptname;//�ű���
	string modFile;//��Ҫ�޸ĵ��ļ���
	string outputfilename = "a.out";//����ļ��� 
	bool ifinit = false;//�Ƿ���سɹ�
	nums slines;//�ű����� 
	nums flines;//�ļ����� 
	//nums load = 2;//�Ѽ��صġ���û���˰�ι�����������á�  <-�����û���� 
	bool written = false;//�Ƿ��д 
	bool debug = false;//IF DEBUG MODE 
	bool override = false;//�Ƿ�д����ļ�
	
	
	//�������ݣ����㿴�ͷֿ�����
	vector<aline> scriptlines;//�ű�����
	vector<aline> filelines;//�ļ�����
	 
	//��һλ������ڶ�λ��ID 
	map<command,int> commandlist;
	//��������

	//�����ﶨ��һ�������б�ͻ�ȡ����ĺ����� 
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
	//����CommandId����Ϊ-1�򲻴��� 
	int getCommandId(command cmd){
			auto it = commandlist.find(cmd);
			if(it == commandlist.end())
				return -1;
			else
			return it -> second; 
	}
	
	//��ȡ��Ҫ�޸ĵ��ļ�,����ֵ0Ϊ��ȡ�ɹ���1Ϊû�м�⵽File���2Ϊ���ļ�ʧ��
	int readModFile(){
		aline File = getaline(scriptlines , 1);//�õ���һ��
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
	//��ȡ�ű�����ȫ�����ݴ���vector<aline> lines����ؽű���������С
	nums getAllScriptline(){
		aline str;
		while (getline(scriptstream, str)){
			scriptlines.push_back(str);
		}
		return scriptlines.size();
	}
	//��ȡ���޸ĵ��ļ������ݴ���vector<aline> flines�������ļ���������С
	nums getAllFileLines(){
		aline str;
		while (getline(filestream, str)){
			filelines.push_back(str);
		}
		return filelines.size();
	}

	//��vector�ж�ȡһ�У�lineΪ��ȡ��������Ĭ��Ϊ0����˳���ȡ(��ȡ��¼������nums load��)
	inline aline getaline(vector<aline> file,nums line = 0){
		if (!line)
			//return file[load++];
			return 0;//�Ȳ������ 
		else
			return file[line - 1];
	}

	//��aline�������л�ȡ�������Line 50��õ�Line
	command getCommand(aline a){
		command i = "";
		for (auto b : a){
			if (b == ' ')  break;
			i += b;
		}
		return i;
	}
	//��ȡ���������ݣ�����Line 50��õ�50
	inline text getText(aline a, command command){
		return a.substr(command.size() + 1);
	}

	//�жϸ������ͷ�����ֵ,0Ϊһ��������(hang),1Ϊ�հ���,2Ϊע����(#��ͷ)
	//һ���÷�if(!getLineType(a)) �������������� 
	int getLineType(aline a){
		DEBUG_PRINT("getLineType");
		if (a.empty())
			return 1;
		else if (a.substr(0, 1) == "#")
			return 2;
		else
			return 0;
	}

	//�����ļ���д��Ϊ��д�ļ���׼��
	//��ʼ���� 
	bool canFileWritten(){
		ifstream file(outputfilename);//����ļ��Ƿ���ڵ� 
		if (file){
			if (!override){
				printError(0, "Output file is exist.Please use override command or change the outfile name");
				return false;
			}
			}
		file.close();//����Ͷ�2333
		return true;
	}
	
	//���޸ĺõ���д���ı�(ȫ����дһ���ι)
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

//���һ������(���)
//����doline(	 pair(int,int) doline)
//doline����;���ṩһ����������int��pair����
//���ڱ�ʾ�кŲ�����ǰֵ��ĩֵ
//��������aline
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
	//���ִ�ӡ��Ϣ�ĺ���������inline������ʡ��Դ 
	
	//�������û�������ͨ��Ϣ 
	inline void printMessage(string message , bool prefix = true){
		if(prefix)
			cout <<"TextEngine: "<< message << endl;
		else
			cout << message << endl;
	}
	
	//�������û����ʹ�����Ϣ 
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
	inline void DEBUG_PRINT(int i){ //+1���� 
	if (debug){
			cout << "[DEBUG] " << i << endl;
		}
	}
	
	//���¶�����
	//����ֵ��ʾִ��״̬,0Ϊ��ȷִ��,1Ϊδ�ɹ�ִ��,2ΪCommandWithText,3ΪCommandReplace,4ΪCommandInsert,5ΪCommandDelete 
	//û���ꣿ���������������� 
	int doCommand(nums line,command cmd,text txt){
		DEBUG_PRINT("Now Working on");
		DEBUG_PRINT(line);
		int id = getCommandId(cmd);
		switch(id){
		default:{
			//�Ҳ������� 
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
			//ֻ�����������һ��
			//�����л������ĵ�� �����ֹ�����ʽ����Lineһ�� 
			/*
			New 123
			Text
			
			New 123-124
			Text
			Text
			*/ 
			//Ϊɶ�����Ī����������е�����....
			//���һ�н������ 
			 
			//�ȴ���һ����
			doline dl = getTextLines(txt);
			if(dl.first <= flines){//��ֹnew���ǵ�ԭ�е��� 
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
				filelines[lines.first - 1] = ""; //���о�ֱ����գ����е��¸㶨
				return 0;
			}
			for(int i = lines.first;i <= lines.second;++i)
			{
				filelines[i - 1] = ""; //forѭ������� 
			}
			return 0;
		}
		case 5:{
			//Text
			printError(line,"Unexpect \"Text\"");//Text������������ 
			return 1;
		}
		case 6:{
			//Insert
			//��CommandInsert���� 
			//*Insert���(Deleteǰ)ͳһ���� 
			return 4;
		}
		case 7:{
			//Delete
			//��CommandDelete����
			//*Delete������� 
			return 5;
		}
		case 8:{
			//Replace
			//����Ҫ�ټӲ�ֻһ������������ 
			//CommandReplace(doline lines,text old,text new) 
			return 3;
		}
		}
	} 
	//дһ������vector�ģ������(���)
	//typedef pair<int,aline> cmdwithline 
	int doCommand(vector<cmdwithline> v){
		for(auto a = v.begin();a != v.end();++a){
			doCommand(a -> first,getCommand(a -> second),getText(a -> second,getCommand(a -> second)));
		}
	}
	
	//������0�ɹ���1ʧ��
	//������ҪText���������� 
	//TodoList:��\n��ת����Ĳ���(�ŵ�getText���)
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
			//getTextLines()����ʱ�ķ�����pair<int,int> (-1,-1) 
			printError(li,"Argument " + l +" is wrong.");
			return 1;
		}
		if(dl.first > flines || dl.second > flines){//�������õ�ֵ���������ڳ����ļ����� 
			printError(li,"Line(s) not found!");
			DEBUG_PRINT(dl.first);
			DEBUG_PRINT(dl.second);
			return 1;
		}
		if(oneLine(dl))
			opt = 1; //get�������� 
		else
			opt = dl.second - dl.first + 1; //get����������������Ϊɶ���뵽��opt���������Ҳ��֪�� 
		if(line.size() != opt + 1){
		printError(li,"Argument Text is missing.");
		return 1;
		}
		//��ʼ���ı����в����� 
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
	
	//�ж�doline��first��second�Ƿ���ͬ 
	inline bool oneLine(doline a){
		return (a.first == a.second);
	}
	
	//�õ����еĵ����� 
	inline vector<aline>::iterator getIt(vector<aline> &vec,nums line){
		return vec.begin() + line - 1;
	}
	
	/*�õ�����Text������Vector������о�������
	Line 1-3
	Text 1
	Text 2
	Text 3
	*/ 
	vector<aline> getVectorWithText(nums &line){//*֮ǰ�Ǹ��ӵ��ˣ��Ƶ���д 
		vector<aline> vec;
		auto it = getIt(scriptlines,line);
		vec.push_back(*it);
		++line;
		++it;
		for(;getCommand(*it) == "Text";++line,++it){
			vec.push_back(*it);
			if(it == scriptlines.end() - 1) //�ž�����δ��Ӧ 
				break;
		}
		return vec;
	}
	
	int replaceString(doline dl, text find, text replacement);
	/*
	*�����õ��ı������� 
	*(vector<aline>)flines - �ݴ��ļ��޸����ݵ�vector������i�����ж�Ӧ��ϵΪflines[i-1] 
	*getIt(vector<aline> vec,nums line) - ������������ȡĳvector<aline>��ĳ�ж�Ӧ�ĵ����� 
    */
};
