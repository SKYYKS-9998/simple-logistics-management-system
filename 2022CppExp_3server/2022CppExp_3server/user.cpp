#include "serverhead.h"

extern fstream userInfoFile;
extern fstream packInfoFile;
extern fstream adminInfoFile;

User::User()
	: locInFile(-1)
{}

User::~User()
{}

//�ӿͻ����յ��û���ָ��
void User::recvFromClient(const string& req)
{
	istringstream iss(req);
	string op, info;

	getline(iss, op, SPLIT);	//��ȡָ���������
	getline(iss, info);			//��ȡ���������

	switch (op[0])	//���ݲ�������ѡ���Ӧ����ִ��ָ��
	{
	case '0':
		checkExistId(info);		
		break;
	case '1':
		sendPw(info);
		break;
	case '2':
		getInfo(info);
		break;
	case '3':
		changePW(info);
		break;
	case '4':
		changeBalance(info);
		break;
	case '5':
		sendPack(info);
		break;
	case '6':
		recvPack(info);
		break;
	case '7':
		loadPacketUserInfo(info);
		break;
	case '8':
		newUser(info);
		break;
	case '9':
		loadUserPackList(info);
		break;
	default:
		break;
	}
}

//����û�ID id�Ƿ���ڣ����ڷ���1��������0
void User::checkExistId(const string& id) const
{
	streampos loc = searchID(id);	//��ȡ�û���Ϊid���û����ļ���λ��

	if (loc == -1)			
		strcpy_s(send_buf, "0");
	else
		strcpy_s(send_buf, "1");
	send(client, send_buf, 1000, 0);
}

//�����û�ID id������
void User::sendPw(const string& id) const
{
	streampos loc = searchID(id);	//��ȡ�û���Ϊid���û����ļ���λ��
	string pw;
	
	userInfoFile.clear();
	userInfoFile.seekg(loc);
	userInfoFile.ignore(LLONG_MAX, SPLIT);
	getline(userInfoFile, pw, SPLIT);	//��ȡ����

	strcpy_s(send_buf, pw.c_str());
	send(client, send_buf, 1000, 0);	//���͸��ͻ���
}

//�����û�ID id���û���Ϣ
void User::getInfo(const string& id)
{
	locInFile = searchID(id);	//��ȡ�û���Ϊid���û����ļ���λ��
	string info;

	userInfoFile.clear();
	userInfoFile.seekg(locInFile);
	userInfoFile.ignore(LLONG_MAX, SPLIT);
	userInfoFile.ignore(LLONG_MAX, SPLIT);
	getline(userInfoFile, info);	//��ȡID������������û���Ϣ

	strcpy_s(send_buf, info.c_str());
	send(client, send_buf, 1000, 0);//���͸��ͻ���
}

//�����û�ID id������
void User::changePW(const string& pw) const
{
	writeToFile(1, pw);				//���ļ��и�������
	strcpy_s(send_buf, "1");		
	send(client, send_buf, 1000, 0);//����1��ʾ���ĳɹ�
}

//�����û�ID id�����
void User::changeBalance(const string& balance) const
{
	writeToFile(2, balance);		//���ļ��и������
	strcpy_s(send_buf, "1");
	send(client, send_buf, 1000, 0);//����1��ʾ���ĳɹ�
}

//���Ϳ��
void User::sendPack(const string& packInfo) const
{
	istringstream iss(packInfo);
	string tmp;
	int price;

	getline(iss, tmp, SPLIT);
	price = atoi(tmp.c_str());	//��ȡ�·��͵Ŀ�ݵ��˷�

	//���Ĺ���Ա�����
	fstream tmpFile;
	int balance;
	tmpFile.open(".\\data\\tmp.txt", ios::in | ios::out | ios::trunc);
	adminInfoFile.clear();
	adminInfoFile.seekg(0);
	//��ȡ�������˷ѵ�һ�루������Ա������ȡ������д����ʱ�ļ�
	adminInfoFile >> balance;
	adminInfoFile.ignore(LLONG_MAX, '\n');
	balance += price / 2;
	tmpFile << balance << endl;
	//����Ա��Ϣ�ļ���������д����ʱ�ļ�
	while (!adminInfoFile.eof())
	{
		getline(adminInfoFile, tmp);
		if (tmp.length())
			tmpFile << tmp << endl;
	}
	//��ʱ�ļ����ݸ��ǹ���Ա��Ϣ�ļ�
	adminInfoFile.close();
	adminInfoFile.open(".\\data\\adminInfo.txt", ios::in | ios::out | ios::trunc);
	tmpFile.clear();
	tmpFile.seekg(0);
	while (!tmpFile.eof())
	{
		getline(tmpFile, tmp);
		if (tmp.length())
			adminInfoFile << tmp << endl;
	}
	tmpFile.close();

	getline(iss, tmp);		//��ȡ�·��͵Ŀ����Ϣ
	packInfoFile.clear();
	packInfoFile.seekg(0, ios::end);	
	packInfoFile << tmp << endl;	//�����ļ���ĩβ���
	strcpy_s(send_buf, "1");
	send(client, send_buf, 1000, 0);//����1��ʾ���ͳɹ�
}

//���տ��
void User::recvPack(const string& packInfo) const
{
	istringstream iss(packInfo);
	string packID, info, sendTime, recvTime, pickUpTime, type, amount, price, state;
	PackUserInfo sender, recver;
	PackPostmanInfo postman;
	int stateInt;
	string tmp;

	getline(iss, packID, SPLIT);	//��ȡ���յĿ�ݵĿ�ݺ�
	getline(iss, recvTime);			//��ȡ����ʱ��

	fstream tmpFile;
	tmpFile.open(".\\data\\tmp.txt", ios::in | ios::out | ios::trunc);
	packInfoFile.clear();
	packInfoFile.seekg(0);
	//�����Ϣ�ļ��У���ǰ���֮ǰ������д����ʱ�ļ�
	while (!packInfoFile.eof())
	{
		getline(packInfoFile, tmp, SPLIT);
		if (!tmp.compare(packID))
			break;
		if (tmp.length())
		{
			tmpFile << tmp << SPLIT;
			getline(packInfoFile, tmp);
			tmpFile << tmp << endl;
		}
	}
	//�������յĿ�ݣ���ȡ�ÿ�ݵ���Ϣ
	getline(packInfoFile, state, SPLIT);
	stateInt = atoi(state.c_str());
	stateInt = RECEIVE;
	getline(packInfoFile, tmp, SPLIT);
	sendTime += tmp + SPLIT;
	getline(packInfoFile, tmp, SPLIT);
	sendTime += tmp + SPLIT;
	getline(packInfoFile, tmp, SPLIT);
	sendTime += tmp + SPLIT;
	getline(packInfoFile, tmp, SPLIT);
	sendTime += tmp + SPLIT;
	getline(packInfoFile, tmp, SPLIT);
	sendTime += tmp + SPLIT;
	getline(packInfoFile, sender.ID, SPLIT);
	getline(packInfoFile, sender.name, SPLIT);
	getline(packInfoFile, sender.addr, SPLIT);
	getline(packInfoFile, sender.phone, SPLIT);
	getline(packInfoFile, recver.ID, SPLIT);
	getline(packInfoFile, recver.name, SPLIT);
	getline(packInfoFile, recver.addr, SPLIT);
	getline(packInfoFile, recver.phone, SPLIT);
	getline(packInfoFile, postman.ID, SPLIT);
	getline(packInfoFile, postman.name, SPLIT);
	getline(packInfoFile, postman.phone, SPLIT);
	getline(packInfoFile, tmp, SPLIT);
	pickUpTime += tmp + SPLIT;
	getline(packInfoFile, tmp, SPLIT);
	pickUpTime += tmp + SPLIT;
	getline(packInfoFile, tmp, SPLIT);
	pickUpTime += tmp + SPLIT;
	getline(packInfoFile, tmp, SPLIT);
	pickUpTime += tmp + SPLIT;
	getline(packInfoFile, tmp, SPLIT);
	pickUpTime += tmp + SPLIT;
	getline(packInfoFile, type, SPLIT);
	getline(packInfoFile, amount, SPLIT);
	getline(packInfoFile, price, SPLIT);
	getline(packInfoFile, info);
	//�������Ϣ��������ʱ���д����ʱ�ļ�
	tmpFile << packID << SPLIT << stateInt << SPLIT;
	tmpFile << sendTime;
	tmpFile << recvTime << SPLIT;
	tmpFile << sender.ID << SPLIT << sender.name << SPLIT << sender.addr << SPLIT << sender.phone << SPLIT;
	tmpFile << recver.ID << SPLIT << recver.name << SPLIT << recver.addr << SPLIT << recver.phone << SPLIT;
	tmpFile << postman.ID << SPLIT << postman.name << SPLIT << postman.phone << SPLIT;
	tmpFile << pickUpTime;
	tmpFile << type << SPLIT << amount << SPLIT << price << SPLIT;
	tmpFile << info << endl;
	//�����Ϣ�ļ�ʣ������д����ʱ�ļ�
	while (!packInfoFile.eof())
	{
		getline(packInfoFile, tmp);
		if (tmp.length())
			tmpFile << tmp << endl;
	}
	//��ʱ�ļ����ݸ��ǿ����Ϣ�ļ�
	tmpFile.clear();
	tmpFile.seekg(0);
	packInfoFile.close();
	packInfoFile.open(".\\data\\packInfo.txt", ios::out | ios::in | ios::trunc);
	while (!tmpFile.eof())
	{
		getline(tmpFile, tmp);
		if (tmp.length())
			packInfoFile << tmp << endl;
	}
	tmpFile.close();
	strcpy_s(send_buf, "1");
	send(client, send_buf, 1000, 0);	//����1��ʾ���ճɹ�
}

//�����û�IDΪid���ռ�����Ϣ
void User::loadPacketUserInfo(const string& id) const
{
	streampos loc = searchID(id);	//��ȡ�û���Ϊid���û����ļ���λ��
	string name, phone, addr, recvInfo;

	userInfoFile.clear();
	userInfoFile.seekg(loc);

	//�����ռ����û���������
	userInfoFile.ignore(LLONG_MAX, SPLIT);
	userInfoFile.ignore(LLONG_MAX, SPLIT);
	//��ȡ�����ռ�����Ϣ
	getline(userInfoFile, name, SPLIT);
	getline(userInfoFile, phone, SPLIT);
	getline(userInfoFile, addr, SPLIT);

	recvInfo = name + SPLIT + phone + SPLIT + addr;
	strcpy_s(send_buf, recvInfo.c_str());
	send(client, send_buf, 1000, 0);	//�����ռ�����Ϣ���ͻ���
}

//���ļ�������û�
void User::newUser(const string& userInfo)
{
	writeToFile(0, userInfo);	//���ļ�д�����û���Ϣ
	strcpy_s(send_buf, "1");
	send(client, send_buf, 1000, 0);	//����1��ʾ��ӳɹ�
}

//���ļ��ж�ȡ���û�IDΪid���û���صĿ�ݲ�����
void User::loadUserPackList(const string& id) const
{
	packInfoFile.clear();
	packInfoFile.seekg(0);

	while (!packInfoFile.eof())
	{
		string s;
		string packID, packInfo;
		PackUserInfo sender, recver;
		PackPostmanInfo post;
		string sendTime, recvTime, pickUpTime;
		string type, amount, price, state;
		int stateInt;

		getline(packInfoFile, packID, SPLIT);
		//�����ȡ����
		if (!packID.length())
			continue;
		getline(packInfoFile, state, SPLIT);
		stateInt = atoi(state.c_str());
		getline(packInfoFile, s, SPLIT);
		sendTime += s + SPLIT;
		getline(packInfoFile, s, SPLIT);
		sendTime += s + SPLIT;
		getline(packInfoFile, s, SPLIT);
		sendTime += s + SPLIT;
		getline(packInfoFile, s, SPLIT);
		sendTime += s + SPLIT;
		getline(packInfoFile, s, SPLIT);
		sendTime += s;
		//�ѽ��յĿ�ݲ��н���ʱ��
		if (stateInt == RECEIVE)
		{
			getline(packInfoFile, s, SPLIT);
			recvTime += s + SPLIT;
			getline(packInfoFile, s, SPLIT);
			recvTime += s + SPLIT;
			getline(packInfoFile, s, SPLIT);
			recvTime += s + SPLIT;
			getline(packInfoFile, s, SPLIT);
			recvTime += s + SPLIT;
			getline(packInfoFile, s, SPLIT);
			recvTime += s;
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
		if (recver.ID.compare(id) && sender.ID.compare(id))
		{
			packInfoFile.ignore(LLONG_MAX, '\n');
			continue;
		}
		getline(packInfoFile, post.ID, SPLIT);
		getline(packInfoFile, post.name, SPLIT);
		getline(packInfoFile, post.phone, SPLIT);
		//δ���յĿ��û������ʱ��
		if (stateInt != NOT_PICK)
		{
			getline(packInfoFile, s, SPLIT);
			pickUpTime += s + SPLIT;
			getline(packInfoFile, s, SPLIT);
			pickUpTime += s + SPLIT;
			getline(packInfoFile, s, SPLIT);
			pickUpTime += s + SPLIT;
			getline(packInfoFile, s, SPLIT);
			pickUpTime += s + SPLIT;
			getline(packInfoFile, s, SPLIT);
			pickUpTime += s;
		}
		getline(packInfoFile, type, SPLIT);
		getline(packInfoFile, amount, SPLIT);
		getline(packInfoFile, price, SPLIT);
		getline(packInfoFile, packInfo);

		if (stateInt == NOT_PICK)
		{
			s = packID + SPLIT + state + SPLIT + sendTime + SPLIT + sender.ID + SPLIT + sender.name + SPLIT + sender.phone + SPLIT + sender.addr + SPLIT +
				recver.ID + SPLIT + recver.name + SPLIT + recver.phone + SPLIT + recver.addr + SPLIT + post.ID + SPLIT + post.name + SPLIT + post.phone + SPLIT +
				type + SPLIT + amount + SPLIT + price + SPLIT + packInfo + SEND_SPLIT;
		}
		else if (stateInt == NOT_RECEIVE)
		{
			s = packID + SPLIT + state + SPLIT + sendTime + SPLIT + sender.ID + SPLIT + sender.name + SPLIT + sender.phone + SPLIT + sender.addr + SPLIT +
				recver.ID + SPLIT + recver.name + SPLIT + recver.phone + SPLIT + recver.addr + SPLIT + post.ID + SPLIT + post.name + SPLIT + post.phone + SPLIT + pickUpTime + SPLIT +
				type + SPLIT + amount + SPLIT + price + SPLIT + packInfo + SEND_SPLIT;
		}
		else
		{
			s = packID + SPLIT + state + SPLIT + sendTime + SPLIT + recvTime + SPLIT + sender.ID + SPLIT + sender.name + SPLIT + sender.phone + SPLIT + sender.addr + SPLIT +
				recver.ID + SPLIT + recver.name + SPLIT + recver.phone + SPLIT + recver.addr + SPLIT + post.ID + SPLIT + post.name + SPLIT + post.phone + SPLIT + pickUpTime + SPLIT +
				type + SPLIT + amount + SPLIT + price + SPLIT + packInfo + SEND_SPLIT;
		}
		
		strcpy_s(send_buf, s.c_str());
		send(client, send_buf, 1000, 0);	//��ͻ��˷��Ϳ����Ϣ
	}
	strcpy_s(send_buf, "");
	send(client, send_buf, 1000, 0);		//����һ���մ���ʾ����
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

//���û���Ϣд���ļ���mode=0д�����û���Ϣ��mode=1����ĳ�û������룬mode=1����ĳ�û������
void User::writeToFile(int mode, const string& info) const
{
	if (mode == 0)
	{
		//���ļ�ĩβд�����û���Ϣ
		userInfoFile.clear();
		userInfoFile.seekg(0, ios::end);
		userInfoFile << info << endl;
		return;
	}

	string ID, pw, name, phone, addr, balance;
	fstream tmpFile;
	tmpFile.open(".\\data\\tmp.txt", ios::in | ios::out | ios::trunc);
	string tmp;
	userInfoFile.clear();
	userInfoFile.seekg(0);

	//���û���Ϣ�ļ��У���ǰ�û���Ϣ֮ǰ������д����ʱ�ļ�
	while (userInfoFile.tellg() != locInFile)
	{
		getline(userInfoFile, tmp);
		//���в�д
		if (tmp.length())
			tmpFile << tmp << endl;
	}
	getline(userInfoFile, ID, SPLIT);
	if (mode == 1)
		userInfoFile.ignore(LLONG_MAX, SPLIT);
	else
		getline(userInfoFile, pw, SPLIT);
	getline(userInfoFile, name, SPLIT);
	getline(userInfoFile, phone, SPLIT);
	getline(userInfoFile, addr, SPLIT);
	if (mode == 2)
		userInfoFile.ignore(LLONG_MAX, '\n');
	else
		getline(userInfoFile, balance);
	//��ǰ�û���Ϣ�����º�д����ʱ�ļ�
	if (mode == 1)
		tmpFile << ID << SPLIT << info << SPLIT << name << SPLIT << phone << SPLIT << addr << SPLIT << balance << endl;
	else if (mode == 2)
		tmpFile << ID << SPLIT << pw << SPLIT << name << SPLIT << phone << SPLIT << addr << SPLIT << info << endl;

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
