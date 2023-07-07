#pragma once
#include <WinSock2.h>
#include <Ws2tcpip.h>
#pragma comment(lib,"WS2_32.lib")
#include <iostream>
#include <string>
#include <fstream>
#include <windows.h>
#include <sstream>

using namespace std;

#define RECEIVE 2		//已接收
#define NOT_RECEIVE 1	//未接收
#define NOT_PICK 0		//未揽收
#define SPLIT '.'		//文件数据分隔符
#define SEND_SPLIT '|'	//发送数据分隔符
#define NORMAL 0
#define FRAGILE 1
#define BOOK 2

//定义发送缓冲区和接受缓冲区
extern char send_buf[1000];
extern char recv_buf[1000];
//定义服务端socket，接受请求socket
extern SOCKET server;
extern SOCKET client;

/*
* 请求格式
* x.y.z
* x=0管理员，1用户，2快递员
* y请求类型
* z所需数据
*/

/*
* 快递寄/收件人信息
* 用户ID
* name用户姓名
* addr地址
* phone电话
* */
typedef struct
{
	string ID;
	string name;
	string addr;
	string phone;
}PackUserInfo;

/*
* 快递所需的快递员信息
* 快递员ID
* name快递员姓名
* phone快递员电话
*/
typedef struct 
{
	string ID;
	string name;
	string phone;
}PackPostmanInfo;

/*
* 指令操作码，操作类型，所需数据
* 0，id是否存在，id
* 1，请求密码，id
* 2，用户登录完成加载信息，id
* 3，更改密码，新密码
* 4，余额变更，新余额
* 5，发送快递，运费.快递信息
* 6，接收快递，快递号.接收时间
* 7，加载收件人信息，id
* 8，新用户，用户信息
* 9，请求用户相关快递，id
* locInFile在2和8后赋值
*/
//用户类
class User
{
public:
	User();
	~User();

	void recvFromClient(const string& req);			//从客户端收到用户的指令
	
private:
	void checkExistId(const string& id) const;			//检查用户ID id是否存在，存在发送1，否则发送0
	void sendPw(const string& id) const;				//发送用户ID id的密码
	void getInfo(const string& id);						//发送用户ID id的用户信息
	void changePW(const string& pw) const;				//更改用户ID id的密码
	void changeBalance(const string& balance) const;	//更改用户ID id的余额
	void sendPack(const string& packInfo) const;		//发送快递
	void recvPack(const string& packInfo) const;		//接收快递
	void loadPacketUserInfo(const string& id) const;	//加载用户ID为id的收件人信息
	void newUser(const string& userInfo);				//向文件添加新用户
	void loadUserPackList(const string& id) const;		//从文件中读取与用户ID为id的用户相关的快递并发送
	streampos searchID(const string& id) const;			//返回用户ID为id的用户信息在文件中的位置
	
	void writeToFile(int mode, const string& info) const;			//将用户信息写入文件，mode=0写入新用户信息，mode=1更改某用户的密码，mode=1更改某用户的余额

public:
	streampos locInFile;		//当前登录客户端的用户的信息在文件的位置
};

/*
* 指令操作码，操作类型，所需数据
* 0，id是否存在，id
* 1，请求密码，id
* 2，登录完成后加载信息，id
* 3，新快递员，快递员信息
* 4，删除快递的快递员信息，packid
* 5，删除快递员，postid
* 6，分配快递员，packid，post信息
* 7，请求所有用户信息
* 8，请求所有快递员信息
* 9，请求所有快递信息
*/
//管理员类
class Admin
{
public:
	Admin();
	~Admin();

	void recvFromClient(const string& req) const;		//从客户端收到管理员的指令
	
private:
	void checkExistId(const string& id) const;				//检查管理员ID id是否存在，存在发送1，否则发送0
	void sendPw(const string& id) const;					//发送管理员ID id的密码
	void getInfo(const string& id) const;					//发送管理员ID id的信息
	void addPostman(const string& postmanInfo) const;		//往文件新增快递员信息
	void deletePostmanFromPack(const string& packId) const;	//删除文件中快递的快递员信息
	void deletePostman(const string& id) const;				//从文件中删除快递员
	void assignPack(const string& info) const;				//为快递分配快递员
	void loadAllUserList() const;				//管理员有权查看所有用户，提前从文件中读取并将所有用户信息并发送
	void loadAllPostmanList() const;			//管理员有权查看所有快递员，提前从文件中读取并将所有快递员信息并发送
	void loadAllPackList() const;				//管理员有权查看所有快递，提前从文件中读取并将所有快递信息并发送
	streampos searchID(const string& id) const;	//返回ID为id的管理员信息在文件中的位置
};

/*
* 指令操作码，操作类型，所需数据
* 0，id是否存在，id
* 1，请求密码，id
* 2，完成登录加载信息，id
* 3，揽收快递，packID.揽收时间
* 4，更新余额，新余额
* 5，加载相关快递，id
* locInFile在2后赋值
*/
//快递员类
class Postman 
{
public:
	Postman();
	~Postman();

	void recvFromClient(const string& req);			//从客户端收到快递员的指令

private:
	void checkExistId(const string& id) const;		//检查快递员ID id是否存在，存在发送1，否则发送0
	void sendPw(const string& id) const;			//发送快递员ID id的密码
	void getInfo(const string& id);					//发送快递员ID id的用户信息
	void pickUp(const string& info) const;			//揽收快递
	void updateBalanceInFile(const string& balance) const;		//在快递员信息文件中更新余额
	void loadPostmanPackList(const string& id) const;			//从文件读取当前快递员负责的快递并发送
	streampos searchID(const string& id) const;					//返回当前登录的快递员的信息在文件中的位置

public:
	streampos locInFile;	//当前登录的快递员的信息在文件中的位置
};
