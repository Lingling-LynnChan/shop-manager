#include"list.h"

char* getBuffer(){
	static char buffer[1024];
	return buffer;
}
List* listFoo(){
	static List load=NULL;
	return &load;
}
Boolean* loadFlagFoo(){
	static Boolean flag=false;
	return &flag;
}
char* passwordFoo(){
	static char password[64];
	return password;
}
void* loading(void*){
	List* alist=listFoo();
	char* password=passwordFoo();
	FILE* fp=fopen("programdata","rb");
	for(register int i=0;i<16;i++){
		password[i]=fgetc(fp);
		if(password[i]==EOF){
			break;
		}
	}
	*alist=File2List(fp);
	fclose(fp);
	Boolean* aflag=loadFlagFoo();
	*aflag=true;
	return NULL;
}
void startLoading(){
	system("cls");
	pthread_t id;
	pthread_create(&id,NULL,loading,NULL);
	printf("服务正在启动...\n");
	pthread_join(id,NULL);
	printf("服务启动成功!\n");
}
void* save(void*){
	List* alist=listFoo();
	Boolean* aflag=loadFlagFoo();
	char* password=passwordFoo();
	*aflag=true;
	FILE* fp=fopen("programdata","wb");
	for(int i=0;i<16;++i){
		fputc(password[i],fp);
	}
	List2File(*alist,fp);
	fclose(fp);
	*aflag=false;
	return NULL;
}
void closeLoading(){
	pthread_create(NULL,NULL,save,NULL);
	printf("服务正在关闭...\n");
	while(*loadFlagFoo());
	printf("服务关闭成功!\n");
}
SOCKET socketLink(){
	WORD sockVersion=MAKEWORD(2,2);
	WSADATA wsaDATA;
	if(WSAStartup(sockVersion,&wsaDATA)){
		goto end_of_start;
	}
	SOCKET slisten=socket(AF_INET,SOCK_STREAM,IPPROTO_TCP);
	if (slisten==INVALID_SOCKET){
		goto end_of_start;
	}
	struct sockaddr_in sin;
	sin.sin_family=AF_INET;
	sin.sin_port=htons(7998);
	sin.sin_addr.S_un.S_addr=INADDR_ANY;
	if (bind(slisten,(LPSOCKADDR)&sin,sizeof(sin))==SOCKET_ERROR){
		goto end_of_start;
	}
	if (listen(slisten,2)==SOCKET_ERROR){
		goto end_of_start;
	}
	SOCKET sClient;
	struct sockaddr_in remoteAddr;
	int nAddrlen=sizeof(remoteAddr);
	sClient=accept(slisten,(SOCKADDR*)&remoteAddr,&nAddrlen);
	if(sClient==INVALID_SOCKET){
		goto end_of_link;
	}
	return sClient;
	end_of_link:
	return 0;
	end_of_start:
	exit(0);
}
#define s2s(str) (!strcmp(tmp,str))
char* analyzeStream(char* buffer,SOCKET client){
	char tmp[1024];
	UTF82GBK(buffer,buffer);
	sscanf(buffer,"%s",tmp);
	if(s2s("add")){//add name getp putp getN (putN)
		sscanf(buffer,"%*s%*c%[^\n]",tmp);
		int v3,v4,v5,v6;
		char v2[64];
		sscanf(tmp,"%s%*c%d%*c%d%*c%d",v2,&v3,&v4,&v5);//v6读不到
		v6=0;
		Boolean add_flag=false;
		*(listFoo())=insertList(*(listFoo()),newData(v2,v3,v4,v5,v6),&add_flag);
		return add_flag?"OK\n":"该商品已存在\n";
	}else if(s2s("update")){//update id/name/getp/putp/getn/putn VALUE name 
		sscanf(buffer,"%*s%*c%s",tmp);
		int index;
		static char name[64];
		if(s2s("name")){
			sscanf(buffer,"%*s%*c%*s%*c%s%*c%[^\n]",name,tmp);
			if(!strstr(name," ")){
				Data data=findByName(*(listFoo()),tmp);
				if(data){
					strcpy(data->name,name);
					return "OK\n";
				}else{
					return "404 Not Found\n";
				}
			}else{
				return "名字不能包含空格\n";
			}
		}else{
			if(s2s("id")){
				index=4;
			}else if(s2s("getp")){
				index=0;
			}else if(s2s("putp")){
				index=1;
			}else if(s2s("getn")){
				index=2;
			}else if(s2s("putn")){
				index=3;
			}
			static int newNum;
			sscanf(buffer,"%*s%*c%*s%*c%d%*c%[^\n]",&newNum,name);
			if(!strstr(name," ")){
				Data data=findByName(*(listFoo()),name);
				if(data){
					int* arr=(int*)data;
					arr[index]=newNum;
					return "OK\n";
				}else{
					return "404 Not Found\n";
				}
			}else{
				return "名字不能包含空格\n";
			}
		}
	}else if(s2s("find")){//find name/names VALUE
		static char name[64];
		sscanf(buffer,"%*s%*c%s%*c%[^\n]",tmp,name);
		if(!strstr(name," ")){
			if(s2s("name")){
				return Datas2Str(insertList(NULL,findByName(*(listFoo()),name),NULL));
			}else if(s2s("names")){
				return Datas2Str(findByNames(*(listFoo()),name));
			}else{
				return "错误的指令\n";
			}
		}else{
			return "名字不能包含空格\n";
		}
	}else if(s2s("show")){//show all_get/profit/default |	show less/more VALUE
		sscanf(buffer,"%*s%*c%[^\n]",tmp);
		if(strstr(tmp," ")){
			int X;
			sscanf(buffer,"%*s%*c%s%*c%d",tmp,&X);
			if(s2s("less")){
				return Datas2Str(getNewList(getNumLessX(*(listFoo()),X)));
			}else if(s2s("more")){
				return Datas2Str(getNewList(getNumMoreX(*(listFoo()),X)));
			}else{
				return "-1\n";
			}
		}else if(s2s("all_get")){
			return Datas2Str(sortNewList(*(listFoo()),cmpBySold));
		}else if(s2s("profit")){
			return Datas2Str(sortNewList(*(listFoo()),cmpByProfit));
		}else/*if(s2s("default"))*/{
			return Datas2Str(getNewList(*(listFoo())));
		}
	}else if(s2s("delete")){//delete name
		sscanf(buffer,"%*s%*c%[^\n]",tmp);
		if(!strstr(tmp," ")){
			Boolean del_flag=false;
			*(listFoo())=deleteNodeByName(*(listFoo()),tmp,&del_flag);
			return del_flag?"OK\n":"404 Not Found\n";
		}else{
			return "名字不能包含空格\n";
		}
	}else if(s2s("buy")){//buy name VALUE
		int num=-1;
		sscanf(buffer,"%*s%*c%s%*c%d",tmp,&num);
		if(num>0){
			num=buyDataByName(*(listFoo()),num,tmp);
			return num!=-1?"OK\n":"404 Not Found\n";
		}else{
			return "请输入正确的数量\n";
		}
	}else if(s2s("sell")){//sell name VALUE
		int num=-1;
		sscanf(buffer,"%*s%*c%s%*c%d",tmp,&num);
		if(num>0){
			num=sellDataByName(*(listFoo()),num,tmp);
			return num!=-1?"OK\n":"404 Not Found\n";
		}else{
			return "请输入正确的数量\n";
		}
	}else if(s2s("close")){//close
		return "Bye\n";
	}else if(s2s("password")){//password xxxxxx mark: 为了传输成功，需在流末尾额外添加一个空格
		sscanf(buffer,"%*s%*c%[^\n]",tmp);
		tmp[strlen(tmp)-1]='\0';
		if(!strcmp(tmp,passwordFoo())){
			return "OK\n";
		}else{
			return "ERROR\n";
		}
	}else if(s2s("setpwd")){//setpwd xxxxxx mark: 为了传输成功，需在流末尾额外添加一个空格
		sscanf(buffer,"%*s%*c%[^\n]",tmp);
		tmp[strlen(tmp)-1]='\0';
		strcpy(passwordFoo(),tmp);
		return "OK\n";
	}else{//mark: if close_server then exit
		if(!s2s("close_server")){
			printf("连接地址: %s\n",buffer);
			return "连接成功\n";
		}else{
			GBK2UTF8(buffer,"服务即将关闭\n");
			send(client,buffer,strlen(buffer),0);
			thread_sleep(100);//延迟一毫秒
			closeLoading();
			exit(0);
		}
	}
}
#undef s2s
Boolean rsWithUTF8(SOCKET client){
	char* buffer=NULL;
	int len=0;
	while(len!=-1){
		buffer=getBuffer();
		len=recv(client,buffer,1024,0);
		if(len!=-1){
			buffer[len]='\0';
			if(strcmp(buffer,"close")){
				strcpy(buffer,analyzeStream(buffer,client));
				GBK2UTF8(buffer,buffer);
				send(client,buffer,strlen(buffer),0);
			}else{
				len=-1;
			}
		}else{
			return false;
		}
		save(NULL);
	}
	return true;
}
int main(int argv,char* argc[]){
	startLoading();
	SOCKET client;
	restart:
	while(true){
		printf("正在等待连接...\n");
		while(!(client=socketLink()));
		printf("连接成功!\n");
		if(rsWithUTF8(client)){
			thread_sleep(100);
			printf("连接断开\n");
			goto close_server;
		}else{
			printf("连接意外终止\n");
			goto close_server;
		}
		save(NULL);
	}
	close_server:
	WSACleanup();
	goto restart;
	closeLoading();
	return 0;
}
