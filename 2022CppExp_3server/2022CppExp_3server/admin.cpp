#include "serverhead.h"

extern fstream userInfoFile;
extern fstream adminInfoFile;
extern fstream packInfoFile;
extern fstream postmanInfoFile;

Admin::Admin()
{}

Admin::~Admin()
{}

//�ӿͻ����յ�����Ա��ָ��
void Admin::recvFromClient(const string& req) const
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
		addPostman(info);
		break;
	case '4':
		deletePostmanFromPack(info);
		break;
	case '5':
		deletePostman(info);
		break;
	case '6':
		assignPack(info);
		break;
	case '7':
		loadAllUserList();
		break;
	case '8':
		loadAllPostmanList();
		break;
	case '9':
		loadAllPackList();
		break;
	default:
		break;
	}
}

//������ԱID id�Ƿ���ڣ����ڷ���1��������0
void Admin::checkExistId(const string& id) const
{
	streampos loc = searchID(id);	//��ȡ����Ա��Ϊid�Ĺ���Ա���ļ���λ��

	if (loc == -1)
		strcpy_s(send_buf, "0");
	else
		strcpy_s(send_buf, "1");
	send(client, send_buf, 1000, 0);
}

//���͹���ԱID id������
void Admin::sendPw(const string& id) const
{
	streampos loc = searchID(id);	//��ȡ����Ա��Ϊid�Ĺ���Ա���ļ���λ��
	string pw;

	adminInfoFile.clear();
	adminInfoFile.seekg(loc);
	adminInfoFile.ignore(LLONG_MAX, SPLIT);
	getline(adminInfoFile, pw, SPLIT);	//��ȡ����

	strcpy_s(send_buf, pw.c_str());
	send(client, send_buf, 1000, 0);	//���͸��ͻ���
}

//���͹���ԱID id���û���Ϣ
void Admin::getInfo(const string& id) const
{
	streampos loc = searchID(id);	//��ȡ����Ա��Ϊid�Ĺ���Ա���ļ���λ��
	string info, balance;

	adminInfoFile.clear();
	adminInfoFile.seekg(loc);
	adminInfoFile.ignore(LLONG_MAX, SPLIT);
	adminInfoFile.ignore(LLONG_MAX, SPLIT);
	getline(adminInfoFile, info);	//��ȡID����������Ĺ���Ա��Ϣ

	adminInfoFile.clear();
	adminInfoFile.seekg(0);
	getline(adminInfoFile, balance);//��ȡ�����й���Ա����һ�������ļ��ͷ

	info += SPLIT + balance;
	strcpy_s(send_buf, info.c_str());
	send(client, send_buf, 1000, 0);	//���Ϳͻ���
}

//���ļ��������Ա��Ϣ
void Admin::addPostman(const string& postmanInfo) const
{
	postmanInfoFile.clear();
	postmanInfoFile.seekg(0, ios::end);
	postmanInfoFile << postmanInfo << endl;	//�¿��Ա��Ϣд���ļ�
	strcpy_s(send_buf, "1");
	send(client, send_buf, 1000, 0);	//����1��ʾ�����ɹ�
}

//ɾ���ļ��п�ݵĿ��Ա��Ϣ
void Admin::deletePostmanFromPack(const string& packId) const
{
	fstream tmpFile;
	string packInfo, sendTime, type, amount, price, state;
	PackUserInfo sender, recver;
	PackPostmanInfo postman;
	int stateInt;
	string tmp;
	tmpFile.open(".\\data\\tmp.txt", ios::in | ios::out | ios::trunc);
	packInfoFile.clear();
	packInfoFile.seekg(0);
	//�����Ϣ�ļ��У���ǰ���֮ǰ������д����ʱ�ļ�
	while (!packInfoFile.eof())
	{
		getline(packInfoFile, tmp, SPLIT);
		if (!tmp.compare(packId))
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
	//ɾȥ���Ա��Ϣ
	getline(packInfoFile, postman.ID, SPLIT);
	postman.ID = "???";
	getline(packInfoFile, postman.name, SPLIT);
	postman.name = "???";
	getline(packInfoFile, postman.phone, SPLIT);
	postman.phone = "???";
	if (stateInt == NOT_RECEIVE)	//����ȡ����ʱ��
	{
		packInfoFile.ignore(LLONG_MAX, SPLIT);
		packInfoFile.ignore(LLONG_MAX, SPLIT);
		packInfoFile.ignore(LLONG_MAX, SPLIT);
		packInfoFile.ignore(LLONG_MAX, SPLIT);
		packInfoFile.ignore(LLONG_MAX, SPLIT);

	}
	getline(packInfoFile, type, SPLIT);
	getline(packInfoFile, amount, SPLIT);
	getline(packInfoFile, price, SPLIT);
	getline(packInfoFile, packInfo);
	//��ǰ�����Ϣд����ʱ�ļ�
	stateInt = NOT_PICK;
	tmpFile << packId << SPLIT << stateInt << SPLIT;
	tmpFile << sendTime;
	tmpFile << sender.ID << SPLIT << sender.name << SPLIT << sender.addr << SPLIT << sender.phone << SPLIT;
	tmpFile << recver.ID << SPLIT << recver.name << SPLIT << recver.addr << SPLIT << recver.phone << SPLIT;
	tmpFile << postman.ID << SPLIT << postman.name << SPLIT << postman.phone << SPLIT;
	tmpFile << type << SPLIT << amount << SPLIT << price << SPLIT;
	tmpFile << packInfo << endl;
	//�����Ϣ�ļ�ʣ������д����ʱ�ļ�
	packInfoFile.ignore(LLONG_MAX, '\n');
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
	send(client, send_buf, 1000, 0);	//����1��ʾɾ���ɹ�
}

//���ļ���ɾ�����Ա
void Admin::deletePostman(const string& id) const
{
	string tmp;
	fstream tmpFile;

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
	strcpy_s(send_buf, "1");
	send(client, send_buf, 1000, 0);	//����1��ʾ�ɹ�ɾ��
}

//Ϊ��ݷ�����Ա
void Admin::assignPack(const string& info) const
{
	fstream tmpFile;
	string packId, packInfo, sendTime, type, amount, price, state;
	PackPostmanInfo postmanAssign;
	PackUserInfo sender, recver;
	PackPostmanInfo postman;
	string tmp;
	istringstream iss(info);

	getline(iss, packId, SPLIT);
	getline(iss, postmanAssign.ID, SPLIT);
	getline(iss, postmanAssign.name, SPLIT);
	getline(iss, postmanAssign.phone);

	tmpFile.open(".\\data\\tmp.txt", ios::in | ios::out | ios::trunc);
	packInfoFile.clear();
	packInfoFile.seekg(0);
	//�����Ϣ�ļ��У���ǰ���֮ǰ������д����ʱ�ļ�
	while (!packInfoFile.eof())
	{
		getline(packInfoFile, tmp, SPLIT);
		if (!tmp.compare(packId))
			break;
		if (tmp.length())
		{
			tmpFile << tmp << SPLIT;
			getline(packInfoFile, tmp);
			tmpFile << tmp << endl;
		}
	}
	getline(packInfoFile, state, SPLIT);
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
	postman.ID = postmanAssign.ID;
	getline(packInfoFile, postman.name, SPLIT);
	postman.name = postmanAssign.name;
	getline(packInfoFile, postman.phone, SPLIT);
	postman.phone = postmanAssign.phone;
	getline(packInfoFile, type, SPLIT);
	getline(packInfoFile, amount, SPLIT);
	getline(packInfoFile, price, SPLIT);
	getline(packInfoFile, packInfo);
	//��ǰ�����Ϣд����ʱ�ļ�
	tmpFile << packId << SPLIT << state << SPLIT;
	tmpFile << sendTime;
	tmpFile << sender.ID << SPLIT << sender.name << SPLIT << sender.addr << SPLIT << sender.phone << SPLIT;
	tmpFile << recver.ID << SPLIT << recver.name << SPLIT << recver.addr << SPLIT << recver.phone << SPLIT;
	tmpFile << postman.ID << SPLIT << postman.name << SPLIT << postman.phone << SPLIT;
	tmpFile << type << SPLIT << amount << SPLIT << price << SPLIT;
	tmpFile << packInfo << endl;
	//�����Ϣ�ļ�ʣ������д����ʱ�ļ�
	packInfoFile.ignore(LLONG_MAX, '\n');
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
	send(client, send_buf, 1000, 0);	//����1��ʾ����ɹ�
}

//����Ա��Ȩ�鿴�����û�����ǰ���ļ��ж�ȡ���������û���Ϣ������
void Admin::loadAllUserList() const
{
	userInfoFile.clear();
	userInfoFile.seekg(0);

	//����Ա��¼ʱ�����������û���Ϣ
	while (!userInfoFile.eof())
	{
		string tmp;

		getline(userInfoFile, tmp);
		//�����ȡ����
		if (!tmp.length())
			continue;
		tmp += '|';
		strcpy_s(send_buf, tmp.c_str());
		send(client, send_buf, 1000, 0);	//���ͻ��˷���
	}
	strcpy_s(send_buf, "");
	send(client, send_buf, 1000, 0);	//����һ���մ���ʾ���ͽ���
}

//����Ա��Ȩ�鿴���п��Ա����ǰ���ļ��ж�ȡ�������п��Ա��Ϣ������
void Admin::loadAllPackList() const
{
	packInfoFile.clear();
	packInfoFile.seekg(0);

	//����Ա��¼ʱ�����ļ���ȡ���п����Ϣ������packList
	while (!packInfoFile.eof())
	{
		string tmp;

		getline(packInfoFile, tmp);
		//�����ȡ����
		if (!tmp.length())
			continue;
		tmp += '|';
		strcpy_s(send_buf, tmp.c_str());
		send(client, send_buf, 1000, 0);	//���͸��ͻ���
	}
	strcpy_s(send_buf, "");
	send(client, send_buf, 1000, 0);	//����һ���մ���ʾ���ͽ���
}

//����Ա��Ȩ�鿴���п�ݣ���ǰ���ļ��ж�ȡ�������п����Ϣ������
void Admin::loadAllPostmanList() const
{
	postmanInfoFile.clear();
	postmanInfoFile.seekg(0);

	//����Ա��¼ʱ�����������û���Ϣ
	while (!postmanInfoFile.eof())
	{
		string tmp;

		getline(postmanInfoFile, tmp);
		//�����ȡ����
		if (!tmp.length())
			continue;
		tmp += '|';
		strcpy_s(send_buf, tmp.c_str());
		send(client, send_buf, 1000, 0);	//���͸��ͻ���
	}
	strcpy_s(send_buf, "");
	send(client, send_buf, 1000, 0);	//����һ���մ���ʾ���ͽ���
}

//����IDΪid�Ĺ���Ա��Ϣ���ļ��е�λ��
streampos Admin::searchID(const string& id) const
{
	string s;
	streampos loc = 0;
	adminInfoFile.clear();
	adminInfoFile.seekg(0);
	adminInfoFile.ignore(LLONG_MAX, '\n');
	loc = adminInfoFile.tellg();

	//�ļ�Ϊ�գ�ֱ�ӷ���0
	if (adminInfoFile.eof())
		return loc;

	//���ҵ�ǰ����ID���о���������
	while (!adminInfoFile.eof())
	{
		loc = adminInfoFile.tellg();
		getline(adminInfoFile, s, SPLIT);
		if (!s.compare(id))
			return loc;
		adminInfoFile.ignore(LLONG_MAX, '\n');
	}

	//�ļ���û�У�����-1
	return -1;
}
