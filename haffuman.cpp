#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <windows.h>
#define _CRT_SECURE_NO_DEPRECATE
#define _CRT_SECURE_NO_WARNINGS
#pragma warning(disable:4996);
#define MaxSize 1000
#define N 1000


//暂时存放编码或者译码输出内容，用来保存到文件
char textTemp[N];

typedef struct HuffmanNode //定义哈夫曼树各结点
{
	char c;
	int weight;  //权值
	int parent;  //父节点
	int lchild, rchild;  //左右孩子编号
}HuffmanNode;

typedef struct
{
	char cd[N];                 //存放哈夫曼码
	int start;                  //从start开始读cd中的哈夫曼码
}HCode;

//创建哈夫曼树
void CreateHT(HuffmanNode ht[], int n)                //调用输入的数组ht[],和节点数n
{
	int i, k, lnode, rnode;
	int min1, min2;
	for (i = 0; i<2 * n - 1; i++) {
		ht[i].parent = ht[i].lchild = ht[i].rchild = -1;    //所有结点的相关域置初值-1
	}
	for (i = n; i<2 * n - 1; i++)                   //构造哈夫曼树
	{
		min1 = min2 = 32767;                //int的范围是-32768-32767
		lnode = rnode = -1;                  //lnode和rnode记录最小权值的两个结点位置
		for (k = 0; k <= i - 1; k++)
		{
			if (ht[k].parent == -1)               //只在尚未构造二叉树的结点中查找
			{
				if (ht[k].weight<min1)           //若权值小于最小的左节点的权值
				{
					min2 = min1;
					rnode = lnode;
					min1 = ht[k].weight;
					lnode = k;
				}
				else if (ht[k].weight<min2)
				{
					min2 = ht[k].weight;
					rnode = k;
				}
			}
		}
		ht[lnode].parent = i;
		ht[rnode].parent = i;                //两个最小节点的父节点是i
		ht[i].weight = ht[lnode].weight + ht[rnode].weight;       //两个最小节点的父节点权值为两个最小节点权值之和
		ht[i].lchild = lnode;
		ht[i].rchild = rnode;                 //父节点的左节点和右节点
		ht[i].parent = -1;         //表示新结点应该是根结点
	}
}

/*
根据哈夫曼树求哈夫曼编码
HuffmanNode[] 哈夫曼树
HCode[]  编码表
*/
void CreateHCode(HuffmanNode ht[], HCode hcd[], int n) //创建编码表
{
	int i, f, c;
	HCode hc;
	for (i = 0; i<n; i++)                            //根据哈夫曼树求哈夫曼编码
	{
		hc.start = n;
		c = i;
		f = ht[i].parent;
		while (f != -1)                              //循序直到树根结点结束循环
		{
			if (ht[f].lchild == c) {                        //处理左孩子结点
				hc.cd[hc.start--] = '0';
			}
			else {                                    //处理右孩子结点
				hc.cd[hc.start--] = '1';
			}
			c = f;
			f = ht[f].parent;
		}
		hc.start++;                               //start指向哈夫曼编码hc.cd[]中最开始字符
		hcd[i] = hc;
	}
}

//输出哈夫曼编码
void DispHCode(HuffmanNode ht[], HCode hcd[], int n)     //输出哈夫曼编码的列表
{
	//if(ht[0].weight==NULL) printf("hh");
	int i, k;
	printf("  输出哈夫曼编码:\n");
	for (i = 0; i<n; i++)									//输出data中的所有数据，即a-z
	{
		printf("      %c:\t", ht[i].c);
		for (k = hcd[i].start; k <= n; k++)                    //输出所有data中数据的编码
		{
			printf("%c", hcd[i].cd[k]);
		}
		printf("\n");
	}
}

void saveText()

{
	FILE *fp;
	char temp[20];
	printf("是否需要把结果保存到文件y/n");
	scanf("%s", temp);
	if (temp[0] == 'n' || temp[0] == 'N')
	{
		return;
	}
	printf("输入文件名");
	scanf("%s", temp);
	fp = fopen(temp, "w");
	if (!fp)
	{
		printf("打开或者生成文件失败\n");
		return;
	}
	for (int i = 0; textTemp[i]!='\0'; i++)
	{
		fprintf(fp, "%c", textTemp[i]);
	}
	fclose(fp);
}

/*
根据编码表对字符串进行编码

char* string  需要编码的字符串数组
HuffmanNode[] 哈夫曼树
HCode[]  编码表
*/
void enHCode(char* string, HuffmanNode ht[], HCode hcd[], int n)
{
	int i, j, k;
	int count = 0;
	memset(textTemp, 0, sizeof(textTemp));
	for (i = 0; string[i] != '\0'; i++)
	{
		for (j = 0; j<n; j++)
		{
			if (string[i] == ht[j].c)            //循环查找与输入字符相同的编号，相同的就输出这个字符的编码
			{
				for (k = hcd[j].start; k <= n; k++)
				{
					printf("%c", hcd[j].cd[k]);
					textTemp[count] = hcd[j].cd[k];
					count++;
				}
				break;                      //输出完成后跳出当前for循环
			}
		}
		if (j == n) {
			printf("\n%c未编码，编码表中不存在该符号", string[i]);
			//return;
		}
		
	}
	printf("\n");
	saveText();
}
	


/*
char* temp 需要译码的01字符串
HuffmanNode[] 哈夫曼树
HCode[]  编码表
*/
void deHcode(HuffmanNode ht[], int n,char* temp)
{
	int i = 0;
	int j = n * 2 - 1 - 1; //从根节点开始搜索;
	int count = 0;
	char te[20];
	memset(textTemp, 0, sizeof(textTemp));
	while (temp[i] != '\0')
	{
		if (temp[i] == '0')
			j = ht[j].lchild;
		else
			j = ht[j].rchild;
		if (ht[j].rchild == -1)
		{
			printf("%c", ht[j].c);
			textTemp[count] = ht[j].c;
			j = n * 2 - 1 - 1; //从根节点开始搜索
			count++;
		}
		i++;
	}
	printf("\n");
	saveText();
}



/*
读取in.txt
文件中格式为
abcde //需要生成编码表的字母
2 5 7 9 12 //对应的权值

HuffmanNode[] 哈夫曼树
HCode[]  编码表
返回不同字符的个数
*/
int readHC( HuffmanNode ht[])
{
	char fileCode[2][1000];
	int i = 0;
	int len = 0;
	FILE *fp = fopen("in.txt", "r");
	memset(fileCode, 0, sizeof(fileCode));
	if (NULL == fp)
	{
		printf("打开文件失败\n");
		return 1;
	}
	printf("文件内容为\n");
	while (!feof(fp))
	{
		fgets(fileCode[i], sizeof(fileCode) - 1, fp); // 包含了\n  
		printf("%s", fileCode[i]);
		i++;
	}
	while (fileCode[0][len]!='\n')  //读取字符
	{
		ht[len].c = fileCode[0][len];
		len++;
	}
	i = 0;
	len = 0;
	while (fileCode[1][i] != 0)//读取字符对应的权值
	{
		if (fileCode[1][i]-32 != 0)
		{
			ht[len].weight = fileCode[1][i]- 48;
			len++;
		}
		i++;
	}
	fclose(fp);
	printf("\n");
	return len;
}

/*
test.txt 一篇文章
table.txt 字符表包括26个字母大小写 和， ‘ 可以按照需要在文件里面增加

HuffmanNode[] 哈夫曼树
HCode[]  编码表
*/

int readText(HuffmanNode ht[])
{
	char table[60];   //记录字母在文章中对于的个数
	int tableCount[60] = { 0 };
	char text[1000][1000] = { 0 };    //暂时存放一片文章
	int i = 0, j = 0, len = 0,cai=0;
	char temp[60];
	FILE *fp1 = fopen("test.txt", "r");
	FILE *fp2 = fopen("table.txt", "r");
	if (NULL == fp1 || fp2 == NULL)
	{
		printf("打不开该文件\n");
		return -1;
	}
	memset(table, 0, sizeof(table));
	memset(temp, 0, sizeof(temp));
	fgets(temp, sizeof(temp) - 1, fp2);
	while (temp[len] != '\0')//读取字母表内容到数组中  len字母个数
	{
		table[len] = temp[len];
		len++;
	}
	while (!feof(fp1))  //暂时存放一片文章
	{
		fgets(text[i], sizeof(text) - 1, fp1); //
		i++;
	}
	printf("字符\t个数\n");
	for (int k = 0; k < len; k++)   //计算字母对应的个数
	{
		i = 0;
		while (text[i][0] != '\0')//读到行尽头
		{
			j = 0;
			while (text[i][j] != '\0')   //读到列尽头
			{
				if (table[k] == text[i][j])
				{
					tableCount[k]++;
				}
				j++;
			}
			i++;
		}
		if (tableCount[k]!=0)
		{
			ht[cai].c = table[k];
			ht[cai].weight = tableCount[k];
			printf("%c\t%d\n", ht[cai].c, ht[cai].weight);
			cai++;
		}
	}
	
	fclose(fp1);
	fclose(fp2);
	return cai;//返回不同类型的字符个数
}

/*
char str[] 字符串数组  用来构建哈夫曼树
HuffmanNode[] 哈夫曼树
*/
int buitlHu(char str[], HuffmanNode ht[])
{
	char table[60];   //记录字母在文章中对于的个数
	int tableCount[60] = { 0 };
	int i = 0, j = 0, len = 0, cai = 0;
	FILE *fp2 = fopen("table.txt", "r");
	if (fp2 == NULL)
	{
		printf("打开文件失败\n");
		return 0;
	}
	memset(table, 0, sizeof(table));
	fgets(table, sizeof(table) - 1, fp2);
	
	printf("字符\t个数\n");
	for (int k = 0; table[k]!='\0'; k++)   //计算字母对应的个数
	{
		i = 0;
		while (str[i] != '\0')//读到行尽头
		{
			if (table[k] == str[i])
			{
				tableCount[k]++;
			}
			i++;
		}
		if (tableCount[k] != '\0')
		{
			ht[cai].c = table[k];
			ht[cai].weight = tableCount[k];
			printf("%c\t%d\n", ht[cai].c, ht[cai].weight);
			cai++;
		}
	}
	fclose(fp2);
	return cai;
}

/*
把文件中的字符串 根据编码表编码为 01串
char* name 文件名
HuffmanNode[] 哈夫曼树
HCode[]  编码表
int n 哈夫曼树的元素个数
*/
void textEnCode(char* name, HuffmanNode ht[], HCode hcd[], int n)
{
	char str[MaxSize];
	FILE* fp1 = fopen(name, "r");
	if (fp1==NULL)
	{
		printf("打开文件失败\n");
		return;
	}
	while (!feof(fp1))
	{
		fgets(str, sizeof(str), fp1); // 包含了\n  
	}
	printf("需要编码的字符串为\n%s\n", str);
	printf("编码为\n");
	enHCode(str, ht, hcd, n);
	fclose(fp1);
}

/*
对文件中的01串进行译码
char* name 文件名
HuffmanNode[] 哈夫曼树
int n 哈夫曼树的元素个数
*/
void textDeCode(char* textNname, HuffmanNode ht[], int n)
{
	char str[MaxSize];
	FILE* fp1 = fopen(textNname, "r");
	if (fp1 == NULL)
	{
		printf("打开文件失败\n");
		return;
	}

	while (!feof(fp1))
	{
		fgets(str, sizeof(str), fp1); // 包含了\n  
	}
	printf("密文为\n%s\n", str);
	printf("译码为\n");
	deHcode( ht,n, str);
	fclose(fp1);
}
/*
1 总菜单
2 子菜单
*/
void menu(int n)
{
	if (n==1)
	{
		printf("/****************电文编码译码系统**********************************/\n");
		printf("		(1)建立哈夫曼编码树\n");
		printf("		(2)输出编码表\n");
		printf("		(3)输入字符串进行编码\n");
		printf("		(4)输入01串进行译码\n");
		printf("		(5)文件编码\n");
		printf("		(6)文件译码\n");
		printf("		(7)离开\n");
		printf("/********************************************************/\n");
	}
	if (n==2)
	{
		system("cls");
		printf("/*********************建立哈夫曼树*****************************/\n");
		printf("		(0)输入字符串进行自动建立哈夫曼树\n");
		//intf("		(1)手动输入字符和对应权值构造哈夫曼树\n");
		printf("		(2)自动读取in.txt中的字符和权值构造哈夫曼树\n");
		printf("		(3)自动读取一篇文章如test.txt构造哈夫曼树\n");
		printf("		取消 (按其他键)\n");
		printf("/********************************************************/\n");
	}

}

//子菜单
void menue2(int &num, HuffmanNode ht[], HCode hcd[])
{
	int select;
	char str[MaxSize];
	menu(2);
	if (ht[0].c == NULL)
	{
		printf("\n!!!!!请先建立哈夫曼树!!!!!!!\n");
	}
	printf("请从清单中选择一个操作: ");
	scanf("%d", &select);
	//如果选择重新建立哈夫曼树则将相关数组清零
	if (select == 0||select == 1 || select == 2 || select == 3)
	{
		memset(ht, 0, sizeof(ht)*N);
		memset(hcd, 0, sizeof(hcd)*MaxSize);
	}

	switch (select)
	{
	case 0:
		printf("输入一串字符串\n");
		scanf("%s", str);
		num=buitlHu(str, ht);
		break;
	case 1:
		printf("请输入编码长度（最大26）：");
		scanf("%d", &num);
		while (num>26)
		{
			printf("\n输入非法请重新输入\n");
			scanf("%d", &num);
		}
		for (int i = 0; i<num; i++)
		{
			printf("请输入第%d字符：", i + 1);
			getchar();
			ht[i].c = getchar();
			getchar();
			printf("请输入权值：");
			scanf("%d", &(ht[i].weight));
		}
		break;
	case 2:
		num = readHC(ht);
		break;
	case 3:
		num = readText(ht);
		break;
	default:
		return;
	}
	if (num==0)
	{
		printf("建立失败\n");
		return;
	}
	CreateHT(ht, num);
	CreateHCode(ht, hcd, num);
	printf("!!哈夫曼编码树建立完成!!\n");

	Sleep(2000);
	return;

}
//菜单
void menue1(HuffmanNode ht[],HCode hcd[])
{
	int select=1;
	int num=0;
	int flag = 0;
	char temp[MaxSize]; //要翻译的数
	char string[MaxSize];//要编的码
	while (1)
	{
		memset(temp, 0, sizeof(temp));
		memset(temp, 0, sizeof(string));
		menu(1);
		if (ht[0].c==NULL)
		{
			printf("请先建立哈夫曼树");
			select = 1;
		}
		switch (select)
		{
		case 1:
			menue2(num, ht, hcd);
			flag = 1;
			system("cls");
			menu(1);
			break;
		case 2:	
			DispHCode(ht, hcd, num); 
			break;
		case 3: 
			printf("请输入要翻译的编码(最多200个字符):");
			scanf("%s", string, MaxSize);                       //把要进行编码的字符串存入string数组中
			printf("\n输出编码结果:\n");
			enHCode(string, ht, hcd, num);
			
			break;
		case 4: 
			
			printf("请输入要翻译的0,1代码串进行译码(最多200个字符)");
			scanf("%s", temp, MaxSize);
			printf("译码为:");
			deHcode(ht, num, temp);
			break;
		case 5:
			printf("输入文件名:  ");
			scanf("%s", temp);
			textEnCode(temp, ht, hcd, num);
			//textEnCode("code.txt", ht, hcd, num);
			break;
		case 6:
			printf("输入文件名:  ");
			scanf("%s", temp);
			textDeCode(temp, ht,num);
			//textDeCode("encode.txt", ht, num);
			break;
		case 7:
			exit(2);
			break;
		default:
			printf("输入错误指令请重新输入\n");
			break;
		}
		printf("请从清单中选择一个操作: ");
		scanf("%d", &select);
		system("cls");
	}
}
int main() //主函数
{
	HuffmanNode ht[N];
	HCode hcd[MaxSize];
	menue1(ht, hcd);
}
