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
	//Lyt99:����д��QAQ 
		//�ű�����Ĭ��Ϊ script.te
		string filename = "script.te";
		if (argc >= 2)
			filename = argv[1]; //�ļ�����ĵ�һ�������������� 
		else
			cout << "Using default script file:" << filename << endl;
		//����textEngine��
		textEngine te(filename);
		te.setDebug() //���õ���ģʽ����ϲ�����Թص� 
		te.init();
		if(te.doIt())
			cout << "Succeed" << endl;
		else
			cout << "Failed" << endl;
	return 0;
}
