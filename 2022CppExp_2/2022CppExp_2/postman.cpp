#include "head2.h"

extern vector<Pack> packList;
extern fstream postmanInfoFile;
extern fstream packInfoFile;

Postman::Postman()
	: balance(0)
{}

//���ļ���������Ա��ӿ��Աʱʹ�õĹ��캯��
Postman::Postman(const string& id, const string& ipw, const string& iname, const string& iphone, const int& ibalance)
	: ID(id)
	, pw(ipw)
	, name(iname)
	, phone(iphone)
	, balance(ibalance)
{}

Postman::~Postman()
{}

//��¼
void Postman::login()
{
	string input;

	while (1)
	{
		cout << "��������ԱID������q�˳�" << endl;
		cin >> input;
		cin.ignore(LLONG_MAX, '\n');

		if (input == "q")
			return;

		ID = input;
		streampos loc = searchID();
		if (loc == -1)
		{
			cout << "���ԱID�����ڣ�����������" << endl;
			continue;
		}
		int flag = 1;
		//���ļ��л�ȡ����ID������
		postmanInfoFile.clear();
		postmanInfoFile.seekg(loc);
		postmanInfoFile.ignore(LLONG_MAX, SPLIT);
		getline(postmanInfoFile, pw, SPLIT);
		while (1)
		{
			cout << "���������룬����q����������ԱID" << endl;
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

	getline(postmanInfoFile, name, SPLIT);
	getline(postmanInfoFile, phone, SPLIT);
	postmanInfoFile >> balance;

	postmanInterface();
}

//������Ա��Ϣ
void Postman::output() const
{
	cout << "���Ա��Ϣ" << endl << "���ԱID��" << ID << endl;
	cout << "���Ա������" << name << endl;
	cout << "���Ա�绰��" << phone << endl;
	cout << "�˻���" << balance << endl;
}

//���ļ���ȡ��ǰ���Ա����Ŀ�ݣ��洢��packList��
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
		//���ǵ�ǰ���Ա����Ŀ�ݣ�û��Ȩ�ޣ����洢
		if (post.ID.compare(ID))
		{
			packInfoFile.ignore(LLONG_MAX, '\n');
			continue;
		}
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

//���Ա����
void Postman::postmanInterface()
{
	char input;

	loadPostmanPackList();
	do {
		cout << "����1���տ��" << endl;
		cout << "����2��ѯ���" << endl;
		cout << "����3�鿴���" << endl;
		cout << "����q�ǳ�" << endl;
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
			cout << "�������" << endl;
			break;
		}
	} while (1);
}

//���տ��
void Postman::pickUp()
{
	string input;
	int locV;

	while (1)
	{
		//û��δ���տ��ֱ�ӷ��أ��о������ݺŽ�������
		cout << "δ���տ�ݣ�" << endl;
		if (!outputUnpickPack())
		{
			cout << "û��δ���տ��" << endl;
			return;
		}

		while (1)
		{
			cout << "�����ݺŽ������գ�����q�˳�" << endl;
			cin >> input;
			cin.ignore(LLONG_MAX, '\n');
			if (input == "q")
				return;
			locV = -1;
			
			//��ȡ�����ݺŵĿ����packList���±꣬û�о���-1
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

		balance += (packList[locV].price + 1) / 2;	//�˷ѵ�һ��ת�����Ա������ȡ��
		updateBalanceInFile();			//�ļ��и��¿��Ա���
		packList[locV].pickUp();		//���տ��
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

//�鿴���
void Postman::checkBalance() const
{
	cout << "���Ϊ" << balance << "Ԫ" << endl;
}

//��ѯ���
void Postman::searchPack() const
{
	char input;

	while (1)
	{
		cout << "����1����ݺŲ�ѯ" << endl;
		cout << "����2���ļ��˲�ѯ" << endl;
		cout << "����3���ռ��˲�ѯ" << endl;
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
void Postman::searchPackBySender() const
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
void Postman::searchPackBySenderID() const
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
void Postman::searchPackBySenderName() const
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
void Postman::searchPackBySenderPhone() const
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
void Postman::searchPackBySenderAddr() const
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
void Postman::searchPackByRecver() const
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
void Postman::searchPackByRecverID() const
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
void Postman::searchPackByRecverName() const
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
void Postman::searchPackByRecverPhone() const
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
void Postman::searchPackByRecverAddr() const
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
void Postman::searchPackBySendTime() const
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
void Postman::searchPackByPackID() const
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
void Postman::searchPackByState() const
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
int Postman::outputUnrecvPack() const
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
int Postman::outputUnpickPack() const
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
int Postman::outputRecvPack() const
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

//���տ�ݺ��ڿ��Ա��Ϣ�ļ��и������
void Postman::updateBalanceInFile() const
{
	fstream tmpFile;
	tmpFile.open(".\\data\\tmp.txt", ios::in | ios::out | ios::trunc);
	string tmp;
	streampos loc = searchID();
	postmanInfoFile.clear();
	postmanInfoFile.seekg(0);

	//�����Ա��Ϣ�ļ��У���ǰ���Ա֮ǰ������д����ʱ�ļ�
	while (postmanInfoFile.tellg() != loc)
	{
		getline(postmanInfoFile, tmp);
		//���в�д
		if (tmp.length())
			tmpFile << tmp << endl;
	}
	//��ǰ���Ա��Ϣ�����º�д����ʱ�ļ�
	tmpFile << ID << SPLIT << pw << SPLIT << name << SPLIT << phone << SPLIT << balance << endl;
	postmanInfoFile.ignore(LLONG_MAX, '\n');
	//��ǰ���Ա֮�������д����ʱ�ļ�
	while (!postmanInfoFile.eof())
	{
		getline(postmanInfoFile, tmp);
		//���в�д
		if (tmp.length())
			tmpFile << tmp << endl;
	}
	tmpFile.clear();
	tmpFile.seekg(0);
	//��ʱ�ļ����ݸ��ǿ��Ա��Ϣ�ļ�
	postmanInfoFile.close();
	postmanInfoFile.open(".\\data\\postmanInfo.txt", ios::in | ios::out | ios::trunc);
	while (!tmpFile.eof())
	{
		getline(tmpFile, tmp);
		//���в�д
		if (tmp.length())
			postmanInfoFile << tmp << endl;
	}

	tmpFile.close();
}

//���ص�ǰ���Ա����Ϣ�ڿ��Ա��Ϣ�ļ��е�λ��
streampos Postman::searchID() const
{
	string s;
	streampos loc = 0;
	postmanInfoFile.clear();
	postmanInfoFile.seekg(0);

	//�ļ�Ϊ�գ�ֱ�ӷ���0
	if (postmanInfoFile.eof())
		return loc;

	//���ҵ�ǰ����ID���о���������
	while (!postmanInfoFile.eof())
	{
		loc = postmanInfoFile.tellg();
		getline(postmanInfoFile, s, SPLIT);
		if (!s.compare(ID))
			return loc;
		postmanInfoFile.ignore(LLONG_MAX, '\n');
	}

	//�ļ���û�У�����-1
	return -1;
}
