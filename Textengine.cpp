#include <iostream>
#include <fstream>
#include <string>
#include "textEngine.h"
using namespace std;

/*
Visual Studio 2013̫��
������ֲ����
�������< <
��TM����ֲ������...
���˫ƽ̨�١��� 
*/ 
int main(int argc, char *argv[])
{
	//���Ҫ��д
		//�ű�����Ĭ��Ϊ script.te
		string filename = "script.te";
		if (argc >= 2)
			filename = argv[1];
		else
			cout << "Using default script file:" << filename << endl;
		//����textEngine��
		textEngine te(filename);
		te.setItDebug();
		te.init();
		te.doIt();
	return 0;
}
