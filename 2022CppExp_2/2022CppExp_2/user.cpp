#include "head2.h"

extern vector<Pack> packList;
extern fstream userInfoFile;
extern fstream packInfoFile;

User::User()
	: balance(0)
{}

//用于注册或从文件读取输入的构造函数
User::User(const string& id, const string& ipw, const string& iname, const string& iphone, const string& iaddr, const int& ibalance)
	: ID(id)
	, pw(ipw)
	, name(iname)
	, phone(iphone)
	, addr(iaddr)
	, balance(ibalance)
{}

User::~User()
{}

//注册
void User::regis()
{
	/*
	* 先输入新用户ID，检查有无重复
	* 没有重复，逐个输入各项用户信息
	*/

	string tmp;
	while (1)
	{
		cout << "请输入您的用户ID，长度为8-12个字符，只能包含数字、大小写英文字母以及下划线" << endl << "输入q退出" << endl;
		cin >> tmp;
		cin.ignore(LLONG_MAX, '\n');
		if (tmp == "q")
			return;
		//检查输入的合法性
		int flag = 1;
		int len = tmp.length();
		if (len < 8 || len > 12)
		{
			cout << "长度错误，重新输入" << endl;
			continue;
		}
		for (int i = 0; i < len; i++)
		{
			//如果不为英文字母、数字或下划线，错误
			if (!isalnum(tmp[i]) && tmp[i] != '_')
			{
				flag = 0;
				break;
			}
		}
		if (flag == 0)
		{
			cout << "含有非法字符，请重新输入" << endl;
			continue;
		}
		//如果已存在该用户名，询问用户是否用该用户进行登录，否则要求重新输入
		if (!(searchID(tmp) < 0))
		{
			do
			{
				cout << "用户名已被使用，是否进行登录？" << endl;
				cout << "输入1进行登录，否则输入2" << endl;
				char input = getchar();
				cin.ignore(LLONG_MAX, '\n');
				switch (input)
				{
				case '1':
					login(tmp);
					return;
				case '2':
					flag = 1;
					break;
				default:
					flag = 0;
					cout << "输入错误，请重新输入" << endl;
					break;
				}
			} while (!flag);
			continue;
		}
		break;
	}
	ID = tmp;

	while (1)
	{
		cout << "请输入密码，密码只能含有大小写字母和数字，长度在8-10个字符" << endl;
		cin >> tmp;
		cin.ignore(LLONG_MAX, '\n');
		//检查输入的合法性
		if (!checkPwValid(tmp))
		{
			cout << "密码格式错误，请重新输入" << endl;
		}
		else
			break;
	}
	pw = tmp;

	cout << "请输入姓名（将作为寄件人/收件人显示）" << endl;
	getline(cin, name);

	while (1)
	{
		cout << "请输入电话号码" << endl;
		cin >> tmp;
		cin.ignore(LLONG_MAX, '\n');
		//检查输入的合法性
		if (tmp.length() != 11)
		{
			cout << "输入错误，请重新输入" << endl;
			continue;
		}
		int flag = 1;
		for (int i = 0; i < tmp.length(); i++)
		{
			//检查是不是全是数字
			if (!isdigit(tmp[i]))
			{
				flag = 0;
				break;
			}
		}
		if (flag == 0)
		{
			cout << "输入错误，请重新输入" << endl;
			continue;
		}
		break;
	}
	phone = tmp;

	cout << "请输入地址" << endl;
	getline(cin, addr);

	//写入新用户信息
	writeToFile(0);
	//进入用户界面
	userInterface();
}

//登录
void User::login()
{
	/*
	* 输入用户ID再输入密码登录，然后加载用户信息
	*/

	string input;

	while (1)
	{
		cout << "请输入用户名，输入q退出" << endl;
		cin >> input;
		cin.ignore(LLONG_MAX, '\n');
		if (input == "q")
			return;
		//检查输入的合法性
		int flag = 1;
		int len = input.length();
		if (len < 8 || len > 12)
		{
			cout << "长度错误，重新输入" << endl;
			continue;
		}
		for (int i = 0; i < len; i++)
		{
			if (!isalnum(input[i]) && input[i] != '_')
			{
				flag = 0;
				break;
			}
		}
		if (flag == 0)
		{
			cout << "含有非法字符，请重新输入" << endl;
			continue;
		}
		ID = input;
		//检查输入的ID是否存在，不存在则询问用户是否用输入进行注册
		streampos loc = searchID(ID);
		if (loc < 0)
		{
			cout << "用户名不存在，是否进行注册" << endl;
			cout << "输入1注册，输入q重新输入" << endl;
			char input = getchar();
			cin.ignore(1, '\n');
			switch (input)
			{
			case '1':
				regis();
				return;
			case 'q':
				flag = 0;
				break;
			}
		}
		if (!flag)
			continue;
		//从文件读取该用户ID的密码
		userInfoFile.clear();
		userInfoFile.seekg(loc);
		userInfoFile.ignore(LLONG_MAX, SPLIT);
		getline(userInfoFile, pw, SPLIT);
		while (1)
		{
			cout << "请输入密码，输入q重新输入用户ID" << endl;
			cin >> input;
			cin.ignore(LLONG_MAX, '\n');
			if (!input.compare(pw))
			{
				cout << "登录成功" << endl;
				break;
			}
			else if (input == "q")
			{
				flag = 0;
				break;
			}
			else
			{
				cout << "密码错误，请重新输入" << endl;
				continue;
			}
		}
		if (!flag)
			continue;
		break;
	}

	//读取其他信息
	getline(userInfoFile, name, SPLIT);
	getline(userInfoFile, phone, SPLIT);
	getline(userInfoFile, addr, SPLIT);
	userInfoFile >> balance;
	//用户界面
	userInterface();
}

//输出用户信息
void User::output() const
{
	cout << "用户信息" << endl << "用户ID：" << ID << endl;
	cout << "用户姓名：" << name << endl;
	cout << "用户电话：" << phone << endl;
	cout << "用户地址：" << addr << endl;
	cout << "账户余额：" << balance << endl;
}

//从文件中读取与当前用户相关的快递，存在向量packList中
void User::loadUserPackList() const
{
	packList.clear();
	packInfoFile.clear();
	packInfoFile.seekg(0);
	string s;

	while (!packInfoFile.eof())
	{
		string packID, packInfo;
		PackUserInfo sender, recver;
		PackPostmanInfo post;
		SYSTEMTIME sendTime, recvTime, pickUpTime;
		int type, amount, price;
		int state;

		getline(packInfoFile, packID, SPLIT);
		//避免读取空行
		if (!packID.length())
			continue;
		getline(packInfoFile, s, SPLIT);
		state = atoi(s.c_str());
		getline(packInfoFile, s, SPLIT);
		sendTime.wYear = atoi(s.c_str());
		getline(packInfoFile, s, SPLIT);
		sendTime.wMonth = atoi(s.c_str());
		getline(packInfoFile, s, SPLIT);
		sendTime.wDay = atoi(s.c_str());
		getline(packInfoFile, s, SPLIT);
		sendTime.wHour = atoi(s.c_str());
		getline(packInfoFile, s, SPLIT);
		sendTime.wMinute = atoi(s.c_str());
		//已接收的快递才有接收时间
		if (state == RECEIVE)
		{
			getline(packInfoFile, s, SPLIT);
			recvTime.wYear = atoi(s.c_str());
			getline(packInfoFile, s, SPLIT);
			recvTime.wMonth = atoi(s.c_str());
			getline(packInfoFile, s, SPLIT);
			recvTime.wDay = atoi(s.c_str());
			getline(packInfoFile, s, SPLIT);
			recvTime.wHour = atoi(s.c_str());
			getline(packInfoFile, s, SPLIT);
			recvTime.wMinute = atoi(s.c_str());
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
		if (recver.ID.compare(ID) && sender.ID.compare(ID))
		{
			packInfoFile.ignore(LLONG_MAX, '\n');
			continue;
		}
		getline(packInfoFile, post.ID, SPLIT);
		getline(packInfoFile, post.name, SPLIT);
		getline(packInfoFile, post.phone, SPLIT);
		//未揽收的快递没有揽收时间
		if (state != NOT_PICK)
		{
			getline(packInfoFile, s, SPLIT);
			pickUpTime.wYear = atoi(s.c_str());
			getline(packInfoFile, s, SPLIT);
			pickUpTime.wMonth = atoi(s.c_str());
			getline(packInfoFile, s, SPLIT);
			pickUpTime.wDay = atoi(s.c_str());
			getline(packInfoFile, s, SPLIT);
			pickUpTime.wHour = atoi(s.c_str());
			getline(packInfoFile, s, SPLIT);
			pickUpTime.wMinute = atoi(s.c_str());
		}
		getline(packInfoFile, s, SPLIT);
		type = atoi(s.c_str());
		getline(packInfoFile, s, SPLIT);
		amount = atoi(s.c_str());
		getline(packInfoFile, s, SPLIT);
		price = atoi(s.c_str());
		getline(packInfoFile, packInfo);

		Pack p(packID, state, sendTime, recvTime, sender, recver, post, pickUpTime, type, amount, price, packInfo);
		packList.push_back(p);
	}
}

//用户界面
void User::userInterface()
{
	char input;

	loadUserPackList();
	do {
		cout << "输入1管理用户信息" << endl;
		cout << "输入2使用物流功能" << endl;
		cout << "输入q登出" << endl;
		input = getchar();
		cin.ignore(LLONG_MAX, '\n');
		switch (input)
		{
		case '1':
			userInfoInterface();
			break;
		case '2':
			userPackInterface();
			break;
		case 'q':
			return;
		default:
			cout << "输入错误" << endl;
			break;
		}
	} while (1);
}

//用户管理个人信息界面
void User::userInfoInterface()
{
	char input;

	do {
		cout << "输入1更改密码" << endl;
		cout << "输入2查询余额" << endl;
		cout << "输入3充值" << endl;
		cout << "输入q退出" << endl;
		input = getchar();
		cin.ignore(LLONG_MAX, '\n');
		switch (input)
		{
		case '1':
			changePW();
			break;
		case '2':
			checkBalance();
			break;
		case '3':
			topUp();
			break;
		case 'q':
			return;
		default:
			cout << "输入错误，请重新输入" << endl;
			break;
		}
	} while (1);
}

//用户管理物流信息界面
void User::userPackInterface()
{
	char input;

	do {
		cout << "输入1发送快递" << endl;
		cout << "输入2接收快递" << endl;
		cout << "输入3查询快递" << endl;
		cout << "输入q退出" << endl;
		input = getchar();
		cin.ignore(LLONG_MAX, '\n');
		switch (input)
		{
		case '1':
			sendPack();
			break;
		case '2':
			recvPack();
			break;
		case '3':
			searchPack();
			break;
		case 'q':
			return;
		default:
			cout << "输入错误，请重新输入" << endl;
			break;
		}
	} while (1);
}

//更改密码
void User::changePW()
{
	string input;

	while (1)
	{
		cout << "请输入旧密码，或者输入q退出" << endl;
		cin >> input;
		cin.ignore(LLONG_MAX, '\n');
		if (input == "q")
			return;
		//检查输入的合法性
		else if (!input.compare(pw))
		{
			cout << "密码正确" << endl;
			break;
		}
		else
		{
			cout << "密码错误，请重新输入" << endl;
			continue;
		}
	}

	while (1)
	{
		cout << "输入新密码，密码只能含有大小写字母和数字，长度在8-10个字符" << endl;
		cin >> input;
		cin.ignore(LLONG_MAX, '\n');
		//检查输入的合法性
		if (checkPwValid(input))
		{
			cout << "更改成功" << endl;
			break;
		}
		else
			cout << "含有非法字符，请重新输入" << endl;
	}

	pw = input;
	//即时在文件中更改密码
	writeToFile(1);
}

//查看余额
void User::checkBalance() const
{
	cout << "余额为" << balance << "元" << endl;
}

//充值
void User::topUp()
{
	int value;
	string input;

	while (1)
	{
		cout << "请输入密码，或者输入q退出" << endl;
		cin >> input;
		cin.ignore(LLONG_MAX, '\n');
		if (input == "q")
			return;
		else if (!input.compare(pw))
		{
			cout << "密码正确" << endl;
			break;
		}
		else
		{
			cout << "密码错误，请重新输入" << endl;
			continue;
		}
	}

	while (1)
	{
		cout << "请输入充值金额（整数）" << endl;
		//检查输入是否合法，如输入字符或浮点数就不合法
		if (!(cin >> value) || cin.peek() == '.')
		{
			//不合法，清除状态，跳过缓冲区，重新输入
			cin.clear();
			cin.ignore(LLONG_MAX, '\n');
			cout << "输入错误，请重新输入" << endl;
			continue;
		}
		cin.ignore(LLONG_MAX, '\n');
		balance += value;
		cout << "充值成功" << endl;
		break;
	}

	//即时在文件中更改余额
	writeToFile(1);
}

//在用户登录时使用不存在用户号时使用，注册输入的ID
void User::regis(const string& id)
{
	string tmp;

	while (1)
	{
		cout << "请输入密码，密码只能含有大小写字母和数字，长度在8-10个字符" << endl;
		cout << "输入q退出" << endl;
		cin >> tmp;
		cin.ignore(LLONG_MAX, '\n');
		if (tmp == "q")
			return;
		//检查输入的合法性
		if (!checkPwValid(tmp))
		{
			cout << "密码格式错误，请重新输入" << endl;
		}
		else
			break;
	}
	pw = tmp;

	cout << "请输入姓名（将作为寄件人/收件人显示）" << endl;
	getline(cin, name);

	while (1)
	{
		cout << "请输入电话号码" << endl;
		cin >> tmp;
		cin.ignore(LLONG_MAX, '\n');
		//检查输入的合法性
		if (tmp.length() != 11)
		{
			cout << "输入错误，请重新输入" << endl;
			continue;
		}
		int flag = 1;
		for (int i = 0; i < tmp.length(); i++)
		{
			if (!isdigit(tmp[i]))
			{
				flag = 0;
				break;
			}
		}
		if (flag == 0)
		{
			cout << "输入错误，请重新输入" << endl;
			continue;
		}
		break;
	}
	phone = tmp;

	cout << "请输入地址" << endl;
	getline(cin, addr);

	//将新用户信息写入文件
	writeToFile(0);
	//用户界面
	userInterface();
}

//在用户注册时使用已存在用户号时使用，登录输入的ID
void User::login(const string& id)
{
	string input;
	ID = id;

	int flag = 1;
	//从文件中获取密码
	streampos loc = searchID(ID);
	userInfoFile.clear();
	userInfoFile.seekg(loc);
	userInfoFile.ignore(LLONG_MAX, SPLIT);
	getline(userInfoFile, pw, SPLIT);
	while (1)
	{
		cout << "请输入密码，输入q重新输入用户ID" << endl;
		cin >> input;
		cin.ignore(LLONG_MAX, '\n');
		if (!input.compare(pw))
		{
			cout << "登录成功" << endl;
			break;
		}
		else if (input == "q")
		{
			login();
			return;
		}
		else
		{
			cout << "密码错误，请重新输入" << endl;
			continue;
		}
	}

	//加载其他信息
	getline(userInfoFile, name, SPLIT);
	getline(userInfoFile, phone, SPLIT);
	getline(userInfoFile, addr, SPLIT);
	userInfoFile >> balance;

	//进入用户界面
	userInterface();
}

//检查newPw是否符合密码格式要求，是则返回1，否则返回0
int User::checkPwValid(const string& newPw) const
{
	int flag = 1;
	int len = newPw.length();

	//密码长度只能为8-10个字符
	if (len < 8 || len > 10)
		return 0;

	//密码只能由数字组成
	for (int i = 0; i < len; i++)
	{
		if (!isalnum(newPw[i]))
		{
			flag = 0;
			break;
		}
	}
	if (flag == 0)
		return 0;

	return 1;
}

//发送快递
void User::sendPack()
{
	PackUserInfo sender, recver;
	string input, pID;
	streampos loc;
	int type, amount;
	SYSTEMTIME now;
	Pack* pack;

	while (1)
	{
		cout << "请输入收件人ID，输入q退出" << endl;
		cin >> input;
		cin.ignore(LLONG_MAX, '\n');
		if (input == "q")
			return;
		//检查输入的合法性
		int flag = 1;
		int len = input.length();
		if (len < 8 || len > 12)
		{
			cout << "长度错误，重新输入" << endl;
			continue;
		}
		for (int i = 0; i < len; i++)
		{
			//检查输入的用户号是否均为合法字符
			if (!isalnum(input[i]) && input[i] != '_')
			{
				flag = 0;
				break;
			}
		}
		if (flag == 0)
		{
			cout << "含有非法字符，请重新输入" << endl;
			continue;
		}
		//检查是否存在该用户
		loc = searchID(input);
		if (loc == -1)
		{
			cout << "不存在该用户，请重新输入" << endl;
			continue;
		}
		sender.ID = this->ID;
		recver.ID = input;
		//加载所需的寄件人和收件人信息
		loadPacketUserInfo(sender);
		loadPacketUserInfo(loc, recver);

		while (1)
		{
			cout << "请选择快递类型，0为普通快递，1为易碎品，2为书籍" << endl;
			char input = getchar();
			cin.ignore(LLONG_MAX, '\n');
			int flag = 1;
			switch (input)
			{
			case '0':
				type = NORMAL;
				while (1)
				{
					cout << "请输入快递重量（整数），5元/kg，不足1kg以1kg计算" << endl;
					if (!(cin >> amount) || cin.peek() == '.')
					{
						//不合法，清除状态，跳过缓冲区，重新输入
						cin.clear();
						cin.ignore(LLONG_MAX, '\n');
						cout << "输入错误，请重新输入" << endl;
						continue;
					}
					cin.ignore(LLONG_MAX, '\n');
					break;
				}
				break;
			case '1':
				type = FRAGILE;
				while (1)
				{
					cout << "请输入快递重量（整数），8元/kg，不足1kg以1kg计算" << endl;
					if (!(cin >> amount) || cin.peek() == '.')
					{
						//不合法，清除状态，跳过缓冲区，重新输入
						cin.clear();
						cin.ignore(LLONG_MAX, '\n');
						cout << "输入错误，请重新输入" << endl;
						continue;
					}
					cin.ignore(LLONG_MAX, '\n');
					break;
				}
				break;
			case '2':
				type = BOOK;
				while (1)
				{
					cout << "请输入快递书籍本数（整数），2元/本" << endl;
					if (!(cin >> amount) || cin.peek() == '.')
					{
						//不合法，清除状态，跳过缓冲区，重新输入
						cin.clear();
						cin.ignore(LLONG_MAX, '\n');
						cout << "输入错误，请重新输入" << endl;
						continue;
					}
					cin.ignore(LLONG_MAX, '\n');
					break;
				}
				break;
			default:
				flag = 0;
				cout << "输入错误，请重新输入" << endl;
				break;
			}
			if (flag)
				break;
		}

		cout << "请输入快递详情" << endl;
		getline(cin, input);

		//获取当前时间，与寄件人ID组成快递号
		GetSystemTime(&now);
		pID = sender.ID + to_string(now.wYear) + to_string(now.wMonth) + to_string(now.wDay) + to_string(now.wHour) + to_string(now.wMinute) + to_string(now.wSecond);

		//根据快递类型，为pack申请相应的类的空间
		if (type == NORMAL)
			pack = new Pack(pID, now, sender, recver, type, amount, input);
		else if(type == FRAGILE)
			pack = new Fragile(pID, now, sender, recver, type, amount, input);
		else 
			pack = new Book(pID, now, sender, recver, type, amount, input);

		pack->getPrice();	//计算运费
		//检查余额是否足够，足够直接扣，不够就给出提示，取消发送
		if (balance >= pack->price)
			balance -= pack->price;
		else
		{
			cout << "余额不足" << endl;
			cout << "余额为" << balance << "元" << endl;
			continue;
		}
		//即时更改文件内容
		writeToFile(1);
		pack->sendPack();	//发送快递
		cout << "发送成功" << endl;
		delete pack;

		cout << "是否继续发送，是则输入任意键，否则输入q" << endl;
		input[0] = getchar();
		cin.ignore(LLONG_MAX, '\n');
		switch (input[0])
		{
		case 'q':
			return;
		default:
			break;
		}
	}
}

//接收快递
void User::recvPack() const
{
	string input;
	int locV;

	while (1)
	{
		//没有未接收快递直接返回，有就输入快递号进行接收
		cout << "未接收快递：" << endl;
		if (!checkUnrecvPackIn())
		{
			cout << "没有未接收快递" << endl;
			return;
		}

		while (1)
		{
			cout << "输入快递号进行接收，输入q退出" << endl;
			cin >> input;
			cin.ignore(LLONG_MAX, '\n');
			if (input == "q")
				return;
			//检查输入的合法性
			locV = -1;
			
			//从packList中查找快递
			for (int i = 0; i < packList.size(); i++)
			{
				if (!packList[i].packID.compare(input))
				{
					locV = i;
					break;
				}
			}
			if (locV == -1)
				cout << "不存在为该快递号的未接收快递，请重新输入" << endl;
			else
				break;
		}

		packList[locV].recvPack();	//接收快递
		cout << "接收成功" << endl;

		cout << "是否继续接收，是则输入任意键，否则输入q" << endl;
		input[0] = getchar();
		cin.ignore(LLONG_MAX, '\n');
		switch (input[0])
		{
		case 'q':
			return;
		default:
			break;
		}
	}
}

//查询快递
void User::searchPack() const
{
	char input;

	while (1)
	{
		cout << "输入1用快递号查询" << endl;
		cout << "输入2用寄件人查询" << endl;
		cout << "输入3用收件人查询" << endl;
		cout << "输入4用发送时间查询" << endl;
		cout << "输入5用快递状态查询" << endl;
		cout << "输入q退出" << endl;
		input = getchar();
		cin.ignore(LLONG_MAX, '\n');
		switch (input)
		{
		case '1':
			searchPackByPackID();
			break;
		case '2':
			searchPackBySender();
			break;
		case '3':
			searchPackByRecver();
			break;
		case '4':
			searchPackBySendTime();
			break;
		case '5':
			searchPackByState();
			break;
		case 'q':
			return;
		default:
			cout << "输入错误" << endl;
			break;
		}
	}
}

//加载寄件人信息
void User::loadPacketUserInfo(PackUserInfo& sender) const
{
	sender.name = name;
	sender.phone = phone;
	sender.addr = addr;
}

//加载收件人信息，参数loc是收件人的信息在用户信息文件的位置
void User::loadPacketUserInfo(streampos loc, PackUserInfo& recver) const
{
	userInfoFile.clear();
	userInfoFile.seekg(loc);

	//跳过收件人用户名和密码
	userInfoFile.ignore(LLONG_MAX, SPLIT);
	userInfoFile.ignore(LLONG_MAX, SPLIT);
	getline(userInfoFile, recver.name, SPLIT);
	getline(userInfoFile, recver.phone, SPLIT);
	getline(userInfoFile, recver.addr, SPLIT);
}

//根据寄件人信息查询快递
void User::searchPackBySender() const
{
	char input;

	while (1)
	{
		cout << "输入1用寄件人ID查询" << endl;
		cout << "输入2用寄件人姓名查询" << endl;
		cout << "输入3用寄件人电话查询" << endl;
		cout << "输入4用寄件人地址查询" << endl;
		cout << "输入q退出" << endl;
		input = getchar();
		cin.ignore(LLONG_MAX, '\n');
		switch (input)
		{
		case '1':
			searchPackBySenderID();
			break;
		case '2':
			searchPackBySenderName();
			break;
		case '3':
			searchPackBySenderPhone();
			break;
		case '4':
			searchPackBySenderAddr();
			break;
		case 'q':
			return;
		default:
			cout << "输入错误" << endl;
			break;
		}
	}
}

//根据寄件人用户ID查询快递
void User::searchPackBySenderID() const
{
	int count = 0;
	int i = 0;
	string input;

	while (1)
	{
		cout << "请输入寄件人ID" << endl;
		getline(cin, input);
		//检查输入的合法性
		int flag = 1;
		int len = input.length();
		if (len < 8 || len > 12)
		{
			cout << "长度错误，重新输入" << endl;
			continue;
		}
		for (int i = 0; i < len; i++)
		{
			if (!isalnum(input[i]) && input[i] != '_')
			{
				flag = 0;
				break;
			}
		}
		if (flag == 0)
		{
			cout << "含有非法字符，请重新输入" << endl;
			continue;
		}

		cout << "搜索结果：" << endl;
		for (i = 0, count = 0; i < packList.size(); i++)
		{
			if (!packList[i].sender.ID.compare(input))
			{
				cout << ++count << endl;
				packList[i].output();
			}
		}

		if (count == 0)
			cout << "查找失败" << endl;

		cout << "是否继续搜索，是则输入任意键，否则输入q" << endl;
		input[0] = getchar();
		cin.ignore(LLONG_MAX, '\n');
		switch (input[0])
		{
		case 'q':
			return;
		default:
			break;
		}
	}
}

//根据寄件人姓名查询快递
void User::searchPackBySenderName() const
{
	int count = 0;
	int i = 0;
	string input;

	while (1)
	{
		cout << "请输入寄件人姓名" << endl;
		getline(cin, input);

		cout << "搜索结果：" << endl;
		for (i = 0, count = 0; i < packList.size(); i++)
		{
			if (!packList[i].sender.name.compare(input))
			{
				cout << ++count << endl;
				packList[i].output();
			}
		}

		if (count == 0)
			cout << "查找失败" << endl;

		cout << "是否继续搜索，是则输入任意键，否则输入q" << endl;
		input[0] = getchar();
		cin.ignore(LLONG_MAX, '\n');
		switch (input[0])
		{
		case 'q':
			return;
		default:
			break;
		}
	}
}

//根据寄件人电话查询快递
void User::searchPackBySenderPhone() const
{
	int count = 0;
	int i = 0;
	string input;

	while (1)
	{
		cout << "请输入寄件人电话" << endl;
		getline(cin, input);
		//检查输入的合法性
		int flag = 1;
		int len = input.length();
		if (len != 11)
		{
			cout << "长度错误，重新输入" << endl;
			continue;
		}
		for (int i = 0; i < len; i++)
		{
			if (!isdigit(input[i]))
			{
				flag = 0;
				break;
			}
		}
		if (flag == 0)
		{
			cout << "含有非法字符，请重新输入" << endl;
			continue;
		}

		cout << "搜索结果：" << endl;
		for (i = 0, count = 0; i < packList.size(); i++)
		{
			if (!packList[i].sender.phone.compare(input))
			{
				cout << ++count << endl;
				packList[i].output();
			}
		}

		if (count == 0)
			cout << "查找失败" << endl;

		cout << "是否继续搜索，是则输入任意键，否则输入q" << endl;
		input[0] = getchar();
		cin.ignore(LLONG_MAX, '\n');
		switch (input[0])
		{
		case 'q':
			return;
		default:
			break;
		}
	}
}

//根据寄件人地址查询快递
void User::searchPackBySenderAddr() const
{
	int count = 0;
	int i = 0;
	string input;

	while (1)
	{
		cout << "请输入寄件人地址" << endl;
		getline(cin, input);

		cout << "搜索结果：" << endl;
		for (i = 0, count = 0; i < packList.size(); i++)
		{
			if (!packList[i].sender.addr.compare(input))
			{
				cout << ++count << endl;
				packList[i].output();
			}
		}

		if (count == 0)
			cout << "查找失败" << endl;

		cout << "是否继续搜索，是则输入任意键，否则输入q" << endl;
		input[0] = getchar();
		cin.ignore(LLONG_MAX, '\n');
		switch (input[0])
		{
		case 'q':
			return;
		default:
			break;
		}
	}
}

//根据收件人信息查询快递
void User::searchPackByRecver() const
{
	char input;

	while (1)
	{
		cout << "输入1用收件人ID查询" << endl;
		cout << "输入2用收件人姓名查询" << endl;
		cout << "输入3用收件人电话查询" << endl;
		cout << "输入4用收件人地址查询" << endl;
		cout << "输入q退出" << endl;
		input = getchar();
		cin.ignore(LLONG_MAX, '\n');
		switch (input)
		{
		case '1':
			searchPackByRecverID();
			break;
		case '2':
			searchPackByRecverName();
			break;
		case '3':
			searchPackByRecverPhone();
			break;
		case '4':
			searchPackByRecverAddr();
			break;
		case 'q':
			return;
		default:
			cout << "输入错误" << endl;
			break;
		}
	}
}

//根据收件人用户ID查询快递
void User::searchPackByRecverID() const
{
	int count = 0;
	int i = 0;
	string input;

	while (1)
	{
		cout << "请输入收件人ID" << endl;
		getline(cin, input);
		//检查输入的合法性
		int flag = 1;
		int len = input.length();
		if (len < 8 || len > 12)
		{
			cout << "长度错误，重新输入" << endl;
			continue;
		}
		for (int i = 0; i < len; i++)
		{
			if (!isalnum(input[i]) && input[i] != '_')
			{
				flag = 0;
				break;
			}
		}
		if (flag == 0)
		{
			cout << "含有非法字符，请重新输入" << endl;
			continue;
		}

		cout << "搜索结果：" << endl;
		for (i = 0, count = 0; i < packList.size(); i++)
		{
			if (!packList[i].recver.ID.compare(input))
			{
				cout << ++count << endl;
				packList[i].output();
			}
		}

		if (count == 0)
			cout << "查找失败" << endl;

		cout << "是否继续搜索，是则输入任意键，否则输入q" << endl;
		input[0] = getchar();
		cin.ignore(LLONG_MAX, '\n');
		switch (input[0])
		{
		case 'q':
			return;
		default:
			break;
		}
	}
}

//根据收件人姓名查询快递
void User::searchPackByRecverName() const
{
	int count = 0;
	int i = 0;
	string input;

	while (1)
	{
		cout << "请输入收件人姓名" << endl;
		getline(cin, input);

		cout << "搜索结果：" << endl;
		for (i = 0, count = 0; i < packList.size(); i++)
		{
			if (!packList[i].recver.name.compare(input))
			{
				cout << ++count << endl;
				packList[i].output();
			}
		}

		if (count == 0)
			cout << "查找失败" << endl;

		cout << "是否继续搜索，是则输入任意键，否则输入q" << endl;
		input[0] = getchar();
		cin.ignore(LLONG_MAX, '\n');
		switch (input[0])
		{
		case 'q':
			return;
		default:
			break;
		}
	}
}

//根据收件人电话查询快递
void User::searchPackByRecverPhone() const
{
	int count = 0;
	int i = 0;
	string input;

	while (1)
	{
		cout << "请输入收件人电话" << endl;
		getline(cin, input);
		//检查输入的合法性
		int flag = 1;
		int len = input.length();
		if (len != 11)
		{
			cout << "长度错误，重新输入" << endl;
			continue;
		}
		for (int i = 0; i < len; i++)
		{
			if (!isdigit(input[i]))
			{
				flag = 0;
				break;
			}
		}
		if (flag == 0)
		{
			cout << "含有非法字符，请重新输入" << endl;
			continue;
		}

		cout << "搜索结果：" << endl;
		for (i = 0, count = 0; i < packList.size(); i++)
		{
			if (!packList[i].recver.phone.compare(input))
			{
				cout << ++count << endl;
				packList[i].output();
			}
		}

		if (count == 0)
			cout << "查找失败" << endl;

		cout << "是否继续搜索，是则输入任意键，否则输入q" << endl;
		input[0] = getchar();
		cin.ignore(LLONG_MAX, '\n');
		switch (input[0])
		{
		case 'q':
			return;
		default:
			break;
		}
	}
}

//根据收件人地址查询快递
void User::searchPackByRecverAddr() const
{
	int count = 0;
	int i = 0;
	string input;

	while (1)
	{
		cout << "请输入收件人地址" << endl;
		getline(cin, input);

		cout << "搜索结果：" << endl;
		for (i = 0, count = 0; i < packList.size(); i++)
		{
			if (!packList[i].recver.addr.compare(input))
			{
				cout << ++count << endl;
				packList[i].output();
			}
		}

		if (count == 0)
			cout << "查找失败" << endl;

		cout << "是否继续搜索，是则输入任意键，否则输入q" << endl;
		input[0] = getchar();
		cin.ignore(LLONG_MAX, '\n');
		switch (input[0])
		{
		case 'q':
			return;
		default:
			break;
		}
	}
}

//根据发送时间查询快递
void User::searchPackBySendTime() const
{
	int count = 0;
	int i = 0;
	int flag = 0;
	string tmp;
	SYSTEMTIME input;

	//输入的发送时间支持不同年、月、日和小时四种查找精度
	//至少要输入年份，其他可用换行跳过
	//flag 表示查找精度，0为年份，1为月份，2为日，3为小时
	while (1)
	{
		cout << "请输入发送年份" << endl;
		cin >> input.wYear;
		cin.ignore(LLONG_MAX, '\n');
		cout << "请输入发送月份（可选）" << endl;
		getline(cin, tmp);
		if (tmp.length() != 0)
		{
			flag = 1;
			input.wMonth = atoi(tmp.c_str());
			cout << "请输入发送日（可选）" << endl;
			getline(cin, tmp);
			if (tmp.length() != 0)
			{
				flag = 2;
				input.wDay = atoi(tmp.c_str());
				cout << "请输入发送时（可选）" << endl;
				getline(cin, tmp);
				if (tmp.length() != 0)
				{
					flag = 3;
					input.wHour = atoi(tmp.c_str());
				}
			}
		}

		cout << "搜索结果：" << endl;
		for (i = 0, count = 0; i < packList.size(); i++)
		{
			switch (flag)
			{
			case 0:
				if (packList[i].sendTime.wYear == input.wYear)
				{
					cout << ++count << endl;
					packList[i].output();
				}
				break;
			case 1:
				if (packList[i].sendTime.wYear == input.wYear && packList[i].sendTime.wMonth == input.wMonth)
				{
					cout << ++count << endl;
					packList[i].output();
				}
				break;
			case 2:
				if (packList[i].sendTime.wYear == input.wYear && packList[i].sendTime.wMonth == input.wMonth && packList[i].sendTime.wDay == input.wDay)
				{
					cout << ++count << endl;
					packList[i].output();
				}
				break;
			case 3:
				if (packList[i].sendTime.wYear == input.wYear && packList[i].sendTime.wMonth == input.wMonth && packList[i].sendTime.wDay == input.wDay && packList[i].sendTime.wHour == input.wHour)
				{
					cout << ++count << endl;
					packList[i].output();
				}
				break;
			default:
				break;
			}
		}

		if (count == 0)
			cout << "查找失败" << endl;

		cout << "是否继续搜索，是则输入任意键，否则输入q" << endl;
		char in = getchar();
		cin.ignore(LLONG_MAX, '\n');
		switch (in)
		{
		case 'q':
			return;
		default:
			break;
		}
	}
}

//根据快递号查询快递
void User::searchPackByPackID() const
{
	int count = 0;
	int i = 0;
	string input;

	while (1)
	{
		cout << "请输入快递号" << endl;
		getline(cin, input);

		cout << "搜索结果：" << endl;
		for (i = 0, count = 0; i < packList.size(); i++)
		{
			if (!packList[i].packID.compare(input))
			{
				packList[i].output();
				count++;
				break;
			}
		}

		if (count == 0)
			cout << "查找失败" << endl;

		cout << "是否继续搜索，是则输入任意键，否则输入q" << endl;
		input[0] = getchar();
		cin.ignore(LLONG_MAX, '\n');
		switch (input[0])
		{
		case 'q':
			return;
		default:
			break;
		}
	}
}

//根据快递状态查询快递
void User::searchPackByState() const
{
	char input;

	while (1)
	{
		cout << "输入1查看本人寄出的未揽收快递" << endl;
		cout << "输入2查看本人寄出的未接收快递" << endl;
		cout << "输入3查看本人寄出的已接收快递" << endl;
		cout << "输入4查看寄给本人的未揽收快递" << endl;
		cout << "输入5查看寄给本人的未接收快递" << endl;
		cout << "输入6查看寄给本人的已接受快递" << endl;
		cout << "输入q退出" << endl;
		input = getchar();
		cin.ignore(LLONG_MAX, '\n');
		switch (input)
		{
		case '1':
			if (!checkUnpickPackOut())
				cout << "没有本人寄出的未揽收快递" << endl;
			break;
		case '2':
			if (!checkUnrecvPackOut())
				cout << "没有本人寄出的未接收快递" << endl;
			break;
		case '3':
			if (!checkRecvPackOut())
				cout << "没有本人寄出的已接收快递" << endl;
			break;
		case '4':
			if (!checkUnpickPackIn())
				cout << "没有寄给本人的未揽收快递" << endl;
			break;
		case '5':
			if (!checkUnrecvPackIn())
				cout << "没有寄给本人的未接收快递" << endl;
			break;
		case '6':
			if (!checkRecvPackIn())
				cout << "没有寄给本人的已接收快递" << endl;
			break;
		case 'q':
			return;
		default:
			cout << "输入错误" << endl;
			break;
		}
	}
}

//输出寄给当前用户的未接收快递
int User::checkUnrecvPackIn() const
{
	int count = 0;

	//输出未接收的快递的信息
	for (int i = 0; i < packList.size(); i++)
	{
		if (packList[i].state == NOT_RECEIVE && !packList[i].recver.ID.compare(ID))
		{
			cout << ++count << endl;
			packList[i].output();
		}
	}

	return count;
}

//输出寄给当前用户的未揽收快递
int User::checkUnpickPackIn() const
{
	int count = 0;

	//输出未揽收的快递的信息
	for (int i = 0; i < packList.size(); i++)
	{
		if (packList[i].state == NOT_PICK && !packList[i].recver.ID.compare(ID))
		{
			cout << ++count << endl;
			packList[i].output();
		}
	}

	return count;
}

//输出寄给当前用户的已接收快递
int User::checkRecvPackIn() const
{
	int count = 0;

	//输出已接收的快递的信息
	for (int i = 0; i < packList.size(); i++)
	{
		if (packList[i].state == RECEIVE && !packList[i].recver.ID.compare(ID))
		{
			cout << ++count << endl;
			packList[i].output();
		}
	}

	return count;
}

//输出当前用户寄出的未接收快递
int User::checkUnrecvPackOut() const
{
	int count = 0;

	//输出未接收的快递的信息
	for (int i = 0; i < packList.size(); i++)
	{
		if (packList[i].state == NOT_RECEIVE && !packList[i].sender.ID.compare(ID))
		{
			cout << ++count << endl;
			packList[i].output();
		}
	}

	return count;
}

//输出当前用户寄出的未揽收快递
int User::checkUnpickPackOut() const
{
	int count = 0;

	//输出未揽收的快递的信息
	for (int i = 0; i < packList.size(); i++)
	{
		if (packList[i].state == NOT_PICK && !packList[i].sender.ID.compare(ID))
		{
			cout << ++count << endl;
			packList[i].output();
		}
	}

	return count;
}

//输出当前用户寄出的已接收快递
int User::checkRecvPackOut() const
{
	int count = 0;

	//输出已接收的快递的信息
	for (int i = 0; i < packList.size(); i++)
	{
		if (packList[i].state == RECEIVE && !packList[i].sender.ID.compare(ID))
		{
			cout << ++count << endl;
			packList[i].output();
		}
	}

	return count;
}

//返回当前对象的用户信息在文件中位置
streampos User::searchID() const
{
	string s;
	streampos loc = 0;
	userInfoFile.clear();
	userInfoFile.seekg(0);

	//文件为空，直接返回0
	if (userInfoFile.eof())
		return loc;

	//查找当前对象用户名，有就立即返回
	while (!userInfoFile.eof())
	{
		loc = userInfoFile.tellg();
		getline(userInfoFile, s, SPLIT);
		if (!s.compare(ID))
			return loc;
		userInfoFile.ignore(LLONG_MAX, '\n');
	}

	//文件中没有，返回-1
	return -1;
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

/*
* mode = 0, 新用户
* mode = 1, 更改部分
* 将用户信息写入文件
*/
void User::writeToFile(int mode) const
{
	if (!mode)
	{
		//在文件末尾写入新用户信息
		userInfoFile.clear();
		userInfoFile.seekg(0, ios::end);
		userInfoFile << ID << SPLIT << pw << SPLIT << name << SPLIT << phone << SPLIT << addr << SPLIT << balance << endl;
		return;
	}

	fstream tmpFile;
	tmpFile.open(".\\data\\tmp.txt", ios::in | ios::out | ios::trunc);
	string tmp;
	streampos loc = searchID();
	userInfoFile.clear();
	userInfoFile.seekg(0);

	//将用户信息文件中，当前用户信息之前的内容写入临时文件
	while (userInfoFile.tellg() != loc)
	{
		getline(userInfoFile, tmp);
		//空行不写
		if (tmp.length())
			tmpFile << tmp << endl;
	}
	//当前用户信息（更新后）写入临时文件
	tmpFile << ID << SPLIT << pw << SPLIT << name << SPLIT << phone << SPLIT << addr << SPLIT << balance << endl;
	userInfoFile.ignore(LLONG_MAX, '\n');
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
