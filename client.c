#include"src.h"
#define SOCKET_ADDRESS "127.0.0.1"
typedef enum{false,true} Boolean;
int inputNumber(){
	int sum=0,i=0,x=getch();
	while(x!='\r'){
		if(x<='9'&&x>='0'||x=='\b'){
			if(x=='\b'){
				if(i>0){
					printf("\b \b");
					i--;
				}
				sum/=10;
			}else{
				i++;
				putchar(x);
				sum=sum*10+x-'0';
			}
		}
		x=getch();
	}
	f();
	putchar('\n');
	return sum;
}
char* int2str(int num){
	static char strNum[16];
	int r=0,rN=0;
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
char* inputStrNumber(){
	return int2str(inputNumber());
}
void inputLine(char* buffer){
	static char str[1024];
	0[str]=0;
	f();
	scanf("%[^\n]",str);
	if(strlen(str)==0){
		strcpy(str,"NULL");
	}
	strcat(buffer,str);
	f();
}
void setXY( int x,int y ){   
    COORD point={x,y};
    HANDLE HOutput=GetStdHandle(STD_OUTPUT_HANDLE);
    SetConsoleCursorPosition(HOutput,point);
}
void showHelp(){
	printf(
		"本系统仿照了mysql的console交互设计\n"
		"help->|add: 添加商品档案\n"
		"      |buy: 进货记录\n"
		"      |delete: 删除商品档案\n"
		"      |exit: 关闭客户端\n"
		"      |find: 通过名字查找商品档案\n"
		"      |help: 获取帮助文档\n"
		"      |update: 更新商品信息\n"
		"      |sell: 记录出售商品数量\n"
		"      |setpwd: 修改密码\n"
		"      |show: 通过多种方式显示商品列表\n"
		"温馨提示: 本管理系统输入价格的单位应该是分哦\n"
	);
}
char* getBuffer(){
	static char buffer[1024];
	return buffer;
}
SOCKET socketLink(){
	WSADATA wsaData;
	WORD socketVersion=MAKEWORD(2,2);
	if(WSAStartup(socketVersion,&wsaData)){
		goto end_of_connect;
	}
	SOCKET server=socket(AF_INET,SOCK_STREAM,0);
	if(server==SOCKET_ERROR){
		goto end_of_connect;
	}
	struct sockaddr_in url;
	url.sin_family=AF_INET;
	url.sin_addr.S_un.S_addr=inet_addr(SOCKET_ADDRESS);
	url.sin_port=htons(7998);
	if(connect(server,(LPSOCKADDR)&url,sizeof(url))==SOCKET_ERROR){  
		goto end_of_connect;
	}
	return server;
	end_of_connect:
	WSACleanup();
	return 0;
}
void sendWithUTF8(SOCKET server,char* msg){
	char* buffer=getBuffer();
	GBK2UTF8(buffer,msg);
	send(server,buffer,strlen(buffer),0);
}
char* recvWithUTF8(SOCKET server){
	char* buffer=getBuffer();
	int len=recv(server,buffer,1024,0);
	buffer[len]='\0';
	UTF82GBK(buffer,buffer);
	return buffer;
}
void analyzeRecvStream(char* buffer){//数据流的前面有streamWithDatas标识，否则为状态标识
	int len=strlen(buffer);
	if(strstr(buffer,"streamWithDatas")==buffer){
		int num;
		char* ptr=buffer;
		sscanf(buffer,"%*s%*c%d%*c",&num);
		if(num!=0){
			char name[64];
			long long id;
			int getn,putn;
			float getp,putp,allget,profit;
			ptr=strstr(ptr,"\n")+1;
			ptr=strstr(ptr,"\n")+1;
			printf("┏━━━━━━━━━━━━━━━━━━━┳━━━━━━━━━━━━━━━━┳━━━━━━━━━━┳━━━━━━━━━━┳━━━━━━━━━━┳━━━━━━━━━━┳━━━━━━━━━━┳━━━━━━━━━━┓\n");
			printf("┃%19s┃%16s┃%10s┃%10s┃%10s┃%10s┃%10s┃%10s┃\n","商品编号","商品名称","进货价","销售价","销售总数","剩余量数","销售额","净利润");
			printf("┣━━━━━━━━━━━━━━━━━━━╋━━━━━━━━━━━━━━━━╋━━━━━━━━━━╋━━━━━━━━━━╋━━━━━━━━━━╋━━━━━━━━━━╋━━━━━━━━━━╋━━━━━━━━━━┫\n");
			for(int i=0;i<num-1;i++){
				sscanf(ptr,"%lld%*c%s%*c%f%*c%f%*c%d%*c%d%*c",&id,name,&getp,&putp,&getn,&putn);
				getp/=100;
				putp/=100;
				allget=putp*putn;
				profit=(float)putn*(putp-getp);
				printf("┃%19lld┃%16s┃%10.2f┃%10.2f┃%10d┃%10d┃%10.2f┃%10.2f┃\n",id,name,getp,putp,putn,getn-putn,allget,profit);
				printf("┣━━━━━━━━━━━━━━━━━━━╋━━━━━━━━━━━━━━━━╋━━━━━━━━━━╋━━━━━━━━━━╋━━━━━━━━━━╋━━━━━━━━━━╋━━━━━━━━━━╋━━━━━━━━━━┫\n");
				for(int j=0;j<6;j++){
					ptr=strstr(ptr,"\n")+1;
				}
			}
			sscanf(ptr,"%lld%*c%s%*c%f%*c%f%*c%d%*c%d%*c",&id,name,&getp,&putp,&getn,&putn);
			getp/=100;
			putp/=100;
			allget=putp*putn;
			profit=(float)putn*(putp-getp);
			printf("┃%19lld┃%16s┃%10.2f┃%10.2f┃%10d┃%10d┃%10.2f┃%10.2f┃\n",id,name,getp,putp,putn,getn-putn,allget,profit);
			printf("┗━━━━━━━━━━━━━━━━━━━┻━━━━━━━━━━━━━━━━┻━━━━━━━━━━┻━━━━━━━━━━┻━━━━━━━━━━┻━━━━━━━━━━┻━━━━━━━━━━┻━━━━━━━━━━┛\n");
		}else{
			printf("404 Not Found\n");
		}
	}else{
		printf("%s",buffer);
	}
}
void srWithShow(SOCKET server,char* buffer){
	sendWithUTF8(server,buffer);
	analyzeRecvStream(recvWithUTF8(server));
}
#define ifs(s1) if(!strcmp(tmp,s1))
#define p(v) printf(v)
Boolean listen2Keyboard(SOCKET server){
	printf("shell>");
	static char tmp[1024];
	char* buffer=getBuffer();
	0[tmp]=0;
	f();
	inputLine(tmp);
	ifs("add"){
		p("添加商品档案\n");
		strcpy(buffer,"add");
		p("商品名称: ");
		strcat(buffer," ");
		inputLine(buffer);
		p("商品进货价: ");
		strcat(buffer," ");
		strcat(buffer,inputStrNumber());
		p("商品销售价: ");
		strcat(buffer," ");
		strcat(buffer,inputStrNumber());
		p("商品进货量: ");
		strcat(buffer," ");
		strcat(buffer,inputStrNumber());
		srWithShow(server,buffer);
	}else ifs("buy"){
		p("进货商品名称: ");
		int num;
		strcpy(buffer,"buy ");
		inputLine(buffer);
		strcat(buffer," ");
		p("进货商品数量: ");
		strcat(buffer,inputStrNumber());
		srWithShow(server,buffer);
	}else ifs("delete"){
		p("删除商品档案\n");
		strcpy(buffer,"delete");
		p("商品名称: ");
		strcat(buffer," ");
		inputLine(buffer);
		srWithShow(server,buffer);
	}else ifs("exit"){
		// sendWithUTF8(server,"close_server");//release
		sendWithUTF8(server,"close");//debug
		return false;
	}else ifs("find"){
		p(
			"请选择查询方式\n"
			"name: 精确查询\n"
			"names: 模糊查询\n"
		);
		p(">");
		0[tmp]=0;
		inputLine(tmp);
		ifs("name"){
			p("精确查询\n");
			strcpy(buffer,"find name");
		}else ifs("names"){
			p("模糊查询\n");
			strcpy(buffer,"find names");
		}else{
			p("未知命令\n");
			return true;
		}
		p("搜索名称: ");
		strcat(buffer," ");
		inputLine(buffer);
		srWithShow(server,buffer);
	}else ifs("show"){
		p(
			"请选择浏览方式\n"
			"default: 默认顺序列表\n"
			"sales: 销售额降序列表\n"
			"profit: 净利润降序列表\n"
			"more: 剩余量多于\n"
			"less: 剩余量少于\n"
		);
		p(">");
		strcpy(buffer,"show ");
		0[tmp]=0;
		inputLine(tmp);
		ifs("sales"){
			strcat(buffer,"all_get");
		}else ifs("profit"){
			strcat(buffer,"profit");
		}else ifs("NULL"){
			strcat(buffer,"default");
		}else ifs("default"){
			strcat(buffer,"default");
		}else{
			ifs("more"){
				strcat(buffer,"more");
				p("剩余量多于: ");
				strcat(buffer," ");
				strcat(buffer,inputStrNumber());
				
			}else ifs("less"){
				strcat(buffer,"less");
				p("剩余量少于: ");
				strcat(buffer," ");
				strcat(buffer,inputStrNumber());
			}else{
				p("未知命令\n");
				return true;
			}
		}
		srWithShow(server,buffer);
	}else ifs("help"){
		showHelp();
	}else ifs("update"){
		static char name[64];
		p("待更新的商品名: ");
		strcpy(buffer,"update");
		0[name]=0;
		inputLine(name);
		p(
			"待修改的属性\n"
			"id: 商品编号\n"
			"name: 商品名称\n"
			"getp: 进货价格\n"
			"putp: 销售价格\n"
			"getn: 进货总量\n"
			"putn: 销售总量\n"
		);
		0[tmp]=0;
		inputLine(tmp);
		ifs("name"){
			strcat(buffer," name");
			p("商品名称更新为: ");
			strcat(buffer," ");
			inputLine(buffer);
		}else{
			ifs("id"){
				p("不给更新\n");
				return true;
				strcat(buffer," id");
				p("商品编号更新为: ");
			}else ifs("getp"){
				strcat(buffer," getp");
				p("进货价格更新为: ");
			}else ifs("putp"){
				strcat(buffer," putp");
				p("销售价格更新为: ");
			}else ifs("getn"){
				strcat(buffer," getn");
				p("进货总量更新为: ");
			}else ifs("putn"){
				strcat(buffer," putn");
				p("销售总量更新为: ");
			}
			strcat(buffer," ");
			strcat(buffer,inputStrNumber());
		}
		strcat(buffer," ");
		strcat(buffer,name);
		srWithShow(server,buffer);
	}else ifs("sell"){//sell name VALUE
		static char name[64];
		p("出售的商品名: ");
		strcpy(buffer,"sell ");
		inputLine(buffer);
		p("出售的商品数量: ");
		strcat(buffer," ");
		strcat(buffer,inputStrNumber());
		srWithShow(server,buffer);
	}else{
		ifs("sendmsg"){
			p("msg: ");
			scanf("%[^\n]",buffer);
			srWithShow(server,buffer);
		}else ifs("gui"){
			WSACleanup();
			system("java\\bin\\java GUI");
			return false;
		}else ifs("setpwd"){
			p("请输入新密码: ");
			strcpy(buffer,"setpwd ");
			0[tmp]=0;
			int i=0,x=getch();
			while(x!='\r'){
				if(x!='\b'){
					tmp[i]=x;
					i++;
					putchar(x);
				}else{
					if(i>0){
						p("\b \b");
						i--;
					}
				}
				x=getch();
			}
			f();
			p("\n");
			tmp[i]='\0';
			strcat(buffer,strcat(tmp," "));
			srWithShow(server,buffer);
		}else{
			printf("未知命令\n");
		}
	}
	return true;
}
#undef ifs
#undef p
Boolean inputPassword(SOCKET server){
	char* buffer=getBuffer();
	static char password[64];
	strcpy(buffer,"password ");
	printf("请输入root密码: ");
	int i=0;
	label_input_password:
	password[i]=getch();
	switch(password[i]){
		case '\r':
			password[i]='\0';
			putchar('\n');
			break;
		case '\b':
			if(i){
				printf("\b \b");
				i--;
			}
			goto label_input_password;
		default:
			putchar('*');
			i++;
			goto label_input_password;
	}
	strcat(buffer,strcat(password," "));
	sendWithUTF8(server,buffer);
	buffer=recvWithUTF8(server);
	f();
	if(!strcmp(buffer,"OK\n")){
		return true;
	}else{
		printf("密码错误，请重新输入\n");
		return false;
	}
}
int main(int argv,char* argc[]){
	system("cls");
	SOCKET server;
	while(!(server=socketLink()));
	while(!inputPassword(server));
	printf("登录成功!\n");
	showHelp();
	while(listen2Keyboard(server));
	close(server);
	WSACleanup();
	printf("Bye!\n");
	return 0;
}
