/*
* ignore(LLONG_MAX, end)用于跳过缓冲区大小为LLONG_MAX的空间或直到下一个end字符，包括该字符
* seek()函数之前需要用clear()函数清除文件流状态，文件流到达EOF无法使用seek()
*/

#include "serverhead.h"

fstream userInfoFile;			//读写用户信息文件流
fstream adminInfoFile;			//读写管理员信息文件流
fstream packInfoFile;			//读写快递信息文件流
fstream postmanInfoFile;		//读写快递员信息文件流
//定义发送缓冲区和接受缓冲区
char send_buf[1000];
char recv_buf[1000];
//定义服务端socket，接受请求socket
SOCKET server;
SOCKET client;

int main(void)
{
	//打开所需文件
	userInfoFile.open(".\\data\\userInfo.txt", ios::in | ios::out);
	if (!userInfoFile.is_open())
	{
		cout << "打开用户信息文件失败" << endl;
		system("pause");
		exit(1);
	}
	adminInfoFile.open(".\\data\\adminInfo.txt", ios::in | ios::out);
	if (!adminInfoFile.is_open())
	{
		cout << "打开管理员信息文件失败" << endl;
		system("pause");
		exit(1);
	}
	packInfoFile.open(".\\data\\packInfo.txt", ios::in | ios::out);
	if (!packInfoFile.is_open())
	{
		cout << "打开物流信息文件失败" << endl;
		system("pause");
		exit(1);
	}
	postmanInfoFile.open(".\\data\\postmanInfo.txt", ios::in | ios::out);
	if (!postmanInfoFile.is_open())
	{
		cout << "打开快递员信息文件失败" << endl;
		system("pause");
		exit(1);
	}

	//定义长度变量
	int recv_len = 0;
	int len = 0;
	//服务端地址客户端地址
	SOCKADDR_IN server_addr;
	SOCKADDR_IN client_addr;
	//初始化socket库
	WORD w_req = MAKEWORD(2, 2);//版本号
	WSADATA wsadata;
	int err;
	err = WSAStartup(w_req, &wsadata);
	if (err != 0) 
		cout << "初始化socket库失败" << endl;
	else 
		cout << "初始化socket库成功" << endl;
	//检测版本号
	if (LOBYTE(wsadata.wVersion) != 2 || HIBYTE(wsadata.wHighVersion) != 2) 
	{
		cout << "socket库版本号不符" << endl;
		WSACleanup();
	}
	else 
		cout << "socket库版本正确" << endl;
	//填充服务端信息
	server_addr.sin_family = AF_INET;
	server_addr.sin_addr.S_un.S_addr = htonl(INADDR_ANY);
	server_addr.sin_port = htons(5000);
	//创建socket
	server = socket(AF_INET, SOCK_STREAM, 0);
	if (bind(server, (SOCKADDR*)&server_addr, sizeof(SOCKADDR)) == SOCKET_ERROR) 
	{
		cout << "socket绑定失败" << endl;
		WSACleanup();
	}
	else 
		cout << "socket绑定成功" << endl;
	//设置socket为监听状态
	if (listen(server, SOMAXCONN) < 0) 
	{
		cout << "设置监听状态失败" << endl;
		WSACleanup();
	}
	else 
		cout << "设置监听状态成功" << endl;
	cout << "服务端正在监听连接，请稍候...." << endl;
	//接受连接请求
	len = sizeof(SOCKADDR);
	
	//接收数据
	while (1) 
	{
		User user;
		Postman postman;
		Admin admin;
		int flag = 1;
		client = accept(server, (SOCKADDR*)&client_addr, &len);	//接收客户端连接
		if (client == SOCKET_ERROR)
		{
			cout << "连接失败" << endl;
			WSACleanup();
			break;
		}
		cout << "连接建立，准备接收客户端信息" << endl;
		while (1)
		{
			recv_len = recv(client, recv_buf, 1000, 0);		//从客户端接收请求指令
			if (recv_len < 1)
			{
				cout << "接收失败" << endl;
				break;
			}
			else
			{
				istringstream iss(recv_buf);
				string id, req;
				getline(iss, id, SPLIT);	//获取客户端用户身份
				getline(iss, req);			//获取指令后部分
				cout << "客户端用户身份:";
				switch (id[0])
				{
				case '1':
					cout << "用户" << endl;
					user.recvFromClient(req);
					break;
				case '2':
					cout << "快递员" << endl;
					postman.recvFromClient(req);
					break;
				case '0':
					cout << "管理员" << endl;
					admin.recvFromClient(req);
					break;
				case 'q':
					cout << "退出" << endl;
					flag = 0;
					break;
				default:
					break;
				}
			}
			if (!flag)
				break;
		}
		
		closesocket(client);
	}
	//关闭socket
	closesocket(server);
	//释放DLL资源
	WSACleanup();

	userInfoFile.close();
	adminInfoFile.close();
	packInfoFile.close();

	system("pause");
	return 0;
}
