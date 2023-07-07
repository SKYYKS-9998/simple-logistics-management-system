#include "head1.h"

extern vector<Pack> packList;
extern vector<User> allUserList;
extern fstream userInfoFile;
extern fstream adminInfoFile;
extern fstream packInfoFile;

Admin::Admin()
	: balance(0)
{}

Admin::~Admin()
{}

//登录
void Admin::login()
{
	string input;

	/*
	* 输入管理员ID和密码登录
	* 加载管理员信息
	*/
	while (1)
	{
		cout << "请输入管理员ID，输入q退出" << endl;
		cin >> input;
		cin.ignore(LLONG_MAX, '\n');

		if (input == "q")
			return;

		//从管理员信息文件中查找输入的管理员ID
		ID = input;
		streampos loc = -1;
		string tmp;
		int flag = 1;
		adminInfoFile.clear();
		adminInfoFile.seekg(0);
		adminInfoFile.ignore(LLONG_MAX, '\n');
		while (!adminInfoFile.eof())
		{
			loc = adminInfoFile.tellg();
			getline(adminInfoFile, tmp, SPLIT);
			if (!tmp.compare(ID))
				break;
			adminInfoFile.ignore(LLONG_MAX, '\n');
		}
		if (adminInfoFile.eof())
		{
			cout << "管理员ID不存在，请重新输入" << endl;
			continue;
		}

		//从文件中读取管理员账号密码，检查和输入是否一致
		adminInfoFile.clear();
		adminInfoFile.seekg(loc);
		adminInfoFile.ignore(LLONG_MAX, SPLIT);
		getline(adminInfoFile, pw, SPLIT);
		while (1)
		{
			cout << "请输入密码，输入q重新输入管理员ID" << endl;
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

	//登录完成，加载其他信息
	getline(adminInfoFile, name, SPLIT);
	adminInfoFile.clear();
	adminInfoFile.seekg(0);
	adminInfoFile >> balance;	

	//进入管理员界面
	adminInterface();
}

//管理员界面
void Admin::adminInterface() const
{
	char input;

	loadAllPacklist();
	loadAllUserList();
	do {
		cout << "输入1管理用户信息" << endl;
		cout << "输入2管理物流信息" << endl;
		cout << "输入q登出" << endl;
		input = getchar();
		cin.ignore(LLONG_MAX, '\n');
		switch (input)
		{
		case '1':
			adminUserInterface();
			break;
		case '2':
			adminPackInterface();
			break;
		case 'q':
			return;
		default:
			cout << "输入错误" << endl;
			break;
		}
	} while (1);
}

//管理员有权访问所有快递，从文件读取所有快递
void Admin::loadAllPacklist() const
{
	packList.clear();
	packInfoFile.clear();
	packInfoFile.seekg(0);
	string s;

	//管理员登录时，从文件读取所有快递信息并存在packList
	while (!packInfoFile.eof())
	{
		string packID, packInfo;
		PackUserInfo sender, recver;
		SYSTEMTIME sendTime, recvTime;
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
		getline(packInfoFile, packInfo);

		Pack p(packID, state, sendTime, recvTime, sender, recver, packInfo);

		packList.push_back(p);
	}
}

//管理员有权访问所有用户，从文件读取所有用户
void Admin::loadAllUserList() const
{
	allUserList.clear();
	userInfoFile.clear();
	userInfoFile.seekg(0);

	//管理员登录时，加载所有用户信息
	while (!userInfoFile.eof())
	{
		string id, pw, name, phone, addr, balance;

		getline(userInfoFile, id, SPLIT);
		//避免读取空行
		if (!id.length())
			continue;
		getline(userInfoFile, pw, SPLIT);
		getline(userInfoFile, name, SPLIT);
		getline(userInfoFile, phone, SPLIT);
		getline(userInfoFile, addr, SPLIT);
		getline(userInfoFile, balance);

		User u(id, pw, name, phone, addr, atoi(balance.c_str()));
		allUserList.push_back(u);
	}
}

//管理员管理用户信息界面
void Admin::adminUserInterface() const
{
	char input;

	do {
		cout << "输入1查看所有用户" << endl;
		cout << "输入2按条件查询用户" << endl;
		cout << "输入q退出" << endl;
		input = getchar();
		cin.ignore(LLONG_MAX, '\n');
		switch (input)
		{
		case '1':
			checkAllUser();
			break;
		case '2':
			searchUser();
			break;
		case 'q':
			return;
		default:
			cout << "输入错误，请重新输入" << endl;
			break;
		}
	} while (1);
}

//管理员管理物流信息界面
void Admin::adminPackInterface() const
{
	char input;

	do {
		cout << "输入1查看所有快递" << endl;
		cout << "输入2按条件查询快递" << endl;
		cout << "输入3查询余额" << endl;
		cout << "输入q退出" << endl;
		input = getchar();
		cin.ignore(LLONG_MAX, '\n');
		switch (input)
		{
		case '1':
			checkAllPack();
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
			cout << "输入错误，请重新输入" << endl;
			break;
		}
	} while (1);
}

//查看所有用户
void Admin::checkAllUser() const
{
	for (int i = 0; i < allUserList.size(); i++)
	{
		cout << i << endl;
		allUserList[i].output();
	}
}

//查看所有快递
void Admin::checkAllPack() const
{
	for (int i = 0; i < packList.size(); i++)
	{
		cout << i << endl;
		packList[i].output();
	}
}

//查询余额
void Admin::checkBalance() const
{
	cout << "余额为" << balance << "元" << endl;
}

//查询用户
void Admin::searchUser() const
{
	char input;

	while (1)
	{
		cout << "输入1按用户号查询" << endl;
		cout << "输入2按用户姓名查询" << endl;
		cout << "输入3按用户电话查询" << endl;
		cout << "输入4按用户地址查询" << endl;
		cout << "输入q退出" << endl;
		input = getchar();
		cin.ignore(LLONG_MAX, '\n');
		switch (input)
		{
		case '1':
			searchUserByID();
			break;
		case '2':
			searchUserByName();
			break;
		case '3':
			searchUserByPhone();
			break;
		case '4':
			searchUserByAddr();
			break;
		case 'q':
			return;
		default:
			cout << "输入错误" << endl;
			break;
		}
	}
}

//查询快递
void Admin::searchPack() const
{
	char input;

	while (1)
	{
		cout << "输入1按快递号查询" << endl;
		cout << "输入2按寄件人查询" << endl;
		cout << "输入3按收件人查询" << endl;
		cout << "输入4按发送时间查询" << endl;
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
		case 'q':
			return;
		default:
			cout << "输入错误" << endl;
			break;
		}
	}
}

//以用户ID查询用户
void Admin::searchUserByID() const
{
	int count = 0;
	int i = 0;
	string input;

	while (1)
	{
		cout << "请输入用户号" << endl;
		getline(cin, input);

		//检查输入ID的合法性
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

		//输出符合条件的结果
		cout << "搜索结果：" << endl;
		for (i = 0, count = 0; i < allUserList.size(); i++)
		{
			if (!allUserList[i].ID.compare(input))
			{
				allUserList[i].output();
				count++;
				break;
			}
		}

		if (count == 0)
			cout << "不存在此用户号" << endl;

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

//以用户姓名查询用户
void Admin::searchUserByName() const
{
	int count = 0;
	int i = 0;
	string input;

	while (1)
	{
		cout << "请输入用户姓名" << endl;
		getline(cin, input);

		//输出符合条件的结果
		cout << "搜索结果：" << endl;
		for (i = 0, count = 0; i < allUserList.size(); i++)
		{
			if (!allUserList[i].name.compare(input))
			{
				cout << ++count << endl;
				allUserList[i].output();
			}
		}

		if (count == 0)
			cout << "没有符合条件的结果" << endl;

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

//以用户电话查询用户
void Admin::searchUserByPhone() const
{
	int count = 0;
	int i = 0;
	string input;

	while (1)
	{
		cout << "请输入用户电话号码" << endl;
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

		//输出符合条件的结果
		cout << "搜索结果：" << endl;
		for (i = 0, count = 0; i < allUserList.size(); i++)
		{
			if (!allUserList[i].phone.compare(input))
			{
				cout << ++count << endl;
				allUserList[i].output();
			}
		}

		if (count == 0)
			cout << "没有符合条件的结果" << endl;

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

//以用户地址查询用户
void Admin::searchUserByAddr() const
{
	int count = 0;
	int i = 0;
	string input;

	while (1)
	{
		cout << "请输入用户地址" << endl;
		getline(cin, input);

		//输出符合条件的结果
		cout << "搜索结果：" << endl;
		for (i = 0, count = 0; i < allUserList.size(); i++)
		{
			if (!allUserList[i].addr.compare(input))
			{
				cout << ++count << endl;
				allUserList[i].output();
			}
		}

		if (count == 0)
			cout << "没有符合条件的结果" << endl;

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

//根据寄件人信息查询快递
void Admin::searchPackBySender() const
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
void Admin::searchPackBySenderID() const
{
	int count = 0;
	int i = 0;
	string input;

	while (1)
	{
		cout << "请输入寄件人ID" << endl;
		getline(cin, input);

		//检查输入ID的合法性
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

		//输出符合条件的结果
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
			cout << "没有符合条件的结果" << endl;

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
void Admin::searchPackBySenderName() const
{
	int count = 0;
	int i = 0;
	string input;

	while (1)
	{
		cout << "请输入寄件人姓名" << endl;
		getline(cin, input);

		//输出符合条件的结果
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
			cout << "没有符合条件的结果" << endl;

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
void Admin::searchPackBySenderPhone() const
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

		//输出符合条件的结果
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
			cout << "没有符合条件的结果" << endl;

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
void Admin::searchPackBySenderAddr() const
{
	int count = 0;
	int i = 0;
	string input;

	while (1)
	{
		cout << "请输入寄件人地址" << endl;
		getline(cin, input);

		//输出符合条件的结果
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
			cout << "没有符合条件的结果" << endl;

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
void Admin::searchPackByRecver() const
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
void Admin::searchPackByRecverID() const
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

		//输出符合条件的结果
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
			cout << "没有符合条件的结果" << endl;

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
void Admin::searchPackByRecverName() const
{
	int count = 0;
	int i = 0;
	string input;

	while (1)
	{
		cout << "请输入收件人姓名" << endl;
		getline(cin, input);

		//输出符合条件的结果
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
			cout << "没有符合条件的结果" << endl;

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
void Admin::searchPackByRecverPhone() const
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

		//输出符合条件的结果
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
			cout << "没有符合条件的结果" << endl;

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
void Admin::searchPackByRecverAddr() const
{
	int count = 0;
	int i = 0;
	string input;

	while (1)
	{
		cout << "请输入收件人地址" << endl;
		getline(cin, input);

		//输出符合条件的结果
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
			cout << "没有符合条件的结果" << endl;

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
void Admin::searchPackBySendTime() const
{
	int count = 0;
	int i = 0;
	int flag = 0;
	string tmp;
	SYSTEMTIME input;

	//支持年、月、日和小时四种精度查找
	//至少输入年份，其他可用换行跳过
	//int flag表示当前的查找精度，0为年，1为月，2为日，3为小时
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

		//输出符合条件的结果
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
			cout << "没有符合条件的结果" << endl;

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
void Admin::searchPackByPackID() const
{
	int count = 0;
	int i = 0;
	string input;

	while (1)
	{
		cout << "请输入快递号" << endl;
		getline(cin, input);

		//输出符合条件的结果
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
			cout << "不存在此快递号" << endl;

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
