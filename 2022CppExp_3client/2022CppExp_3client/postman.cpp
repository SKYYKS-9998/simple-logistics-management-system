#include "clienthead.h"

extern vector<Pack> packList;

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
	string input, req;

	while (1)
	{
		cout << "��������ԱID������q�˳�" << endl;
		cin >> input;
		cin.ignore(LLONG_MAX, '\n');
		if (input == "q")
			return;
		ID = input;
		//�����˷��ͣ����ID�Ƿ����
		req = string("2") + SPLIT + '0' + SPLIT + ID;
		strcpy_s(send_buf, req.c_str());
		send(server, send_buf, 1000, 0);
		recv(server, recv_buf, 1000, 0);
		if (recv_buf[0] == '0')
		{
			cout << "���ԱID�����ڣ�����������" << endl;
			continue;
		}
		//����������ID������
		req = string("2") + SPLIT + '1' + SPLIT + ID;
		strcpy_s(send_buf, req.c_str());
		send(server, send_buf, 1000, 0);
		recv(server, recv_buf, 1000, 0);		
		pw = recv_buf;
		int flag = 1;
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
	
	//������������Ա��������Ϣ
	req = string("2") + SPLIT + '2' + SPLIT + ID;
	strcpy_s(send_buf, req.c_str());
	send(server, send_buf, 1000, 0);
	recv(server, recv_buf, 1000, 0);
	istringstream iss(recv_buf);
	//��ȡ������Ϣ
	getline(iss, name, SPLIT);
	getline(iss, phone, SPLIT);
	string balanceString;
	getline(iss, balanceString, SPLIT);
	balance = atoi(balanceString.c_str());

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
	string tmp, packs;
	string req = string("2") + SPLIT + '5' + SPLIT + ID;
	strcpy_s(send_buf, req.c_str());
	send(server, send_buf, 1000, 0);	//�����������͵�ǰ���Ա����Ŀ��

	while (1)
	{
		//���շ���˷��͵����ݣ�ֱ���յ���ʾ���ͽ����Ŀմ�
		recv(server, recv_buf, 1000, 0);
		if (strlen(recv_buf) == 0)
			break;
		tmp = recv_buf;
		packs += tmp;	//packs��洢�����н��յ��Ŀ����Ϣ
	}
	//��packs���ȡ�����Ϣ
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

		packList[locV].pickUp();		//���տ��
		balance += (packList[locV].price + 1) / 2;	//�˷ѵ�һ��ת�����Ա������ȡ��
		updateBalanceInFile();			//�ļ��и��¿��Ա���

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
	//���͸�����ˣ��������
	string req = string("2") + SPLIT + '4' + SPLIT + to_string(balance);
	strcpy_s(send_buf, req.c_str());
	send(server, send_buf, 1000, 0);
	recv(server, recv_buf, 1000, 0);
	if (recv_buf[0] != '1')
		cout << "�տ�ʧ��" << endl;
	else
		cout << "�տ�ɹ�" << endl;
}
