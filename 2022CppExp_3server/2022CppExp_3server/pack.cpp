#include "serverhead.h"

extern vector<Pack> packList;
extern fstream adminInfoFile;
extern fstream packInfoFile;

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
	//往packList中添加新快递
	packList.push_back(*this);

	//向快递信息文件末尾写入新快递信息
	packInfoFile.clear();
	packInfoFile.seekg(0, ios::end);
	packInfoFile << packID << SPLIT << state << SPLIT;
	packInfoFile << sendTime.wYear << SPLIT << sendTime.wMonth << SPLIT << sendTime.wDay << SPLIT << sendTime.wHour << SPLIT << sendTime.wMinute << SPLIT;
	packInfoFile << sender.ID << SPLIT << sender.name << SPLIT << sender.addr << SPLIT << sender.phone << SPLIT;
	packInfoFile << recver.ID << SPLIT << recver.name << SPLIT << recver.addr << SPLIT << recver.phone << SPLIT;
	packInfoFile << postman.ID << SPLIT << postman.name << SPLIT << postman.phone << SPLIT;
	packInfoFile << type << SPLIT << amount << SPLIT << price << SPLIT;
	packInfoFile << packInfo << endl;

	//更改管理员的余额
	fstream tmpFile;
	string tmp;
	int balance;
	tmpFile.open(".\\data\\tmp.txt", ios::in | ios::out | ios::trunc);
	adminInfoFile.clear();
	adminInfoFile.seekg(0);
	//读取余额，余额加运费的一半（给管理员的向下取整），写入临时文件
	adminInfoFile >> balance;
	adminInfoFile.ignore(LLONG_MAX, '\n');
	balance += price / 2;
	tmpFile << balance << endl;
	//管理员信息文件其他内容写入临时文件
	while (!adminInfoFile.eof())
	{
		getline(adminInfoFile, tmp);
		if (tmp.length())
			tmpFile << tmp << endl;
	}
	//临时文件内容覆盖管理员信息文件
	adminInfoFile.close();
	adminInfoFile.open(".\\data\\adminInfo.txt", ios::in | ios::out | ios::trunc);
	tmpFile.clear();
	tmpFile.seekg(0);
	while (!tmpFile.eof())
	{
		getline(tmpFile, tmp);
		if (tmp.length())
			adminInfoFile << tmp << endl;
	}
	tmpFile.close();
}

//接收快递
void Pack::recvPack()
{
	//改快递状态并获取当前接收时间
	state = RECEIVE;
	GetSystemTime(&recvTime);

	fstream tmpFile;
	string tmp;
	tmpFile.open(".\\data\\tmp.txt", ios::in | ios::out | ios::trunc);
	packInfoFile.clear();
	packInfoFile.seekg(0);
	//快递信息文件中，当前快递之前的内容写入临时文件
	while (!packInfoFile.eof())
	{
		getline(packInfoFile, tmp, SPLIT);
		if (!tmp.compare(packID))
			break;
		if (tmp.length())
		{
			tmpFile << tmp << SPLIT;
			getline(packInfoFile, tmp);
			tmpFile << tmp << endl;
		}
	}
	//当前快递信息写入临时文件
	tmpFile << packID << SPLIT << state << SPLIT;
	tmpFile << sendTime.wYear << SPLIT << sendTime.wMonth << SPLIT << sendTime.wDay << SPLIT << sendTime.wHour << SPLIT << sendTime.wMinute << SPLIT;
	tmpFile << recvTime.wYear << SPLIT << recvTime.wMonth << SPLIT << recvTime.wDay << SPLIT << recvTime.wHour << SPLIT << recvTime.wMinute << SPLIT;
	tmpFile << sender.ID << SPLIT << sender.name << SPLIT << sender.addr << SPLIT << sender.phone << SPLIT;
	tmpFile << recver.ID << SPLIT << recver.name << SPLIT << recver.addr << SPLIT << recver.phone << SPLIT;
	tmpFile << postman.ID << SPLIT << postman.name << SPLIT << postman.phone << SPLIT;
	tmpFile << pickUpTime.wYear << SPLIT << pickUpTime.wMonth << SPLIT << pickUpTime.wDay << SPLIT << pickUpTime.wHour << SPLIT << pickUpTime.wMinute << SPLIT;
	tmpFile << type << SPLIT << amount << SPLIT << price << SPLIT;
	tmpFile << packInfo << endl;
	//快递信息文件剩余内容写入临时文件
	packInfoFile.ignore(LLONG_MAX, '\n');
	while (!packInfoFile.eof())
	{
		getline(packInfoFile, tmp);
		if (tmp.length())
			tmpFile << tmp << endl;
	}
	//临时文件内容覆盖快递信息文件
	tmpFile.clear();
	tmpFile.seekg(0);
	packInfoFile.close();
	packInfoFile.open(".\\data\\packInfo.txt", ios::out | ios::in | ios::trunc);
	while (!tmpFile.eof())
	{
		getline(tmpFile, tmp);
		if (tmp.length())
			packInfoFile << tmp << endl;
	}
	tmpFile.close();
}

//揽收快递
void Pack::pickUp()
{
	//改快递状态，获取当前时间作为揽收时间
	state = NOT_RECEIVE;
	GetSystemTime(&pickUpTime);

	fstream tmpFile;
	string tmp;
	tmpFile.open(".\\data\\tmp.txt", ios::in | ios::out | ios::trunc);
	packInfoFile.clear();
	packInfoFile.seekg(0);
	//快递信息文件中，当前快递之前的内容写入临时文件
	while (!packInfoFile.eof())
	{
		getline(packInfoFile, tmp, SPLIT);
		if (!tmp.compare(packID))
			break;
		if (tmp.length())
		{
			tmpFile << tmp << SPLIT;
			getline(packInfoFile, tmp);
			tmpFile << tmp << endl;
		}
	}
	//当前快递信息写入临时文件
	tmpFile << packID << SPLIT << state << SPLIT;
	tmpFile << sendTime.wYear << SPLIT << sendTime.wMonth << SPLIT << sendTime.wDay << SPLIT << sendTime.wHour << SPLIT << sendTime.wMinute << SPLIT;
	tmpFile << sender.ID << SPLIT << sender.name << SPLIT << sender.addr << SPLIT << sender.phone << SPLIT;
	tmpFile << recver.ID << SPLIT << recver.name << SPLIT << recver.addr << SPLIT << recver.phone << SPLIT;
	tmpFile << postman.ID << SPLIT << postman.name << SPLIT << postman.phone << SPLIT;
	tmpFile << pickUpTime.wYear << SPLIT << pickUpTime.wMonth << SPLIT << pickUpTime.wDay << SPLIT << pickUpTime.wHour << SPLIT << pickUpTime.wMinute << SPLIT;
	tmpFile << type << SPLIT << amount << SPLIT << price << SPLIT;
	tmpFile << packInfo << endl;
	//快递信息文件剩余内容写入临时文件
	packInfoFile.ignore(LLONG_MAX, '\n');
	while (!packInfoFile.eof())
	{
		getline(packInfoFile, tmp);
		if (tmp.length())
			tmpFile << tmp << endl;
	}
	//临时文件内容覆盖快递信息文件
	tmpFile.clear();
	tmpFile.seekg(0);
	packInfoFile.close();
	packInfoFile.open(".\\data\\packInfo.txt", ios::out | ios::in | ios::trunc);
	while (!tmpFile.eof())
	{
		getline(tmpFile, tmp);
		if (tmp.length())
			packInfoFile << tmp << endl;
	}
	tmpFile.close();
}

//为快递分配快递员，参数为快递员信息
void Pack::assignPostman(const PackPostmanInfo& postmanInfo)
{
	//改快递的快递员信息
	postman.ID = postmanInfo.ID;
	postman.name = postmanInfo.name;
	postman.phone = postmanInfo.phone;

	fstream tmpFile;
	string tmp;
	tmpFile.open(".\\data\\tmp.txt", ios::in | ios::out | ios::trunc);
	packInfoFile.clear();
	packInfoFile.seekg(0);
	//快递信息文件中，当前快递之前的内容写入临时文件
	while (!packInfoFile.eof())
	{
		getline(packInfoFile, tmp, SPLIT);
		if (!tmp.compare(packID))
			break;
		if (tmp.length())
		{
			tmpFile << tmp << SPLIT;
			getline(packInfoFile, tmp);
			tmpFile << tmp << endl;
		}
	}
	//当前快递信息写入临时文件
	tmpFile << packID << SPLIT << state << SPLIT;
	tmpFile << sendTime.wYear << SPLIT << sendTime.wMonth << SPLIT << sendTime.wDay << SPLIT << sendTime.wHour << SPLIT << sendTime.wMinute << SPLIT;
	tmpFile << sender.ID << SPLIT << sender.name << SPLIT << sender.addr << SPLIT << sender.phone << SPLIT;
	tmpFile << recver.ID << SPLIT << recver.name << SPLIT << recver.addr << SPLIT << recver.phone << SPLIT;
	tmpFile << postman.ID << SPLIT << postman.name << SPLIT << postman.phone << SPLIT;
	tmpFile << type << SPLIT << amount << SPLIT << price << SPLIT;
	tmpFile << packInfo << endl;
	//快递信息文件剩余内容写入临时文件
	packInfoFile.ignore(LLONG_MAX, '\n');
	while (!packInfoFile.eof())
	{
		getline(packInfoFile, tmp);
		if (tmp.length())
			tmpFile << tmp << endl;
	}
	//临时文件内容覆盖快递信息文件
	tmpFile.clear();
	tmpFile.seekg(0);
	packInfoFile.close();
	packInfoFile.open(".\\data\\packInfo.txt", ios::out | ios::in | ios::trunc);
	while (!tmpFile.eof())
	{
		getline(tmpFile, tmp);
		if (tmp.length())
			packInfoFile << tmp << endl;
	}
	tmpFile.close();
}

//删除快递的快递员信息
void Pack::deletePostman()
{
	//将状态改回未揽收，并删去已分配的快递员信息
	state = NOT_PICK;
	postman.ID = "???";
	postman.name = "???";
	postman.phone = "???";

	fstream tmpFile;
	string tmp;
	tmpFile.open(".\\data\\tmp.txt", ios::in | ios::out | ios::trunc);
	packInfoFile.clear();
	packInfoFile.seekg(0);
	//快递信息文件中，当前快递之前的内容写入临时文件
	while (!packInfoFile.eof())
	{
		getline(packInfoFile, tmp, SPLIT);
		if (!tmp.compare(packID))
			break;
		if (tmp.length())
		{
			tmpFile << tmp << SPLIT;
			getline(packInfoFile, tmp);
			tmpFile << tmp << endl;
		}
	}
	//当前快递信息写入临时文件
	tmpFile << packID << SPLIT << state << SPLIT;
	tmpFile << sendTime.wYear << SPLIT << sendTime.wMonth << SPLIT << sendTime.wDay << SPLIT << sendTime.wHour << SPLIT << sendTime.wMinute << SPLIT;
	tmpFile << sender.ID << SPLIT << sender.name << SPLIT << sender.addr << SPLIT << sender.phone << SPLIT;
	tmpFile << recver.ID << SPLIT << recver.name << SPLIT << recver.addr << SPLIT << recver.phone << SPLIT;
	tmpFile << postman.ID << SPLIT << postman.name << SPLIT << postman.phone << SPLIT;
	tmpFile << type << SPLIT << amount << SPLIT << price << SPLIT;
	tmpFile << packInfo << endl;
	//快递信息文件剩余内容写入临时文件
	packInfoFile.ignore(LLONG_MAX, '\n');
	while (!packInfoFile.eof())
	{
		getline(packInfoFile, tmp);
		if (tmp.length())
			tmpFile << tmp << endl;
	}
	//临时文件内容覆盖快递信息文件
	tmpFile.clear();
	tmpFile.seekg(0);
	packInfoFile.close();
	packInfoFile.open(".\\data\\packInfo.txt", ios::out | ios::in | ios::trunc);
	while (!tmpFile.eof())
	{
		getline(tmpFile, tmp);
		if (tmp.length())
			packInfoFile << tmp << endl;
	}
	tmpFile.close();
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
