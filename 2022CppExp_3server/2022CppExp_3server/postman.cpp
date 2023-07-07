#include "serverhead.h"

extern fstream postmanInfoFile;
extern fstream packInfoFile;

Postman::Postman()
	: locInFile(-1)
{}

Postman::~Postman()
{}

//从客户端收到快递员的指令
void Postman::recvFromClient(const string& req)
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
		pickUp(info);
		break;
	case '4':
		updateBalanceInFile(info);
		break;
	case '5':
		loadPostmanPackList(info);
		break;
	default:
		break;
	}
}

//检查快递员ID id是否存在，存在发送1，否则发送0
void Postman::checkExistId(const string& id) const
{
	streampos loc = searchID(id);	//获取用户号为id的快递员在文件的位置

	if (loc == -1)
		strcpy_s(send_buf, "0");
	else
		strcpy_s(send_buf, "1");
	send(client, send_buf, 1000, 0);
}

//发送快递员ID id的密码
void Postman::sendPw(const string& id) const
{
	streampos loc = searchID(id);	//获取用户号为id的快递员在文件的位置
	string pw;

	postmanInfoFile.clear();
	postmanInfoFile.seekg(loc);
	postmanInfoFile.ignore(LLONG_MAX, SPLIT);
	getline(postmanInfoFile, pw, SPLIT);	//读取密码

	strcpy_s(send_buf, pw.c_str());
	send(client, send_buf, 1000, 0);	//发送给客户端
}

//发送快递员ID id的用户信息
void Postman::getInfo(const string& id)
{
	locInFile = searchID(id);	//获取用户号为id的快递员在文件的位置
	string info;

	postmanInfoFile.clear();
	postmanInfoFile.seekg(locInFile);
	postmanInfoFile.ignore(LLONG_MAX, SPLIT);
	postmanInfoFile.ignore(LLONG_MAX, SPLIT);
	getline(postmanInfoFile, info);	//读取ID和密码以外的快递员信息

	strcpy_s(send_buf, info.c_str());
	send(client, send_buf, 1000, 0);//发送给客户端
}

//揽收快递
void Postman::pickUp(const string& info) const
{
	istringstream iss(info);
	string packID, packInfo, sendTime, pickUpTime, type, amount, price, state;
	PackUserInfo sender, recver;
	PackPostmanInfo postman;
	int stateInt;
	string tmp;
	fstream tmpFile;

	getline(iss, packID, SPLIT);	//获取揽收的快递的快递号
	getline(iss, pickUpTime);		//获取揽收时间

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
	getline(packInfoFile, state, SPLIT);
	stateInt = atoi(state.c_str());
	stateInt = NOT_RECEIVE;
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
	getline(packInfoFile, type, SPLIT);
	getline(packInfoFile, amount, SPLIT);
	getline(packInfoFile, price, SPLIT);
	getline(packInfoFile, packInfo);
	//当前快递信息写入临时文件
	tmpFile << packID << SPLIT << stateInt << SPLIT;
	tmpFile << sendTime;
	tmpFile << sender.ID << SPLIT << sender.name << SPLIT << sender.addr << SPLIT << sender.phone << SPLIT;
	tmpFile << recver.ID << SPLIT << recver.name << SPLIT << recver.addr << SPLIT << recver.phone << SPLIT;
	tmpFile << postman.ID << SPLIT << postman.name << SPLIT << postman.phone << SPLIT;
	tmpFile << pickUpTime << SPLIT;
	tmpFile << type << SPLIT << amount << SPLIT << price << SPLIT;
	tmpFile << packInfo << endl;
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

//在快递员信息文件中更新余额
void Postman::updateBalanceInFile(const string& balance) const
{
	string ID, pw, name, phone;
	fstream tmpFile;
	tmpFile.open(".\\data\\tmp.txt", ios::in | ios::out | ios::trunc);
	string tmp;
	postmanInfoFile.clear();
	postmanInfoFile.seekg(0);

	//将快递员信息文件中，当前快递员之前的内容写入临时文件
	while (postmanInfoFile.tellg() != locInFile)
	{
		getline(postmanInfoFile, tmp);
		//空行不写
		if (tmp.length())
			tmpFile << tmp << endl;
	}
	getline(postmanInfoFile, ID, SPLIT);
	getline(postmanInfoFile, pw, SPLIT);
	getline(postmanInfoFile, name, SPLIT);
	getline(postmanInfoFile, phone, SPLIT);
	//当前快递员信息（更新后）写入临时文件
	tmpFile << ID << SPLIT << pw << SPLIT << name << SPLIT << phone << SPLIT << balance << endl;
	postmanInfoFile.ignore(LLONG_MAX, '\n');
	//当前快递员之后的内容写入临时文件
	while (!postmanInfoFile.eof())
	{
		getline(postmanInfoFile, tmp);
		//空行不写
		if (tmp.length())
			tmpFile << tmp << endl;
	}
	tmpFile.clear();
	tmpFile.seekg(0);
	//临时文件内容覆盖快递员信息文件
	postmanInfoFile.close();
	postmanInfoFile.open(".\\data\\postmanInfo.txt", ios::in | ios::out | ios::trunc);
	while (!tmpFile.eof())
	{
		getline(tmpFile, tmp);
		//空行不写
		if (tmp.length())
			postmanInfoFile << tmp << endl;
	}
	tmpFile.close();
	strcpy_s(send_buf, "1");
	send(client, send_buf, 1000, 0);	//发送1表示接收成功
}

//从文件读取当前快递员负责的快递并发送
void Postman::loadPostmanPackList(const string& id) const
{
	packInfoFile.clear();
	packInfoFile.seekg(0);
	while (!packInfoFile.eof())
	{
		string s, tmp;
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
		getline(packInfoFile, post.ID, SPLIT);
		if (post.ID.compare(id))
		{
			packInfoFile.ignore(LLONG_MAX, '\n');
			continue;
		}
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
			tmp = packID + SPLIT + state + SPLIT + sendTime + SPLIT + sender.ID + SPLIT + sender.name + SPLIT + sender.phone + SPLIT + sender.addr + SPLIT + recver.ID + SPLIT + recver.name + SPLIT + recver.phone + SPLIT + recver.addr + SPLIT + post.ID + SPLIT + post.name + SPLIT + post.phone + SPLIT + type + SPLIT + amount + SPLIT + price + SPLIT + packInfo + SEND_SPLIT;

		}
		else if (stateInt == NOT_RECEIVE)
		{
			tmp = packID + SPLIT + state + SPLIT + sendTime + SPLIT + sender.ID + SPLIT + sender.name + SPLIT + sender.phone + SPLIT + sender.addr + SPLIT + recver.ID + SPLIT + recver.name + SPLIT + recver.phone + SPLIT + recver.addr + SPLIT + post.ID + SPLIT + post.name + SPLIT + post.phone + SPLIT + pickUpTime + SPLIT + type + SPLIT + amount + SPLIT + price + SPLIT + packInfo + SEND_SPLIT;
		}
		else
		{
			tmp = packID + SPLIT + state + SPLIT + sendTime + SPLIT + recvTime + SPLIT + sender.ID + SPLIT + sender.name + SPLIT + sender.phone + SPLIT + sender.addr + SPLIT + recver.ID + SPLIT + recver.name + SPLIT + recver.phone + SPLIT + recver.addr + SPLIT + post.ID + SPLIT + post.name + SPLIT + post.phone + SPLIT + pickUpTime + SPLIT + type + SPLIT + amount + SPLIT + price + SPLIT + packInfo + SEND_SPLIT;
		}
		strcpy_s(send_buf, tmp.c_str());
		send(client, send_buf, 1000, 0);//向客户端发送快递信息
	}
	strcpy_s(send_buf, "");
	send(client, send_buf, 1000, 0);	//发送1表示接收成功
}

//返回当前登录的快递员的信息在文件中的位置
streampos Postman::searchID(const string& id) const
{
	string s;
	streampos loc = 0;
	postmanInfoFile.clear();
	postmanInfoFile.seekg(0);

	//文件为空，直接返回0
	if (postmanInfoFile.eof())
		return loc;

	//查找当前对象ID，有就立即返回
	while (!postmanInfoFile.eof())
	{
		loc = postmanInfoFile.tellg();
		getline(postmanInfoFile, s, SPLIT);
		if (!s.compare(id))
			return loc;
		postmanInfoFile.ignore(LLONG_MAX, '\n');
	}

	//文件中没有，返回-1
	return -1;
}
