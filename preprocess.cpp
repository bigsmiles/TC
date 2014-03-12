#include "preprocess.h"

#pragma comment(lib, "ICTCLAS50.lib") //ICTCLAS50.lib库加入到工程中

string stopWordsPath = "E:\\final\\final\\myData\\stopwords.txt"; //记录停用词词典的路径
string myDicPath = "E:\\final\\final\\myData\\myDic.txt"; //语料库词典的路径
map<string,map<int,int> >myDic;
set<string>stopDic;	//停用词词典


void testICTCLAS_ParagraphProcess(string folderPath,int folderId)  //path开始路径
{

	long Handle;
	struct _finddata_t FileInfo;

	string fpath = folderPath + "\\*";
	if((Handle = _findfirst(fpath.c_str(),&FileInfo)) == -1L)  //遍历目录下的文件
	{
		printf("没有找到匹配的项目");
		exit(-1);
	}

	do{
			//判断是否有子目录
			if (FileInfo.attrib & _A_SUBDIR)    
			{
				 //这个语句很重要
				if( (strcmp(FileInfo.name,".") != 0 ) &&(strcmp(FileInfo.name,"..") != 0))   
				{
					string newPath = folderPath + "\\" + FileInfo.name;
					cout<<"目录名"<<newPath<<endl; 
					testICTCLAS_ParagraphProcess(newPath,folderId+1);
					folderId += 1;
				}
			}
			else  
			{
				char* sSentence =  (char*)malloc(FileInfo.size);
				char ch;
				int len = 0;
				string filepath = folderPath + "\\" + FileInfo.name;
				ifstream ifile(filepath.c_str());

				while(ifile>>ch)	//这样读入为了将换行符去掉，ASCII码10、13
				{
					 if(ch == '\n' || ch == '\r')
						 continue;
					 sSentence[len++] = ch;
				}
		
				sSentence[len] = '\0';
				unsigned int nPaLen=strlen(sSentence); // 需要分词的长度
				char* sRst=0;   //用户自行分配空间，用于保存结果；
				sRst=(char *)malloc(nPaLen*6); //建议长度为字符串长度的倍。
				int nRstLen=0; //分词结果的长度
			
				nRstLen = ICTCLAS_ParagraphProcess(sSentence,nPaLen,sRst,CODE_TYPE_UNKNOWN,0);  //字符串处理
				/*收集单词，形成字典*/

				//cout<<"目录为："<<folderId<<endl;
				string words;
				istringstream istream(sRst);
				set<string> txtDic; //表示一篇文章的词典,为了统计词的文档频率DF
				while(istream>>words)
				{

					if((!txtDic.count(words)) && (!stopDic.count(words)))
					{
						++myDic[words][9];
						++myDic[words][folderId];
						txtDic.insert(words);
					}
				}
				txtDic.clear();
				free(sRst);
			}
	
    }while (_findnext(Handle, &FileInfo) == 0);

	_findclose(Handle);
	return ;  
}

int myParagraphProcess(string folderPath)
{
	if(!ICTCLAS_Init()) //初始化分词组件。
	{
		printf("Init fails\n");  
		return 0;
	}
	else
	{
		printf("Init ok\n");
	}

	ifstream stopDicFile(stopWordsPath.c_str());//E:\final\final\myData\stopwords.txt
	string stopWord;
	while(stopDicFile>>stopWord)
		stopDic.insert(stopWord);

   //设置词性标注集(0 计算所二级标注集，1 计算所一级标注集，2 北大二级标注集，3 北大一级标注集)
	ICTCLAS_SetPOSmap(2);
    testICTCLAS_ParagraphProcess(folderPath,0);//分词用例
	ICTCLAS_Exit();	//释放资源退出
	
	ofstream ofile(myDicPath.c_str());	//E:\final\final\myData\myDic.txt
	map<string,map<int,int> >::iterator map_it2;
	map<int,int>::iterator map_it3;
	int dic_num = 0;
	dic_num = 0;
	int flag = 0;
	for(map_it2 = myDic.begin(); map_it2 != myDic.end(); map_it2++)
	{
		ofile<<map_it2->first<<endl;
		ofile<<flag<<" "<<map_it2->second.size();
		dic_num++;
		for(map_it3 = map_it2->second.begin(); map_it3 != map_it2->second.end(); map_it3++)
		{
			ofile<<" "<<map_it3->first<<" "<<map_it3->second;
		}
		ofile<<endl;
	}
	printf("\n共出现词：%d\n",dic_num);
	myDic.clear(); //已经保存在硬盘了，可以清空了吧？
	return 1;
}