/******************************************************************************* 

*******************************************************************************/

#ifndef OS_LINUX
#include <Windows.h>
#endif

#define _VSM

#include "preprocess.h"
#include "feature.h"
#include "vsm.h"
#include "classifier.h"

string textDataPath = "E:\\finalData\\1_train";
string textDataPath2 = "E:\\finalData\\2_answer";
string VSMPath = "E:\\final\\final\\myData\\SVM.txt";
//string testVSMPath = "E:\\final\\final\\myData\\VSMtest.txt";
int main()
{
    //myParagraphProcess("E:\\finalData\\1_train");//��Ŀ¼�µ��ı��ִʺ󣬱���ΪmyDic.txt
	//chiFeatureSelect();
	 //myParagraphProcessToVSM(textDataPath,VSMPath);
	 //myParagraphProcessToVSM(textDataPath2);
	 KNN();

	return 0;
}


