#include "serverhead.h"

extern fstream postmanInfoFile;
extern fstream packInfoFile;

Postman::Postman()
	: locInFile(-1)
{}

Postman::~Postman()
{}

//�ӿͻ����յ����Ա��ָ��
void Postman::recvFromClient(const string& req)
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
		pickUp(info);
		break;
	case '4':
		updateBalanceInFile(info);
		break;
	case '5':
		loadPostmanPackList(info);
		break;
	default:
		break;
	}
}

//�����ԱID id�Ƿ���ڣ����ڷ���1��������0
void Postman::checkExistId(const string& id) const
{
	streampos loc = searchID(id);	//��ȡ�û���Ϊid�Ŀ��Ա���ļ���λ��

	if (loc == -1)
		strcpy_s(send_buf, "0");
	else
		strcpy_s(send_buf, "1");
	send(client, send_buf, 1000, 0);
}

//���Ϳ��ԱID id������
void Postman::sendPw(const string& id) const
{
	streampos loc = searchID(id);	//��ȡ�û���Ϊid�Ŀ��Ա���ļ���λ��
	string pw;

	postmanInfoFile.clear();
	postmanInfoFile.seekg(loc);
	postmanInfoFile.ignore(LLONG_MAX, SPLIT);
	getline(postmanInfoFile, pw, SPLIT);	//��ȡ����

	strcpy_s(send_buf, pw.c_str());
	send(client, send_buf, 1000, 0);	//���͸��ͻ���
}

//���Ϳ��ԱID id���û���Ϣ
void Postman::getInfo(const string& id)
{
	locInFile = searchID(id);	//��ȡ�û���Ϊid�Ŀ��Ա���ļ���λ��
	string info;

	postmanInfoFile.clear();
	postmanInfoFile.seekg(locInFile);
	postmanInfoFile.ignore(LLONG_MAX, SPLIT);
	postmanInfoFile.ignore(LLONG_MAX, SPLIT);
	getline(postmanInfoFile, info);	//��ȡID����������Ŀ��Ա��Ϣ

	strcpy_s(send_buf, info.c_str());
	send(client, send_buf, 1000, 0);//���͸��ͻ���
}

//���տ��
void Postman::pickUp(const string& info) const
{
	istringstream iss(info);
	string packID, packInfo, sendTime, pickUpTime, type, amount, price, state;
	PackUserInfo sender, recver;
	PackPostmanInfo postman;
	int stateInt;
	string tmp;
	fstream tmpFile;

	getline(iss, packID, SPLIT);	//��ȡ���յĿ�ݵĿ�ݺ�
	getline(iss, pickUpTime);		//��ȡ����ʱ��

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
	getline(packInfoFile, state, SPLIT);
	stateInt = atoi(state.c_str());
	stateInt = NOT_RECEIVE;
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
	getline(packInfoFile, type, SPLIT);
	getline(packInfoFile, amount, SPLIT);
	getline(packInfoFile, price, SPLIT);
	getline(packInfoFile, packInfo);
	//��ǰ�����Ϣд����ʱ�ļ�
	tmpFile << packID << SPLIT << stateInt << SPLIT;
	tmpFile << sendTime;
	tmpFile << sender.ID << SPLIT << sender.name << SPLIT << sender.addr << SPLIT << sender.phone << SPLIT;
	tmpFile << recver.ID << SPLIT << recver.name << SPLIT << recver.addr << SPLIT << recver.phone << SPLIT;
	tmpFile << postman.ID << SPLIT << postman.name << SPLIT << postman.phone << SPLIT;
	tmpFile << pickUpTime << SPLIT;
	tmpFile << type << SPLIT << amount << SPLIT << price << SPLIT;
	tmpFile << packInfo << endl;
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

//�ڿ��Ա��Ϣ�ļ��и������
void Postman::updateBalanceInFile(const string& balance) const
{
	string ID, pw, name, phone;
	fstream tmpFile;
	tmpFile.open(".\\data\\tmp.txt", ios::in | ios::out | ios::trunc);
	string tmp;
	postmanInfoFile.clear();
	postmanInfoFile.seekg(0);

	//�����Ա��Ϣ�ļ��У���ǰ���Ա֮ǰ������д����ʱ�ļ�
	while (postmanInfoFile.tellg() != locInFile)
	{
		getline(postmanInfoFile, tmp);
		//���в�д
		if (tmp.length())
			tmpFile << tmp << endl;
	}
	getline(postmanInfoFile, ID, SPLIT);
	getline(postmanInfoFile, pw, SPLIT);
	getline(postmanInfoFile, name, SPLIT);
	getline(postmanInfoFile, phone, SPLIT);
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
	strcpy_s(send_buf, "1");
	send(client, send_buf, 1000, 0);	//����1��ʾ���ճɹ�
}

//���ļ���ȡ��ǰ���Ա����Ŀ�ݲ�����
void Postman::loadPostmanPackList(const string& id) const
{
	packInfoFile.clear();
	packInfoFile.seekg(0);
	while (!packInfoFile.eof())
	{
		string s, tmp;
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
		getline(packInfoFile, post.ID, SPLIT);
		if (post.ID.compare(id))
		{
			packInfoFile.ignore(LLONG_MAX, '\n');
			continue;
		}
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
			tmp = packID + SPLIT + state + SPLIT + sendTime + SPLIT + sender.ID + SPLIT + sender.name + SPLIT + sender.phone + SPLIT + sender.addr + SPLIT + recver.ID + SPLIT + recver.name + SPLIT + recver.phone + SPLIT + recver.addr + SPLIT + post.ID + SPLIT + post.name + SPLIT + post.phone + SPLIT + type + SPLIT + amount + SPLIT + price + SPLIT + packInfo + SEND_SPLIT;

		}
		else if (stateInt == NOT_RECEIVE)
		{
			tmp = packID + SPLIT + state + SPLIT + sendTime + SPLIT + sender.ID + SPLIT + sender.name + SPLIT + sender.phone + SPLIT + sender.addr + SPLIT + recver.ID + SPLIT + recver.name + SPLIT + recver.phone + SPLIT + recver.addr + SPLIT + post.ID + SPLIT + post.name + SPLIT + post.phone + SPLIT + pickUpTime + SPLIT + type + SPLIT + amount + SPLIT + price + SPLIT + packInfo + SEND_SPLIT;
		}
		else
		{
			tmp = packID + SPLIT + state + SPLIT + sendTime + SPLIT + recvTime + SPLIT + sender.ID + SPLIT + sender.name + SPLIT + sender.phone + SPLIT + sender.addr + SPLIT + recver.ID + SPLIT + recver.name + SPLIT + recver.phone + SPLIT + recver.addr + SPLIT + post.ID + SPLIT + post.name + SPLIT + post.phone + SPLIT + pickUpTime + SPLIT + type + SPLIT + amount + SPLIT + price + SPLIT + packInfo + SEND_SPLIT;
		}
		strcpy_s(send_buf, tmp.c_str());
		send(client, send_buf, 1000, 0);//��ͻ��˷��Ϳ����Ϣ
	}
	strcpy_s(send_buf, "");
	send(client, send_buf, 1000, 0);	//����1��ʾ���ճɹ�
}

//���ص�ǰ��¼�Ŀ��Ա����Ϣ���ļ��е�λ��
streampos Postman::searchID(const string& id) const
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
		if (!s.compare(id))
			return loc;
		postmanInfoFile.ignore(LLONG_MAX, '\n');
	}

	//�ļ���û�У�����-1
	return -1;
}
