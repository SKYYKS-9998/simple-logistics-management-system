#pragma once
#include <WinSock2.h>
#include <Ws2tcpip.h>
#pragma comment(lib, "WS2_32.lib")
#include <iostream>
#include <string>
#include <windows.h>
#include <vector>
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

/*
* 快递寄/收件人信息
* 用户ID
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

//快递类
class Pack
{
public:
	Pack();
	//用于读取文件输入的构造函数
	Pack(const string& ipackID, const int& istate, const SYSTEMTIME& isendTime, const SYSTEMTIME& irecvTime, const PackUserInfo& isender, const PackUserInfo& irecver, const PackPostmanInfo& ipostman, const SYSTEMTIME& ipickUpTime, const int& itype, const int& iamount, const int& iprice, const string& ipackInfo);
	//用于寄件的构造函数
	Pack(const string& ipackID, const SYSTEMTIME& isendTime, const PackUserInfo& isender, const PackUserInfo& irecver, const int& itype, const int& iamount, const string& ipackInfo);
	~Pack();

	void output() const;	//输出快递信息
	void sendPack() const;	//发送快递
	void recvPack();		//接收快递
	void pickUp();			//揽收快递
	void assignPostman(const PackPostmanInfo& postmanInfo);	//为快递分配快递员，参数为快递员信息
	void deletePostman();	//删除快递的快递员信息
	virtual void getPrice();//计算快递的运费
public:
	string packID;			//快递ID，由寄件人用户ID和发送时间构成
	int state;				//快递状态。分为未揽收、未接收、已接收三种状态
	SYSTEMTIME sendTime;	//发送时间
	SYSTEMTIME recvTime;	//接收时间
	PackUserInfo sender;	//寄件人信息
	PackUserInfo recver;	//收件人信息
	PackPostmanInfo postman;//快递员信息
	SYSTEMTIME pickUpTime;	//揽收时间
	int type;				//快递类型
	int amount;				//快递重量/本数
	int price;				//快递运费
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
	void loadUserPackList() const;				//从文件中读取与当前用户相关的快递，存在向量packList中
	void userInterface();						//用户界面
	void userInfoInterface();					//用户管理个人信息界面
	void userPackInterface();					//用户管理物流信息界面

	void changePW();							//更改密码
	void checkBalance() const;					//查看余额
	void topUp();								//充值
	void regis(const string& id);				//在用户登录时使用不存在用户号时使用，注册输入的ID
	void login(const string& id);				//在用户注册时使用已存在用户号时使用，登录输入的ID
	int checkPwValid(const string& newPw) const;//检查newPw是否符合密码格式要求，是则返回1，否则返回0

	void sendPack();							//发送快递
	void recvPack() const;						//接收快递
	void searchPack() const;					//查询快递
	void loadSenderInfo(PackUserInfo& sender) const;	//加载寄件人信息
	void loadRecverInfo(PackUserInfo& recver) const;	//加载收件人信息
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
	void searchPackByState() const;				//根据快递状态查询快递
	int checkUnrecvPackIn() const;				//输出寄给当前用户的未接收快递
	int checkUnpickPackIn() const;				//输出寄给当前用户的未揽收快递
	int checkRecvPackIn() const;				//输出寄给当前用户的已接收快递
	int checkUnrecvPackOut() const;				//输出当前用户寄出的未接收快递
	int checkUnpickPackOut() const;				//输出当前用户寄出的未揽收快递
	int checkRecvPackOut() const;				//输出当前用户寄出的已接收快递

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

	void login();		//登录
	
private:
	void loadAllUserList() const;		//管理员有权查看所有用户，提前从文件中读取并将所有用户信息存储到向量allUserList
	void loadAllPackList() const;		//管理员有权查看所有快递，提前从文件中读取并将所有快递信息存储到向量packList
	void loadAllPostmanList() const;	//管理员有权查看所有快递员，提前从文件中读取并将所有快递员信息存储到allPostmanList
	void adminInterface() const;		//管理员界面
	void adminUserInterface() const;	//管理员管理用户信息界面
	void adminPackInterface() const;	//管理员管理快递信息界面
	void adminPostmanInterface() const;	//管理员管理快递员信息界面

	void checkAllUser() const;			//查看所有用户
	void searchUser() const;			//查询用户
	void searchUserByID() const;		//以用户ID查找用户
	void searchUserByName() const;		//以用户姓名查找用户
	void searchUserByPhone() const;		//以用户电话查找用户
	void searchUserByAddr() const;		//以用户地址查找用户

	void checkAllPack() const;				//查看所有快递
	void checkBalance() const;				//查询余额
	void searchPack() const;				//查询快递
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
	void searchPackByState() const;			//根据快递状态查询快递
	int outputUnrecvPack() const;			//输出未接收快递
	int outputUnpickPack() const;			//输出未揽收快递
	int outputRecvPack() const;				//输出已接收快递

	void checkAllPostman() const;						//查看所有快递员
	void addPostman() const;							//新增快递员
	void deletePostman() const;							//删除快递员
	int searchPostman(const string& id) const;			//查看ID为参数id的快递员是否存在，是则返回在allPostman向量的下标，否则返回-1
	int checkPwValid(const string& newPw) const;		//检查输入的新密码是否符合格式要求，是则返回1，否则返回0
	void addPostmanToFile() const;						//向快递员信息文件中写入新快递员信息
	void deletePostmanFromFile(const string& id) const;	//从快递员信息文件删除ID为参数id的快递员信息
	void assignPack() const;							//为快递分配快递员
	int checkUnassignPack() const;						//输出未分配快递员的快递
	
	void searchPostman() const;				//查询快递员
	void searchPostmanByID() const;			//以ID查询快递员
	void searchPostmanByName() const;		//以姓名查询快递员
	void searchPostmanByPhone() const;		//以电话查询快递员

	void showAllPackAssignedToOne() const;						//输出分配给指定一个快递员的所有快递
	void searchPackAssignedToOne() const;						//在指定的一个快递员负责的快递中查找快递
	void searchAssignPackBySender(const string& id) const;		//根据寄件人信息查询快递
	void searchAssignPackBySenderID(const string& id) const;	//根据寄件人用户ID查询快递
	void searchAssignPackBySenderName(const string& id) const;	//根据寄件人姓名查询快递
	void searchAssignPackBySenderPhone(const string& id) const;	//根据寄件人电话查询快递
	void searchAssignPackBySenderAddr(const string& id) const;	//根据寄件人地址查询快递
	void searchAssignPackByRecver(const string& id) const;		//根据收件人信息查询快递
	void searchAssignPackByRecverID(const string& id) const;	//根据收件人用户ID查询快递
	void searchAssignPackByRecverName(const string& id) const;	//根据收件人姓名查询快递
	void searchAssignPackByRecverPhone(const string& id) const;	//根据收件人电话查询快递
	void searchAssignPackByRecverAddr(const string& id) const;	//根据收件人地址查询快递
	void searchAssignPackBySendTime(const string& id) const;	//根据发送时间查询快递
	void searchAssignPackByPackID(const string& id) const;		//根据快递号查询快递
	void searchAssignPackByState(const string& id) const;		//根据快递状态查询快递
	int outputUnrecvAssignPack(const string& id) const;			//输出未接收快递
	int outputUnpickAssignPack(const string& id) const;			//输出未揽收快递
	int outputRecvAssignPack(const string& id) const;			//输出已接收快递

public:
	string ID;		//管理员ID
	string name;	//管理员姓名
	int balance;	//余额

private:
	string pw;		//管理员密码
};

//快递员类
class Postman 
{
public:
	Postman();
	//从文件输入或管理员添加快递员时使用的构造函数
	Postman(const string& id, const string& ipw, const string& iname, const string& iphone, const int& ibalance);
	~Postman();

	void login();		//登录
	void output() const;//输出快递员信息

private:
	void loadPostmanPackList() const;	//从文件读取当前快递员负责的快递，存储在packList中
	void postmanInterface();			//快递员界面

	void pickUp();				//揽收快递
	void checkBalance() const;	//查看余额

	void searchPack() const;					//查询快递
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
	void searchPackByState() const;				//根据快递状态查询快递
	int outputUnrecvPack() const;				//输出未接收快递
	int outputUnpickPack() const;				//输出未揽收快递
	int outputRecvPack() const;					//输出已接收快递

	void updateBalanceInFile() const;			//揽收快递后，在快递员信息文件中更新余额

public:
	string ID;		//快递员ID
	string pw;		//密码
	string name;	//姓名
	string phone;	//电话
	int balance;	//余额
};

//易碎品类
class Fragile : public Pack
{
public:
	Fragile();
	//用于寄件的构造函数
	Fragile(const string& id, const SYSTEMTIME& sTime, const PackUserInfo& send, const PackUserInfo& recv, const int& ty, const int& am, const string& info);
	~Fragile();

	void getPrice();	//计算快递的运费
};

//书籍类
class Book : public Pack
{
public:
	Book();
	//用于寄件的构造函数
	Book(const string& id, const SYSTEMTIME& sTime, const PackUserInfo& send, const PackUserInfo& recv, const int& ty, const int& am, const string& info);
	~Book();

	void getPrice();	//计算快递的运费
};