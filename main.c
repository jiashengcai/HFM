#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#define MaxSize 50
#define N 51

typedef struct HuffmanNode //定义哈夫曼树各结点
{
	char c;		
	int weight;  //频率
	int parent;  //双亲编号
	int lchild,rchild;  //左右孩子编号
}HuffmanNode; 

typedef struct
{
    char cd[N];                 //存放哈夫曼码
    int start;                  //从start开始读cd中的哈夫曼码
}HCode;


void CreateHT(HuffmanNode ht[],int n)                //调用输入的数组ht[],和节点数n
{
    int i,k,lnode,rnode;
    int min1,min2;
    for (i=0;i<2*n-1;i++){       
        ht[i].parent=ht[i].lchild=ht[i].rchild=-1;    //所有结点的相关域置初值-1
	}
    for (i=n;i<2*n-1;i++)                   //构造哈夫曼树
	{
        min1=min2=32767;                //int的范围是-32768-32767
        lnode=rnode=-1;                  //lnode和rnode记录最小权值的两个结点位置
        for (k=0;k<=i-1;k++)
        {
			if (ht[k].parent==-1)               //只在尚未构造二叉树的结点中查找
			{
                if (ht[k].weight<min1)           //若权值小于最小的左节点的权值
				{
                    min2=min1;
					rnode=lnode;
                    min1=ht[k].weight;
					lnode=k;
				}
                else if (ht[k].weight<min2)
				{
                    min2=ht[k].weight;
					rnode=k;
				}
			}
		}
        ht[lnode].parent=i;
		ht[rnode].parent=i;                //两个最小节点的父节点是i
        ht[i].weight=ht[lnode].weight+ht[rnode].weight;       //两个最小节点的父节点权值为两个最小节点权值之和
        ht[i].lchild=lnode;
		ht[i].rchild=rnode;                 //父节点的左节点和右节点
		ht[i].parent=-1;         //表示新结点应该是根结点
	}
}


void CreateHCode(HuffmanNode ht[],HCode hcd[],int n) //创建编码表
{
	 int i,f,c;
	 HCode hc;
	 for (i=0;i<n;i++)                            //根据哈夫曼树求哈夫曼编码
	 {
		  hc.start=n;
		  c=i;
		  f=ht[i].parent;
		  while (f!=-1)                              //循序直到树根结点结束循环
		  {
			   if (ht[f].lchild==c){                        //处理左孩子结点
				   hc.cd[hc.start--]='0';
			   }
			   else{                                    //处理右孩子结点
				   hc.cd[hc.start--]='1';
			   }
			   c=f;
			   f=ht[f].parent;
		  }
		  hc.start++;                               //start指向哈夫曼编码hc.cd[]中最开始字符
		  hcd[i]=hc;
	 }
}

void DispHCode(HuffmanNode ht[],HCode hcd[],int n)     //输出哈夫曼编码的列表
{
	 //if(ht[0].weight==NULL) printf("hh");
	 int i,k;
	 printf("  输出哈夫曼编码:\n"); 
	 for (i=0;i<n;i++)									//输出data中的所有数据，即a-z
	 {
		  printf("      %c:\t",ht[i].c);               
		  for (k=hcd[i].start;k<=n;k++)                    //输出所有data中数据的编码
		  {
		   printf("%c",hcd[i].cd[k]);                     
		  }
		  printf("\n");
	 }
}

void editHCode(HuffmanNode ht[],HCode hcd[],int n)    //编码函数
{
	char string[MaxSize];   
	int i,j,k;
	printf("请输入要翻译的编码(最多50个字符):");
	scanf("%s",string);                       //把要进行编码的字符串存入string数组中
	printf("\n输出编码结果:\n");
	for (i=0;string[i]!='\0';i++)
	{
		for (j=0;j<n;j++)
		{
			if(string[i]==ht[j].c)            //循环查找与输入字符相同的编号，相同的就输出这个字符的编码
			{
				for (k=hcd[j].start;k<=n;k++)
				{
                    printf("%c",hcd[j].cd[k]);
				}
				break;                      //输出完成后跳出当前for循环
			}
		}
		if(j==n){
			printf("\n%c未编码，请重新输入\n",string[i]);
			return;
		}
	}
	printf("\n");
}

void Decoder(HuffmanNode ht[],int n)//译码
{

		int i=0;
		int j=n*2-1-1; //从根节点开始搜索;
		char temp[MaxSize]; //要翻译的数
		printf("请输入要翻译的0,1代码串进行译码(最多50个字符)");
		scanf("%s",temp);
		printf("译码为:");
		while(temp[i]!='\0')
		{
			if(temp[i]=='0') 
				j=ht[j].lchild;
			else 
				j=ht[j].rchild;
			if(ht[j].rchild==-1)
			{
				printf("%c",ht[j].c);
				j=n*2-1-1; //从根节点开始搜索
			}
			i++;
		}
		printf("\n");
	
}

int main() //主函数
{
	HuffmanNode ht[N];
	HCode hcd[MaxSize];
	int num,i,select;
	while(1)
	{
		printf("/********************************************************/\n");
		printf("		(1)建立哈夫曼编码树\n");
		printf("		(2)输出编码表\n");
		printf("		(3)编码\n");
		printf("		(4)译码\n");
		printf("		(5)离开\n");
		printf("/********************************************************/\n");
		printf("请从清单中选择一个操作:");
		scanf("%d",&select);
		switch(select)
		{
			case 1: 
				{
					printf("请输入编码长度（最大26）：");
					scanf("%d",&num);
					while(num>26)
					{
						printf("\n输入非法请重新输入\n");
						scanf("%d",&num);
					}
					for(i=0;i<num;i++)
					{
						printf("请输入第%d字符：",i+1);
						getchar();
						ht[i].c=getchar();
						getchar();
						printf("请输入权值：");
						scanf("%d",&(ht[i].weight));
					}
					CreateHT(ht,num);
					CreateHCode(ht,hcd,num);
					printf("!!哈夫曼编码树建立完成!!\n");
					break;
				}
			case 2:	DispHCode(ht,hcd,num);break;
			case 3: editHCode(ht,hcd,num);break;
			case 4: Decoder(ht,num);break;
			case 5: return(0);break;
		}
	}
}
