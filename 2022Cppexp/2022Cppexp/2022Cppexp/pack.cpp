#include "head1.h"

extern vector<Pack> packList;
extern fstream packInfoFile;
extern fstream adminInfoFile;


Pack::Pack()
	: state(NOT_RECEIVE)
	, sendTime()
	, recvTime()
{}

//用于读取文件输入的构造函数
Pack::Pack(const string& ipackID, const int& istate, const SYSTEMTIME& isendTime, const SYSTEMTIME& irecvTime, const PackUserInfo& isender, const PackUserInfo& irecver, const string& ipackInfo)
	: packID(ipackID)
	, state(istate)
	, sendTime(isendTime)
	, recvTime(irecvTime)
	, sender(isender)
	, recver(irecver)
	, packInfo(ipackInfo)
{}

//用于寄件的构造函数
Pack::Pack(const string& ipackID, const SYSTEMTIME& isendTime, const PackUserInfo& isender, const PackUserInfo& irecver, const string& ipackInfo)
	: packID(ipackID)
	, state(NOT_RECEIVE)
	, sendTime(isendTime)
	, recvTime()
	, sender(isender)
	, recver(irecver)
	, packInfo(ipackInfo)
{}

Pack::~Pack()
{}

//输出快递信息
void Pack::output() const
{
	cout << "快递号：" << packID << endl;
	cout << "状态：" << ((state == RECEIVE) ? "已接收" : "未接收") << endl;
	cout << "发送时间：" << sendTime.wYear << "年" << sendTime.wMonth << "月" << sendTime.wDay << "日" << sendTime.wHour << "时" << sendTime.wMinute << "分" << endl;
	//只有当快递已被接收时，才有接收时间
	if (state == RECEIVE)
		cout << "接收时间：" << recvTime.wYear << "年" << recvTime.wMonth << "月" << recvTime.wDay << "日" << recvTime.wHour << "时" << recvTime.wMinute << "分" << endl;
	cout << "寄件人：" << endl;
	cout << "寄件人ID：" << sender.ID << endl;
	cout << "寄件人姓名：" << sender.name << endl;
	cout << "寄件人电话：" << sender.phone << endl;
	cout << "寄件人地址：" << sender.addr << endl;
	cout << "收件人：" << endl;
	cout << "收件人ID：" << recver.ID << endl;
	cout << "收件人姓名：" << recver.name << endl;
	cout << "收件人电话：" << recver.phone << endl;
	cout << "收件人地址：" << recver.addr << endl;
	cout << "快递详情：" << endl;
	cout << packInfo << endl;
}

//发送快递
void Pack::sendPack() const
{
	//往packList中添加新快递
	packList.push_back(*this);

	//向快递信息文件末尾写入新快递信息
	//刚发送的快递一定是未接收的状态，不需输出接收时间
	packInfoFile.clear();
	packInfoFile.seekg(0, ios::end);
	packInfoFile << packID << SPLIT << state << SPLIT;
	packInfoFile << sendTime.wYear << SPLIT << sendTime.wMonth << SPLIT << sendTime.wDay << SPLIT << sendTime.wHour << SPLIT << sendTime.wMinute << SPLIT;
	packInfoFile << sender.ID << SPLIT << sender.name << SPLIT << sender.addr << SPLIT << sender.phone << SPLIT;
	packInfoFile << recver.ID << SPLIT << recver.name << SPLIT << recver.addr << SPLIT << recver.phone << SPLIT;
	packInfoFile << packInfo << endl;

	//更改管理员的余额
	fstream tmpFile;
	string tmp;
	int balance;
	tmpFile.open(".\\data\\tmp.txt", ios::in | ios::out | ios::trunc);
	adminInfoFile.clear();
	adminInfoFile.seekg(0);
	//读取余额，余额+15，写入临时文件
	adminInfoFile >> balance;
	adminInfoFile.ignore(LLONG_MAX, '\n');
	balance += 15;
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
		if(tmp.length())
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
		//读到当前快递则退出循环
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
