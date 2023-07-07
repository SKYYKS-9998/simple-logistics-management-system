#include "head1.h"

extern vector<Pack> packList;
extern fstream userInfoFile;
extern fstream packInfoFile;

User::User()
	: balance(0)
{}

//����ע�����ļ���ȡ����Ĺ��캯��
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

//ע��
void User::regis()
{
	/*
	* ���������û�ID����������ظ�
	* û���ظ��������������û���Ϣ
	*/

	string tmp;
	while (1)
	{
		cout << "�����������û�ID������Ϊ8-12���ַ���ֻ�ܰ������֡���СдӢ����ĸ�Լ��»���" << endl << "����q�˳�" << endl;
		cin >> tmp;
		cin.ignore(LLONG_MAX, '\n');
		if (tmp == "q")
			return;
		int flag = 1;
		int len = tmp.length();
		if (len < 8 || len > 12)
		{
			cout << "���ȴ�����������" << endl;
			continue;
		}
		for (int i = 0; i < len; i++)
		{
			//�����ΪӢ����ĸ�����ֻ��»��ߣ�����
			if (!isalnum(tmp[i]) && tmp[i] != '_')
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
		//����Ѵ��ڸ��û�����ѯ���Ƿ��¼
		if (!(searchID(tmp) < 0))
		{
			do
			{
				cout << "���û�ID�ѱ�ʹ�ã��Ƿ���е�¼��" << endl;
				cout << "����1���е�¼����������2" << endl;
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
					cout << "�����������������" << endl;
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
		cout << "���������룬����ֻ�ܺ��д�Сд��ĸ�����֣�������8-10���ַ�" << endl;
		cin >> tmp;
		cin.ignore(LLONG_MAX, '\n');
		if (!checkPwValid(tmp))
		{
			cout << "�����ʽ��������������" << endl;
		}
		else
			break;
	}
	pw = tmp;

	cout << "����������������Ϊ�ļ���/�ռ�����ʾ��" << endl;
	getline(cin, name);

	while (1)
	{
		cout << "������绰����" << endl;
		cin >> tmp;
		cin.ignore(LLONG_MAX, '\n');
		if (tmp.length() != 11)
		{
			cout << "�����������������" << endl;
			continue;
		}
		int flag = 1;
		for (int i = 0; i < tmp.length(); i++)
		{
			//����ǲ���ȫ������
			if (!isdigit(tmp[i]))
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
	phone = tmp;

	cout << "�������ַ" << endl;
	getline(cin, addr);

	//д�����û���Ϣ
	writeToFile(0);
	//ע����ֱ�ӵ�¼�������û�����
	userInterface();
}

//��¼
void User::login()
{
	/*
	* �����û�ID�����������¼��Ȼ������û���Ϣ
	*/

	string input;

	while (1)
	{
		cout << "�������û�ID������q�˳�" << endl;
		cin >> input;
		cin.ignore(LLONG_MAX, '\n');
		if (input == "q")
			return;
		int flag = 1;
		int len = input.length();
		//�������ID�ĺϷ��ԣ����Ⱥ��ַ����ͣ�
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
		ID = input;
		//��������ID�Ƿ���ڣ���������ѯ���Ƿ�ע��
		streampos loc = searchID(ID);
		if (loc < 0)
		{
			cout << "���û�ID�����ڣ��Ƿ����ע��" << endl;
			cout << "����1ע�ᣬ����q��������" << endl;
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
		//���ļ���ȡ���û���������
		userInfoFile.clear();
		userInfoFile.seekg(loc);
		userInfoFile.ignore(LLONG_MAX, SPLIT);
		getline(userInfoFile, pw, SPLIT);
		while (1)
		{
			cout << "���������룬����q���������û�ID" << endl;
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

	//��ȡ������Ϣ
	getline(userInfoFile, name, SPLIT);
	getline(userInfoFile, phone, SPLIT);
	getline(userInfoFile, addr, SPLIT);
	userInfoFile >> balance;
	//��¼��ɣ������û�����
	userInterface();
}

//����û���Ϣ
void User::output() const
{
	cout << "�û���Ϣ" << endl << "�û�ID��" << ID << endl;
	cout << "�û�������" << name << endl;
	cout << "�û��绰��" << phone << endl;
	cout << "�û���ַ��" << addr << endl;
	cout << "�˻���" << balance << endl;
}

//��ȡ����û���صĿ�ݣ���������packList��
void User::loadUserPacklist()
{
	packList.clear();
	packInfoFile.clear();
	packInfoFile.seekg(0);
	string s;

	while (!packInfoFile.eof())
	{
		string packID, packInfo;
		PackUserInfo sender, recver;
		SYSTEMTIME sendTime, recvTime;
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
		getline(packInfoFile, packInfo);

		//���ռ�������͵�ǰ�û��޹أ��û�û��Ȩ�޲鿴������packList�洢
		if (recver.ID.compare(ID) && sender.ID.compare(ID))
			continue;

		Pack p(packID, state, sendTime, recvTime, sender, recver, packInfo);
		packList.push_back(p);
	}
}

//�û�����
void User::userInterface()
{
	char input;

	loadUserPacklist();
	do {
		cout << "����1�����û�������Ϣ" << endl;
		cout << "����2ʹ����������" << endl;
		cout << "����q�ǳ�" << endl;
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
			cout << "�������" << endl;
			break;
		}
	} while (1);
}

//�û�������Ϣ�������
void User::userInfoInterface()
{
	char input;

	do {
		cout << "����1��������" << endl;
		cout << "����2��ѯ���" << endl;
		cout << "����3��ֵ" << endl;
		cout << "����q�˳�" << endl;
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
			cout << "�����������������" << endl;
			break;
		}
	} while (1);
}

//�û�������Ϣ�������
void User::userPackInterface()
{
	char input;

	do {
		cout << "����1���Ϳ��" << endl;
		cout << "����2���տ��" << endl;
		cout << "����3��ѯ���" << endl;
		cout << "����q�˳�" << endl;
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
			cout << "�����������������" << endl;
			break;
		}
	} while (1);
}

//��������
void User::changePW()
{
	//����������룬������ȷ��������������и���

	string input;

	while (1)
	{
		cout << "����������룬��������q�˳�" << endl;
		cin >> input;
		cin.ignore(LLONG_MAX, '\n');
		if (input == "q")
			return;
		else if (!input.compare(pw))
		{
			cout << "������ȷ" << endl;
			break;
		}
		else
		{
			cout << "�����������������" << endl;
			continue;
		}
	}

	while (1)
	{
		cout << "���������룬����ֻ�ܺ��д�Сд��ĸ�����֣�������8-10���ַ�" << endl;
		cin >> input;
		cin.ignore(LLONG_MAX, '\n');
		if (checkPwValid(input))
		{
			cout << "���ĳɹ�" << endl;
			break;
		}
		else
			cout << "���зǷ��ַ�������������" << endl;
	}

	pw = input;
	//��ʱ���ļ��и�������
	writeToFile(1);
}

//�鿴���
void User::checkBalance() const
{
	cout << "���Ϊ" << balance << "Ԫ" << endl;
}

//��ֵ
void User::topUp()
{
	//���������룬������ȷ�������ֵ���

	int value;
	string input;

	while (1)
	{
		cout << "���������룬��������q�˳�" << endl;
		cin >> input;
		cin.ignore(LLONG_MAX, '\n');
		if (input == "q")
			return;
		else if (!input.compare(pw))
		{
			cout << "������ȷ" << endl;
			break;
		}
		else
		{
			cout << "�����������������" << endl;
			continue;
		}
	}

	while (1)
	{
		cout << "�������ֵ��������" << endl;
		//��������Ƿ�Ϸ�
		//���ȡ�ַ��򸡵������ǲ��Ϸ���
		if (!(cin >> value) || cin.peek() == '.')
		{
			//���Ϸ������״̬����������������������
			cin.clear();
			cin.ignore(LLONG_MAX, '\n');
			cout << "�����������������" << endl;
			continue;
		}
		cin.ignore(LLONG_MAX, '\n');
		balance += value;
		cout << "��ֵ�ɹ�" << endl;
		break;
	}

	//��ʱ���ļ��и������
	writeToFile(1);
}

//�ڵ�¼ʱʹ�ò������û�IDʱʹ�ã�ע��id
void User::regis(const string& id)
{
	string tmp;

	while (1)
	{
		cout << "���������룬����ֻ�ܺ��д�Сд��ĸ�����֣�������8-10���ַ�" << endl;
		cout << "����q�˳�" << endl;
		cin >> tmp;
		cin.ignore(LLONG_MAX, '\n');
		if (tmp == "q")
			return;
		if (!checkPwValid(tmp))
		{
			cout << "�����ʽ��������������" << endl;
		}
		else
			break;
	}
	pw = tmp;

	cout << "����������������Ϊ�ļ���/�ռ�����ʾ��" << endl;
	getline(cin, name);

	while (1)
	{
		cout << "������绰����" << endl;
		cin >> tmp;
		cin.ignore(LLONG_MAX, '\n');
		if (tmp.length() != 11)
		{
			cout << "�����������������" << endl;
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
			cout << "�����������������" << endl;
			continue;
		}
		break;
	}
	phone = tmp;

	cout << "�������ַ" << endl;
	getline(cin, addr);

	//�����ļ��е��û���Ϣ
	writeToFile(0);
	//ע����ϣ������û�����
	userInterface();
}

//��ע��ʱʹ���Ѵ����û�IDʱʹ�ã���¼id
void User::login(const string& id)
{
	string input;
	ID = id;

	//���ļ���Ѱ�ҵ�ǰ�û�id���û���Ϣ��λ�ã�����ȡ���룬�ж������Ƿ���ȷ
	int flag = 1;
	streampos loc = searchID(ID);
	userInfoFile.clear();
	userInfoFile.seekg(loc);
	userInfoFile.ignore(LLONG_MAX, SPLIT);
	getline(userInfoFile, pw, SPLIT);
	while (1)
	{
		cout << "���������룬����q���������û�ID" << endl;
		cin >> input;
		cin.ignore(LLONG_MAX, '\n');
		if (!input.compare(pw))
		{
			cout << "��¼�ɹ�" << endl;
			break;
		}
		else if (input == "q")
		{
			login();
			return;
		}
		else
		{
			cout << "�����������������" << endl;
			continue;
		}
	}

	//���ļ�����������Ϣ
	getline(userInfoFile, name, SPLIT);
	getline(userInfoFile, phone, SPLIT);
	getline(userInfoFile, addr, SPLIT);
	userInfoFile >> balance;
	userInterface();
}

//���newPw�Ƿ���������ʽҪ�󣬷����򷵻�1�����򷵻�0
int User::checkPwValid(const string& newPw) const
{
	//���볤�Ȳ���8-10���ַ�֮�䣬���ߴ��ڲ���Ӣ����ĸ�����ֵ��ַ�����Ϊ����
	int flag = 1;
	int len = newPw.length();

	if (len < 8 || len > 10)
		return 0;

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

//���Ϳ��
void User::sendPack()
{
	PackUserInfo sender, recver;
	string input, pID;
	streampos loc;
	SYSTEMTIME now;

	while (1)
	{
		//�ȼ���û����
		if (balance < 15)
		{
			cout << "����" << endl;
			return;
		}

		cout << "�������ռ���ID������q�˳�" << endl;
		cin >> input;
		cin.ignore(LLONG_MAX, '\n');
		if (input == "q")
			return;
		//�������ID�ĺϷ��Ժ��Ƿ�Ϊ�Ѵ����û�
		int flag = 1;
		int len = input.length();
		if (len < 8 || len > 12)
		{
			cout << "���ȴ�����������" << endl;
			continue;
		}
		for (int i = 0; i < len; i++)
		{
			//���������û�ID�Ƿ��Ϊ�Ϸ��ַ�
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
		loc = searchID(input);
		if (loc == -1)
		{
			cout << "�����ڸ��û�������������" << endl;
			continue;
		}
		//���ط��Ϳ������ļļ��˺��ռ�����Ϣ
		sender.ID = this->ID;
		recver.ID = input;
		loadPacketUserInfo(sender);
		loadPacketUserInfo(loc, recver);

		cout << "������������" << endl;
		getline(cin, input);

		//��ȡ��ǰϵͳʱ��
		GetSystemTime(&now);
		//���ɿ�ݺţ���ʽΪ�ļ���ID���ϵ�ǰʱ�䣨�ȷ���ʱ�����磩��������ʱ����
		pID = sender.ID + to_string(now.wYear) + to_string(now.wMonth) + to_string(now.wDay) + to_string(now.wHour) + to_string(now.wMinute) + to_string(now.wSecond);

		//���Ϳ��
		Pack pack(pID, now, sender, recver, input);
		pack.sendPack();
		balance -= 15;		//�۳��˷�
		writeToFile(1);		//��ʱ�����ļ�����
		cout << "���ͳɹ�" << endl;

		cout << "�Ƿ�������ͣ������������������������q" << endl;
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

//���տ��
void User::recvPack() const
{
	string input;
	int locV;

	while (1)
	{
		//û��δ���տ��ֱ�ӷ��أ��о������ݺŽ��н���
		cout << "δ���տ�ݣ�" << endl;
		if (!checkUnrecvPack())
		{
			cout << "û��δ���տ��" << endl;
			return;
		}

		while (1)
		{
			cout << "�����ݺŽ��н��գ�����q�˳�" << endl;
			cin >> input;
			cin.ignore(LLONG_MAX, '\n');
			if (input == "q")
				return;
			locV = -1;
			
			for (int i = 0; i < packList.size(); i++)
			{
				if (!packList[i].packID.compare(input))
				{
					locV = i;
					break;
				}
			}
			if (locV == -1)
				cout << "������Ϊ�ÿ�ݺŵ�δ���տ�ݣ�����������" << endl;
			else
				break;
		}

		//���տ��
		packList[locV].recvPack();
		cout << "���ճɹ�" << endl;

		cout << "�Ƿ�������գ������������������������q" << endl;
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

//��ѯ���
void User::searchPack() const
{
	char input;

	while (1)
	{
		cout << "����1�ÿ�ݺŲ�ѯ" << endl;
		cout << "����2�üļ�����Ϣ��ѯ" << endl;
		cout << "����3���ռ�����Ϣ��ѯ" << endl;
		cout << "����4�÷���ʱ���ѯ" << endl;
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
		case 'q':
			return;
		default:
			cout << "�������" << endl;
			break;
		}
	}
}

//���ؼļ�����Ϣ
void User::loadPacketUserInfo(PackUserInfo& sender) const
{
	sender.name = name;
	sender.phone = phone;
	sender.addr = addr;
}

//�����ռ�����Ϣ��streampos loc�Ǽļ�����Ϣ���û���Ϣ�ļ���λ��
void User::loadPacketUserInfo(const streampos& loc, PackUserInfo& recver) const
{
	userInfoFile.clear();
	userInfoFile.seekg(loc);

	//�����ռ����û�ID������ǰ�ѵõ���������
	userInfoFile.ignore(LLONG_MAX, SPLIT);
	userInfoFile.ignore(LLONG_MAX, SPLIT);
	getline(userInfoFile, recver.name, SPLIT);
	getline(userInfoFile, recver.phone, SPLIT);
	getline(userInfoFile, recver.addr, SPLIT);
}

//���ݼļ�����Ϣ��ѯ���
void User::searchPackBySender() const
{
	char input;

	while (1)
	{
		cout << "����1�üļ���ID��ѯ" << endl;
		cout << "����2�üļ���������ѯ" << endl;
		cout << "����3�üļ��˵绰��ѯ" << endl;
		cout << "����4�üļ��˵�ַ��ѯ" << endl;
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
void User::searchPackBySenderID() const
{
	int count = 0;
	int i = 0;
	string input;

	while (1)
	{
		cout << "������ļ���ID" << endl;
		getline(cin, input);

		//�������ID�Ϸ���
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

		//���Ҳ�������������Ŀ����Ϣ
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
			cout << "û�з��������Ľ��" << endl;

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
void User::searchPackBySenderName() const
{
	int count = 0;
	int i = 0;
	string input;

	while (1)
	{
		cout << "������ļ�������" << endl;
		getline(cin, input);

		//���Ҳ�������������Ŀ����Ϣ
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
			cout << "û�з��������Ľ��" << endl;

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
void User::searchPackBySenderPhone() const
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

		//���Ҳ�������������Ŀ����Ϣ
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
			cout << "û�з��������Ľ��" << endl;

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
void User::searchPackBySenderAddr() const
{
	int count = 0;
	int i = 0;
	string input;

	while (1)
	{
		cout << "������ļ��˵�ַ" << endl;
		getline(cin, input);

		//���Ҳ�������������Ŀ����Ϣ
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
			cout << "û�з��������Ľ��" << endl;

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
void User::searchPackByRecver() const
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
void User::searchPackByRecverID() const
{
	int count = 0;
	int i = 0;
	string input;

	while (1)
	{
		cout << "�������ռ���ID" << endl;
		getline(cin, input);

		//�������Ϸ���
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

		//���Ҳ�������������Ŀ����Ϣ
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
			cout << "û�з��������Ľ��" << endl;

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
void User::searchPackByRecverName() const
{
	int count = 0;
	int i = 0;
	string input;

	while (1)
	{
		cout << "�������ռ�������" << endl;
		getline(cin, input);

		//���Ҳ�������������Ŀ����Ϣ
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
			cout << "û�з��������Ľ��" << endl;

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
void User::searchPackByRecverPhone() const
{
	int count = 0;
	int i = 0;
	string input;

	while (1)
	{
		cout << "�������ռ��˵绰" << endl;
		getline(cin, input);

		//�������Ϸ���
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

		//���Ҳ�������������Ŀ����Ϣ
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
			cout << "û�з��������Ľ��" << endl;

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
void User::searchPackByRecverAddr() const
{
	int count = 0;
	int i = 0;
	string input;

	while (1)
	{
		cout << "�������ռ��˵�ַ" << endl;
		getline(cin, input);

		//���Ҳ�������������Ŀ����Ϣ
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
			cout << "û�з��������Ľ��" << endl;

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
void User::searchPackBySendTime() const
{
	int count = 0;
	int i = 0;
	int flag = 0;
	string tmp;
	SYSTEMTIME input;

	//����ķ���ʱ��֧�ֲ�ͬ�ꡢ�¡��ա�Сʱ�ĸ����Ҿ���
	//����������ݣ��������û�������
	//int flag ��ʾ���Ҿ��ȣ�0Ϊ�꣬1Ϊ�£�2Ϊ�գ�3ΪСʱ
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

		//���Ҳ�������������Ŀ����Ϣ
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
			cout << "û�з��������Ľ��" << endl;

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
void User::searchPackByPackID() const
{
	int count = 0;
	int i = 0;
	string input;

	while (1)
	{
		cout << "�������ݺ�" << endl;
		getline(cin, input);

		//���Ҳ�������������Ŀ����Ϣ
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
			cout << "�����ڴ˿�ݺ�" << endl;

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

//���δ���տ��
int User::checkUnrecvPack() const
{
	int count = 0;

	//���δ���յĿ�ݵ���Ϣ
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

//���ļ��в��Ҹö���id���û���Ϣ���������ļ��е�λ�ã��������򷵻�-1
streampos User::searchID() const
{
	string s;
	streampos loc = 0;
	userInfoFile.clear();
	userInfoFile.seekg(0);

	//�ļ�Ϊ�գ�ֱ�ӷ���0
	if (userInfoFile.eof())
		return loc;

	//���ҵ�ǰ�����û�ID���о���������λ��
	while (!userInfoFile.eof())
	{
		loc = userInfoFile.tellg();
		getline(userInfoFile, s, SPLIT);
		if (!s.compare(ID))
			return loc;
		userInfoFile.ignore(LLONG_MAX, '\n');
	}

	//�ļ���û�У�����-1
	return -1;
}

//���ļ��в����û�IDΪid���û���Ϣ���������ļ��е�λ�ã��������򷵻�-1
streampos User::searchID(const string& id) const
{
	string s;
	streampos loc = 0;
	userInfoFile.clear();
	userInfoFile.seekg(0);

	//�ļ�Ϊ�գ�ֱ�ӷ���0
	if (userInfoFile.eof())
		return loc;

	//���ļ��в���id���о���������λ��
	while (!userInfoFile.eof())
	{
		loc = userInfoFile.tellg();
		getline(userInfoFile, s, SPLIT);
		if (!s.compare(id))
			return loc;
		userInfoFile.ignore(LLONG_MAX, '\n');
	}

	//�Ҳ���������-1
	return -1;
}

/*
* ���û���Ϣд���ļ�
* mode = 0, ���û�
* mode = 1, ���Ĳ���
*/
void User::writeToFile(int mode) const
{
	//д�����û���Ϣ
	if (!mode)
	{
		//ֱ���ƶ����ļ�ĩβ���
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

	//���û���Ϣ�ļ��У���ǰ�û���Ϣ֮ǰ������д����ʱ�ļ�
	while (userInfoFile.tellg() != loc)
	{
		getline(userInfoFile, tmp);
		//���в�д
		if (tmp.length())
			tmpFile << tmp << endl;
	}
	//��ǰ�û���Ϣ�����º�д����ʱ�ļ�
	tmpFile << ID << SPLIT << pw << SPLIT << name << SPLIT << phone << SPLIT << addr << SPLIT << balance << endl;
	userInfoFile.ignore(LLONG_MAX, '\n');
	//���û���Ϣ�ļ��У���ǰ�û���Ϣ֮�������д����ʱ�ļ�
	while (!userInfoFile.eof())
	{
		getline(userInfoFile, tmp);
		//���в�д
		if (tmp.length())
			tmpFile << tmp << endl;
	}
	tmpFile.clear();
	tmpFile.seekg(0);
	//ˢ���û���Ϣ�ļ�������ʱ�ļ���д��������
	userInfoFile.close();
	userInfoFile.open(".\\data\\userInfo.txt", ios::in | ios::out | ios::trunc);
	while (!tmpFile.eof())
	{
		getline(tmpFile, tmp);
		//���в�д
		if (tmp.length())
			userInfoFile << tmp << endl;
	}

	tmpFile.close();
}
