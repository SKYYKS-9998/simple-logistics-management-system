#include "clienthead.h"

extern vector<Pack> packList;

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

	string tmp, req;
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

		//���͸�����ˣ���������ID�Ƿ����
		req = string("1") + SPLIT + '0' + SPLIT + tmp;
		strcpy_s(send_buf, req.c_str());
		send(server, send_buf, 1000, 0);
		recv(server, recv_buf, 1000, 0);

		//����Ѵ��ڸ�ID��ѯ���û��Ƿ��ø��û����е�¼������Ҫ����������
		if (recv_buf[0] == '1')
		{
			do
			{
				cout << "�û����ѱ�ʹ�ã��Ƿ���е�¼��" << endl;
				cout << "����1���е�¼����������q" << endl;
				char input = getchar();
				cin.ignore(LLONG_MAX, '\n');
				switch (input)
				{
				case '1':
					login(tmp);
					return;
				case 'q':
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

	//���͸��ͻ��ˣ���ע������û���Ϣ�洢
	req = string("1") + SPLIT + '8' + SPLIT + ID + SPLIT + pw + SPLIT + name + SPLIT + phone + SPLIT + addr + SPLIT + to_string(balance);
	strcpy_s(send_buf, req.c_str());
	send(server, send_buf, 1000, 0);
	recv(server, recv_buf, 1000, 0);

	userInterface();
}

//��¼
void User::login()
{
	/*
	* �����û�ID�����������¼��Ȼ������û���Ϣ
	*/

	string input, req;

	while (1)
	{
		cout << "�������û�ID������q�˳�" << endl;
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

		//���͸�����ˣ���������ID�Ƿ����
		req = string("1") + SPLIT + '0' + SPLIT + ID;
		strcpy_s(send_buf, req.c_str());
		send(server, send_buf, 1000, 0);
		recv(server, recv_buf, 1000, 0);

		if (recv_buf[0] == '0')
		{
			cout << "�û��������ڣ��Ƿ����ע��" << endl;
			cout << "���������ע�ᣬ����q��������" << endl;
			char input = getchar();
			cin.ignore(1, '\n');
			switch (input)
			{
			case 'q':
				flag = 0;
				break;
			default:
				regis();
				return;
			}
		}
		if (!flag)
			continue;

		//�����������������ID������
		req = string("1") + SPLIT + '1' + SPLIT + ID;
		strcpy_s(send_buf, req.c_str());
		send(server, send_buf, 1000, 0);
		recv(server, recv_buf, 1000, 0);
		pw = recv_buf;
		while (1)
		{
			cout << "���������룬����q���������û�ID" << endl;
			cin >> input;
			cin.ignore(LLONG_MAX, '\n');
			if (!input.compare(pw))
			{
				flag = 1;
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

	//�������������������û���Ϣ
	req = string("1") + SPLIT + '2' + SPLIT + ID;
	strcpy_s(send_buf, req.c_str());
	send(server, send_buf, 1000, 0);
	recv(server, recv_buf, 1000, 0);	
	istringstream iss(recv_buf);
	//��ȡ������Ϣ
	getline(iss, name, SPLIT);
	getline(iss, phone, SPLIT);
	getline(iss, addr, SPLIT);
	string balanceString;
	getline(iss, balanceString, SPLIT);
	balance = atoi(balanceString.c_str());
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
	string tmp, packs;
	string req = string("1") + SPLIT + '9' + SPLIT + ID;
	strcpy_s(send_buf, req.c_str());
	send(server, send_buf, 1000, 0);	//���͸�����ˣ���������ؿ����Ϣ

	while (1)
	{
		//�ӷ������������ݣ�ֱ���յ���ʾ���ͽ����Ŀմ�
		recv(server, recv_buf, 1000, 0);
		if (strlen(recv_buf) == 0)
			break;
		tmp = recv_buf;
		packs += tmp;	//packs��洢�����н��յ��Ŀ��
	}

	//��packs�ж�ȡ�����Ϣ����packList���
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
			//���͸�����˸�������
			string req = string("1") + SPLIT + '3' + SPLIT + input;
			strcpy_s(send_buf, req.c_str());
			send(server, send_buf, 1000, 0);
			recv(server, recv_buf, 1000, 0);
			if (recv_buf[0] == '1')
			{
				pw = input;
				cout << "���ĳɹ�" << endl;
			}
			break;
		}
		else
			cout << "���зǷ��ַ�������������" << endl;
	}
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
		//���͸�����˸������
		string req = string("1") + SPLIT + '4' + SPLIT + to_string(balance + value);
		strcpy_s(send_buf, req.c_str());
		send(server, send_buf, 1000, 0);
		recv(server, recv_buf, 1000, 0);
		if (recv_buf[0] == '1')
		{
			balance += value;
			cout << "��ֵ�ɹ�" << endl;
		}
		break;
	}
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

	//���͸�����ˣ���ע������û���Ϣ�洢
	string req = string("1") + SPLIT + '8' + SPLIT + ID + SPLIT + pw + SPLIT + name + SPLIT + phone + SPLIT + addr + SPLIT + to_string(balance);
	strcpy_s(send_buf, req.c_str());
	send(server, send_buf, 1000, 0);
	recv(server, recv_buf, 1000, 0);

	userInterface();
}

//���û�ע��ʱʹ���Ѵ����û���ʱʹ�ã���¼�����ID
void User::login(const string& id)
{
	string input;
	ID = id;

	//����������id���û�������
	string req = string("1") + SPLIT + '0' + SPLIT + ID;
	strcpy_s(send_buf, req.c_str());
	send(server, send_buf, 1000, 0);
	recv(server, recv_buf, 1000, 0);
	pw = recv_buf;
	//���ļ��л�ȡ����
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

	//����������ǰ�û���������Ϣ
	req = string("1") + SPLIT + '2' + SPLIT + ID;
	strcpy_s(send_buf, req.c_str());
	send(server, send_buf, 1000, 0);
	recv(server, recv_buf, 1000, 0);
	istringstream iss(recv_buf);
	//��ȡ������Ϣ
	getline(iss, name, SPLIT);
	getline(iss, phone, SPLIT);
	getline(iss, addr, SPLIT);
	string balanceString;
	getline(iss, balanceString, SPLIT);
	balance = atoi(balanceString.c_str());
	//�û�����
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
	string req;

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
		//�����˷��ͣ�����Ƿ���ڸ��û�
		req = string("1") + SPLIT + '0' + SPLIT + ID;
		strcpy_s(send_buf, req.c_str());
		send(server, send_buf, 1000, 0);
		recv(server, recv_buf, 1000, 0);
		if (recv_buf[0] == '0')
		{
			cout << "�����ڸ��û�������������" << endl;
			continue;
		}
		sender.ID = this->ID;
		recver.ID = input;
		//��������ļļ��˺��ռ�����Ϣ
		loadSenderInfo(sender);
		loadRecverInfo(recver);

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
		{
			balance -= pack->price;
			//�����˷��ͣ��������
			req = string("1") + SPLIT + '4' + SPLIT + to_string(balance);
			strcpy_s(send_buf, req.c_str());
			send(server, send_buf, 1000, 0);
			recv(server, recv_buf, 1000, 0);
			if (recv_buf[0] == '1')
				cout << "����ɹ�" << endl;
		}
		else
		{
			cout << "����" << endl;
			cout << "���Ϊ" << balance << "Ԫ" << endl;
			continue;
		}

		pack->sendPack();	//���Ϳ��
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
void User::loadSenderInfo(PackUserInfo& sender) const
{
	sender.name = name;
	sender.phone = phone;
	sender.addr = addr;
}

//�����ռ�����Ϣ������loc���ռ��˵���Ϣ���û���Ϣ�ļ���λ��
void User::loadRecverInfo(PackUserInfo& recver) const
{
	//�����������ռ�����Ϣ
	string req = string("1") + SPLIT + '7' + SPLIT + recver.ID;
	strcpy_s(send_buf, req.c_str());
	send(server, send_buf, 1000, 0);
	recv(server, recv_buf, 1000, 0);
	istringstream iss(recv_buf);

	getline(iss, recver.name, SPLIT);
	getline(iss, recver.phone, SPLIT);
	getline(iss, recver.addr);
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
			string tmp = "�зǷ��ַ�����������";
			cout << tmp << endl;
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
