#include "head2.h"

extern vector<Pack> packList;
extern vector<User> allUserList;
extern vector<Postman> allPostmanList;
extern fstream userInfoFile;
extern fstream adminInfoFile;
extern fstream packInfoFile;
extern fstream postmanInfoFile;

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

		//检查输入的ID是否存在
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

		//从管理员信息文件获取该ID的密码
		adminInfoFile.clear();
		adminInfoFile.seekg(loc);
		adminInfoFile.ignore(LLONG_MAX, SPLIT);
		getline(adminInfoFile, pw, SPLIT);
		//用户输入密码
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

	//加载其他信息
	getline(adminInfoFile, name, SPLIT);
	adminInfoFile.clear();
	adminInfoFile.seekg(0);
	adminInfoFile >> balance;	//所有管理员共用一个余额，存在文件的最开头

	adminInterface();	//管理员界面
}

//管理员有权查看所有用户，提前从文件中读取并将所有用户信息存储到向量allUserList
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

//管理员有权查看所有快递，提前从文件中读取并将所有快递信息存储到向量packList
void Admin::loadAllPackList() const
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

//管理员有权查看所有快递员，提前从文件中读取并将所有快递员信息存储到allPostmanList
void Admin::loadAllPostmanList() const
{
	allPostmanList.clear();
	postmanInfoFile.clear();
	postmanInfoFile.seekg(0);

	//管理员登录时，加载所有用户信息
	while (!postmanInfoFile.eof())
	{
		string id, pw, name, phone, balance;

		getline(postmanInfoFile, id, SPLIT);
		//避免读取空行
		if (!id.length())
			continue;
		getline(postmanInfoFile, pw, SPLIT);
		getline(postmanInfoFile, name, SPLIT);
		getline(postmanInfoFile, phone, SPLIT);
		getline(postmanInfoFile, balance);

		Postman p(id, pw, name, phone, atoi(balance.c_str()));
		allPostmanList.push_back(p);
	}
}

//管理员界面
void Admin::adminInterface() const
{
	char input;

	loadAllUserList();
	loadAllPackList();
	loadAllPostmanList();
	do {
		cout << "输入1管理用户信息" << endl;
		cout << "输入2管理物流信息" << endl;
		cout << "输入3管理快递员信息" << endl;
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
		case '3':
			adminPostmanInterface();
			break;
		case 'q':
			return;
		default:
			cout << "输入错误" << endl;
			break;
		}
	} while (1);
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

//管理员管理快递信息界面
void Admin::adminPackInterface() const
{
	char input;

	do {
		cout << "输入1查看所有快递" << endl;
		cout << "输入2按条件查询快递" << endl;
		cout << "输入3查询余额" << endl;
		cout << "输入4为快递分配快递员" << endl;
		cout << "输入5查看指定快递员揽收的所有快递" << endl;
		cout << "输入6从指定快递员揽收的快递中查找快递" << endl;
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
		case '4':
			assignPack();
			break;
		case '5':
			showAllPackAssignedToOne();
			break;
		case '6':
			searchPackAssignedToOne();
			break;
		case 'q':
			return;
		default:
			cout << "输入错误，请重新输入" << endl;
			break;
		}
	} while (1);
}

//管理员管理快递员信息界面
void Admin::adminPostmanInterface() const
{
	char input;

	do {
		cout << "输入1查看所有快递员" << endl;
		cout << "输入2查找快递员" << endl;
		cout << "输入3添加快递员" << endl;
		cout << "输入4删除快递员" << endl;
		cout << "输入q退出" << endl;
		input = getchar();
		cin.ignore(LLONG_MAX, '\n');
		switch (input)
		{
		case '1':
			checkAllPostman();
			break;
		case '2':
			searchPostman();
			break;
		case '3':
			addPostman();
			break;
		case '4':
			deletePostman();
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
	if (!allUserList.size())
	{
		cout << "没有用户" << endl;
		return;
	}

	for (int i = 0; i < allUserList.size(); i++)
	{
		allUserList[i].output();
	}
}

//查询用户
void Admin::searchUser() const
{
	char input;

	while (1)
	{
		cout << "输入1按用户ID查询" << endl;
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

//以用户ID查找用户
void Admin::searchUserByID() const
{
	int count = 0;
	int i = 0;
	string input;

	while (1)
	{
		cout << "请输入用户ID" << endl;
		getline(cin, input);

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

//以用户姓名查找用户
void Admin::searchUserByName() const
{
	int count = 0;
	int i = 0;
	string input;

	while (1)
	{
		cout << "请输入用户姓名" << endl;
		getline(cin, input);

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

//以用户电话查找用户
void Admin::searchUserByPhone() const
{
	int count = 0;
	int i = 0;
	string input;

	while (1)
	{
		cout << "请输入用户电话号码" << endl;
		getline(cin, input);

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

//以用户地址查找用户
void Admin::searchUserByAddr() const
{
	int count = 0;
	int i = 0;
	string input;

	while (1)
	{
		cout << "请输入用户地址" << endl;
		getline(cin, input);

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

//查看所有快递
void Admin::checkAllPack() const
{
	if (!packList.size())
	{
		cout << "没有快递" << endl;
		return;
	}

	for (int i = 0; i < packList.size(); i++)
	{
		packList[i].output();
	}
}

//查询余额
void Admin::checkBalance() const
{
	cout << "余额为" << balance << "元" << endl;
}

//查询快递
void Admin::searchPack() const
{
	char input;

	while (1)
	{
		cout << "输入1按快递号查询" << endl;
		cout << "输入2按寄件人查询" << endl;
		cout << "输入3按发件人查询" << endl;
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
void Admin::searchPackBySenderName() const
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
void Admin::searchPackBySenderPhone() const
{
	int count = 0;
	int i = 0;
	string input;

	while (1)
	{
		cout << "请输入寄件人电话" << endl;
		getline(cin, input);

		//检查输入电话的合法性
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
void Admin::searchPackBySenderAddr() const
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
void Admin::searchPackByRecverName() const
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
void Admin::searchPackByRecverPhone() const
{
	int count = 0;
	int i = 0;
	string input;

	while (1)
	{
		cout << "请输入收件人电话" << endl;
		getline(cin, input);

		//检查输入电话的合法性
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
void Admin::searchPackByRecverAddr() const
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
void Admin::searchPackBySendTime() const
{
	int count = 0;
	int i = 0;
	int flag = 0;
	string tmp;
	SYSTEMTIME input;

	//支持年、月、日和小时四种精度查找
	//至少输入年份，其他可用换行跳过
	//flag表示查找精度，0为年份，1为月，2为日，3为小时
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
void Admin::searchPackByPackID() const
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
void Admin::searchPackByState() const
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
int Admin::outputUnrecvPack() const
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
int Admin::outputUnpickPack() const
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
int Admin::outputRecvPack() const
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

//查看所有快递员
void Admin::checkAllPostman() const
{
	if (!allPostmanList.size())
	{
		cout << "没有快递员" << endl;
		return;
	}

	for (int i = 0; i < allPostmanList.size(); i++)
	{
		allPostmanList[i].output();
	}
}

//新增快递员
void Admin::addPostman() const
{
	string ID, name, phone, pw;
	int loc;

	while (1)
	{
		while (1)
		{
			cout << "请输入快递员ID，长度为8-12个字符，只能包含数字、大小写英文字母以及下划线" << endl << "输入q退出" << endl;
			cin >> ID;
			cin.ignore(LLONG_MAX, '\n');
			if (ID == "q")
				return;
			//检查输入ID的合法性
			int flag = 1;
			int len = ID.length();
			if (len < 8 || len > 12)
			{
				cout << "长度错误，重新输入" << endl;
				continue;
			}
			for (int i = 0; i < len; i++)
			{
				//如果不为英文字母、数字或下划线，错误
				if (!isalnum(ID[i]) && ID[i] != '_')
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
			loc = searchPostman(ID);
			//如果已存在该用户名
			if (loc >= 0)
			{
				cout << "该快递员用户ID已被使用，请重新输入" << endl;
				continue;
			}
			break;
		}

		while (1)
		{
			cout << "请输入密码，密码只能含有大小写字母和数字，长度在8-10个字符" << endl;
			cin >> pw;
			cin.ignore(LLONG_MAX, '\n');
			//检查输入密码的合法性
			if (!checkPwValid(pw))
			{
				cout << "密码格式错误，请重新输入" << endl;
			}
			else
				break;
		}

		cout << "请输入快递员姓名" << endl;
		getline(cin, name);

		while (1)
		{
			cout << "请输入电话号码" << endl;
			cin >> phone;
			cin.ignore(LLONG_MAX, '\n');
			//检查输入电话的合法性
			if (phone.length() != 11)
			{
				cout << "输入错误，请重新输入" << endl;
				continue;
			}
			int flag = 1;
			for (int i = 0; i < phone.length(); i++)
			{
				//检查是不是全是数字
				if (!isdigit(phone[i]))
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

		Postman postman(ID, pw, name, phone, 0);
		allPostmanList.push_back(postman);
		//写入新快递员信息
		addPostmanToFile();

		cout << "是否继续添加，是则输入任意键，否则输入q" << endl;
		char input = getchar();
		cin.ignore(LLONG_MAX, '\n');
		switch (input)
		{
		case 'q':
			return;
		default:
			break;
		}
	}
}

//删除快递员
void Admin::deletePostman() const
{
	string ID;
	int loc;

	//先输出所有的快递员信息
	cout << "所有的快递员：" << endl;
	for (int i = 0; i < allPostmanList.size(); i++)
		allPostmanList[i].output();

	while (1)
	{
		while (1)
		{
			cout << "请输入要删除的快递员ID" << endl << "输入q退出" << endl;
			cin >> ID;
			cin.ignore(LLONG_MAX, '\n');
			if (ID == "q")
				return;
			//检查输入ID的合法性
			int flag = 1;
			int len = ID.length();
			if (len < 8 || len > 12)
			{
				cout << "长度错误，重新输入" << endl;
				continue;
			}
			for (int i = 0; i < len; i++)
			{
				//如果不为英文字母、数字或下划线，错误
				if (!isalnum(ID[i]) && ID[i] != '_')
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
			loc = searchPostman(ID);
			//如果不存在该用户名
			if (loc < 0)
			{
				cout << "该快递员不存在，请重新输入" << endl;
				continue;
			}
			break;
		}

		//检查被删除的快递员是否有未完成的运送任务
		int flag = 0;
		for (int i = 0; i < packList.size(); i++)
		{
			if (!packList[i].postman.ID.compare(ID) && packList[i].state != RECEIVE)
			{
				flag = 1;
				break;
			}
		}
		if (flag)
		{
			while (1)
			{
				cout << "该快递员有未完成的运送任务，请问是否删除？" << endl;
				cout << "继续删除请按1，需要重新为快递分配快递员。不删除请按2" << endl;
				char input = getchar();
				cin.ignore(LLONG_MAX, '\n');
				if (input == '1')
				{
					flag = 2;
					break;
				}
				else if (input == '2')
					break;
				else
					cout << "输入错误，请重新输入" << endl;
			}
		}
		if (flag == 1)
			continue;
		else if (flag == 2)		//修改该快递员未完成运送的快递的信息
		{
			for (int i = 0; i < packList.size(); i++)
			{
				if (!packList[i].postman.ID.compare(ID) && packList[i].state != RECEIVE)
					packList[i].deletePostman();
			}
		}

		//删除该快递员
		allPostmanList.erase(allPostmanList.begin() + loc);
		deletePostmanFromFile(ID);

		cout << "是否继续删除，是则输入任意键，否则输入q" << endl;
		char input = getchar();
		cin.ignore(LLONG_MAX, '\n');
		switch (input)
		{
		case 'q':
			return;
		default:
			break;
		}
	}
}

//查看ID为参数id的快递员是否存在，是则返回在allPostman向量的下标，否则返回-1
int Admin::searchPostman(const string& id) const
{
	string s;
	streampos loc = 0;

	//文件为空，直接返回0
	if (!allPostmanList.size())
		return -1;

	//查找当前对象用户名，有就立即返回
	for (int i = 0; i < allPostmanList.size(); i++)
	{
		if (!allPostmanList[i].ID.compare(id))
			return i;
	}

	//文件中没有，返回0
	return -1;
}

//检查输入的新密码是否符合格式要求，是则返回1，否则返回0
int Admin::checkPwValid(const string& newPw) const
{
	int flag = 1;
	int len = newPw.length();

	//密码长度在8-10个字符
	if (len < 8 || len > 10)
		return 0;

	//密码只能由英文字母和数字构成
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

//向快递员信息文件中写入新快递员信息
void Admin::addPostmanToFile() const
{
	int loc = allPostmanList.size() - 1;	//新快递员的信息刚刚添加，一定是向量最后一个元素

	//往文件末尾写入
	postmanInfoFile.clear();
	postmanInfoFile.seekg(0, ios::end);
	postmanInfoFile << allPostmanList[loc].ID << SPLIT << allPostmanList[loc].pw << SPLIT << allPostmanList[loc].name << SPLIT << allPostmanList[loc].phone << SPLIT << 0 << endl;
}

//从快递员信息文件删除ID为参数id的快递员信息
void Admin::deletePostmanFromFile(const string& id) const
{
	fstream tmpFile;
	string tmp;
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
}

//为快递分配快递员
void Admin::assignPack() const
{
	string packId, postmanId;
	int packLocV, postmanLocV;

	while (1)
	{
		//查看是否有未分配快递员的快递，有就一一输出
		if (!checkUnassignPack())
		{
			cout << "没有未分配快递员的快递" << endl;
			return;
		}

		while (1)
		{
			cout << "输入快递号进行分配，输入q退出" << endl;
			cin >> packId;
			cin.ignore(LLONG_MAX, '\n');
			if (packId == "q")
				return;
			//检查输入快递号的合法性
			packLocV = -1;
			
			//获取该快递号的快递在packList的下标
			for (int i = 0; i < packList.size(); i++)
			{
				if (!packList[i].packID.compare(packId))
				{
					packLocV = i;
					break;
				}
			}
			if (packLocV == -1)
				cout << "不存在快递号与输入一致的未分配快递，请重新输入" << endl;
			else
				break;
		}

		//输入要分配的快递员
		while (1)
		{
			cout << "请输入快递员用户ID" << endl << "输入q退出" << endl;
			cin >> postmanId;
			cin.ignore(LLONG_MAX, '\n');
			if (postmanId == "q")
				return;
			//检查输入ID的合法性
			postmanLocV = -1;
			int flag = 1;
			int len = postmanId.length();
			if (len < 8 || len > 12)
			{
				cout << "长度错误，重新输入" << endl;
				continue;
			}
			for (int i = 0; i < len; i++)
			{
				//如果不为英文字母、数字或下划线，错误
				if (!isalnum(postmanId[i]) && postmanId[i] != '_')
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
			postmanLocV = searchPostman(postmanId);
			//如果不存在该用户名
			if (postmanLocV < 0)
			{
				cout << "该快递员不存在，请重新输入" << endl;
				continue;
			}
			break;
		}

		PackPostmanInfo postmanInfo;
		postmanInfo.ID = allPostmanList[postmanLocV].ID;
		postmanInfo.phone = allPostmanList[postmanLocV].phone;
		postmanInfo.name = allPostmanList[postmanLocV].name;
		packList[packLocV].assignPostman(postmanInfo);		//为快递分配快递员

		cout << "是否继续分配，是则输入任意键，否则输入q" << endl;
		char input = getchar();
		cin.ignore(LLONG_MAX, '\n');
		switch (input)
		{
		case 'q':
			return;
		default:
			break;
		}
	}

}

//输出未分配快递员的快递
int Admin::checkUnassignPack() const
{
	int count = 0;

	for (int i = 0; i < packList.size(); i++)
	{
		//未分配快递员的快递中，快递员ID为"???"
		if (!packList[i].postman.ID.compare("???"))
		{
			cout << count++ << endl;
			packList[i].output();
		}
	}

	return count;
}

//查询快递员
void Admin::searchPostman() const
{
	char input;

	while (1)
	{
		cout << "输入1按快递员用户ID查询" << endl;
		cout << "输入2按快递员姓名查询" << endl;
		cout << "输入3按快递员电话查询" << endl;
		cout << "输入q退出" << endl;
		input = getchar();
		cin.ignore(LLONG_MAX, '\n');
		switch (input)
		{
		case '1':
			searchPostmanByID();
			break;
		case '2':
			searchPostmanByName();
			break;
		case '3':
			searchPostmanByPhone();
			break;
		case 'q':
			return;
		default:
			cout << "输入错误" << endl;
			break;
		}
	}
}

//以ID查询快递员
void Admin::searchPostmanByID() const
{
	int count = 0;
	int i = 0;
	string input;

	while (1)
	{
		cout << "请输入用户号" << endl;
		getline(cin, input);

		cout << "搜索结果：" << endl;
		for (i = 0, count = 0; i < allPostmanList.size(); i++)
		{
			if (!allPostmanList[i].ID.compare(input))
			{
				allPostmanList[i].output();
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

//以姓名查询快递员
void Admin::searchPostmanByName() const
{
	int count = 0;
	int i = 0;
	string input;

	while (1)
	{
		cout << "请输入用户姓名" << endl;
		getline(cin, input);

		cout << "搜索结果：" << endl;
		for (i = 0, count = 0; i < allPostmanList.size(); i++)
		{
			if (!allPostmanList[i].name.compare(input))
			{
				cout << ++count << endl;
				allPostmanList[i].output();
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

//以电话查询快递员
void Admin::searchPostmanByPhone() const
{
	int count = 0;
	int i = 0;
	string input;

	while (1)
	{
		cout << "请输入用户电话号码" << endl;
		getline(cin, input);

		cout << "搜索结果：" << endl;
		for (i = 0, count = 0; i < allPostmanList.size(); i++)
		{
			if (!allPostmanList[i].phone.compare(input))
			{
				cout << ++count << endl;
				allPostmanList[i].output();
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

//输出分配给指定一个快递员的所有快递
void Admin::showAllPackAssignedToOne() const
{
	int count = 0;
	int i = 0;
	string input;

	while (1)
	{
		cout << "请输入快递员ID" << endl;
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

		//输出所有该快递员负责的运送任务
		cout << "搜索结果：" << endl;
		for (i = 0, count = 0; i < packList.size(); i++)
		{
			if (!packList[i].postman.ID.compare(input))
			{
				packList[i].output();
				count++;
			}
		}

		if (count == 0)
			cout << "该快递员没有负责任何运送任务" << endl;

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

//在指定的一个快递员负责的快递中查找快递
void Admin::searchPackAssignedToOne() const
{
	int count = 0;
	int i = 0;
	string id;

	//先输入快递员ID，再选择查找方式
	while (1)
	{
		cout << "请输入快递员ID，输入q退出" << endl;
		getline(cin, id);

		if (id == "q")
			return;
		//检查输入的合法性
		int flag = 1;
		int len = id.length();
		if (len < 8 || len > 12)
		{
			cout << "长度错误" << endl;
			continue;
		}
		for (int i = 0; i < len; i++)
		{
			if (!isalnum(id[i]) && id[i] != '_')
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

		//选择查找方式
		while (1)
		{
			cout << "输入1按快递号查询" << endl;
			cout << "输入2按寄件人查询" << endl;
			cout << "输入3按收件人查询" << endl;
			cout << "输入4按发送时间查询" << endl;
			cout << "输入5按快递状态查询" << endl;
			cout << "输入q退出" << endl;
			char input = getchar();
			cin.ignore(LLONG_MAX, '\n');
			switch (input)
			{
			case '1':
				searchAssignPackByPackID(id);
				break;
			case '2':
				searchAssignPackBySender(id);
				break;
			case '3':
				searchAssignPackByRecver(id);
				break;
			case '4':
				searchAssignPackBySendTime(id);
				break;
			case '5':
				searchAssignPackByState(id);
				break;
			case 'q':
				return;
			default:
				cout << "输入错误" << endl;
				break;
			}
		}
	}
}

//根据寄件人信息查询快递
void Admin::searchAssignPackBySender(const string& id) const
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
			searchAssignPackBySenderID(id);
			break;
		case '2':
			searchAssignPackBySenderName(id);
			break;
		case '3':
			searchAssignPackBySenderPhone(id);
			break;
		case '4':
			searchAssignPackBySenderAddr(id);
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
void Admin::searchAssignPackBySenderID(const string& id) const
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

		cout << "搜索结果：" << endl;
		for (i = 0, count = 0; i < packList.size(); i++)
		{
			if (!packList[i].sender.ID.compare(input) && !packList[i].postman.ID.compare(id))
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
void Admin::searchAssignPackBySenderName(const string& id) const
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
			if (!packList[i].sender.name.compare(input) && !packList[i].postman.ID.compare(id))
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
void Admin::searchAssignPackBySenderPhone(const string& id) const
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
			if (!packList[i].sender.phone.compare(input) && !packList[i].postman.ID.compare(id))
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
void Admin::searchAssignPackBySenderAddr(const string& id) const
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
			if (!packList[i].sender.addr.compare(input) && !packList[i].postman.ID.compare(id))
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
void Admin::searchAssignPackByRecver(const string& id) const
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
			searchAssignPackByRecverID(id);
			break;
		case '2':
			searchAssignPackByRecverName(id);
			break;
		case '3':
			searchAssignPackByRecverPhone(id);
			break;
		case '4':
			searchAssignPackByRecverAddr(id);
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
void Admin::searchAssignPackByRecverID(const string& id) const
{
	int count = 0;
	int i = 0;
	string input;

	while (1)
	{
		cout << "请输入收件人ID" << endl;
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

		cout << "搜索结果：" << endl;
		for (i = 0, count = 0; i < packList.size(); i++)
		{
			if (!packList[i].recver.ID.compare(input) && !packList[i].postman.ID.compare(id))
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
void Admin::searchAssignPackByRecverName(const string& id) const
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
			if (!packList[i].recver.name.compare(input) && !packList[i].postman.ID.compare(id))
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
void Admin::searchAssignPackByRecverPhone(const string& id) const
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
			if (!packList[i].recver.phone.compare(input) && !packList[i].postman.ID.compare(id))
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
void Admin::searchAssignPackByRecverAddr(const string& id) const
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
			if (!packList[i].recver.addr.compare(input) && !packList[i].postman.ID.compare(id))
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
void Admin::searchAssignPackBySendTime(const string& id) const
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
			if (packList[i].postman.ID.compare(id))
				continue;
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
void Admin::searchAssignPackByPackID(const string& id) const
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
			if (!packList[i].packID.compare(input) && !packList[i].postman.ID.compare(id))
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
void Admin::searchAssignPackByState(const string& id) const
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
			if (!outputUnpickAssignPack(id))
				cout << "没有未揽收快递" << endl;
			break;
		case '2':
			if (!outputUnrecvAssignPack(id))
				cout << "没有未接收快递" << endl;
			break;
		case '3':
			if (!outputRecvAssignPack(id))
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
int Admin::outputUnrecvAssignPack(const string& id) const
{
	int count = 0;

	//输出未接收的快递的信息
	for (int i = 0; i < packList.size(); i++)
	{
		if (packList[i].state == NOT_RECEIVE && !packList[i].postman.ID.compare(id))
		{
			cout << ++count << endl;
			packList[i].output();
		}
	}

	return count;
}

//输出未揽收快递
int Admin::outputUnpickAssignPack(const string& id) const
{
	int count = 0;

	//输出未揽收的快递的信息
	for (int i = 0; i < packList.size(); i++)
	{
		if (packList[i].state == NOT_PICK && !packList[i].postman.ID.compare(id))
		{
			cout << ++count << endl;
			packList[i].output();
		}
	}

	return count;
}

//输出已接收快递
int Admin::outputRecvAssignPack(const string& id) const
{
	int count = 0;

	//输出已接收的快递的信息
	for (int i = 0; i < packList.size(); i++)
	{
		if (packList[i].state == RECEIVE && !packList[i].postman.ID.compare(id))
		{
			cout << ++count << endl;
			packList[i].output();
		}
	}

	return count;
}
