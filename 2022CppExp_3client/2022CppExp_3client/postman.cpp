#include "clienthead.h"

extern vector<Pack> packList;

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
	string input, req;

	while (1)
	{
		cout << "请输入快递员ID，输入q退出" << endl;
		cin >> input;
		cin.ignore(LLONG_MAX, '\n');
		if (input == "q")
			return;
		ID = input;
		//向服务端发送，检查ID是否存在
		req = string("2") + SPLIT + '0' + SPLIT + ID;
		strcpy_s(send_buf, req.c_str());
		send(server, send_buf, 1000, 0);
		recv(server, recv_buf, 1000, 0);
		if (recv_buf[0] == '0')
		{
			cout << "快递员ID不存在，请重新输入" << endl;
			continue;
		}
		//向服务端请求ID的密码
		req = string("2") + SPLIT + '1' + SPLIT + ID;
		strcpy_s(send_buf, req.c_str());
		send(server, send_buf, 1000, 0);
		recv(server, recv_buf, 1000, 0);		
		pw = recv_buf;
		int flag = 1;
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
	
	//向服务端请求快递员的其他信息
	req = string("2") + SPLIT + '2' + SPLIT + ID;
	strcpy_s(send_buf, req.c_str());
	send(server, send_buf, 1000, 0);
	recv(server, recv_buf, 1000, 0);
	istringstream iss(recv_buf);
	//读取其他信息
	getline(iss, name, SPLIT);
	getline(iss, phone, SPLIT);
	string balanceString;
	getline(iss, balanceString, SPLIT);
	balance = atoi(balanceString.c_str());

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
	string tmp, packs;
	string req = string("2") + SPLIT + '5' + SPLIT + ID;
	strcpy_s(send_buf, req.c_str());
	send(server, send_buf, 1000, 0);	//向服务端请求发送当前快递员负责的快递

	while (1)
	{
		//接收服务端发送的数据，直到收到表示发送结束的空串
		recv(server, recv_buf, 1000, 0);
		if (strlen(recv_buf) == 0)
			break;
		tmp = recv_buf;
		packs += tmp;	//packs里存储了所有接收到的快递信息
	}
	//从packs里读取快递信息
	packList.clear();
	istringstream iss(packs);
	while (!iss.eof())
	{
		string packID, packInfo, tmp;
		PackUserInfo sender, recver;
		PackPostmanInfo post;
		SYSTEMTIME sendTime, recvTime, pickUpTime;
		int type, amount, price, state;

		getline(iss, packID, SPLIT);
		if (!packID.length())
			break;
		getline(iss, tmp, SPLIT);
		state = atoi(tmp.c_str());
		getline(iss, tmp, SPLIT);
		sendTime.wYear = atoi(tmp.c_str());
		getline(iss, tmp, SPLIT);
		sendTime.wMonth = atoi(tmp.c_str());
		getline(iss, tmp, SPLIT);
		sendTime.wDay = atoi(tmp.c_str());
		getline(iss, tmp, SPLIT);
		sendTime.wHour = atoi(tmp.c_str());
		getline(iss, tmp, SPLIT);
		sendTime.wMinute = atoi(tmp.c_str());
		if (state == RECEIVE)
		{
			getline(iss, tmp, SPLIT);
			recvTime.wYear = atoi(tmp.c_str());
			getline(iss, tmp, SPLIT);
			recvTime.wMonth = atoi(tmp.c_str());
			getline(iss, tmp, SPLIT);
			recvTime.wDay = atoi(tmp.c_str());
			getline(iss, tmp, SPLIT);
			recvTime.wHour = atoi(tmp.c_str());
			getline(iss, tmp, SPLIT);
			recvTime.wMinute = atoi(tmp.c_str());
		}
		getline(iss, sender.ID, SPLIT);
		getline(iss, sender.name, SPLIT);
		getline(iss, sender.phone, SPLIT);
		getline(iss, sender.addr, SPLIT);
		getline(iss, recver.ID, SPLIT);
		getline(iss, recver.name, SPLIT);
		getline(iss, recver.phone, SPLIT);
		getline(iss, recver.addr, SPLIT);
		getline(iss, post.ID, SPLIT);
		getline(iss, post.name, SPLIT);
		getline(iss, post.phone, SPLIT);
		if (state != NOT_PICK)
		{
			getline(iss, tmp, SPLIT);
			pickUpTime.wYear = atoi(tmp.c_str());
			getline(iss, tmp, SPLIT);
			pickUpTime.wMonth = atoi(tmp.c_str());
			getline(iss, tmp, SPLIT);
			pickUpTime.wDay = atoi(tmp.c_str());
			getline(iss, tmp, SPLIT);
			pickUpTime.wHour = atoi(tmp.c_str());
			getline(iss, tmp, SPLIT);
			pickUpTime.wMinute = atoi(tmp.c_str());
		}
		getline(iss, tmp, SPLIT);
		type = atoi(tmp.c_str());
		getline(iss, tmp, SPLIT);
		amount = atoi(tmp.c_str());
		getline(iss, tmp, SPLIT);
		price = atoi(tmp.c_str());
		getline(iss, packInfo, SEND_SPLIT);

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

		packList[locV].pickUp();		//揽收快递
		balance += (packList[locV].price + 1) / 2;	//运费的一半转给快递员，向上取整
		updateBalanceInFile();			//文件中更新快递员余额

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
	//发送给服务端，更新余额
	string req = string("2") + SPLIT + '4' + SPLIT + to_string(balance);
	strcpy_s(send_buf, req.c_str());
	send(server, send_buf, 1000, 0);
	recv(server, recv_buf, 1000, 0);
	if (recv_buf[0] != '1')
		cout << "收款失败" << endl;
	else
		cout << "收款成功" << endl;
}
