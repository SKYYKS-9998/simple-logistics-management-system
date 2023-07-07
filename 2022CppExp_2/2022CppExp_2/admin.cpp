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

//��¼
void Admin::login()
{
	string input;

	/*
	* �������ԱID�������¼
	* ���ع���Ա��Ϣ
	*/
	while (1)
	{
		cout << "���������ԱID������q�˳�" << endl;
		cin >> input;
		cin.ignore(LLONG_MAX, '\n');

		if (input == "q")
			return;

		//��������ID�Ƿ����
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
			cout << "����ԱID�����ڣ�����������" << endl;
			continue;
		}

		//�ӹ���Ա��Ϣ�ļ���ȡ��ID������
		adminInfoFile.clear();
		adminInfoFile.seekg(loc);
		adminInfoFile.ignore(LLONG_MAX, SPLIT);
		getline(adminInfoFile, pw, SPLIT);
		//�û���������
		while (1)
		{
			cout << "���������룬����q�����������ԱID" << endl;
			cin >> input;
			cin.ignore(LLONG_MAX, '\n');
			if (!input.compare(pw))
			{
				cout << "��¼�ɹ�" << endl;
				break;
			}
			else if (input == "q")
			{
				flag = 0;
				break;
			}
			else
			{
				cout << "�����������������" << endl;
				continue;
			}
		}
		if (!flag)
			continue;
		break;
	}

	//����������Ϣ
	getline(adminInfoFile, name, SPLIT);
	adminInfoFile.clear();
	adminInfoFile.seekg(0);
	adminInfoFile >> balance;	//���й���Ա����һ���������ļ����ͷ

	adminInterface();	//����Ա����
}

//����Ա��Ȩ�鿴�����û�����ǰ���ļ��ж�ȡ���������û���Ϣ�洢������allUserList
void Admin::loadAllUserList() const
{
	allUserList.clear();
	userInfoFile.clear();
	userInfoFile.seekg(0);

	//����Ա��¼ʱ�����������û���Ϣ
	while (!userInfoFile.eof())
	{
		string id, pw, name, phone, addr, balance;

		getline(userInfoFile, id, SPLIT);
		//�����ȡ����
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

//����Ա��Ȩ�鿴���п�ݣ���ǰ���ļ��ж�ȡ�������п����Ϣ�洢������packList
void Admin::loadAllPackList() const
{
	packList.clear();
	packInfoFile.clear();
	packInfoFile.seekg(0);
	string s;

	//����Ա��¼ʱ�����ļ���ȡ���п����Ϣ������packList
	while (!packInfoFile.eof())
	{
		string packID, packInfo;
		PackUserInfo sender, recver;
		PackPostmanInfo post;
		SYSTEMTIME sendTime, recvTime, pickUpTime;
		int type, amount, price;
		int state;

		getline(packInfoFile, packID, SPLIT);
		//�����ȡ����
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
		//δ���յĿ��û�н���ʱ��
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
		//δ���յĿ��û������ʱ��
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

//����Ա��Ȩ�鿴���п��Ա����ǰ���ļ��ж�ȡ�������п��Ա��Ϣ�洢��allPostmanList
void Admin::loadAllPostmanList() const
{
	allPostmanList.clear();
	postmanInfoFile.clear();
	postmanInfoFile.seekg(0);

	//����Ա��¼ʱ�����������û���Ϣ
	while (!postmanInfoFile.eof())
	{
		string id, pw, name, phone, balance;

		getline(postmanInfoFile, id, SPLIT);
		//�����ȡ����
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

//����Ա����
void Admin::adminInterface() const
{
	char input;

	loadAllUserList();
	loadAllPackList();
	loadAllPostmanList();
	do {
		cout << "����1�����û���Ϣ" << endl;
		cout << "����2����������Ϣ" << endl;
		cout << "����3������Ա��Ϣ" << endl;
		cout << "����q�ǳ�" << endl;
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
			cout << "�������" << endl;
			break;
		}
	} while (1);
}

//����Ա�����û���Ϣ����
void Admin::adminUserInterface() const
{
	char input;

	do {
		cout << "����1�鿴�����û�" << endl;
		cout << "����2��������ѯ�û�" << endl;
		cout << "����q�˳�" << endl;
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
			cout << "�����������������" << endl;
			break;
		}
	} while (1);
}

//����Ա��������Ϣ����
void Admin::adminPackInterface() const
{
	char input;

	do {
		cout << "����1�鿴���п��" << endl;
		cout << "����2��������ѯ���" << endl;
		cout << "����3��ѯ���" << endl;
		cout << "����4Ϊ��ݷ�����Ա" << endl;
		cout << "����5�鿴ָ�����Ա���յ����п��" << endl;
		cout << "����6��ָ�����Ա���յĿ���в��ҿ��" << endl;
		cout << "����q�˳�" << endl;
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
			cout << "�����������������" << endl;
			break;
		}
	} while (1);
}

//����Ա������Ա��Ϣ����
void Admin::adminPostmanInterface() const
{
	char input;

	do {
		cout << "����1�鿴���п��Ա" << endl;
		cout << "����2���ҿ��Ա" << endl;
		cout << "����3��ӿ��Ա" << endl;
		cout << "����4ɾ�����Ա" << endl;
		cout << "����q�˳�" << endl;
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
			cout << "�����������������" << endl;
			break;
		}
	} while (1);
}

//�鿴�����û�
void Admin::checkAllUser() const
{
	if (!allUserList.size())
	{
		cout << "û���û�" << endl;
		return;
	}

	for (int i = 0; i < allUserList.size(); i++)
	{
		allUserList[i].output();
	}
}

//��ѯ�û�
void Admin::searchUser() const
{
	char input;

	while (1)
	{
		cout << "����1���û�ID��ѯ" << endl;
		cout << "����2���û�������ѯ" << endl;
		cout << "����3���û��绰��ѯ" << endl;
		cout << "����4���û���ַ��ѯ" << endl;
		cout << "����q�˳�" << endl;
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
			cout << "�������" << endl;
			break;
		}
	}
}

//���û�ID�����û�
void Admin::searchUserByID() const
{
	int count = 0;
	int i = 0;
	string input;

	while (1)
	{
		cout << "�������û�ID" << endl;
		getline(cin, input);

		cout << "���������" << endl;
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
			cout << "����ʧ��" << endl;

		cout << "�Ƿ���������������������������������q" << endl;
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

//���û����������û�
void Admin::searchUserByName() const
{
	int count = 0;
	int i = 0;
	string input;

	while (1)
	{
		cout << "�������û�����" << endl;
		getline(cin, input);

		cout << "���������" << endl;
		for (i = 0, count = 0; i < allUserList.size(); i++)
		{
			if (!allUserList[i].name.compare(input))
			{
				cout << ++count << endl;
				allUserList[i].output();
			}
		}

		if (count == 0)
			cout << "����ʧ��" << endl;

		cout << "�Ƿ���������������������������������q" << endl;
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

//���û��绰�����û�
void Admin::searchUserByPhone() const
{
	int count = 0;
	int i = 0;
	string input;

	while (1)
	{
		cout << "�������û��绰����" << endl;
		getline(cin, input);

		cout << "���������" << endl;
		for (i = 0, count = 0; i < allUserList.size(); i++)
		{
			if (!allUserList[i].phone.compare(input))
			{
				cout << ++count << endl;
				allUserList[i].output();
			}
		}

		if (count == 0)
			cout << "����ʧ��" << endl;

		cout << "�Ƿ���������������������������������q" << endl;
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

//���û���ַ�����û�
void Admin::searchUserByAddr() const
{
	int count = 0;
	int i = 0;
	string input;

	while (1)
	{
		cout << "�������û���ַ" << endl;
		getline(cin, input);

		cout << "���������" << endl;
		for (i = 0, count = 0; i < allUserList.size(); i++)
		{
			if (!allUserList[i].addr.compare(input))
			{
				cout << ++count << endl;
				allUserList[i].output();
			}
		}

		if (count == 0)
			cout << "����ʧ��" << endl;

		cout << "�Ƿ���������������������������������q" << endl;
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

//�鿴���п��
void Admin::checkAllPack() const
{
	if (!packList.size())
	{
		cout << "û�п��" << endl;
		return;
	}

	for (int i = 0; i < packList.size(); i++)
	{
		packList[i].output();
	}
}

//��ѯ���
void Admin::checkBalance() const
{
	cout << "���Ϊ" << balance << "Ԫ" << endl;
}

//��ѯ���
void Admin::searchPack() const
{
	char input;

	while (1)
	{
		cout << "����1����ݺŲ�ѯ" << endl;
		cout << "����2���ļ��˲�ѯ" << endl;
		cout << "����3�������˲�ѯ" << endl;
		cout << "����4������ʱ���ѯ" << endl;
		cout << "����5�����״̬��ѯ" << endl;
		cout << "����q�˳�" << endl;
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
			cout << "�������" << endl;
			break;
		}
	}
}

//���ݼļ�����Ϣ��ѯ���
void Admin::searchPackBySender() const
{
	char input;

	while (1)
	{
		cout << "����1���ļ���ID��ѯ" << endl;
		cout << "����2���ļ���������ѯ" << endl;
		cout << "����3���ļ��˵绰��ѯ" << endl;
		cout << "����4���ļ��˵�ַ��ѯ" << endl;
		cout << "����q�˳�" << endl;
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
			cout << "�������" << endl;
			break;
		}
	}
}

//���ݼļ����û�ID��ѯ���
void Admin::searchPackBySenderID() const
{
	int count = 0;
	int i = 0;
	string input;

	while (1)
	{
		cout << "������ļ���ID" << endl;
		getline(cin, input);

		//�������ID�ĺϷ���
		int flag = 1;
		int len = input.length();
		if (len < 8 || len > 12)
		{
			cout << "���ȴ�����������" << endl;
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
			cout << "���зǷ��ַ�������������" << endl;
			continue;
		}

		cout << "���������" << endl;
		for (i = 0, count = 0; i < packList.size(); i++)
		{
			if (!packList[i].sender.ID.compare(input))
			{
				cout << ++count << endl;
				packList[i].output();
			}
		}

		if (count == 0)
			cout << "����ʧ��" << endl;

		cout << "�Ƿ���������������������������������q" << endl;
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

//���ݼļ���������ѯ���
void Admin::searchPackBySenderName() const
{
	int count = 0;
	int i = 0;
	string input;

	while (1)
	{
		cout << "������ļ�������" << endl;
		getline(cin, input);

		cout << "���������" << endl;
		for (i = 0, count = 0; i < packList.size(); i++)
		{
			if (!packList[i].sender.name.compare(input))
			{
				cout << ++count << endl;
				packList[i].output();
			}
		}

		if (count == 0)
			cout << "����ʧ��" << endl;

		cout << "�Ƿ���������������������������������q" << endl;
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

//���ݼļ��˵绰��ѯ���
void Admin::searchPackBySenderPhone() const
{
	int count = 0;
	int i = 0;
	string input;

	while (1)
	{
		cout << "������ļ��˵绰" << endl;
		getline(cin, input);

		//�������绰�ĺϷ���
		int flag = 1;
		int len = input.length();
		if (len != 11)
		{
			cout << "���ȴ�����������" << endl;
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
			cout << "���зǷ��ַ�������������" << endl;
			continue;
		}

		cout << "���������" << endl;
		for (i = 0, count = 0; i < packList.size(); i++)
		{
			if (!packList[i].sender.phone.compare(input))
			{
				cout << ++count << endl;
				packList[i].output();
			}
		}

		if (count == 0)
			cout << "����ʧ��" << endl;

		cout << "�Ƿ���������������������������������q" << endl;
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

//���ݼļ��˵�ַ��ѯ���
void Admin::searchPackBySenderAddr() const
{
	int count = 0;
	int i = 0;
	string input;

	while (1)
	{
		cout << "������ļ��˵�ַ" << endl;
		getline(cin, input);

		cout << "���������" << endl;
		for (i = 0, count = 0; i < packList.size(); i++)
		{
			if (!packList[i].sender.addr.compare(input))
			{
				cout << ++count << endl;
				packList[i].output();
			}
		}

		if (count == 0)
			cout << "����ʧ��" << endl;

		cout << "�Ƿ���������������������������������q" << endl;
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

//�����ռ�����Ϣ��ѯ���
void Admin::searchPackByRecver() const
{
	char input;

	while (1)
	{
		cout << "����1���ռ���ID��ѯ" << endl;
		cout << "����2���ռ���������ѯ" << endl;
		cout << "����3���ռ��˵绰��ѯ" << endl;
		cout << "����4���ռ��˵�ַ��ѯ" << endl;
		cout << "����q�˳�" << endl;
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
			cout << "�������" << endl;
			break;
		}
	}
}

//�����ռ����û�ID��ѯ���
void Admin::searchPackByRecverID() const
{
	int count = 0;
	int i = 0;
	string input;

	while (1)
	{
		cout << "�������ռ���ID" << endl;
		getline(cin, input);

		//�������ID�ĺϷ���
		int flag = 1;
		int len = input.length();
		if (len < 8 || len > 12)
		{
			cout << "���ȴ�����������" << endl;
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
			cout << "���зǷ��ַ�������������" << endl;
			continue;
		}

		cout << "���������" << endl;
		for (i = 0, count = 0; i < packList.size(); i++)
		{
			if (!packList[i].recver.ID.compare(input))
			{
				cout << ++count << endl;
				packList[i].output();
			}
		}

		if (count == 0)
			cout << "����ʧ��" << endl;

		cout << "�Ƿ���������������������������������q" << endl;
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

//�����ռ���������ѯ���
void Admin::searchPackByRecverName() const
{
	int count = 0;
	int i = 0;
	string input;

	while (1)
	{
		cout << "�������ռ�������" << endl;
		getline(cin, input);

		cout << "���������" << endl;
		for (i = 0, count = 0; i < packList.size(); i++)
		{
			if (!packList[i].recver.name.compare(input))
			{
				cout << ++count << endl;
				packList[i].output();
			}
		}

		if (count == 0)
			cout << "����ʧ��" << endl;

		cout << "�Ƿ���������������������������������q" << endl;
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

//�����ռ��˵绰��ѯ���
void Admin::searchPackByRecverPhone() const
{
	int count = 0;
	int i = 0;
	string input;

	while (1)
	{
		cout << "�������ռ��˵绰" << endl;
		getline(cin, input);

		//�������绰�ĺϷ���
		int flag = 1;
		int len = input.length();
		if (len != 11)
		{
			cout << "���ȴ�����������" << endl;
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
			cout << "���зǷ��ַ�������������" << endl;
			continue;
		}

		cout << "���������" << endl;
		for (i = 0, count = 0; i < packList.size(); i++)
		{
			if (!packList[i].recver.phone.compare(input))
			{
				cout << ++count << endl;
				packList[i].output();
			}
		}

		if (count == 0)
			cout << "����ʧ��" << endl;

		cout << "�Ƿ���������������������������������q" << endl;
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

//�����ռ��˵�ַ��ѯ���
void Admin::searchPackByRecverAddr() const
{
	int count = 0;
	int i = 0;
	string input;

	while (1)
	{
		cout << "�������ռ��˵�ַ" << endl;
		getline(cin, input);

		cout << "���������" << endl;
		for (i = 0, count = 0; i < packList.size(); i++)
		{
			if (!packList[i].recver.addr.compare(input))
			{
				cout << ++count << endl;
				packList[i].output();
			}
		}

		if (count == 0)
			cout << "����ʧ��" << endl;

		cout << "�Ƿ���������������������������������q" << endl;
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

//���ݷ���ʱ���ѯ���
void Admin::searchPackBySendTime() const
{
	int count = 0;
	int i = 0;
	int flag = 0;
	string tmp;
	SYSTEMTIME input;

	//֧���ꡢ�¡��պ�Сʱ���־��Ȳ���
	//����������ݣ��������û�������
	//flag��ʾ���Ҿ��ȣ�0Ϊ��ݣ�1Ϊ�£�2Ϊ�գ�3ΪСʱ
	while (1)
	{
		cout << "�����뷢�����" << endl;
		cin >> input.wYear;
		cin.ignore(LLONG_MAX, '\n');
		cout << "�����뷢���·ݣ���ѡ��" << endl;
		getline(cin, tmp);
		if (tmp.length() != 0)
		{
			flag = 1;
			input.wMonth = atoi(tmp.c_str());
			cout << "�����뷢���գ���ѡ��" << endl;
			getline(cin, tmp);
			if (tmp.length() != 0)
			{
				flag = 2;
				input.wDay = atoi(tmp.c_str());
				cout << "�����뷢��ʱ����ѡ��" << endl;
				getline(cin, tmp);
				if (tmp.length() != 0)
				{
					flag = 3;
					input.wHour = atoi(tmp.c_str());
				}
			}
		}

		cout << "���������" << endl;
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
			cout << "����ʧ��" << endl;

		cout << "�Ƿ���������������������������������q" << endl;
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

//���ݿ�ݺŲ�ѯ���
void Admin::searchPackByPackID() const
{
	int count = 0;
	int i = 0;
	string input;

	while (1)
	{
		cout << "�������ݺ�" << endl;
		getline(cin, input);

		cout << "���������" << endl;
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
			cout << "����ʧ��" << endl;

		cout << "�Ƿ���������������������������������q" << endl;
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

//���ݿ��״̬��ѯ���
void Admin::searchPackByState() const
{
	char input;

	while (1)
	{
		cout << "����1�鿴δ���տ��" << endl;
		cout << "����2�鿴δ���տ��" << endl;
		cout << "����3�鿴�ѽ��տ��" << endl;
		cout << "����q�˳�" << endl;
		input = getchar();
		cin.ignore(LLONG_MAX, '\n');
		switch (input)
		{
		case '1':
			if (!outputUnpickPack())
				cout << "û��δ���տ��" << endl;
			break;
		case '2':
			if (!outputUnrecvPack())
				cout << "û��δ���տ��" << endl;
			break;
		case '3':
			if (!outputRecvPack())
				cout << "û���ѽ��տ��" << endl;
			break;
		case 'q':
			return;
		default:
			cout << "�������" << endl;
			break;
		}
	}
}

//���δ���տ��
int Admin::outputUnrecvPack() const
{
	int count = 0;

	//���δ���յĿ�ݵ���Ϣ
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

//���δ���տ��
int Admin::outputUnpickPack() const
{
	int count = 0;

	//���δ���յĿ�ݵ���Ϣ
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

//����ѽ��տ��
int Admin::outputRecvPack() const
{
	int count = 0;

	//����ѽ��յĿ�ݵ���Ϣ
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

//�鿴���п��Ա
void Admin::checkAllPostman() const
{
	if (!allPostmanList.size())
	{
		cout << "û�п��Ա" << endl;
		return;
	}

	for (int i = 0; i < allPostmanList.size(); i++)
	{
		allPostmanList[i].output();
	}
}

//�������Ա
void Admin::addPostman() const
{
	string ID, name, phone, pw;
	int loc;

	while (1)
	{
		while (1)
		{
			cout << "��������ԱID������Ϊ8-12���ַ���ֻ�ܰ������֡���СдӢ����ĸ�Լ��»���" << endl << "����q�˳�" << endl;
			cin >> ID;
			cin.ignore(LLONG_MAX, '\n');
			if (ID == "q")
				return;
			//�������ID�ĺϷ���
			int flag = 1;
			int len = ID.length();
			if (len < 8 || len > 12)
			{
				cout << "���ȴ�����������" << endl;
				continue;
			}
			for (int i = 0; i < len; i++)
			{
				//�����ΪӢ����ĸ�����ֻ��»��ߣ�����
				if (!isalnum(ID[i]) && ID[i] != '_')
				{
					flag = 0;
					break;
				}
			}
			if (flag == 0)
			{
				cout << "���зǷ��ַ�������������" << endl;
				continue;
			}
			loc = searchPostman(ID);
			//����Ѵ��ڸ��û���
			if (loc >= 0)
			{
				cout << "�ÿ��Ա�û�ID�ѱ�ʹ�ã�����������" << endl;
				continue;
			}
			break;
		}

		while (1)
		{
			cout << "���������룬����ֻ�ܺ��д�Сд��ĸ�����֣�������8-10���ַ�" << endl;
			cin >> pw;
			cin.ignore(LLONG_MAX, '\n');
			//�����������ĺϷ���
			if (!checkPwValid(pw))
			{
				cout << "�����ʽ��������������" << endl;
			}
			else
				break;
		}

		cout << "��������Ա����" << endl;
		getline(cin, name);

		while (1)
		{
			cout << "������绰����" << endl;
			cin >> phone;
			cin.ignore(LLONG_MAX, '\n');
			//�������绰�ĺϷ���
			if (phone.length() != 11)
			{
				cout << "�����������������" << endl;
				continue;
			}
			int flag = 1;
			for (int i = 0; i < phone.length(); i++)
			{
				//����ǲ���ȫ������
				if (!isdigit(phone[i]))
				{
					flag = 0;
					break;
				}
			}
			if (flag == 0)
			{
				cout << "�����������������" << endl;
				continue;
			}
			break;
		}

		Postman postman(ID, pw, name, phone, 0);
		allPostmanList.push_back(postman);
		//д���¿��Ա��Ϣ
		addPostmanToFile();

		cout << "�Ƿ������ӣ������������������������q" << endl;
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

//ɾ�����Ա
void Admin::deletePostman() const
{
	string ID;
	int loc;

	//��������еĿ��Ա��Ϣ
	cout << "���еĿ��Ա��" << endl;
	for (int i = 0; i < allPostmanList.size(); i++)
		allPostmanList[i].output();

	while (1)
	{
		while (1)
		{
			cout << "������Ҫɾ���Ŀ��ԱID" << endl << "����q�˳�" << endl;
			cin >> ID;
			cin.ignore(LLONG_MAX, '\n');
			if (ID == "q")
				return;
			//�������ID�ĺϷ���
			int flag = 1;
			int len = ID.length();
			if (len < 8 || len > 12)
			{
				cout << "���ȴ�����������" << endl;
				continue;
			}
			for (int i = 0; i < len; i++)
			{
				//�����ΪӢ����ĸ�����ֻ��»��ߣ�����
				if (!isalnum(ID[i]) && ID[i] != '_')
				{
					flag = 0;
					break;
				}
			}
			if (flag == 0)
			{
				cout << "���зǷ��ַ�������������" << endl;
				continue;
			}
			loc = searchPostman(ID);
			//��������ڸ��û���
			if (loc < 0)
			{
				cout << "�ÿ��Ա�����ڣ�����������" << endl;
				continue;
			}
			break;
		}

		//��鱻ɾ���Ŀ��Ա�Ƿ���δ��ɵ���������
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
				cout << "�ÿ��Ա��δ��ɵ��������������Ƿ�ɾ����" << endl;
				cout << "����ɾ���밴1����Ҫ����Ϊ��ݷ�����Ա����ɾ���밴2" << endl;
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
					cout << "�����������������" << endl;
			}
		}
		if (flag == 1)
			continue;
		else if (flag == 2)		//�޸ĸÿ��Աδ������͵Ŀ�ݵ���Ϣ
		{
			for (int i = 0; i < packList.size(); i++)
			{
				if (!packList[i].postman.ID.compare(ID) && packList[i].state != RECEIVE)
					packList[i].deletePostman();
			}
		}

		//ɾ���ÿ��Ա
		allPostmanList.erase(allPostmanList.begin() + loc);
		deletePostmanFromFile(ID);

		cout << "�Ƿ����ɾ���������������������������q" << endl;
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

//�鿴IDΪ����id�Ŀ��Ա�Ƿ���ڣ����򷵻���allPostman�������±꣬���򷵻�-1
int Admin::searchPostman(const string& id) const
{
	string s;
	streampos loc = 0;

	//�ļ�Ϊ�գ�ֱ�ӷ���0
	if (!allPostmanList.size())
		return -1;

	//���ҵ�ǰ�����û������о���������
	for (int i = 0; i < allPostmanList.size(); i++)
	{
		if (!allPostmanList[i].ID.compare(id))
			return i;
	}

	//�ļ���û�У�����0
	return -1;
}

//���������������Ƿ���ϸ�ʽҪ�����򷵻�1�����򷵻�0
int Admin::checkPwValid(const string& newPw) const
{
	int flag = 1;
	int len = newPw.length();

	//���볤����8-10���ַ�
	if (len < 8 || len > 10)
		return 0;

	//����ֻ����Ӣ����ĸ�����ֹ���
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

//����Ա��Ϣ�ļ���д���¿��Ա��Ϣ
void Admin::addPostmanToFile() const
{
	int loc = allPostmanList.size() - 1;	//�¿��Ա����Ϣ�ո���ӣ�һ�����������һ��Ԫ��

	//���ļ�ĩβд��
	postmanInfoFile.clear();
	postmanInfoFile.seekg(0, ios::end);
	postmanInfoFile << allPostmanList[loc].ID << SPLIT << allPostmanList[loc].pw << SPLIT << allPostmanList[loc].name << SPLIT << allPostmanList[loc].phone << SPLIT << 0 << endl;
}

//�ӿ��Ա��Ϣ�ļ�ɾ��IDΪ����id�Ŀ��Ա��Ϣ
void Admin::deletePostmanFromFile(const string& id) const
{
	fstream tmpFile;
	string tmp;
	tmpFile.open(".\\data\\tmp.txt", ios::in | ios::out | ios::trunc);
	postmanInfoFile.clear();
	postmanInfoFile.seekg(0);
	//���Ա��Ϣ�ļ��У���ǰ���Ա֮ǰ������д����ʱ�ļ�
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
	//��ǰ���Ա֮�������д����ʱ�ļ�
	postmanInfoFile.ignore(LLONG_MAX, '\n');
	while (!postmanInfoFile.eof())
	{
		getline(postmanInfoFile, tmp);
		if (tmp.length())
			tmpFile << tmp << endl;
	}
	//��ʱ�ļ����ݸ��ǿ��Ա��Ϣ�ļ�
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

//Ϊ��ݷ�����Ա
void Admin::assignPack() const
{
	string packId, postmanId;
	int packLocV, postmanLocV;

	while (1)
	{
		//�鿴�Ƿ���δ������Ա�Ŀ�ݣ��о�һһ���
		if (!checkUnassignPack())
		{
			cout << "û��δ������Ա�Ŀ��" << endl;
			return;
		}

		while (1)
		{
			cout << "�����ݺŽ��з��䣬����q�˳�" << endl;
			cin >> packId;
			cin.ignore(LLONG_MAX, '\n');
			if (packId == "q")
				return;
			//��������ݺŵĺϷ���
			packLocV = -1;
			
			//��ȡ�ÿ�ݺŵĿ����packList���±�
			for (int i = 0; i < packList.size(); i++)
			{
				if (!packList[i].packID.compare(packId))
				{
					packLocV = i;
					break;
				}
			}
			if (packLocV == -1)
				cout << "�����ڿ�ݺ�������һ�µ�δ�����ݣ�����������" << endl;
			else
				break;
		}

		//����Ҫ����Ŀ��Ա
		while (1)
		{
			cout << "��������Ա�û�ID" << endl << "����q�˳�" << endl;
			cin >> postmanId;
			cin.ignore(LLONG_MAX, '\n');
			if (postmanId == "q")
				return;
			//�������ID�ĺϷ���
			postmanLocV = -1;
			int flag = 1;
			int len = postmanId.length();
			if (len < 8 || len > 12)
			{
				cout << "���ȴ�����������" << endl;
				continue;
			}
			for (int i = 0; i < len; i++)
			{
				//�����ΪӢ����ĸ�����ֻ��»��ߣ�����
				if (!isalnum(postmanId[i]) && postmanId[i] != '_')
				{
					flag = 0;
					break;
				}
			}
			if (flag == 0)
			{
				cout << "���зǷ��ַ�������������" << endl;
				continue;
			}
			postmanLocV = searchPostman(postmanId);
			//��������ڸ��û���
			if (postmanLocV < 0)
			{
				cout << "�ÿ��Ա�����ڣ�����������" << endl;
				continue;
			}
			break;
		}

		PackPostmanInfo postmanInfo;
		postmanInfo.ID = allPostmanList[postmanLocV].ID;
		postmanInfo.phone = allPostmanList[postmanLocV].phone;
		postmanInfo.name = allPostmanList[postmanLocV].name;
		packList[packLocV].assignPostman(postmanInfo);		//Ϊ��ݷ�����Ա

		cout << "�Ƿ�������䣬�����������������������q" << endl;
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

//���δ������Ա�Ŀ��
int Admin::checkUnassignPack() const
{
	int count = 0;

	for (int i = 0; i < packList.size(); i++)
	{
		//δ������Ա�Ŀ���У����ԱIDΪ"???"
		if (!packList[i].postman.ID.compare("???"))
		{
			cout << count++ << endl;
			packList[i].output();
		}
	}

	return count;
}

//��ѯ���Ա
void Admin::searchPostman() const
{
	char input;

	while (1)
	{
		cout << "����1�����Ա�û�ID��ѯ" << endl;
		cout << "����2�����Ա������ѯ" << endl;
		cout << "����3�����Ա�绰��ѯ" << endl;
		cout << "����q�˳�" << endl;
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
			cout << "�������" << endl;
			break;
		}
	}
}

//��ID��ѯ���Ա
void Admin::searchPostmanByID() const
{
	int count = 0;
	int i = 0;
	string input;

	while (1)
	{
		cout << "�������û���" << endl;
		getline(cin, input);

		cout << "���������" << endl;
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
			cout << "����ʧ��" << endl;

		cout << "�Ƿ���������������������������������q" << endl;
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

//��������ѯ���Ա
void Admin::searchPostmanByName() const
{
	int count = 0;
	int i = 0;
	string input;

	while (1)
	{
		cout << "�������û�����" << endl;
		getline(cin, input);

		cout << "���������" << endl;
		for (i = 0, count = 0; i < allPostmanList.size(); i++)
		{
			if (!allPostmanList[i].name.compare(input))
			{
				cout << ++count << endl;
				allPostmanList[i].output();
			}
		}

		if (count == 0)
			cout << "����ʧ��" << endl;

		cout << "�Ƿ���������������������������������q" << endl;
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

//�Ե绰��ѯ���Ա
void Admin::searchPostmanByPhone() const
{
	int count = 0;
	int i = 0;
	string input;

	while (1)
	{
		cout << "�������û��绰����" << endl;
		getline(cin, input);

		cout << "���������" << endl;
		for (i = 0, count = 0; i < allPostmanList.size(); i++)
		{
			if (!allPostmanList[i].phone.compare(input))
			{
				cout << ++count << endl;
				allPostmanList[i].output();
			}
		}

		if (count == 0)
			cout << "����ʧ��" << endl;

		cout << "�Ƿ���������������������������������q" << endl;
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

//��������ָ��һ�����Ա�����п��
void Admin::showAllPackAssignedToOne() const
{
	int count = 0;
	int i = 0;
	string input;

	while (1)
	{
		cout << "��������ԱID" << endl;
		getline(cin, input);

		//�������ID�ĺϷ���
		int flag = 1;
		int len = input.length();
		if (len < 8 || len > 12)
		{
			cout << "���ȴ�����������" << endl;
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
			cout << "���зǷ��ַ�������������" << endl;
			continue;
		}

		//������иÿ��Ա�������������
		cout << "���������" << endl;
		for (i = 0, count = 0; i < packList.size(); i++)
		{
			if (!packList[i].postman.ID.compare(input))
			{
				packList[i].output();
				count++;
			}
		}

		if (count == 0)
			cout << "�ÿ��Աû�и����κ���������" << endl;

		cout << "�Ƿ���������������������������������q" << endl;
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

//��ָ����һ�����Ա����Ŀ���в��ҿ��
void Admin::searchPackAssignedToOne() const
{
	int count = 0;
	int i = 0;
	string id;

	//��������ԱID����ѡ����ҷ�ʽ
	while (1)
	{
		cout << "��������ԱID������q�˳�" << endl;
		getline(cin, id);

		if (id == "q")
			return;
		//�������ĺϷ���
		int flag = 1;
		int len = id.length();
		if (len < 8 || len > 12)
		{
			cout << "���ȴ���" << endl;
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
			cout << "���зǷ��ַ�������������" << endl;
			continue;
		}

		//ѡ����ҷ�ʽ
		while (1)
		{
			cout << "����1����ݺŲ�ѯ" << endl;
			cout << "����2���ļ��˲�ѯ" << endl;
			cout << "����3���ռ��˲�ѯ" << endl;
			cout << "����4������ʱ���ѯ" << endl;
			cout << "����5�����״̬��ѯ" << endl;
			cout << "����q�˳�" << endl;
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
				cout << "�������" << endl;
				break;
			}
		}
	}
}

//���ݼļ�����Ϣ��ѯ���
void Admin::searchAssignPackBySender(const string& id) const
{
	char input;

	while (1)
	{
		cout << "����1���ļ���ID��ѯ" << endl;
		cout << "����2���ļ���������ѯ" << endl;
		cout << "����3���ļ��˵绰��ѯ" << endl;
		cout << "����4���ļ��˵�ַ��ѯ" << endl;
		cout << "����q�˳�" << endl;
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
			cout << "�������" << endl;
			break;
		}
	}
}

//���ݼļ����û�ID��ѯ���
void Admin::searchAssignPackBySenderID(const string& id) const
{
	int count = 0;
	int i = 0;
	string input;

	while (1)
	{
		cout << "������ļ���ID" << endl;
		getline(cin, input);
		//�������ID�ĺϷ���
		int flag = 1;
		int len = input.length();
		if (len < 8 || len > 12)
		{
			cout << "���ȴ�����������" << endl;
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
			cout << "���зǷ��ַ�������������" << endl;
			continue;
		}

		cout << "���������" << endl;
		for (i = 0, count = 0; i < packList.size(); i++)
		{
			if (!packList[i].sender.ID.compare(input) && !packList[i].postman.ID.compare(id))
			{
				cout << ++count << endl;
				packList[i].output();
			}
		}

		if (count == 0)
			cout << "����ʧ��" << endl;

		cout << "�Ƿ���������������������������������q" << endl;
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

//���ݼļ���������ѯ���
void Admin::searchAssignPackBySenderName(const string& id) const
{
	int count = 0;
	int i = 0;
	string input;

	while (1)
	{
		cout << "�������ռ�������" << endl;
		getline(cin, input);

		cout << "���������" << endl;
		for (i = 0, count = 0; i < packList.size(); i++)
		{
			if (!packList[i].sender.name.compare(input) && !packList[i].postman.ID.compare(id))
			{
				cout << ++count << endl;
				packList[i].output();
			}
		}

		if (count == 0)
			cout << "����ʧ��" << endl;

		cout << "�Ƿ���������������������������������q" << endl;
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

//���ݼļ��˵绰��ѯ���
void Admin::searchAssignPackBySenderPhone(const string& id) const
{
	int count = 0;
	int i = 0;
	string input;

	while (1)
	{
		cout << "������ļ��˵绰" << endl;
		getline(cin, input);
		//�������ĺϷ���
		int flag = 1;
		int len = input.length();
		if (len != 11)
		{
			cout << "���ȴ�����������" << endl;
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
			cout << "���зǷ��ַ�������������" << endl;
			continue;
		}

		cout << "���������" << endl;
		for (i = 0, count = 0; i < packList.size(); i++)
		{
			if (!packList[i].sender.phone.compare(input) && !packList[i].postman.ID.compare(id))
			{
				cout << ++count << endl;
				packList[i].output();
			}
		}

		if (count == 0)
			cout << "����ʧ��" << endl;

		cout << "�Ƿ���������������������������������q" << endl;
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

//���ݼļ��˵�ַ��ѯ���
void Admin::searchAssignPackBySenderAddr(const string& id) const
{
	int count = 0;
	int i = 0;
	string input;

	while (1)
	{
		cout << "������ļ��˵�ַ" << endl;
		getline(cin, input);

		cout << "���������" << endl;
		for (i = 0, count = 0; i < packList.size(); i++)
		{
			if (!packList[i].sender.addr.compare(input) && !packList[i].postman.ID.compare(id))
			{
				cout << ++count << endl;
				packList[i].output();
			}
		}

		if (count == 0)
			cout << "����ʧ��" << endl;

		cout << "�Ƿ���������������������������������q" << endl;
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

//�����ռ�����Ϣ��ѯ���
void Admin::searchAssignPackByRecver(const string& id) const
{
	char input;

	while (1)
	{
		cout << "����1���ռ���ID��ѯ" << endl;
		cout << "����2���ռ���������ѯ" << endl;
		cout << "����3���ռ��˵绰��ѯ" << endl;
		cout << "����4���ռ��˵�ַ��ѯ" << endl;
		cout << "����q�˳�" << endl;
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
			cout << "�������" << endl;
			break;
		}
	}
}

//�����ռ����û�ID��ѯ���
void Admin::searchAssignPackByRecverID(const string& id) const
{
	int count = 0;
	int i = 0;
	string input;

	while (1)
	{
		cout << "�������ռ���ID" << endl;
		getline(cin, input);
		//�������ID�ĺϷ���
		int flag = 1;
		int len = input.length();
		if (len < 8 || len > 12)
		{
			cout << "���ȴ�����������" << endl;
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
			cout << "���зǷ��ַ�������������" << endl;
			continue;
		}

		cout << "���������" << endl;
		for (i = 0, count = 0; i < packList.size(); i++)
		{
			if (!packList[i].recver.ID.compare(input) && !packList[i].postman.ID.compare(id))
			{
				cout << ++count << endl;
				packList[i].output();
			}
		}

		if (count == 0)
			cout << "����ʧ��" << endl;

		cout << "�Ƿ���������������������������������q" << endl;
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

//�����ռ���������ѯ���
void Admin::searchAssignPackByRecverName(const string& id) const
{
	int count = 0;
	int i = 0;
	string input;

	while (1)
	{
		cout << "�������ռ�������" << endl;
		getline(cin, input);

		cout << "���������" << endl;
		for (i = 0, count = 0; i < packList.size(); i++)
		{
			if (!packList[i].recver.name.compare(input) && !packList[i].postman.ID.compare(id))
			{
				cout << ++count << endl;
				packList[i].output();
			}
		}

		if (count == 0)
			cout << "����ʧ��" << endl;

		cout << "�Ƿ���������������������������������q" << endl;
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

//�����ռ��˵绰��ѯ���
void Admin::searchAssignPackByRecverPhone(const string& id) const
{
	int count = 0;
	int i = 0;
	string input;

	while (1)
	{
		cout << "�������ռ��˵绰" << endl;
		getline(cin, input);
		//�������ĺϷ���
		int flag = 1;
		int len = input.length();
		if (len != 11)
		{
			cout << "���ȴ�����������" << endl;
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
			cout << "���зǷ��ַ�������������" << endl;
			continue;
		}

		cout << "���������" << endl;
		for (i = 0, count = 0; i < packList.size(); i++)
		{
			if (!packList[i].recver.phone.compare(input) && !packList[i].postman.ID.compare(id))
			{
				cout << ++count << endl;
				packList[i].output();
			}
		}

		if (count == 0)
			cout << "����ʧ��" << endl;

		cout << "�Ƿ���������������������������������q" << endl;
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

//�����ռ��˵�ַ��ѯ���
void Admin::searchAssignPackByRecverAddr(const string& id) const
{
	int count = 0;
	int i = 0;
	string input;

	while (1)
	{
		cout << "�������ռ��˵�ַ" << endl;
		getline(cin, input);

		cout << "���������" << endl;
		for (i = 0, count = 0; i < packList.size(); i++)
		{
			if (!packList[i].recver.addr.compare(input) && !packList[i].postman.ID.compare(id))
			{
				cout << ++count << endl;
				packList[i].output();
			}
		}

		if (count == 0)
			cout << "����ʧ��" << endl;

		cout << "�Ƿ���������������������������������q" << endl;
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

//���ݷ���ʱ���ѯ���
void Admin::searchAssignPackBySendTime(const string& id) const
{
	int count = 0;
	int i = 0;
	int flag = 0;
	string tmp;
	SYSTEMTIME input;

	//֧���ꡢ�¡��պ�Сʱ���־��Ȳ���
	//����������ݣ��������û��з�����
	//flag��ʾ���Ҿ��ȣ�0Ϊ��ݣ�1Ϊ�·ݣ�2Ϊ�գ�3ΪСʱ
	while (1)
	{
		cout << "�����뷢�����" << endl;
		cin >> input.wYear;
		cin.ignore(LLONG_MAX, '\n');
		cout << "�����뷢���·ݣ���ѡ��" << endl;
		getline(cin, tmp);
		if (tmp.length() != 0)
		{
			flag = 1;
			input.wMonth = atoi(tmp.c_str());
			cout << "�����뷢���գ���ѡ��" << endl;
			getline(cin, tmp);
			if (tmp.length() != 0)
			{
				flag = 2;
				input.wDay = atoi(tmp.c_str());
				cout << "�����뷢��ʱ����ѡ��" << endl;
				getline(cin, tmp);
				if (tmp.length() != 0)
				{
					flag = 3;
					input.wHour = atoi(tmp.c_str());
				}
			}
		}

		cout << "���������" << endl;
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
			cout << "����ʧ��" << endl;

		cout << "�Ƿ���������������������������������q" << endl;
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

//���ݿ�ݺŲ�ѯ���
void Admin::searchAssignPackByPackID(const string& id) const
{
	int count = 0;
	int i = 0;
	string input;

	while (1)
	{
		cout << "�������ݺ�" << endl;
		getline(cin, input);

		cout << "���������" << endl;
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
			cout << "����ʧ��" << endl;

		cout << "�Ƿ���������������������������������q" << endl;
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

//���ݿ��״̬��ѯ���
void Admin::searchAssignPackByState(const string& id) const
{
	char input;

	while (1)
	{
		cout << "����1�鿴δ���տ��" << endl;
		cout << "����2�鿴δ���տ��" << endl;
		cout << "����3�鿴�ѽ��տ��" << endl;
		cout << "����q�˳�" << endl;
		input = getchar();
		cin.ignore(LLONG_MAX, '\n');
		switch (input)
		{
		case '1':
			if (!outputUnpickAssignPack(id))
				cout << "û��δ���տ��" << endl;
			break;
		case '2':
			if (!outputUnrecvAssignPack(id))
				cout << "û��δ���տ��" << endl;
			break;
		case '3':
			if (!outputRecvAssignPack(id))
				cout << "û���ѽ��տ��" << endl;
			break;
		case 'q':
			return;
		default:
			cout << "�������" << endl;
			break;
		}
	}
}

//���δ���տ��
int Admin::outputUnrecvAssignPack(const string& id) const
{
	int count = 0;

	//���δ���յĿ�ݵ���Ϣ
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

//���δ���տ��
int Admin::outputUnpickAssignPack(const string& id) const
{
	int count = 0;

	//���δ���յĿ�ݵ���Ϣ
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

//����ѽ��տ��
int Admin::outputRecvAssignPack(const string& id) const
{
	int count = 0;

	//����ѽ��յĿ�ݵ���Ϣ
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
