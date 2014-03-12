#include "preprocess.h"

#pragma comment(lib, "ICTCLAS50.lib") //ICTCLAS50.lib����뵽������

string stopWordsPath = "E:\\final\\final\\myData\\stopwords.txt"; //��¼ͣ�ôʴʵ��·��
string myDicPath = "E:\\final\\final\\myData\\myDic.txt"; //���Ͽ�ʵ��·��
map<string,map<int,int> >myDic;
set<string>stopDic;	//ͣ�ôʴʵ�


void testICTCLAS_ParagraphProcess(string folderPath,int folderId)  //path��ʼ·��
{

	long Handle;
	struct _finddata_t FileInfo;

	string fpath = folderPath + "\\*";
	if((Handle = _findfirst(fpath.c_str(),&FileInfo)) == -1L)  //����Ŀ¼�µ��ļ�
	{
		printf("û���ҵ�ƥ�����Ŀ");
		exit(-1);
	}

	do{
			//�ж��Ƿ�����Ŀ¼
			if (FileInfo.attrib & _A_SUBDIR)    
			{
				 //���������Ҫ
				if( (strcmp(FileInfo.name,".") != 0 ) &&(strcmp(FileInfo.name,"..") != 0))   
				{
					string newPath = folderPath + "\\" + FileInfo.name;
					cout<<"Ŀ¼��"<<newPath<<endl; 
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

				while(ifile>>ch)	//��������Ϊ�˽����з�ȥ����ASCII��10��13
				{
					 if(ch == '\n' || ch == '\r')
						 continue;
					 sSentence[len++] = ch;
				}
		
				sSentence[len] = '\0';
				unsigned int nPaLen=strlen(sSentence); // ��Ҫ�ִʵĳ���
				char* sRst=0;   //�û����з���ռ䣬���ڱ�������
				sRst=(char *)malloc(nPaLen*6); //���鳤��Ϊ�ַ������ȵı���
				int nRstLen=0; //�ִʽ���ĳ���
			
				nRstLen = ICTCLAS_ParagraphProcess(sSentence,nPaLen,sRst,CODE_TYPE_UNKNOWN,0);  //�ַ�������
				/*�ռ����ʣ��γ��ֵ�*/

				//cout<<"Ŀ¼Ϊ��"<<folderId<<endl;
				string words;
				istringstream istream(sRst);
				set<string> txtDic; //��ʾһƪ���µĴʵ�,Ϊ��ͳ�ƴʵ��ĵ�Ƶ��DF
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
	if(!ICTCLAS_Init()) //��ʼ���ִ������
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

   //���ô��Ա�ע��(0 ������������ע����1 ������һ����ע����2 ���������ע����3 ����һ����ע��)
	ICTCLAS_SetPOSmap(2);
    testICTCLAS_ParagraphProcess(folderPath,0);//�ִ�����
	ICTCLAS_Exit();	//�ͷ���Դ�˳�
	
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
	printf("\n�����ִʣ�%d\n",dic_num);
	myDic.clear(); //�Ѿ�������Ӳ���ˣ���������˰ɣ�
	return 1;
}