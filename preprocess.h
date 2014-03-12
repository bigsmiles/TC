#ifndef _PREPROCESS_H
#define _PREPROCESS_H
/*
这个文件主要完成对文本的分词，然后形成字典myDic.txt;

*/
#pragma warning(disable: 4786)  //预防warning 太多。。。。
#include "common.h"

using namespace std;





int myParagraphProcess(string folderPath);
void testICTCLAS_ParagraphProcess(string folderPath,int folderId);

#endif