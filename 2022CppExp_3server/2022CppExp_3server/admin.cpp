#include "serverhead.h"

extern fstream userInfoFile;
extern fstream adminInfoFile;
extern fstream packInfoFile;
extern fstream postmanInfoFile;

Admin::Admin()
{}

Admin::~Admin()
{}

//从客户端收到管理员的指令
void Admin::recvFromClient(const string& req) const
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
		addPostman(info);
		break;
	case '4':
		deletePostmanFromPack(info);
		break;
	case '5':
		deletePostman(info);
		break;
	case '6':
		assignPack(info);
		break;
	case '7':
		loadAllUserList();
		break;
	case '8':
		loadAllPostmanList();
		break;
	case '9':
		loadAllPackList();
		break;
	default:
		break;
	}
}

//检查管理员ID id是否存在，存在发送1，否则发送0
void Admin::checkExistId(const string& id) const
{
	streampos loc = searchID(id);	//获取管理员号为id的管理员在文件的位置

	if (loc == -1)
		strcpy_s(send_buf, "0");
	else
		strcpy_s(send_buf, "1");
	send(client, send_buf, 1000, 0);
}

//发送管理员ID id的密码
void Admin::sendPw(const string& id) const
{
	streampos loc = searchID(id);	//获取管理员号为id的管理员在文件的位置
	string pw;

	adminInfoFile.clear();
	adminInfoFile.seekg(loc);
	adminInfoFile.ignore(LLONG_MAX, SPLIT);
	getline(adminInfoFile, pw, SPLIT);	//读取密码

	strcpy_s(send_buf, pw.c_str());
	send(client, send_buf, 1000, 0);	//发送给客户端
}

//发送管理员ID id的用户信息
void Admin::getInfo(const string& id) const
{
	streampos loc = searchID(id);	//获取管理员号为id的管理员在文件的位置
	string info, balance;

	adminInfoFile.clear();
	adminInfoFile.seekg(loc);
	adminInfoFile.ignore(LLONG_MAX, SPLIT);
	adminInfoFile.ignore(LLONG_MAX, SPLIT);
	getline(adminInfoFile, info);	//读取ID和密码以外的管理员信息

	adminInfoFile.clear();
	adminInfoFile.seekg(0);
	getline(adminInfoFile, balance);//读取余额，所有管理员共用一个余额，在文件最开头

	info += SPLIT + balance;
	strcpy_s(send_buf, info.c_str());
	send(client, send_buf, 1000, 0);	//发送客户端
}

//往文件新增快递员信息
void Admin::addPostman(const string& postmanInfo) const
{
	postmanInfoFile.clear();
	postmanInfoFile.seekg(0, ios::end);
	postmanInfoFile << postmanInfo << endl;	//新快递员信息写入文件
	strcpy_s(send_buf, "1");
	send(client, send_buf, 1000, 0);	//发送1表示新增成功
}

//删除文件中快递的快递员信息
void Admin::deletePostmanFromPack(const string& packId) const
{
	fstream tmpFile;
	string packInfo, sendTime, type, amount, price, state;
	PackUserInfo sender, recver;
	PackPostmanInfo postman;
	int stateInt;
	string tmp;
	tmpFile.open(".\\data\\tmp.txt", ios::in | ios::out | ios::trunc);
	packInfoFile.clear();
	packInfoFile.seekg(0);
	//快递信息文件中，当前快递之前的内容写入临时文件
	while (!packInfoFile.eof())
	{
		getline(packInfoFile, tmp, SPLIT);
		if (!tmp.compare(packId))
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
	//删去快递员信息
	getline(packInfoFile, postman.ID, SPLIT);
	postman.ID = "???";
	getline(packInfoFile, postman.name, SPLIT);
	postman.name = "???";
	getline(packInfoFile, postman.phone, SPLIT);
	postman.phone = "???";
	if (stateInt == NOT_RECEIVE)	//不读取揽收时间
	{
		packInfoFile.ignore(LLONG_MAX, SPLIT);
		packInfoFile.ignore(LLONG_MAX, SPLIT);
		packInfoFile.ignore(LLONG_MAX, SPLIT);
		packInfoFile.ignore(LLONG_MAX, SPLIT);
		packInfoFile.ignore(LLONG_MAX, SPLIT);

	}
	getline(packInfoFile, type, SPLIT);
	getline(packInfoFile, amount, SPLIT);
	getline(packInfoFile, price, SPLIT);
	getline(packInfoFile, packInfo);
	//当前快递信息写入临时文件
	stateInt = NOT_PICK;
	tmpFile << packId << SPLIT << stateInt << SPLIT;
	tmpFile << sendTime;
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
	strcpy_s(send_buf, "1");
	send(client, send_buf, 1000, 0);	//发送1表示删除成功
}

//从文件中删除快递员
void Admin::deletePostman(const string& id) const
{
	string tmp;
	fstream tmpFile;

	tmpFile.open(".\\data\\tmp.txt", ios::in | ios::out | ios::trunc);
	postmanInfoFile.clear();
	postmanInfoFile.seekg(0);
	//快递员信息文件中，当前快递员之前的内容写入临时文件
	while (!postmanInfoFile.eof())
	{
		getline(postmanInfoFile, tmp, SPLIT);
		if (!tmp.compare(id))
			break;
		if (tmp.length())
		{
			tmpFile << tmp << SPLIT;
			getline(postmanInfoFile, tmp);
			tmpFile << tmp << endl;
		}
	}
	//当前快递员之后的内容写入临时文件
	postmanInfoFile.ignore(LLONG_MAX, '\n');
	while (!postmanInfoFile.eof())
	{
		getline(postmanInfoFile, tmp);
		if (tmp.length())
			tmpFile << tmp << endl;
	}
	//临时文件内容覆盖快递员信息文件
	tmpFile.clear();
	tmpFile.seekg(0);
	postmanInfoFile.close();
	postmanInfoFile.open(".\\data\\postmanInfo.txt", ios::out | ios::in | ios::trunc);
	while (!tmpFile.eof())
	{
		getline(tmpFile, tmp);
		if (tmp.length())
			postmanInfoFile << tmp << endl;
	}
	tmpFile.close();
	strcpy_s(send_buf, "1");
	send(client, send_buf, 1000, 0);	//发送1表示成功删除
}

//为快递分配快递员
void Admin::assignPack(const string& info) const
{
	fstream tmpFile;
	string packId, packInfo, sendTime, type, amount, price, state;
	PackPostmanInfo postmanAssign;
	PackUserInfo sender, recver;
	PackPostmanInfo postman;
	string tmp;
	istringstream iss(info);

	getline(iss, packId, SPLIT);
	getline(iss, postmanAssign.ID, SPLIT);
	getline(iss, postmanAssign.name, SPLIT);
	getline(iss, postmanAssign.phone);

	tmpFile.open(".\\data\\tmp.txt", ios::in | ios::out | ios::trunc);
	packInfoFile.clear();
	packInfoFile.seekg(0);
	//快递信息文件中，当前快递之前的内容写入临时文件
	while (!packInfoFile.eof())
	{
		getline(packInfoFile, tmp, SPLIT);
		if (!tmp.compare(packId))
			break;
		if (tmp.length())
		{
			tmpFile << tmp << SPLIT;
			getline(packInfoFile, tmp);
			tmpFile << tmp << endl;
		}
	}
	getline(packInfoFile, state, SPLIT);
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
	postman.ID = postmanAssign.ID;
	getline(packInfoFile, postman.name, SPLIT);
	postman.name = postmanAssign.name;
	getline(packInfoFile, postman.phone, SPLIT);
	postman.phone = postmanAssign.phone;
	getline(packInfoFile, type, SPLIT);
	getline(packInfoFile, amount, SPLIT);
	getline(packInfoFile, price, SPLIT);
	getline(packInfoFile, packInfo);
	//当前快递信息写入临时文件
	tmpFile << packId << SPLIT << state << SPLIT;
	tmpFile << sendTime;
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
	strcpy_s(send_buf, "1");
	send(client, send_buf, 1000, 0);	//发送1表示分配成功
}

//管理员有权查看所有用户，提前从文件中读取并将所有用户信息并发送
void Admin::loadAllUserList() const
{
	userInfoFile.clear();
	userInfoFile.seekg(0);

	//管理员登录时，加载所有用户信息
	while (!userInfoFile.eof())
	{
		string tmp;

		getline(userInfoFile, tmp);
		//避免读取空行
		if (!tmp.length())
			continue;
		tmp += '|';
		strcpy_s(send_buf, tmp.c_str());
		send(client, send_buf, 1000, 0);	//往客户端发送
	}
	strcpy_s(send_buf, "");
	send(client, send_buf, 1000, 0);	//发送一个空串表示发送结束
}

//管理员有权查看所有快递员，提前从文件中读取并将所有快递员信息并发送
void Admin::loadAllPackList() const
{
	packInfoFile.clear();
	packInfoFile.seekg(0);

	//管理员登录时，从文件读取所有快递信息并存在packList
	while (!packInfoFile.eof())
	{
		string tmp;

		getline(packInfoFile, tmp);
		//避免读取空行
		if (!tmp.length())
			continue;
		tmp += '|';
		strcpy_s(send_buf, tmp.c_str());
		send(client, send_buf, 1000, 0);	//发送给客户端
	}
	strcpy_s(send_buf, "");
	send(client, send_buf, 1000, 0);	//发送一个空串表示发送结束
}

//管理员有权查看所有快递，提前从文件中读取并将所有快递信息并发送
void Admin::loadAllPostmanList() const
{
	postmanInfoFile.clear();
	postmanInfoFile.seekg(0);

	//管理员登录时，加载所有用户信息
	while (!postmanInfoFile.eof())
	{
		string tmp;

		getline(postmanInfoFile, tmp);
		//避免读取空行
		if (!tmp.length())
			continue;
		tmp += '|';
		strcpy_s(send_buf, tmp.c_str());
		send(client, send_buf, 1000, 0);	//发送给客户端
	}
	strcpy_s(send_buf, "");
	send(client, send_buf, 1000, 0);	//发送一个空串表示发送结束
}

//返回ID为id的管理员信息在文件中的位置
streampos Admin::searchID(const string& id) const
{
	string s;
	streampos loc = 0;
	adminInfoFile.clear();
	adminInfoFile.seekg(0);
	adminInfoFile.ignore(LLONG_MAX, '\n');
	loc = adminInfoFile.tellg();

	//文件为空，直接返回0
	if (adminInfoFile.eof())
		return loc;

	//查找当前对象ID，有就立即返回
	while (!adminInfoFile.eof())
	{
		loc = adminInfoFile.tellg();
		getline(adminInfoFile, s, SPLIT);
		if (!s.compare(id))
			return loc;
		adminInfoFile.ignore(LLONG_MAX, '\n');
	}

	//文件中没有，返回-1
	return -1;
}
