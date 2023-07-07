#pragma once
#include <iostream>
#include <string>
#include <fstream>
#include <windows.h>
#include <vector>
using namespace std;

#define RECEIVE 1		//已接收
#define NOT_RECEIVE 0	//未接收
#define SPLIT '.'		//文件数据分隔符

/*
* 快递寄/收件人信息
* ID用户号
* name用户姓名
* addr地址
* phone电话
*/
typedef struct
{
	string ID;
	string name;
	string addr;
	string phone;
}PackUserInfo;

//快递类
class Pack
{
public:
	Pack();
	//用于读取文件输入的构造函数
	Pack(const string& ipackID, const int& istate, const SYSTEMTIME& isendTime, const SYSTEMTIME& irecvTime, const PackUserInfo& isender, const PackUserInfo& irecver, const string& ipackInfo);
	//用于寄件的构造函数
	Pack(const string& ipackID, const SYSTEMTIME& isendTime, const PackUserInfo& isender, const PackUserInfo& irecver, const string& ipackInfo);
	~Pack();

	void output() const;	//输出快递信息
	void sendPack() const;	//发送快递
	void recvPack();		//接收快递
public:
	string packID;			//快递号，由寄件人用户号和发送时间构成
	int state;				//接收状态
	SYSTEMTIME sendTime;	//发送时间
	SYSTEMTIME recvTime;	//接收时间
	PackUserInfo sender;	//寄件人信息
	PackUserInfo recver;	//收件人信息
	string packInfo;		//快递详情
};

//用户类
class User
{
public:
	User();
	//用于注册或从文件读取输入的构造函数
	User(const string& id, const string& ipw, const string& iname, const string& iphone, const string& iaddr, const int& ibalance);
	~User();

	void regis();				//注册
	void login();				//登录
	void output() const;		//输出用户信息

private:
	void loadUserPacklist();	//读取与该用户相关的快递，存在向量packList中
	void userInterface();		//用户界面
	void userInfoInterface();	//用户个人信息管理界面
	void userPackInterface();	//用户物流信息管理界面

	void changePW();							//更改密码
	void checkBalance() const;					//查看余额
	void topUp();								//充值
	void regis(const string& id);				//在登录时使用不存在用户ID时使用，注册id
	void login(const string& id);				//在注册时使用已存在用户ID时使用，登录id
	int checkPwValid(const string& newPw) const;//检查newPw是否符合密码格式要求，符合则返回1，否则返回0

	void sendPack();							//发送快递
	void recvPack() const;						//接收快递
	void searchPack() const;					//查询快递
	void loadPacketUserInfo(PackUserInfo& sender) const;				//加载寄件人信息
	void loadPacketUserInfo(const streampos& loc, PackUserInfo& recver) const;	//加载收件人信息，streampos loc是寄件人信息在用户信息文件的位置
	void searchPackBySender() const;			//根据寄件人信息查询快递
	void searchPackBySenderID() const;			//根据寄件人用户ID查询快递
	void searchPackBySenderName() const;		//根据寄件人姓名查询快递
	void searchPackBySenderPhone() const;		//根据寄件人电话查询快递
	void searchPackBySenderAddr() const;		//根据寄件人地址查询快递
	void searchPackByRecver() const;			//根据收件人信息查询快递
	void searchPackByRecverID() const;			//根据收件人用户ID查询快递
	void searchPackByRecverName() const;		//根据收件人姓名查询快递
	void searchPackByRecverPhone() const;		//根据收件人电话查询快递
	void searchPackByRecverAddr() const;		//根据收件人地址查询快递
	void searchPackBySendTime() const;			//根据发送时间查询快递
	void searchPackByPackID() const;			//根据快递号查询快递
	int checkUnrecvPack() const;				//输出未接收快递

	streampos searchID() const;					//从文件中查找该对象id的用户信息，返回在文件中的位置，不存在则返回-1
	streampos searchID(const string& id) const;	//从文件中查找用户ID为id的用户信息，返回在文件中的位置，不存在则返回-1
	void writeToFile(int mode) const;			//将用户信息写入文件，mode=0表示写入新用户信息，mode=1表示更改原有用户的信息

public:
	string ID;		//用户ID
	string name;	//用户姓名
	string phone;	//用户电话
	string addr;	//用户地址
	int balance;	//余额

private:
	string pw;		//用户密码
};

//管理员类
class Admin
{
public:
	Admin();
	~Admin();

	void login();				//登录
	
private:
	void adminInterface() const;		//管理员界面
	void loadAllPacklist() const;		//管理员有权访问所有快递，从文件读取所有快递
	void loadAllUserList() const;		//管理员有权访问所有用户，从文件读取所有用户
	void adminUserInterface() const;	//管理员管理用户信息界面
	void adminPackInterface() const;	//管理员管理物流信息界面

	void checkAllUser() const;			//查看所有用户
	void checkAllPack() const;			//查看所有快递
	void checkBalance() const;			//查询余额
	void searchUser() const;			//查询用户
	void searchPack() const;			//查询快递

	void searchUserByID() const;			//以用户ID查询用户
	void searchUserByName() const;			//以用户姓名查询用户
	void searchUserByPhone() const;			//以用户电话查询用户
	void searchUserByAddr() const;			//以用户地址查询用户

	void searchPackBySender() const;		//根据寄件人信息查询快递
	void searchPackBySenderID() const;		//根据寄件人用户ID查询快递
	void searchPackBySenderName() const;	//根据寄件人姓名查询快递
	void searchPackBySenderPhone() const;	//根据寄件人电话查询快递
	void searchPackBySenderAddr() const;	//根据寄件人地址查询快递
	void searchPackByRecver() const;		//根据收件人信息查询快递
	void searchPackByRecverID() const;		//根据收件人用户ID查询快递
	void searchPackByRecverName() const;	//根据收件人姓名查询快递
	void searchPackByRecverPhone() const;	//根据收件人电话查询快递
	void searchPackByRecverAddr() const;	//根据收件人地址查询快递
	void searchPackBySendTime() const;		//根据发送时间查询快递
	void searchPackByPackID() const;		//根据快递号查询快递

public:
	string ID;		//管理员ID
	string name;	//管理员姓名
	int balance;	//余额。余额是所有管理员共用的，单独存储在文件的第一行

private:
	string pw;		//管理员密码
};
