#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<ctype.h>
#define branchNum 26
//char Stop_words[500][30];//停词
char Stop_words[6000];
char Article[6377000+10];//文章

typedef struct TrieNode
{
	int count; //单词出现的频度
	struct TrieNode *child[branchNum];
}TrieNode,*TrieTree;

typedef struct sentence{//句子 后期考虑桶排序
    char whole_sentence[1500];//完整的将句子存储
    int sum_fre;//句子的总频率
}sentence;

int Stop_lenth;
sentence sentence_list[215000];//句子表
char word_temp[25];//temp

void sep_Stop_words(TrieTree t);
int find_stop_words(TrieTree t,char word[]);//判断单词是否停词
int work_out_sum_fre(TrieTree t,char sentence_1[]);//统计一个句子中非停词的总频度
char lower_to_upper(char x);//小写->大写

TrieNode *trie_create_node();//创建一个结点
void trie_insert_stop_word(TrieTree t,char word[]);//插入一个停词
void trie_insert_word(TrieTree t,char word[]);//插入一个单词
int trie_search_word(TrieTree t,char word[]);//查找单词
int trie_word_count(TrieTree t,char word[]);//统计单词出现的频度

int comp_sentence(const void*a,const void *b) {
    sentence aa = *(sentence*)a;
    sentence bb = *(sentence*)b;
    return bb.sum_fre - aa.sum_fre;
}

int main()
{
    TrieTree root=trie_create_node();
    FILE *stop,*in,*out;
    int N,i,Article_lenth,p;
    char al_x;
	stop=fopen("stopwords.txt","r");
	in=fopen("article.txt","r");
	Article_lenth=fread(Article,1,6377010,in);
	Article_lenth=strlen(Article);
	Stop_lenth=fread(Stop_words,1,6500,stop);
    sep_Stop_words(root);
	//Stop_lenth=0;
	//while(fscanf(stop,"%s",Stop_words[Stop_lenth++])!=EOF) ;
	fclose(in);
	fclose(stop);
	//读入
	scanf("%d",&N);//输出N个
	int sentence_pos=0,word_temp_pos=0,single_sentence_pos=0;
	for(p=0;p<25;p++){
        word_temp[p]='\0';
    }
    for(i=0;i<Article_lenth;i++){
        if(single_sentence_pos==0&&Article[i]==' ') continue;
        sentence_list[sentence_pos].whole_sentence[single_sentence_pos++]=Article[i];
        if(isalpha(Article[i])){
            al_x=lower_to_upper(Article[i]);
            word_temp[word_temp_pos++]=al_x;
        }
        else{
            if(word_temp_pos==0) ;
            else if(!find_stop_words(root,word_temp)){
                trie_insert_word(root,word_temp);
            }
            for(p=0;p<word_temp_pos+1;p++){
                word_temp[p]='\0';
            }
            word_temp_pos=0;
        }
        if(Article[i]=='.'||Article[i]=='?'||Article[i]=='!'){
            sentence_pos++;
            single_sentence_pos=0;
            continue;
        }
    }
    for(i=0;i<sentence_pos;i++){
        sentence_list[i].sum_fre=work_out_sum_fre(root,sentence_list[i].whole_sentence);
    }
    qsort(sentence_list,sentence_pos,sizeof(sentence_list[1]),comp_sentence);
    /*
    for(i=0;i<sentence_pos;i++){
        printf("%d %s\n\n",sentence_list[i].sum_fre,sentence_list[i].whole_sentence);
    }
    */
    for(i=0;i<5;i++){
        printf("%d %s\n",sentence_list[i].sum_fre,sentence_list[i].whole_sentence);
    }
    out=fopen("results.txt","w");
    for(i=0;i<N;i++){
        fprintf(out,"%d %s\n",sentence_list[i].sum_fre,sentence_list[i].whole_sentence);
    }
    fclose(out);
    return 0;
}
void sep_Stop_words(TrieTree t)
{
    char al_x;
    int i;
    int word_temp_pos=0,p;
    for(p=0;p<25;p++){
        word_temp[p]='\0';
    }
	for(i=0;i<Stop_lenth;i++){
        if(isalpha(Stop_words[i])){
            al_x=lower_to_upper(Stop_words[i]);
            word_temp[word_temp_pos++]=al_x;
        }
        else{
            if(word_temp_pos==0) continue;
            trie_insert_stop_word(t,word_temp);//插入停词表
            for(p=0;p<word_temp_pos;p++){
                word_temp[p]='\0';
            }
            word_temp_pos=0;
        }
	}
}

int find_stop_words(TrieTree t,char word[])//判断一个单词是不是停词
{
    TrieNode *node=t;
	while(*word&&node!=NULL)
	{
		node=node->child[*word-'a'];
		word++;
	}
	if(node!=NULL&&node->count==-1)//频度为-1且node不为空
        return 1;
	else
		return 0;
}

int work_out_sum_fre(TrieTree t,char sentence_1[])
{
    int res=0;
    int i,p;
    int lenth=strlen(sentence_1);
    int word_temp_pos=0;
    char al_x;
    for(p=0;p<25;p++){
        word_temp[p]='\0';
    }
    for(i=0;i<lenth;i++){
        if(isalpha(sentence_1[i])){//如果是字母
            al_x=lower_to_upper(sentence_1[i]);//转为小写
            word_temp[word_temp_pos++]=al_x;//记录这个字母
        }
        else{//不是字母，说明上一个单词结束了
            if(word_temp_pos==0) continue;
            //printf("%d %s\n",word_temp_pos,word_temp);
            res+=trie_word_count(t,word_temp);//寻找这个单词的频度
            word_temp_pos=0;
            for( p=0;p<25;p++){
                word_temp[p]='\0';
            }
        }
    }
    return res;
}

char lower_to_upper(char x)
{
    return (x>='A'&&x<='Z')?(x|0x20):x;//这里用其他方式也可以
}


TrieNode *trie_create_node()//创建一个tTrieNode节点
{
	TrieNode *temp=(TrieNode*)malloc(sizeof(TrieNode));
	temp->count=0;
	memset(temp->child,NULL,sizeof(temp->child));
	return temp;
}

void trie_insert_stop_word(TrieTree t,char word[])//插入一个停词
{
    TrieNode *node;
	node=t;
	int index;
	while(*word)
	{
		index=*word-'a';
		if(node->child[index]==NULL)
		node->child[index]=trie_create_node();
		node=node->child[index];
		word++;
	}
	node->count=-1;//频度区分于其他单词
}
void trie_insert_word(TrieTree t,char word[])//插入单词
{
	TrieNode *node;
	node=t;
	int index;
	while(*word)
	{
		index=*word-'a';
		if(node->child[index]==NULL)
		node->child[index]=trie_create_node();
		node=node->child[index];
		word++;
	}
	node->count++;//频度++
}

int trie_search_word(TrieTree t,char word[])//查找单词
{
	TrieNode *node=t;
	while(*word&&node!=NULL)
	{
		node=node->child[*word-'a'];
		word++;
	}
	if(node!=NULL&&node->count>0)
        return 1;
	else
		return 0;
}

int trie_word_count(TrieTree t,char word[])//统计单词出现的频度
{
    if(!trie_search_word(t,word)) return 0;
	TrieNode *node=t;
	while(*word&&node!=NULL)
	{
		node=node->child[*word-'a'];
	    word++;
	}
    return node->count;
}




