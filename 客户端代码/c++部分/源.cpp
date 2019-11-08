#include <stdlib.h>
#include <iostream>  
#include <tchar.h> 
#include<time.h>
#include "opencv2/core/core.hpp"
#include "opencv2/highgui/highgui.hpp"
#include "opencv2/imgproc/imgproc.hpp"
#include "opencv2/photo/photo.hpp"
#include <iostream>
#include <string>
#include<Windows.h> 
#include <direct.h>
#include <corecrt_io.h>
using namespace std;
using namespace cv;
void string_replace(std::string& strBig, const std::string& strsrc, const std::string& strdst)
{
	std::string::size_type pos = 0;
	std::string::size_type srclen = strsrc.size();
	std::string::size_type dstlen = strdst.size();

	while ((pos = strBig.find(strsrc, pos)) != std::string::npos)
	{
		strBig.replace(pos, srclen, strdst);
		pos += dstlen;
	}
}
std::string GetPathOrURLShortName(std::string strFullName)
{
	if (strFullName.empty())
	{
		return "";
	}

	string_replace(strFullName, "/", "\\");

	std::string::size_type iPos = strFullName.find_last_of('\\') + 1;

	return strFullName.substr(iPos, strFullName.length() - iPos);
}
bool exists_file(const std::string& name) {
	if (FILE * file = fopen(name.c_str(), "r")) {
		fclose(file);
		return true;
	}
	else {
		return false;
	}
}

void fromServerGetResult(cv::String exePath,String outputDir, cv::String filename) {
	String param1 = "--outputDir "+ outputDir;
	String param2 = "--file " + filename;

	//���θ�ʽΪ  xxx.exe param1 param2
	String command = exePath + " " + param1 + " "+param2;
	LPTSTR sConLin = (LPTSTR)(command.c_str());
	//���ú���,
	STARTUPINFO si;
	PROCESS_INFORMATION pi;
	ZeroMemory(&si, sizeof(si));
	ZeroMemory(&pi, sizeof(pi));

	//����һ���½���  
	if (CreateProcess(
		NULL,   //  ָ��һ��NULL��β�ġ�����ָ����ִ��ģ��Ŀ��ֽ��ַ���  
		sConLin, // �������ַ���  
		NULL, //    ָ��һ��SECURITY_ATTRIBUTES�ṹ�壬����ṹ������Ƿ񷵻صľ�����Ա��ӽ��̼̳С�  
		NULL, //    ���lpProcessAttributes����Ϊ�գ�NULL������ô������ܱ��̳С�<ͬ��>  
		false,//    ָʾ�½����Ƿ�ӵ��ý��̴��̳��˾����   
		0,  //  ָ�����ӵġ���������������ͽ��̵Ĵ����ı�  
			//  CREATE_NEW_CONSOLE  �¿���̨���ӽ���  
			//  CREATE_SUSPENDED    �ӽ��̴��������ֱ������ResumeThread����  
		NULL, //    ָ��һ���½��̵Ļ����顣����˲���Ϊ�գ��½���ʹ�õ��ý��̵Ļ���  
		NULL, //    ָ���ӽ��̵Ĺ���·��  
		&si, // �����½��̵������������ʾ��STARTUPINFO�ṹ��  
		&pi  // �����½��̵�ʶ����Ϣ��PROCESS_INFORMATION�ṹ��  
	))
	{
		cout << "exe���óɹ����ȴ����" << endl;
		//�������йرվ������������̺��½��̵Ĺ�ϵ����Ȼ�п��ܲ�С�ĵ���TerminateProcess�����ص��ӽ���  
//      CloseHandle(pi.hProcess);  
//      CloseHandle(pi.hThread); 
		 // �ȴ��������
		WaitForSingleObject(pi.hProcess, INFINITE);
		CloseHandle(pi.hProcess);
	}
	else {
		cerr << "exe����ʧ��" << endl;
	}

}
int _tmain(int argc, _TCHAR* argv[])
{
	clock_t start, finish;
	double totaltime;
	start = clock();

	//������ָ�� pyinstaller���ɵ�exe·�� 
	String exePath = "C:/dist/simple_request/simple_request.exe";//
	String inputFile = "C:/images/63413.png";//ָ��ͼƬ·��
	String outputDir = "C:/out3/";//ָ�����·��
	if (!exists_file(inputFile)) {
		cout << "�ļ������ڣ���ֹ";
		return 0;
	}
	if (_access(outputDir.c_str(), 0) == -1){	//����ļ��в�����
		cout << "�ļ��в����ڣ���ʼ����";
		_mkdir(outputDir.c_str());				//�򴴽�
	}
	fromServerGetResult(exePath,outputDir, inputFile);
	finish = clock();
	totaltime = (double)(finish - start) / CLOCKS_PER_SEC;
	cout << "\n�˳��������ʱ��Ϊ" << totaltime << "�룡" << endl;
	return 0;
}
