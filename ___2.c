#pragma GCC optimize ("O3")
#pragma once
#pragma pack (16)
#include<stdio.h>
#include<stdlib.h>
#define SIZE 1000005
#define Sens 120000
struct sentence{
	int sen[2];
	int frequency;
	int sentenceorder; 
}Sen[Sens]; 
int min(int x, int y);
int cmp(const void*p1,const void*p2);
int i,j,m,l=0,ll,n,flag=0,temp;
int SenNum=0,senbegin[SIZE],VacNum=0,VacFre[SIZE*5],*VacFrePtr;
char s[SIZE*7],tmp[200],buff[2000],ch;
int Trie[1000010][26];
int Freqc[1000010]={0};
int pos=1,p=0;
int main()
{

	scanf("%d",&n);
	
	FILE *fp = fopen("article.txt","r");
	fread(s,sizeof(char),SIZE*7,fp);
	m = ftell(fp)+1; 
	flag = 1, l=0;    
	for (i = 0; s[i]; i++)
	{
		if (flag!=0)
		{   
			senbegin[SenNum] = i; 
			SenNum++;
			flag = 0;
		}
		if ((s[i] >= 'A'&&s[i] <= 'Z') || (s[i] >= 'a'&&s[i] <= 'z'))
		{ 
			temp = (s[i] <= 'Z') ? s[i] - 'A' : s[i] - 'a'; 
			if (!Trie[p][temp])
				Trie[p][temp] = ++pos;	
			p = Trie[p][temp];
		}
		else if (s[i] == '.' || s[i] == '!' || s[i] == '?'){
			Freqc[p]++;
			VacFre[++VacNum] = p;
			VacFre[++VacNum] = -1;
			p = 0;
			flag = 1;
		}
		else
		{
			Freqc[p]++;
			VacFre[++VacNum] = p;
			p=0;
		}	
	} 
	fclose(fp);

	freopen("stopwords.txt", "r", stdin);
	while (scanf("%s", buff) != EOF)
	{  
		p=0;
		for(i=0;buff[i];i++){
			temp = buff[i] - 'a';
			if (!Trie[p][temp])
				Trie[p][temp] = ++pos;	
			p = Trie[p][temp];
		}
		Freqc[p]=0;
	}  
	fclose(stdin);
	
	VacFrePtr = &VacFre[0];
	VacFre[++VacNum]=-1;
	senbegin[SenNum] = 0x3f3f3f3f;
	for (i = 0; i < SenNum; i++) 
	{ 
		l = 0,p = 0,temp=0;
		Sen[i].sen[0]=senbegin[i];
		Sen[i].sen[1]=min(m, senbegin[i + 1] - 1);
		Sen[i].sentenceorder=i;
		Freqc[0] = 0;
		for (; *VacFrePtr != -1; ++VacFrePtr) {
			Sen[i].frequency += Freqc[*VacFrePtr];
		}
		VacFrePtr++;
	}
	qsort(Sen,SenNum,sizeof(struct sentence),cmp);
	
	for(i=0;i<5;i++)
	{
		printf("%d ",Sen[i].frequency);
		int l=Sen[i].sen[1];
		j=Sen[i].sen[0];
		while (s[j] ==' '&& j <=l) j++;
		for(;j<=l;j++)
		{
			putchar(s[j]);
		}
		puts("");
	}
	freopen("results.txt", "w", stdout);
	for(i=0;i<n;i++)
	{
		printf("%d ",Sen[i].frequency);
		int l=Sen[i].sen[1];
		j=Sen[i].sen[0];
		while (s[j] ==' '&& j <=l) j++;
		for(;j<=l;j++)
		{
			putchar(s[j]);
		}
		puts("");
	}
	fclose(stdout);
	return 0;
}
int min(int x, int y)
{   
	return x <= y ? x : y;
}
int cmp(const void*p1,const void*p2)
{
	struct sentence *a=(struct sentence*)p1;
	struct sentence *b=(struct sentence*)p2;
	if(a->frequency!=b->frequency)	return b->frequency-a->frequency; 
	else return a->sentenceorder-b->sentenceorder ; 
}
