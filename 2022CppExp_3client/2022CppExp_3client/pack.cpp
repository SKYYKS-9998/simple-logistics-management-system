#include "clienthead.h"

extern vector<Pack> packList;

Pack::Pack()
	: state(NOT_RECEIVE)
	, sendTime()
	, recvTime()
	, pickUpTime()
	, type(NORMAL)
	, amount(0)
	, price(0)
{}

//用于读取文件输入的构造函数
Pack::Pack(const string& ipackID, const int& istate, const SYSTEMTIME& isendTime, const SYSTEMTIME& irecvTime, const PackUserInfo& isender, const PackUserInfo& irecver, const PackPostmanInfo& ipostman, const SYSTEMTIME& ipickUpTime, const int& itype, const int& iamount, const int& iprice, const string& ipackInfo)
	: packID(ipackID)
	, state(istate)
	, sendTime(isendTime)
	, recvTime(irecvTime)
	, sender(isender)
	, recver(irecver)
	, postman(ipostman)
	, pickUpTime(ipickUpTime)
	, type(itype)
	, amount(iamount)
	, price(iprice)
	, packInfo(ipackInfo)
{}

//用于寄件的构造函数
Pack::Pack(const string& ipackID, const SYSTEMTIME& isendTime, const PackUserInfo& isender, const PackUserInfo& irecver, const int& itype, const int& iamount, const string& ipackInfo)
	: packID(ipackID)
	, state(NOT_PICK)
	, sendTime(isendTime)
	, recvTime()
	, sender(isender)
	, recver(irecver)
	, pickUpTime()
	, type(itype)
	, amount(iamount)
	, price(0)
	, packInfo(ipackInfo)
{
	//由于要求快递只能三个状态，未分配快递员时将快递员的各项信息设为"???"
	postman.ID = "???";
	postman.name = "???";
	postman.phone = "???";
}

Pack::~Pack()
{}

//输出快递信息
void Pack::output() const
{
	cout << "快递号：" << packID << endl;
	cout << "状态：";
	switch (state)
	{
	case NOT_PICK:
		cout << "未揽收" << endl;
		break;
	case NOT_RECEIVE:
		cout << "未接收" << endl;
		break;
	case RECEIVE:
		cout << "已接收" << endl;
		break;
	default:
		break;
	}
	cout << "寄件人：" << endl;
	cout << "寄件人ID：" << sender.ID << endl;
	cout << "寄件人姓名：" << sender.name << endl;
	cout << "寄件人电话：" << sender.phone << endl;
	cout << "寄件人地址：" << sender.addr << endl;
	cout << "发送时间：" << sendTime.wYear << "年" << sendTime.wMonth << "月" << sendTime.wDay << "日" << sendTime.wHour << "时" << sendTime.wMinute << "分" << endl;
	cout << "快递员ID：" << postman.ID << endl;
	cout << "快递员姓名：" << postman.name << endl;
	cout << "快递员电话：" << postman.phone << endl;
	if (state != NOT_PICK)
		cout << "揽收时间：" << pickUpTime.wYear << "年" << pickUpTime.wMonth << "月" << pickUpTime.wDay << "日" << pickUpTime.wHour << "时" << pickUpTime.wMinute << "分" << endl;
	cout << "收件人：" << endl;
	cout << "收件人ID：" << recver.ID << endl;
	cout << "收件人姓名：" << recver.name << endl;
	cout << "收件人电话：" << recver.phone << endl;
	cout << "收件人地址：" << recver.addr << endl;
	if (state == RECEIVE)
		cout << "接收时间：" << recvTime.wYear << "年" << recvTime.wMonth << "月" << recvTime.wDay << "日" << recvTime.wHour << "时" << recvTime.wMinute << "分" << endl;
	cout << "快递类型:";
	switch (type)
	{
	case NORMAL:
		cout << "普通快递" << endl;
		break;
	case FRAGILE:
		cout << "易碎品" << endl;
		break;
	case BOOK:
		cout << "书籍" << endl;
		break;
	default:
		break;
	}
	if (type == FRAGILE || type == NORMAL)
		cout << "快递重量：" << amount << "kg" << endl;
	else if (type == BOOK)
		cout << "快递书籍数量：" << amount << "本" << endl;
	cout << "运费：" << price << "元" << endl;

	cout << "快递详情：" << endl;
	cout << packInfo << endl;
}

//发送快递
void Pack::sendPack() const
{
	string tmp, req;
	//往packList中添加新快递
	packList.push_back(*this);

	//向服务端发送新快递信息
	tmp = to_string(price) + SPLIT + packID + SPLIT + to_string(state) + SPLIT + to_string(sendTime.wYear) + SPLIT + to_string(sendTime.wMonth) + SPLIT + to_string(sendTime.wDay) + SPLIT + to_string(sendTime.wHour) + SPLIT + to_string(sendTime.wMinute) 
		+ SPLIT + sender.ID + SPLIT + sender.name + SPLIT + sender.phone + SPLIT + sender.addr + SPLIT + recver.ID + SPLIT + recver.name + SPLIT + recver.phone + SPLIT + recver.addr + SPLIT + postman.ID + SPLIT + postman.name + SPLIT + postman.phone + SPLIT 
		+ to_string(type) + SPLIT + to_string(amount) + SPLIT + to_string(price) + SPLIT + packInfo;
	req = string("1") + SPLIT + '5' + SPLIT + tmp;
	strcpy_s(send_buf, req.c_str());
	send(server, send_buf, 1000, 0);
	recv(server, recv_buf, 1000, 0);
	if (recv_buf[0] != '1')
		cout << "发送错误" << endl;
	else
		cout << "发送成功" << endl;
}

//接收快递
void Pack::recvPack()
{
	string tmp, req;
	state = RECEIVE;
	GetSystemTime(&recvTime);
	send(server, packID.c_str(), packID.size(), 0);

	//向服务端发送接收的快递号和接收时间
	tmp = to_string(recvTime.wYear) + SPLIT + to_string(recvTime.wMonth) + SPLIT + to_string(recvTime.wDay) + SPLIT + to_string(recvTime.wHour) + SPLIT + to_string(recvTime.wMinute);
	req = string("1") + SPLIT + '6' + SPLIT + packID + SPLIT + tmp;
	strcpy_s(send_buf, req.c_str());
	send(server, send_buf, 1000, 0);
	recv(server, recv_buf, 1000, 0);
	if (recv_buf[0] != '1')
		cout << "接收失败" << endl;
	else
		cout << "接收成功" << endl;
}

//揽收快递
void Pack::pickUp()
{
	string tmp, req;
	state = NOT_RECEIVE;
	GetSystemTime(&pickUpTime);

	//向服务端发送揽收的快递号和揽收时间
	tmp = to_string(pickUpTime.wYear) + SPLIT + to_string(pickUpTime.wMonth) + SPLIT + to_string(pickUpTime.wDay) + SPLIT + to_string(pickUpTime.wHour) + SPLIT + to_string(pickUpTime.wMinute);
	req = string("2") + SPLIT + '3' + SPLIT + packID + SPLIT + tmp;
	strcpy_s(send_buf, req.c_str());
	send(server, send_buf, 1000, 0);

	recv(server, recv_buf, 1000, 0);
	if (recv_buf[0] != '1')
		cout << "揽收失败" << endl;
	else
		cout << "揽收成功" << endl;
}

//为快递分配快递员，参数为快递员信息
void Pack::assignPostman(const PackPostmanInfo& postmanInfo)
{
	string tmp, req;
	//改快递的快递员信息
	postman.ID = postmanInfo.ID;
	postman.name = postmanInfo.name;
	postman.phone = postmanInfo.phone;
	//向服务端发送要分配的快递和快递员信息
	tmp = packID + SPLIT + postmanInfo.ID + SPLIT + postmanInfo.name + SPLIT + postmanInfo.phone;
	req = string("0") + SPLIT + '6' + SPLIT + tmp;
	strcpy_s(send_buf, req.c_str());
	send(server, send_buf, 1000, 0);
	recv(server, recv_buf, 1000, 0);
	if (recv_buf[0] != '1')
		cout << "分配失败" << endl;
	else
		cout << "分配成功" << endl;
}

//删除快递的快递员信息
void Pack::deletePostman()
{
	state = NOT_PICK;
	postman.ID = "???";
	postman.name = "???";
	postman.phone = "???";
	
	//向服务端发送要修改的快递号
	string req = string("0") + SPLIT + '4' + SPLIT + packID;
	strcpy_s(send_buf, req.c_str());
	send(server, send_buf, 1000, 0);
	recv(server, recv_buf, 1000, 0);
	if (recv_buf[0] != '1')
		cout << "修改失败" << endl;
	else
		cout << "修改成功" << endl;
}

//计算快递的运费
void Pack::getPrice()
{
	//普通快递，5元/kg
	price = amount * 5;
}

Fragile::Fragile()
{}

//用于寄件的构造函数
Fragile::Fragile(const string& id, const SYSTEMTIME& sTime, const PackUserInfo& send, const PackUserInfo& recv, const int& ty, const int& am, const string& info)
	: Pack(id, sTime, send, recv, ty, am, info)
{}

Fragile::~Fragile()
{}

//计算快递的运费
void Fragile::getPrice()
{
	//易碎品，8元/kg
	price = amount * 8;
}

Book::Book()
{}

//用于寄件的构造函数
Book::Book(const string& id, const SYSTEMTIME& sTime, const PackUserInfo& send, const PackUserInfo& recv, const int& ty, const int& am, const string& info)
	: Pack(id, sTime, send, recv, ty, am, info)
{}

Book::~Book()
{}

//计算快递的运费
void Book::getPrice()
{
	//书籍，3元/本
	price = amount * 3;
}
