#include "serverhead.h"

extern fstream userInfoFile;
extern fstream packInfoFile;
extern fstream adminInfoFile;

User::User()
	: locInFile(-1)
{}

User::~User()
{}

//从客户端收到用户的指令
void User::recvFromClient(const string& req)
{
	istringstream iss(req);
	string op, info;

	getline(iss, op, SPLIT);	//获取指令操作类型
	getline(iss, info);			//获取所需的数据

	switch (op[0])	//根据操作类型选择对应函数执行指令
	{
	case '0':
		checkExistId(info);		
		break;
	case '1':
		sendPw(info);
		break;
	case '2':
		getInfo(info);
		break;
	case '3':
		changePW(info);
		break;
	case '4':
		changeBalance(info);
		break;
	case '5':
		sendPack(info);
		break;
	case '6':
		recvPack(info);
		break;
	case '7':
		loadPacketUserInfo(info);
		break;
	case '8':
		newUser(info);
		break;
	case '9':
		loadUserPackList(info);
		break;
	default:
		break;
	}
}

//检查用户ID id是否存在，存在发送1，否则发送0
void User::checkExistId(const string& id) const
{
	streampos loc = searchID(id);	//获取用户号为id的用户在文件的位置

	if (loc == -1)			
		strcpy_s(send_buf, "0");
	else
		strcpy_s(send_buf, "1");
	send(client, send_buf, 1000, 0);
}

//发送用户ID id的密码
void User::sendPw(const string& id) const
{
	streampos loc = searchID(id);	//获取用户号为id的用户在文件的位置
	string pw;
	
	userInfoFile.clear();
	userInfoFile.seekg(loc);
	userInfoFile.ignore(LLONG_MAX, SPLIT);
	getline(userInfoFile, pw, SPLIT);	//读取密码

	strcpy_s(send_buf, pw.c_str());
	send(client, send_buf, 1000, 0);	//发送给客户端
}

//发送用户ID id的用户信息
void User::getInfo(const string& id)
{
	locInFile = searchID(id);	//获取用户号为id的用户在文件的位置
	string info;

	userInfoFile.clear();
	userInfoFile.seekg(locInFile);
	userInfoFile.ignore(LLONG_MAX, SPLIT);
	userInfoFile.ignore(LLONG_MAX, SPLIT);
	getline(userInfoFile, info);	//读取ID和密码以外的用户信息

	strcpy_s(send_buf, info.c_str());
	send(client, send_buf, 1000, 0);//发送给客户端
}

//更改用户ID id的密码
void User::changePW(const string& pw) const
{
	writeToFile(1, pw);				//在文件中更改密码
	strcpy_s(send_buf, "1");		
	send(client, send_buf, 1000, 0);//发送1表示更改成功
}

//更改用户ID id的余额
void User::changeBalance(const string& balance) const
{
	writeToFile(2, balance);		//在文件中更改余额
	strcpy_s(send_buf, "1");
	send(client, send_buf, 1000, 0);//发送1表示更改成功
}

//发送快递
void User::sendPack(const string& packInfo) const
{
	istringstream iss(packInfo);
	string tmp;
	int price;

	getline(iss, tmp, SPLIT);
	price = atoi(tmp.c_str());	//获取新发送的快递的运费

	//更改管理员的余额
	fstream tmpFile;
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

	getline(iss, tmp);		//获取新发送的快递信息
	packInfoFile.clear();
	packInfoFile.seekg(0, ios::end);	
	packInfoFile << tmp << endl;	//向快递文件的末尾添加
	strcpy_s(send_buf, "1");
	send(client, send_buf, 1000, 0);//发送1表示发送成功
}

//接收快递
void User::recvPack(const string& packInfo) const
{
	istringstream iss(packInfo);
	string packID, info, sendTime, recvTime, pickUpTime, type, amount, price, state;
	PackUserInfo sender, recver;
	PackPostmanInfo postman;
	int stateInt;
	string tmp;

	getline(iss, packID, SPLIT);	//获取接收的快递的快递号
	getline(iss, recvTime);			//获取接收时间

	fstream tmpFile;
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
	//读到接收的快递，读取该快递的信息
	getline(packInfoFile, state, SPLIT);
	stateInt = atoi(state.c_str());
	stateInt = RECEIVE;
	getline(packInfoFile, tmp, SPLIT);
	sendTime += tmp + SPLIT;
	getline(packInfoFile, tmp, SPLIT);
	sendTime += tmp + SPLIT;
	getline(packInfoFile, tmp, SPLIT);
	sendTime += tmp + SPLIT;
	getline(packInfoFile, tmp, SPLIT);
	sendTime += tmp + SPLIT;
	getline(packInfoFile, tmp, SPLIT);
	sendTime += tmp + SPLIT;
	getline(packInfoFile, sender.ID, SPLIT);
	getline(packInfoFile, sender.name, SPLIT);
	getline(packInfoFile, sender.addr, SPLIT);
	getline(packInfoFile, sender.phone, SPLIT);
	getline(packInfoFile, recver.ID, SPLIT);
	getline(packInfoFile, recver.name, SPLIT);
	getline(packInfoFile, recver.addr, SPLIT);
	getline(packInfoFile, recver.phone, SPLIT);
	getline(packInfoFile, postman.ID, SPLIT);
	getline(packInfoFile, postman.name, SPLIT);
	getline(packInfoFile, postman.phone, SPLIT);
	getline(packInfoFile, tmp, SPLIT);
	pickUpTime += tmp + SPLIT;
	getline(packInfoFile, tmp, SPLIT);
	pickUpTime += tmp + SPLIT;
	getline(packInfoFile, tmp, SPLIT);
	pickUpTime += tmp + SPLIT;
	getline(packInfoFile, tmp, SPLIT);
	pickUpTime += tmp + SPLIT;
	getline(packInfoFile, tmp, SPLIT);
	pickUpTime += tmp + SPLIT;
	getline(packInfoFile, type, SPLIT);
	getline(packInfoFile, amount, SPLIT);
	getline(packInfoFile, price, SPLIT);
	getline(packInfoFile, info);
	//将快递信息新增接收时间后写入临时文件
	tmpFile << packID << SPLIT << stateInt << SPLIT;
	tmpFile << sendTime;
	tmpFile << recvTime << SPLIT;
	tmpFile << sender.ID << SPLIT << sender.name << SPLIT << sender.addr << SPLIT << sender.phone << SPLIT;
	tmpFile << recver.ID << SPLIT << recver.name << SPLIT << recver.addr << SPLIT << recver.phone << SPLIT;
	tmpFile << postman.ID << SPLIT << postman.name << SPLIT << postman.phone << SPLIT;
	tmpFile << pickUpTime;
	tmpFile << type << SPLIT << amount << SPLIT << price << SPLIT;
	tmpFile << info << endl;
	//快递信息文件剩余内容写入临时文件
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
	strcpy_s(send_buf, "1");
	send(client, send_buf, 1000, 0);	//发送1表示接收成功
}

//加载用户ID为id的收件人信息
void User::loadPacketUserInfo(const string& id) const
{
	streampos loc = searchID(id);	//获取用户号为id的用户在文件的位置
	string name, phone, addr, recvInfo;

	userInfoFile.clear();
	userInfoFile.seekg(loc);

	//跳过收件人用户名和密码
	userInfoFile.ignore(LLONG_MAX, SPLIT);
	userInfoFile.ignore(LLONG_MAX, SPLIT);
	//读取所需收件人信息
	getline(userInfoFile, name, SPLIT);
	getline(userInfoFile, phone, SPLIT);
	getline(userInfoFile, addr, SPLIT);

	recvInfo = name + SPLIT + phone + SPLIT + addr;
	strcpy_s(send_buf, recvInfo.c_str());
	send(client, send_buf, 1000, 0);	//发送收件人信息给客户端
}

//向文件添加新用户
void User::newUser(const string& userInfo)
{
	writeToFile(0, userInfo);	//向文件写入新用户信息
	strcpy_s(send_buf, "1");
	send(client, send_buf, 1000, 0);	//发送1表示添加成功
}

//从文件中读取与用户ID为id的用户相关的快递并发送
void User::loadUserPackList(const string& id) const
{
	packInfoFile.clear();
	packInfoFile.seekg(0);

	while (!packInfoFile.eof())
	{
		string s;
		string packID, packInfo;
		PackUserInfo sender, recver;
		PackPostmanInfo post;
		string sendTime, recvTime, pickUpTime;
		string type, amount, price, state;
		int stateInt;

		getline(packInfoFile, packID, SPLIT);
		//避免读取空行
		if (!packID.length())
			continue;
		getline(packInfoFile, state, SPLIT);
		stateInt = atoi(state.c_str());
		getline(packInfoFile, s, SPLIT);
		sendTime += s + SPLIT;
		getline(packInfoFile, s, SPLIT);
		sendTime += s + SPLIT;
		getline(packInfoFile, s, SPLIT);
		sendTime += s + SPLIT;
		getline(packInfoFile, s, SPLIT);
		sendTime += s + SPLIT;
		getline(packInfoFile, s, SPLIT);
		sendTime += s;
		//已接收的快递才有接收时间
		if (stateInt == RECEIVE)
		{
			getline(packInfoFile, s, SPLIT);
			recvTime += s + SPLIT;
			getline(packInfoFile, s, SPLIT);
			recvTime += s + SPLIT;
			getline(packInfoFile, s, SPLIT);
			recvTime += s + SPLIT;
			getline(packInfoFile, s, SPLIT);
			recvTime += s + SPLIT;
			getline(packInfoFile, s, SPLIT);
			recvTime += s;
		}
		getline(packInfoFile, sender.ID, SPLIT);
		getline(packInfoFile, sender.name, SPLIT);
		getline(packInfoFile, sender.addr, SPLIT);
		getline(packInfoFile, sender.phone, SPLIT);
		getline(packInfoFile, recver.ID, SPLIT);
		getline(packInfoFile, recver.name, SPLIT);
		getline(packInfoFile, recver.addr, SPLIT);
		getline(packInfoFile, recver.phone, SPLIT);
		//寄收件人如果和当前用户无关，用户没有权限查看，不向packList存储
		if (recver.ID.compare(id) && sender.ID.compare(id))
		{
			packInfoFile.ignore(LLONG_MAX, '\n');
			continue;
		}
		getline(packInfoFile, post.ID, SPLIT);
		getline(packInfoFile, post.name, SPLIT);
		getline(packInfoFile, post.phone, SPLIT);
		//未揽收的快递没有揽收时间
		if (stateInt != NOT_PICK)
		{
			getline(packInfoFile, s, SPLIT);
			pickUpTime += s + SPLIT;
			getline(packInfoFile, s, SPLIT);
			pickUpTime += s + SPLIT;
			getline(packInfoFile, s, SPLIT);
			pickUpTime += s + SPLIT;
			getline(packInfoFile, s, SPLIT);
			pickUpTime += s + SPLIT;
			getline(packInfoFile, s, SPLIT);
			pickUpTime += s;
		}
		getline(packInfoFile, type, SPLIT);
		getline(packInfoFile, amount, SPLIT);
		getline(packInfoFile, price, SPLIT);
		getline(packInfoFile, packInfo);

		if (stateInt == NOT_PICK)
		{
			s = packID + SPLIT + state + SPLIT + sendTime + SPLIT + sender.ID + SPLIT + sender.name + SPLIT + sender.phone + SPLIT + sender.addr + SPLIT +
				recver.ID + SPLIT + recver.name + SPLIT + recver.phone + SPLIT + recver.addr + SPLIT + post.ID + SPLIT + post.name + SPLIT + post.phone + SPLIT +
				type + SPLIT + amount + SPLIT + price + SPLIT + packInfo + SEND_SPLIT;
		}
		else if (stateInt == NOT_RECEIVE)
		{
			s = packID + SPLIT + state + SPLIT + sendTime + SPLIT + sender.ID + SPLIT + sender.name + SPLIT + sender.phone + SPLIT + sender.addr + SPLIT +
				recver.ID + SPLIT + recver.name + SPLIT + recver.phone + SPLIT + recver.addr + SPLIT + post.ID + SPLIT + post.name + SPLIT + post.phone + SPLIT + pickUpTime + SPLIT +
				type + SPLIT + amount + SPLIT + price + SPLIT + packInfo + SEND_SPLIT;
		}
		else
		{
			s = packID + SPLIT + state + SPLIT + sendTime + SPLIT + recvTime + SPLIT + sender.ID + SPLIT + sender.name + SPLIT + sender.phone + SPLIT + sender.addr + SPLIT +
				recver.ID + SPLIT + recver.name + SPLIT + recver.phone + SPLIT + recver.addr + SPLIT + post.ID + SPLIT + post.name + SPLIT + post.phone + SPLIT + pickUpTime + SPLIT +
				type + SPLIT + amount + SPLIT + price + SPLIT + packInfo + SEND_SPLIT;
		}
		
		strcpy_s(send_buf, s.c_str());
		send(client, send_buf, 1000, 0);	//向客户端发送快递信息
	}
	strcpy_s(send_buf, "");
	send(client, send_buf, 1000, 0);		//发送一个空串表示结束
}

//返回用户ID为id的用户信息在文件中的位置
streampos User::searchID(const string& id) const
{
	string s;
	streampos loc = 0;
	userInfoFile.clear();
	userInfoFile.seekg(0);

	//文件为空，直接返回0
	if (userInfoFile.eof())
		return loc;

	//在文件中查找id
	while (!userInfoFile.eof())
	{
		loc = userInfoFile.tellg();
		getline(userInfoFile, s, SPLIT);
		if (!s.compare(id))
			return loc;
		userInfoFile.ignore(LLONG_MAX, '\n');
	}

	//找不到，返回-1
	return -1;
}

//将用户信息写入文件，mode=0写入新用户信息，mode=1更改某用户的密码，mode=1更改某用户的余额
void User::writeToFile(int mode, const string& info) const
{
	if (mode == 0)
	{
		//在文件末尾写入新用户信息
		userInfoFile.clear();
		userInfoFile.seekg(0, ios::end);
		userInfoFile << info << endl;
		return;
	}

	string ID, pw, name, phone, addr, balance;
	fstream tmpFile;
	tmpFile.open(".\\data\\tmp.txt", ios::in | ios::out | ios::trunc);
	string tmp;
	userInfoFile.clear();
	userInfoFile.seekg(0);

	//将用户信息文件中，当前用户信息之前的内容写入临时文件
	while (userInfoFile.tellg() != locInFile)
	{
		getline(userInfoFile, tmp);
		//空行不写
		if (tmp.length())
			tmpFile << tmp << endl;
	}
	getline(userInfoFile, ID, SPLIT);
	if (mode == 1)
		userInfoFile.ignore(LLONG_MAX, SPLIT);
	else
		getline(userInfoFile, pw, SPLIT);
	getline(userInfoFile, name, SPLIT);
	getline(userInfoFile, phone, SPLIT);
	getline(userInfoFile, addr, SPLIT);
	if (mode == 2)
		userInfoFile.ignore(LLONG_MAX, '\n');
	else
		getline(userInfoFile, balance);
	//当前用户信息（更新后）写入临时文件
	if (mode == 1)
		tmpFile << ID << SPLIT << info << SPLIT << name << SPLIT << phone << SPLIT << addr << SPLIT << balance << endl;
	else if (mode == 2)
		tmpFile << ID << SPLIT << pw << SPLIT << name << SPLIT << phone << SPLIT << addr << SPLIT << info << endl;

	//将用户信息文件中，当前用户信息之后的内容写入临时文件
	while (!userInfoFile.eof())
	{
		getline(userInfoFile, tmp);
		//空行不写
		if (tmp.length())
			tmpFile << tmp << endl;
	}
	tmpFile.clear();
	tmpFile.seekg(0);
	//刷新用户信息文件，从临时文件中写入新内容
	userInfoFile.close();
	userInfoFile.open(".\\data\\userInfo.txt", ios::in | ios::out | ios::trunc);
	while (!tmpFile.eof())
	{
		getline(tmpFile, tmp);
		//空行不写
		if (tmp.length())
			userInfoFile << tmp << endl;
	}
	tmpFile.close();
}
