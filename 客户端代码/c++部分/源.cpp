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

	//传参格式为  xxx.exe param1 param2
	String command = exePath + " " + param1 + " "+param2;
	LPTSTR sConLin = (LPTSTR)(command.c_str());
	//调用函数,
	STARTUPINFO si;
	PROCESS_INFORMATION pi;
	ZeroMemory(&si, sizeof(si));
	ZeroMemory(&pi, sizeof(pi));

	//创建一个新进程  
	if (CreateProcess(
		NULL,   //  指向一个NULL结尾的、用来指定可执行模块的宽字节字符串  
		sConLin, // 命令行字符串  
		NULL, //    指向一个SECURITY_ATTRIBUTES结构体，这个结构体决定是否返回的句柄可以被子进程继承。  
		NULL, //    如果lpProcessAttributes参数为空（NULL），那么句柄不能被继承。<同上>  
		false,//    指示新进程是否从调用进程处继承了句柄。   
		0,  //  指定附加的、用来控制优先类和进程的创建的标  
			//  CREATE_NEW_CONSOLE  新控制台打开子进程  
			//  CREATE_SUSPENDED    子进程创建后挂起，直到调用ResumeThread函数  
		NULL, //    指向一个新进程的环境块。如果此参数为空，新进程使用调用进程的环境  
		NULL, //    指定子进程的工作路径  
		&si, // 决定新进程的主窗体如何显示的STARTUPINFO结构体  
		&pi  // 接收新进程的识别信息的PROCESS_INFORMATION结构体  
	))
	{
		cout << "exe调用成功，等待结果" << endl;
		//下面两行关闭句柄，解除本进程和新进程的关系，不然有可能不小心调用TerminateProcess函数关掉子进程  
//      CloseHandle(pi.hProcess);  
//      CloseHandle(pi.hThread); 
		 // 等待程序结束
		WaitForSingleObject(pi.hProcess, INFINITE);
		CloseHandle(pi.hProcess);
	}
	else {
		cerr << "exe调用失败" << endl;
	}

}
int _tmain(int argc, _TCHAR* argv[])
{
	clock_t start, finish;
	double totaltime;
	start = clock();

	//在这里指定 pyinstaller生成的exe路径 
	String exePath = "C:/dist/simple_request/simple_request.exe";//
	String inputFile = "C:/images/63413.png";//指定图片路径
	String outputDir = "C:/out3/";//指定输出路径
	if (!exists_file(inputFile)) {
		cout << "文件不存在，终止";
		return 0;
	}
	if (_access(outputDir.c_str(), 0) == -1){	//如果文件夹不存在
		cout << "文件夹不存在，开始创建";
		_mkdir(outputDir.c_str());				//则创建
	}
	fromServerGetResult(exePath,outputDir, inputFile);
	finish = clock();
	totaltime = (double)(finish - start) / CLOCKS_PER_SEC;
	cout << "\n此程序的运行时间为" << totaltime << "秒！" << endl;
	return 0;
}
