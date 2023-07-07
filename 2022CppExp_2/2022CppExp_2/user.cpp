#include "head2.h"

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
		//�������ĺϷ���
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
		//����Ѵ��ڸ��û�����ѯ���û��Ƿ��ø��û����е�¼������Ҫ����������
		if (!(searchID(tmp) < 0))
		{
			do
			{
				cout << "�û����ѱ�ʹ�ã��Ƿ���е�¼��" << endl;
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
		//�������ĺϷ���
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
		//�������ĺϷ���
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
	//�����û�����
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
		cout << "�������û���������q�˳�" << endl;
		cin >> input;
		cin.ignore(LLONG_MAX, '\n');
		if (input == "q")
			return;
		//�������ĺϷ���
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
		ID = input;
		//��������ID�Ƿ���ڣ���������ѯ���û��Ƿ����������ע��
		streampos loc = searchID(ID);
		if (loc < 0)
		{
			cout << "�û��������ڣ��Ƿ����ע��" << endl;
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
		//���ļ���ȡ���û�ID������
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
	//�û�����
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

//���ļ��ж�ȡ�뵱ǰ�û���صĿ�ݣ���������packList��
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
		//�ѽ��յĿ�ݲ��н���ʱ��
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
		//���ռ�������͵�ǰ�û��޹أ��û�û��Ȩ�޲鿴������packList�洢
		if (recver.ID.compare(ID) && sender.ID.compare(ID))
		{
			packInfoFile.ignore(LLONG_MAX, '\n');
			continue;
		}
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

//�û�����
void User::userInterface()
{
	char input;

	loadUserPackList();
	do {
		cout << "����1�����û���Ϣ" << endl;
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

//�û����������Ϣ����
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

//�û�����������Ϣ����
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
	string input;

	while (1)
	{
		cout << "����������룬��������q�˳�" << endl;
		cin >> input;
		cin.ignore(LLONG_MAX, '\n');
		if (input == "q")
			return;
		//�������ĺϷ���
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
		//�������ĺϷ���
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
		//��������Ƿ�Ϸ����������ַ��򸡵����Ͳ��Ϸ�
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

//���û���¼ʱʹ�ò������û���ʱʹ�ã�ע�������ID
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
		//�������ĺϷ���
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
		//�������ĺϷ���
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

	//�����û���Ϣд���ļ�
	writeToFile(0);
	//�û�����
	userInterface();
}

//���û�ע��ʱʹ���Ѵ����û���ʱʹ�ã���¼�����ID
void User::login(const string& id)
{
	string input;
	ID = id;

	int flag = 1;
	//���ļ��л�ȡ����
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

	//����������Ϣ
	getline(userInfoFile, name, SPLIT);
	getline(userInfoFile, phone, SPLIT);
	getline(userInfoFile, addr, SPLIT);
	userInfoFile >> balance;

	//�����û�����
	userInterface();
}

//���newPw�Ƿ���������ʽҪ�����򷵻�1�����򷵻�0
int User::checkPwValid(const string& newPw) const
{
	int flag = 1;
	int len = newPw.length();

	//���볤��ֻ��Ϊ8-10���ַ�
	if (len < 8 || len > 10)
		return 0;

	//����ֻ�����������
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
	int type, amount;
	SYSTEMTIME now;
	Pack* pack;

	while (1)
	{
		cout << "�������ռ���ID������q�˳�" << endl;
		cin >> input;
		cin.ignore(LLONG_MAX, '\n');
		if (input == "q")
			return;
		//�������ĺϷ���
		int flag = 1;
		int len = input.length();
		if (len < 8 || len > 12)
		{
			cout << "���ȴ�����������" << endl;
			continue;
		}
		for (int i = 0; i < len; i++)
		{
			//���������û����Ƿ��Ϊ�Ϸ��ַ�
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
		//����Ƿ���ڸ��û�
		loc = searchID(input);
		if (loc == -1)
		{
			cout << "�����ڸ��û�������������" << endl;
			continue;
		}
		sender.ID = this->ID;
		recver.ID = input;
		//��������ļļ��˺��ռ�����Ϣ
		loadPacketUserInfo(sender);
		loadPacketUserInfo(loc, recver);

		while (1)
		{
			cout << "��ѡ�������ͣ�0Ϊ��ͨ��ݣ�1Ϊ����Ʒ��2Ϊ�鼮" << endl;
			char input = getchar();
			cin.ignore(LLONG_MAX, '\n');
			int flag = 1;
			switch (input)
			{
			case '0':
				type = NORMAL;
				while (1)
				{
					cout << "����������������������5Ԫ/kg������1kg��1kg����" << endl;
					if (!(cin >> amount) || cin.peek() == '.')
					{
						//���Ϸ������״̬����������������������
						cin.clear();
						cin.ignore(LLONG_MAX, '\n');
						cout << "�����������������" << endl;
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
					cout << "����������������������8Ԫ/kg������1kg��1kg����" << endl;
					if (!(cin >> amount) || cin.peek() == '.')
					{
						//���Ϸ������״̬����������������������
						cin.clear();
						cin.ignore(LLONG_MAX, '\n');
						cout << "�����������������" << endl;
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
					cout << "���������鼮��������������2Ԫ/��" << endl;
					if (!(cin >> amount) || cin.peek() == '.')
					{
						//���Ϸ������״̬����������������������
						cin.clear();
						cin.ignore(LLONG_MAX, '\n');
						cout << "�����������������" << endl;
						continue;
					}
					cin.ignore(LLONG_MAX, '\n');
					break;
				}
				break;
			default:
				flag = 0;
				cout << "�����������������" << endl;
				break;
			}
			if (flag)
				break;
		}

		cout << "������������" << endl;
		getline(cin, input);

		//��ȡ��ǰʱ�䣬��ļ���ID��ɿ�ݺ�
		GetSystemTime(&now);
		pID = sender.ID + to_string(now.wYear) + to_string(now.wMonth) + to_string(now.wDay) + to_string(now.wHour) + to_string(now.wMinute) + to_string(now.wSecond);

		//���ݿ�����ͣ�Ϊpack������Ӧ����Ŀռ�
		if (type == NORMAL)
			pack = new Pack(pID, now, sender, recver, type, amount, input);
		else if(type == FRAGILE)
			pack = new Fragile(pID, now, sender, recver, type, amount, input);
		else 
			pack = new Book(pID, now, sender, recver, type, amount, input);

		pack->getPrice();	//�����˷�
		//�������Ƿ��㹻���㹻ֱ�ӿۣ������͸�����ʾ��ȡ������
		if (balance >= pack->price)
			balance -= pack->price;
		else
		{
			cout << "����" << endl;
			cout << "���Ϊ" << balance << "Ԫ" << endl;
			continue;
		}
		//��ʱ�����ļ�����
		writeToFile(1);
		pack->sendPack();	//���Ϳ��
		cout << "���ͳɹ�" << endl;
		delete pack;

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
		if (!checkUnrecvPackIn())
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
			//�������ĺϷ���
			locV = -1;
			
			//��packList�в��ҿ��
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

		packList[locV].recvPack();	//���տ��
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
		cout << "����2�üļ��˲�ѯ" << endl;
		cout << "����3���ռ��˲�ѯ" << endl;
		cout << "����4�÷���ʱ���ѯ" << endl;
		cout << "����5�ÿ��״̬��ѯ" << endl;
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

//���ؼļ�����Ϣ
void User::loadPacketUserInfo(PackUserInfo& sender) const
{
	sender.name = name;
	sender.phone = phone;
	sender.addr = addr;
}

//�����ռ�����Ϣ������loc���ռ��˵���Ϣ���û���Ϣ�ļ���λ��
void User::loadPacketUserInfo(streampos loc, PackUserInfo& recver) const
{
	userInfoFile.clear();
	userInfoFile.seekg(loc);

	//�����ռ����û���������
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
		//�������ĺϷ���
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
void User::searchPackBySenderName() const
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
void User::searchPackBySenderPhone() const
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
void User::searchPackBySenderAddr() const
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
		//�������ĺϷ���
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
void User::searchPackByRecverName() const
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
void User::searchPackByRecverPhone() const
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
void User::searchPackByRecverAddr() const
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
void User::searchPackBySendTime() const
{
	int count = 0;
	int i = 0;
	int flag = 0;
	string tmp;
	SYSTEMTIME input;

	//����ķ���ʱ��֧�ֲ�ͬ�ꡢ�¡��պ�Сʱ���ֲ��Ҿ���
	//����Ҫ������ݣ��������û�������
	//flag ��ʾ���Ҿ��ȣ�0Ϊ��ݣ�1Ϊ�·ݣ�2Ϊ�գ�3ΪСʱ
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
void User::searchPackByPackID() const
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
void User::searchPackByState() const
{
	char input;

	while (1)
	{
		cout << "����1�鿴���˼ĳ���δ���տ��" << endl;
		cout << "����2�鿴���˼ĳ���δ���տ��" << endl;
		cout << "����3�鿴���˼ĳ����ѽ��տ��" << endl;
		cout << "����4�鿴�ĸ����˵�δ���տ��" << endl;
		cout << "����5�鿴�ĸ����˵�δ���տ��" << endl;
		cout << "����6�鿴�ĸ����˵��ѽ��ܿ��" << endl;
		cout << "����q�˳�" << endl;
		input = getchar();
		cin.ignore(LLONG_MAX, '\n');
		switch (input)
		{
		case '1':
			if (!checkUnpickPackOut())
				cout << "û�б��˼ĳ���δ���տ��" << endl;
			break;
		case '2':
			if (!checkUnrecvPackOut())
				cout << "û�б��˼ĳ���δ���տ��" << endl;
			break;
		case '3':
			if (!checkRecvPackOut())
				cout << "û�б��˼ĳ����ѽ��տ��" << endl;
			break;
		case '4':
			if (!checkUnpickPackIn())
				cout << "û�мĸ����˵�δ���տ��" << endl;
			break;
		case '5':
			if (!checkUnrecvPackIn())
				cout << "û�мĸ����˵�δ���տ��" << endl;
			break;
		case '6':
			if (!checkRecvPackIn())
				cout << "û�мĸ����˵��ѽ��տ��" << endl;
			break;
		case 'q':
			return;
		default:
			cout << "�������" << endl;
			break;
		}
	}
}

//����ĸ���ǰ�û���δ���տ��
int User::checkUnrecvPackIn() const
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

//����ĸ���ǰ�û���δ���տ��
int User::checkUnpickPackIn() const
{
	int count = 0;

	//���δ���յĿ�ݵ���Ϣ
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

//����ĸ���ǰ�û����ѽ��տ��
int User::checkRecvPackIn() const
{
	int count = 0;

	//����ѽ��յĿ�ݵ���Ϣ
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

//�����ǰ�û��ĳ���δ���տ��
int User::checkUnrecvPackOut() const
{
	int count = 0;

	//���δ���յĿ�ݵ���Ϣ
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

//�����ǰ�û��ĳ���δ���տ��
int User::checkUnpickPackOut() const
{
	int count = 0;

	//���δ���յĿ�ݵ���Ϣ
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

//�����ǰ�û��ĳ����ѽ��տ��
int User::checkRecvPackOut() const
{
	int count = 0;

	//����ѽ��յĿ�ݵ���Ϣ
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

//���ص�ǰ������û���Ϣ���ļ���λ��
streampos User::searchID() const
{
	string s;
	streampos loc = 0;
	userInfoFile.clear();
	userInfoFile.seekg(0);

	//�ļ�Ϊ�գ�ֱ�ӷ���0
	if (userInfoFile.eof())
		return loc;

	//���ҵ�ǰ�����û������о���������
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

//�����û�IDΪid���û���Ϣ���ļ��е�λ��
streampos User::searchID(const string& id) const
{
	string s;
	streampos loc = 0;
	userInfoFile.clear();
	userInfoFile.seekg(0);

	//�ļ�Ϊ�գ�ֱ�ӷ���0
	if (userInfoFile.eof())
		return loc;

	//���ļ��в���id
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
* mode = 0, ���û�
* mode = 1, ���Ĳ���
* ���û���Ϣд���ļ�
*/
void User::writeToFile(int mode) const
{
	if (!mode)
	{
		//���ļ�ĩβд�����û���Ϣ
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
