#include <iostream>
#include <fstream>
#include <string>
#include "textEngine.h"
using namespace std;

/*
* ������VS2012 ����DevC++ϵ�� 
*/ 
int main(int argc, char *argv[])
{
	
	//���Ҫ��д
	//Lyt99:����д��QAQ 
	//����д�����< <
		//�ű�����Ĭ��Ϊ script.te
		string filename = "script.te";
		if (argc >= 2)
			filename = argv[1]; //�ļ�����ĵ�һ��������������Ϊ�ű��� 
		else
			cout << "Using default script file:" << filename << endl;
		//����textEngine��
		textEngine te(filename);
		te.setItDebug(); //���õ���ģʽ����ϲ������(�ֶ�)�ص� 
		te.init();
		te.setOverride(true); //��д< < 
		if(te.doIt() == 0)
			cout << "Succeed" << endl;
		else
			cout << "Failed" << endl;
	return 0;
}
/**
 * ������ע�͸�ʽ��һ��Java����Ա�ı���
 * ���ˣ��������ʽ�����滻�ƶ��е����ֵ�
 *
 * Lyt99:��Ӧ����textEngine::replaceString����ò�ƻ����Ƿ�������ļ����...
 * ���д���һ��textEngine.cpp ��
 * ���˰���ת����
 * �Բ����ļ��ͽ�Textengine.cpp 
 */
//��ʵ��ôдע���������Ƶ������Ͼͻ�����ʾ��23333(VS2013�ײ����) 
int textEngine::replaceString(doline dl, text find, text replacement)
{
	// TODO: GET TEXT
	// TODO: REPLACE
	// TODO: RETURN TEXT
}
