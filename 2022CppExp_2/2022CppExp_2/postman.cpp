#include "head2.h"

extern vector<Pack> packList;
extern fstream postmanInfoFile;
extern fstream packInfoFile;

Postman::Postman()
	: balance(0)
{}

//从文件输入或管理员添加快递员时使用的构造函数
Postman::Postman(const string& id, const string& ipw, const string& iname, const string& iphone, const int& ibalance)
	: ID(id)
	, pw(ipw)
	, name(iname)
	, phone(iphone)
	, balance(ibalance)
{}

Postman::~Postman()
{}

//登录
void Postman::login()
{
	string input;

	while (1)
	{
		cout << "请输入快递员ID，输入q退出" << endl;
		cin >> input;
		cin.ignore(LLONG_MAX, '\n');

		if (input == "q")
			return;

		ID = input;
		streampos loc = searchID();
		if (loc == -1)
		{
			cout << "快递员ID不存在，请重新输入" << endl;
			continue;
		}
		int flag = 1;
		//从文件中获取输入ID的密码
		postmanInfoFile.clear();
		postmanInfoFile.seekg(loc);
		postmanInfoFile.ignore(LLONG_MAX, SPLIT);
		getline(postmanInfoFile, pw, SPLIT);
		while (1)
		{
			cout << "请输入密码，输入q重新输入快递员ID" << endl;
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

	getline(postmanInfoFile, name, SPLIT);
	getline(postmanInfoFile, phone, SPLIT);
	postmanInfoFile >> balance;

	postmanInterface();
}

//输出快递员信息
void Postman::output() const
{
	cout << "快递员信息" << endl << "快递员ID：" << ID << endl;
	cout << "快递员姓名：" << name << endl;
	cout << "快递员电话：" << phone << endl;
	cout << "账户余额：" << balance << endl;
}

//从文件读取当前快递员负责的快递，存储在packList中
void Postman::loadPostmanPackList() const
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
		//未接收的快递没有接收时间
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
		getline(packInfoFile, post.ID, SPLIT);
		//不是当前快递员负责的快递，没有权限，不存储
		if (post.ID.compare(ID))
		{
			packInfoFile.ignore(LLONG_MAX, '\n');
			continue;
		}
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

//快递员界面
void Postman::postmanInterface()
{
	char input;

	loadPostmanPackList();
	do {
		cout << "输入1揽收快递" << endl;
		cout << "输入2查询快递" << endl;
		cout << "输入3查看余额" << endl;
		cout << "输入q登出" << endl;
		input = getchar();
		cin.ignore(LLONG_MAX, '\n');
		switch (input)
		{
		case '1':
			pickUp();
			break;
		case '2':
			searchPack();
			break;
		case '3':
			checkBalance();
			break;
		case 'q':
			return;
		default:
			cout << "输入错误" << endl;
			break;
		}
	} while (1);
}

//揽收快递
void Postman::pickUp()
{
	string input;
	int locV;

	while (1)
	{
		//没有未揽收快递直接返回，有就输入快递号进行揽收
		cout << "未揽收快递：" << endl;
		if (!outputUnpickPack())
		{
			cout << "没有未揽收快递" << endl;
			return;
		}

		while (1)
		{
			cout << "输入快递号进行揽收，输入q退出" << endl;
			cin >> input;
			cin.ignore(LLONG_MAX, '\n');
			if (input == "q")
				return;
			locV = -1;
			
			//获取输入快递号的快递在packList的下标，没有就是-1
			for (int i = 0; i < packList.size(); i++)
			{
				if (!packList[i].packID.compare(input))
				{
					locV = i;
					break;
				}
			}
			if (locV == -1)
				cout << "不存在为该快递号的未揽收快递，请重新输入" << endl;
			else
				break;
		}

		balance += (packList[locV].price + 1) / 2;	//运费的一半转给快递员，向上取整
		updateBalanceInFile();			//文件中更新快递员余额
		packList[locV].pickUp();		//揽收快递
		cout << "揽收成功" << endl;

		cout << "是否继续揽收，是则输入任意键，否则输入q" << endl;
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

//查看余额
void Postman::checkBalance() const
{
	cout << "余额为" << balance << "元" << endl;
}

//查询快递
void Postman::searchPack() const
{
	char input;

	while (1)
	{
		cout << "输入1按快递号查询" << endl;
		cout << "输入2按寄件人查询" << endl;
		cout << "输入3按收件人查询" << endl;
		cout << "输入4按发送时间查询" << endl;
		cout << "输入5按快递状态查询" << endl;
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

//根据寄件人信息查询快递
void Postman::searchPackBySender() const
{
	char input;

	while (1)
	{
		cout << "输入1按寄件人ID查询" << endl;
		cout << "输入2按寄件人姓名查询" << endl;
		cout << "输入3按寄件人电话查询" << endl;
		cout << "输入4按寄件人地址查询" << endl;
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
void Postman::searchPackBySenderID() const
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
void Postman::searchPackBySenderName() const
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
void Postman::searchPackBySenderPhone() const
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
void Postman::searchPackBySenderAddr() const
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
void Postman::searchPackByRecver() const
{
	char input;

	while (1)
	{
		cout << "输入1按收件人ID查询" << endl;
		cout << "输入2按收件人姓名查询" << endl;
		cout << "输入3按收件人电话查询" << endl;
		cout << "输入4按收件人地址查询" << endl;
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
void Postman::searchPackByRecverID() const
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
void Postman::searchPackByRecverName() const
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
void Postman::searchPackByRecverPhone() const
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
void Postman::searchPackByRecverAddr() const
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
void Postman::searchPackBySendTime() const
{
	int count = 0;
	int i = 0;
	int flag = 0;
	string tmp;
	SYSTEMTIME input;

	//支持年、月、日和小时四种精度查找
	//至少输入年份，其他可用换行符跳过
	//flag表示查找精度，0为年份，1为月份，2为日，3为小时
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
void Postman::searchPackByPackID() const
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
void Postman::searchPackByState() const
{
	char input;

	while (1)
	{
		cout << "输入1查看未揽收快递" << endl;
		cout << "输入2查看未接收快递" << endl;
		cout << "输入3查看已接收快递" << endl;
		cout << "输入q退出" << endl;
		input = getchar();
		cin.ignore(LLONG_MAX, '\n');
		switch (input)
		{
		case '1':
			if (!outputUnpickPack())
				cout << "没有未揽收快递" << endl;
			break;
		case '2':
			if (!outputUnrecvPack())
				cout << "没有未接收快递" << endl;
			break;
		case '3':
			if (!outputRecvPack())
				cout << "没有已接收快递" << endl;
			break;
		case 'q':
			return;
		default:
			cout << "输入错误" << endl;
			break;
		}
	}
}

//输出未接收快递
int Postman::outputUnrecvPack() const
{
	int count = 0;

	//输出未接收的快递的信息
	for (int i = 0; i < packList.size(); i++)
	{
		if (packList[i].state == NOT_RECEIVE)
		{
			cout << ++count << endl;
			packList[i].output();
		}
	}

	return count;
}

//输出未揽收快递
int Postman::outputUnpickPack() const
{
	int count = 0;

	//输出未揽收的快递的信息
	for (int i = 0; i < packList.size(); i++)
	{
		if (packList[i].state == NOT_PICK)
		{
			cout << ++count << endl;
			packList[i].output();
		}
	}

	return count;
}

//输出已接收快递
int Postman::outputRecvPack() const
{
	int count = 0;

	//输出已接收的快递的信息
	for (int i = 0; i < packList.size(); i++)
	{
		if (packList[i].state == RECEIVE)
		{
			cout << ++count << endl;
			packList[i].output();
		}
	}

	return count;
}

//揽收快递后，在快递员信息文件中更新余额
void Postman::updateBalanceInFile() const
{
	fstream tmpFile;
	tmpFile.open(".\\data\\tmp.txt", ios::in | ios::out | ios::trunc);
	string tmp;
	streampos loc = searchID();
	postmanInfoFile.clear();
	postmanInfoFile.seekg(0);

	//将快递员信息文件中，当前快递员之前的内容写入临时文件
	while (postmanInfoFile.tellg() != loc)
	{
		getline(postmanInfoFile, tmp);
		//空行不写
		if (tmp.length())
			tmpFile << tmp << endl;
	}
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
}

//返回当前快递员的信息在快递员信息文件中的位置
streampos Postman::searchID() const
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
		if (!s.compare(ID))
			return loc;
		postmanInfoFile.ignore(LLONG_MAX, '\n');
	}

	//文件中没有，返回-1
	return -1;
}
