/*
* ignore(LLONG_MAX, end)用于跳过缓冲区大小为LLONG_MAX的空间或直到下一个end字符，包括该字符
* seek()函数之前需要用clear()函数清除文件流状态，文件流到达EOF无法使用seek()
*/

#include "clienthead.h"

User* user = NULL;				//存储登录中的用户的信息
Admin* admin = NULL;			//存储登录中的管理员的信息
Postman* postman = NULL;		//存储登录中的快递员的信息
vector<Pack> packList;			//相关快递列表
vector<User> allUserList;		//所有用户列表，仅管理员可用
vector<Postman> allPostmanList;	//所有快递员列表，仅管理员可用
//定义发送缓冲区和接受缓冲区
char send_buf[1000];
char recv_buf[1000];
//定义服务端socket，接受请求socket
SOCKET server;

int main(void)
{
	//定义长度变量
	int send_len = 0;
	//服务端地址客户端地址
	SOCKADDR_IN server_addr;
	//初始化socket库
	WORD w_req = MAKEWORD(2, 2);//版本号
	WSADATA wsadata;
	int err;
	err = WSAStartup(w_req, &wsadata);
	if (err != 0) 
	{
		cout << "初始化socket库失败" << endl;
	}
	else 
	{
		cout << "初始化socket库成功" << endl;
	}
	//检测版本号
	if (LOBYTE(wsadata.wVersion) != 2 || HIBYTE(wsadata.wHighVersion) != 2) 
	{
		cout << "socket库版本号不符" << endl;
		WSACleanup();
	}
	else 
	{
		cout << "socket库版本正确" << endl;
	}
	//填充服务端信息
	server_addr.sin_family = AF_INET;
	inet_pton(AF_INET, "127.0.0.1", &server_addr.sin_addr.S_un.S_addr);
	server_addr.sin_port = htons(5000);
	//创建socket
	server = socket(AF_INET, SOCK_STREAM, 0);
	if (connect(server, (SOCKADDR*)&server_addr, sizeof(SOCKADDR)) == SOCKET_ERROR) 
	{
		cout << "服务器连接失败" << endl;
		WSACleanup();
	}
	else 
	{
		cout << "服务器连接成功" << endl;
		char input;
		do {
			cout << "登录请按1" << endl << "新用户注册请按2" << endl << "快递员登录请按3" << endl << "管理员登录请按4" << endl << "输入q退出" << endl;;
			input = getchar();
			cin.ignore(LLONG_MAX, '\n');
			switch (input)
			{
			case '1':
				user = new User;
				user->login();
				delete user;
				break;
			case '2':
				user = new User;
				user->regis();
				delete user;
				break;
			case '3':
				postman = new Postman;
				postman->login();
				delete postman;
				break;
			case '4':
				admin = new Admin;
				admin->login();
				delete admin;
				break;
			case 'q':
				strcpy_s(send_buf, "q");
				send_len = send(server, send_buf, 1000, 0);
				if (send_len < 1)
				{
					cout << "发送失败" << endl;
					break;
				}
				break;
			default:
				cout << "输入错误，请重新输入" << endl;
				break;
			}
		} while (input != 'q');
	}
	
	//关闭socket
	closesocket(server);
	//释放DLL资源
	WSACleanup();

	system("pause");
	return 0;
}
