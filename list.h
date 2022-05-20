#ifndef _LIST_H_
#define _LIST_H_

#include"src.h"
typedef enum{false,true} Boolean;

typedef struct{
	int getp;//进货价
	int putp;//销售价
	int getN;//进货量
	int putN;//销售量
	//销售额=销售量*销售价
	//剩余量=进货量-出售量
	//净利润=销售量*(销售价-进货价)
	long long id;//8
	char name[32];//4
}E;
typedef E* Data;
Data newData(char* name,int getp,int putp,int getN,int putN){
	Data data=(Data)malloc(sizeof(E));
	strcpy(data->name,name);
	data->id=getId();
	data->getp=getp;
	data->putp=putp;
	data->getN=getN;
	data->putN=putN;
	return data;
}
struct _lnode{
	Data data;
	struct _lnode* next;
};
typedef struct _lnode LNode;
typedef LNode* List;
List newList(Data data){
	List list=(List)malloc(sizeof(LNode));
	list->data=data;
	list->next=NULL;
	return list;
}
//默认插入保证顺序
List insertList(List list,Data data,Boolean* aflag){
	if(data){
		if(list){
			if(strcmp(list->data->name,data->name)){
				list->next=insertList(list->next,data,aflag);
			}
			return list;
		}
		if(aflag){
			*aflag=true;
		}
		return newList(data);
	}else{
		return list;
	}
}
//插入到表头，在排序时使用
List insertListAtFirst(List list,Data data){
	if(data){
		List newlist=newList(data);
		newlist->next=list;
		list=newlist;
	}
	return list;
}
//插入到i位后面
List insertListBeforeIndex(List list,Data data,int index){
	if(list||index==0){
		if(index){
			list->next=insertListBeforeIndex(list->next,data,index-1);
		}else{
			List before=newList(data);
			before->next=list;
			return before;
		}
	}
	return list;
}
//插入到i位前面
List insertListAfterIndex(List list,Data data,int index){
	if(list||index==0){
		if(index){
			list->next=insertListAfterIndex(list->next,data,index-1);
		}else{
			List after=list->next;
			list->next=newList(data);
			list->next=after;
		}
	}
	return list;
}
Boolean updateDataByName(List list,Data data,char* name){
	while(list){
		if(strcmp(list->data->name,name)){
			list=list->next;
		}else{
			Data del=list->data;
			list->data=data;
			free(del);
			return true;
		}
	}
	return false;
}
int sellDataByName(List list,int N,char* name){
	while(list){
		if(strcmp(list->data->name,name)){
			list=list->next;
		}else{
			int* arr=(int*)(list->data);
			if(arr[2]-arr[3]/*剩余量*/>=N){
				arr[3]+=N;
				return N;
			}else{
				N=arr[2]-arr[3];
				arr[3]=arr[2];
				return N;
			}
		}
	}
	return -1;
}
int buyDataByName(List list,int N,char* name){
	while(list){
		if(strcmp(list->data->name,name)){
			list=list->next;
		}else{
			list->data->getN+=N;
			return N;
		}
	}
	return -1;
}
int getSold(Data data){
	int* arr=(int*)data;
	return data?arr[1]*arr[3]:0;
}
int getSoldByName(List list,char* name){//获取销售额
	while(list){
		if(strcmp(list->data->name,name)){
			list=list->next;
		}else{
			return getSold(list->data);
		}
	}
	return -1;
}
int getProfit(Data data){
	int* arr=(int*)data;
	return data?arr[3]*(arr[1]-arr[0]):-1;
}
int getProfitByName(List list,char* name){
	while(list){
		if(strcmp(list->data->name,name)){
			list=list->next;
		}else{
			return getProfit(list->data);
		}
	}
	return -1;
}
Data findByName(List list,char* name){
	while(list){
		if(strcmp(list->data->name,name)){
			list=list->next;
		}else{
			return list->data;
		}
	}
	return NULL;
}
typedef List Datas;
Datas findByNames(List list,char* names){
	Datas datas=NULL;
	while(list){
		if(strstr(list->data->name,names)){
			datas=insertList(datas,list->data,NULL);
		}
		list=list->next;
	}
	return datas;
}
int getDataNum(Data data){
	return data->getN-data->putN;
}
Datas getNumLessX(List list,int X){
	Datas datas=NULL;
	while(list){
		if(getDataNum(list->data)<X){
			datas=insertList(datas,list->data,NULL);
		}
		list=list->next;
	}
	return datas;
}
Datas getNumMoreX(List list,int X){
	Datas datas=NULL;
	while(list){
		if(getDataNum(list->data)>X){
			datas=insertList(datas,list->data,NULL);
		}
		list=list->next;
	}
	return datas;
}
int getListSize(List list){
	int index=0;
	while(list){
		index++;
		list=list->next;
	}
	return index;
}
List deleteNodeByName(List list,char* name,Boolean* aflag){
	if(list){
		if(strcmp(list->data->name,name)){
			list->next=deleteNodeByName(list->next,name,aflag);
		}else{
			List next=list->next;
			free(list);
			*aflag=true;
			return next;
		}
	}
	return list;
}
List deleteList(List list){
	if(list){
		List after=list->next;
		free(list);
		deleteList(after);
	}
	return NULL;
}
List deleteListAll(List list){
	if(list){
		List after=list->next;
		free(list->data);
		free(list);
		deleteList(after);
	}
	return NULL;
}
/*借用了新的结构体方便排序*/
struct _sort{
	Data data;
	struct _sort* next1;
	struct _sort* next2;
};
typedef struct _sort SLNode;
typedef SLNode* SortList;
SortList newSortList(Data data){
	SortList sort=(SortList)malloc(sizeof(SLNode));
	sort->data=data;
	sort->next1=NULL;
	sort->next2=NULL;
	return sort;
}
SortList insertSortList(SortList sort,Data data,int cmp(Data x,Data y)){
	if(sort){
		if(cmp(sort->data,data)>0){
			sort->next1=insertSortList(sort->next1,data,cmp);
		}else{
			sort->next2=insertSortList(sort->next2,data,cmp);
		}
	}else{
		sort=newSortList(data);
	}
	return sort;
}
void deleteSortList(SortList sl){
	if(sl){
		deleteSortList(sl->next1);
		deleteSortList(sl->next2);
		free(sl);
	}
}
void SortList2List(List* alist,SortList sort){
	if(sort){
		SortList2List(alist,sort->next1);
		*alist=insertListAtFirst(*alist,sort->data);
		SortList2List(alist,sort->next2);
	}
}
List List2SortList2List(List list,int cmp(Data x,Data y)){
	SortList sort=NULL;
	while(list){
		sort=insertSortList(sort,list->data,cmp);
		list=list->next;
	}
	SortList2List(&list,sort);
	deleteSortList(sort);
	return list;
}
List sortNewList(List list,int cmp(Data x,Data y)){
	return List2SortList2List(list,cmp);
}
List getNewList(List list){
	List newList=NULL;
	while(list){
		newList=insertList(newList,list->data,NULL);
		list=list->next;
	}
	return newList;
}
int cmpBySold(Data x,Data y){
	return getSold(x)-getSold(y);
}
int cmpByProfit(Data x,Data y){
	return getProfit(x)-getProfit(y);
}
/*FILE文件部分*/
void Data2File(Data data,FILE * fp){
	static const int size=sizeof(E);
	char* ptr;
	ptr=(char*)data;
	for(register int i=0;i<size;++i){
		fputc(ptr[i],fp);
	}
}
void List2File(List list,FILE * fp){
	static const int size=sizeof(E);
	if(list){
		Data2File(list->data,fp);
		List2File(list->next,fp);
	}else{
		for(register int i=0;i<size;++i){
			fputc(0,fp);
		}
	}
}
Data File2Data(FILE* fp){
	static const int size=sizeof(E);
	Data data=(Data)malloc(sizeof(E));
	char* ptr=(char*)data;
	for(register int i=0;i<size;++i){
		ptr[i]=fgetc(fp);
	}
	return data;
}
List File2List(FILE * fp){
	if(fp){
		static const int size=sizeof(E);
		Data data=File2Data(fp);
		char* ptr=(char*)data;
		char isEOF(char * ptr){
			for(register int i=0;i<size;++i){
				if(ptr[i]){
					return 0;
				}
			}
			return 1;
		}
		if(!isEOF(ptr)){
			List list=newList(data);
			list->next=File2List(fp);
			return list;
		}else{
			free(data);
			return NULL;
		}
	}else{
		return NULL;
	}
}
char* Int2Str(long long num){
	static char strNum[32];
	long long r=0,rN=0;
	while(num){
		r++;
		rN=rN*10+num%10;
		num/=10;
	}
	if(r!=0){}else r=1;
	for(int i=0;i<r;i++){
		strNum[i]=rN%10+'0';
		rN/=10;
	}
	strNum[r]='\0';
	return strNum;
}
void nextLine(char* msg){
	strcat(msg,"\n");
}
char* Data2Str(Data data){
	static char String[256];
	0[String]='\0';
	strcat(String,Int2Str(data->id));
	nextLine(String);

	strcat(String,data->name);
	nextLine(String);
	
	strcat(String,Int2Str(data->getp));
	nextLine(String);

	strcat(String,Int2Str(data->putp));
	nextLine(String);

	strcat(String,Int2Str(data->getN));
	nextLine(String);

	strcat(String,Int2Str(data->putN));
	nextLine(String);

	return String;
}
char* Datas2Str(Datas datas){
	static char str[1024];
	strcpy(str,"streamWithDatas");
	nextLine(str);
	strcat(str,Int2Str(getListSize(datas)));
	nextLine(str);
	Datas del=datas;
	while(datas){
		strcat(str,Data2Str(datas->data));
		datas=datas->next;
	}
	if(del){
		deleteList(del);
	}
	return str;
}
#endif
