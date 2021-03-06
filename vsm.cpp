



#include "vsm.h"
using namespace std;

typedef std::map<string, vector<double> > STRING2VECTOR;

extern string myDicPath;
extern string featureDicPath;

map<string,int>IDFDic; //只提取字典中IDF(即最后那对值)的字典
set<string> featureDic;//特征词词词典
map<string,vector<double> > txtVSM;
double total = 3903;
//ofstream outFile("E:\\final\\final\\myData\\VSM.txt");
string VSMtestPath = "E:\\final\\final\\myData\\VSMtest11.txt";
ofstream outFile(VSMtestPath.c_str());

void testICTCLAS_ParagraphProcessToSVM(string folderPath,int folderId)  //path开始路径
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
                testICTCLAS_ParagraphProcessToSVM(newPath,folderId+1);
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
				double TF;
				//int txtCnt;//文章总词数
				string words;
				istringstream istream(sRst);
				map<string,int>txt;
				vector<double> tmpVSM;
				set<string> txtWords;
				while(istream>>words)
				{
					txtWords.insert(words);
					if(featureDic.count(words))
					{
						txt[words]++;
					}
				}
				outFile<<FileInfo.name<<endl;
				for(set<string>::iterator setItor = featureDic.begin(); setItor != featureDic.end(); setItor++)
				{
				
					TF = (1.0*txt[(*setItor)]) / txtWords.size();
					TF *= log(0.01 + total/IDFDic[(*setItor)]);
					tmpVSM.push_back(TF);
					outFile<<TF<<" ";
				
				}
				outFile<<endl;
				txtVSM.insert(STRING2VECTOR::value_type(FileInfo.name,tmpVSM));
				tmpVSM.clear();
				txtWords.clear();

		 free(sRst);
        }
	
    }while (_findnext(Handle, &FileInfo) == 0);

		_findclose(Handle);
  
	return ;  
}

int myParagraphProcessToVSM(string folderPath)
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
	
	ifstream featureDicFile(featureDicPath.c_str());
	string featureWord;
	double featureVal;
	while(featureDicFile>>featureWord>>featureVal)
		featureDic.insert(featureWord);
	ifstream dicFile(myDicPath.c_str());
	string term;
	int id,cnt,df;
	while(dicFile>>term)
	{
		
		dicFile>>id>>cnt;
		for(int i = 0; i < cnt; i++)
		{
			dicFile>>id>>df;
		}
		if(featureDic.count(term))
			IDFDic[term] = df;
	}
	for(map<string,int>::iterator it = IDFDic.begin(); it != IDFDic.end(); it++)
		cout<<it->first<<" "<<it->second<<endl;
   //设置词性标注集(0 计算所二级标注集，1 计算所一级标注集，2 北大二级标注集，3 北大一级标注集)
	ICTCLAS_SetPOSmap(2);

    testICTCLAS_ParagraphProcessToSVM(folderPath,0);//分词用例
	
	ICTCLAS_Exit();	//释放资源退出
	outFile.close();
	return 1;
}