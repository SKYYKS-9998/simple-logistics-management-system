#include "serverhead.h"

extern vector<Pack> packList;
extern fstream adminInfoFile;
extern fstream packInfoFile;

Pack::Pack()
	: state(NOT_RECEIVE)
	, sendTime()
	, recvTime()
	, pickUpTime()
	, type(NORMAL)
	, amount(0)
	, price(0)
{}

//���ڶ�ȡ�ļ�����Ĺ��캯��
Pack::Pack(const string& ipackID, const int& istate, const SYSTEMTIME& isendTime, const SYSTEMTIME& irecvTime, const PackUserInfo& isender, const PackUserInfo& irecver, const PackPostmanInfo& ipostman, const SYSTEMTIME& ipickUpTime, const int& itype, const int& iamount, const int& iprice, const string& ipackInfo)
	: packID(ipackID)
	, state(istate)
	, sendTime(isendTime)
	, recvTime(irecvTime)
	, sender(isender)
	, recver(irecver)
	, postman(ipostman)
	, pickUpTime(ipickUpTime)
	, type(itype)
	, amount(iamount)
	, price(iprice)
	, packInfo(ipackInfo)
{}

//���ڼļ��Ĺ��캯��
Pack::Pack(const string& ipackID, const SYSTEMTIME& isendTime, const PackUserInfo& isender, const PackUserInfo& irecver, const int& itype, const int& iamount, const string& ipackInfo)
	: packID(ipackID)
	, state(NOT_PICK)
	, sendTime(isendTime)
	, recvTime()
	, sender(isender)
	, recver(irecver)
	, pickUpTime()
	, type(itype)
	, amount(iamount)
	, price(0)
	, packInfo(ipackInfo)
{
	//����Ҫ����ֻ������״̬��δ������Աʱ�����Ա�ĸ�����Ϣ��Ϊ"???"
	postman.ID = "???";
	postman.name = "???";
	postman.phone = "???";
}

Pack::~Pack()
{}

//��������Ϣ
void Pack::output() const
{
	cout << "��ݺţ�" << packID << endl;
	cout << "״̬��";
	switch (state)
	{
	case NOT_PICK:
		cout << "δ����" << endl;
		break;
	case NOT_RECEIVE:
		cout << "δ����" << endl;
		break;
	case RECEIVE:
		cout << "�ѽ���" << endl;
		break;
	default:
		break;
	}
	cout << "�ļ��ˣ�" << endl;
	cout << "�ļ���ID��" << sender.ID << endl;
	cout << "�ļ���������" << sender.name << endl;
	cout << "�ļ��˵绰��" << sender.phone << endl;
	cout << "�ļ��˵�ַ��" << sender.addr << endl;
	cout << "����ʱ�䣺" << sendTime.wYear << "��" << sendTime.wMonth << "��" << sendTime.wDay << "��" << sendTime.wHour << "ʱ" << sendTime.wMinute << "��" << endl;
	cout << "���ԱID��" << postman.ID << endl;
	cout << "���Ա������" << postman.name << endl;
	cout << "���Ա�绰��" << postman.phone << endl;
	if (state != NOT_PICK)
		cout << "����ʱ�䣺" << pickUpTime.wYear << "��" << pickUpTime.wMonth << "��" << pickUpTime.wDay << "��" << pickUpTime.wHour << "ʱ" << pickUpTime.wMinute << "��" << endl;
	cout << "�ռ��ˣ�" << endl;
	cout << "�ռ���ID��" << recver.ID << endl;
	cout << "�ռ���������" << recver.name << endl;
	cout << "�ռ��˵绰��" << recver.phone << endl;
	cout << "�ռ��˵�ַ��" << recver.addr << endl;
	if (state == RECEIVE)
		cout << "����ʱ�䣺" << recvTime.wYear << "��" << recvTime.wMonth << "��" << recvTime.wDay << "��" << recvTime.wHour << "ʱ" << recvTime.wMinute << "��" << endl;
	cout << "�������:";
	switch (type)
	{
	case NORMAL:
		cout << "��ͨ���" << endl;
		break;
	case FRAGILE:
		cout << "����Ʒ" << endl;
		break;
	case BOOK:
		cout << "�鼮" << endl;
		break;
	default:
		break;
	}
	if (type == FRAGILE || type == NORMAL)
		cout << "���������" << amount << "kg" << endl;
	else if (type == BOOK)
		cout << "����鼮������" << amount << "��" << endl;
	cout << "�˷ѣ�" << price << "Ԫ" << endl;

	cout << "������飺" << endl;
	cout << packInfo << endl;
}

//���Ϳ��
void Pack::sendPack() const
{
	//��packList������¿��
	packList.push_back(*this);

	//������Ϣ�ļ�ĩβд���¿����Ϣ
	packInfoFile.clear();
	packInfoFile.seekg(0, ios::end);
	packInfoFile << packID << SPLIT << state << SPLIT;
	packInfoFile << sendTime.wYear << SPLIT << sendTime.wMonth << SPLIT << sendTime.wDay << SPLIT << sendTime.wHour << SPLIT << sendTime.wMinute << SPLIT;
	packInfoFile << sender.ID << SPLIT << sender.name << SPLIT << sender.addr << SPLIT << sender.phone << SPLIT;
	packInfoFile << recver.ID << SPLIT << recver.name << SPLIT << recver.addr << SPLIT << recver.phone << SPLIT;
	packInfoFile << postman.ID << SPLIT << postman.name << SPLIT << postman.phone << SPLIT;
	packInfoFile << type << SPLIT << amount << SPLIT << price << SPLIT;
	packInfoFile << packInfo << endl;

	//���Ĺ���Ա�����
	fstream tmpFile;
	string tmp;
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
}

//���տ��
void Pack::recvPack()
{
	//�Ŀ��״̬����ȡ��ǰ����ʱ��
	state = RECEIVE;
	GetSystemTime(&recvTime);

	fstream tmpFile;
	string tmp;
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
	//��ǰ�����Ϣд����ʱ�ļ�
	tmpFile << packID << SPLIT << state << SPLIT;
	tmpFile << sendTime.wYear << SPLIT << sendTime.wMonth << SPLIT << sendTime.wDay << SPLIT << sendTime.wHour << SPLIT << sendTime.wMinute << SPLIT;
	tmpFile << recvTime.wYear << SPLIT << recvTime.wMonth << SPLIT << recvTime.wDay << SPLIT << recvTime.wHour << SPLIT << recvTime.wMinute << SPLIT;
	tmpFile << sender.ID << SPLIT << sender.name << SPLIT << sender.addr << SPLIT << sender.phone << SPLIT;
	tmpFile << recver.ID << SPLIT << recver.name << SPLIT << recver.addr << SPLIT << recver.phone << SPLIT;
	tmpFile << postman.ID << SPLIT << postman.name << SPLIT << postman.phone << SPLIT;
	tmpFile << pickUpTime.wYear << SPLIT << pickUpTime.wMonth << SPLIT << pickUpTime.wDay << SPLIT << pickUpTime.wHour << SPLIT << pickUpTime.wMinute << SPLIT;
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
}

//���տ��
void Pack::pickUp()
{
	//�Ŀ��״̬����ȡ��ǰʱ����Ϊ����ʱ��
	state = NOT_RECEIVE;
	GetSystemTime(&pickUpTime);

	fstream tmpFile;
	string tmp;
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
	//��ǰ�����Ϣд����ʱ�ļ�
	tmpFile << packID << SPLIT << state << SPLIT;
	tmpFile << sendTime.wYear << SPLIT << sendTime.wMonth << SPLIT << sendTime.wDay << SPLIT << sendTime.wHour << SPLIT << sendTime.wMinute << SPLIT;
	tmpFile << sender.ID << SPLIT << sender.name << SPLIT << sender.addr << SPLIT << sender.phone << SPLIT;
	tmpFile << recver.ID << SPLIT << recver.name << SPLIT << recver.addr << SPLIT << recver.phone << SPLIT;
	tmpFile << postman.ID << SPLIT << postman.name << SPLIT << postman.phone << SPLIT;
	tmpFile << pickUpTime.wYear << SPLIT << pickUpTime.wMonth << SPLIT << pickUpTime.wDay << SPLIT << pickUpTime.wHour << SPLIT << pickUpTime.wMinute << SPLIT;
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
}

//Ϊ��ݷ�����Ա������Ϊ���Ա��Ϣ
void Pack::assignPostman(const PackPostmanInfo& postmanInfo)
{
	//�Ŀ�ݵĿ��Ա��Ϣ
	postman.ID = postmanInfo.ID;
	postman.name = postmanInfo.name;
	postman.phone = postmanInfo.phone;

	fstream tmpFile;
	string tmp;
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
	//��ǰ�����Ϣд����ʱ�ļ�
	tmpFile << packID << SPLIT << state << SPLIT;
	tmpFile << sendTime.wYear << SPLIT << sendTime.wMonth << SPLIT << sendTime.wDay << SPLIT << sendTime.wHour << SPLIT << sendTime.wMinute << SPLIT;
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
}

//ɾ����ݵĿ��Ա��Ϣ
void Pack::deletePostman()
{
	//��״̬�Ļ�δ���գ���ɾȥ�ѷ���Ŀ��Ա��Ϣ
	state = NOT_PICK;
	postman.ID = "???";
	postman.name = "???";
	postman.phone = "???";

	fstream tmpFile;
	string tmp;
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
	//��ǰ�����Ϣд����ʱ�ļ�
	tmpFile << packID << SPLIT << state << SPLIT;
	tmpFile << sendTime.wYear << SPLIT << sendTime.wMonth << SPLIT << sendTime.wDay << SPLIT << sendTime.wHour << SPLIT << sendTime.wMinute << SPLIT;
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
}

//�����ݵ��˷�
void Pack::getPrice()
{
	//��ͨ��ݣ�5Ԫ/kg
	price = amount * 5;
}

Fragile::Fragile()
{}

//���ڼļ��Ĺ��캯��
Fragile::Fragile(const string& id, const SYSTEMTIME& sTime, const PackUserInfo& send, const PackUserInfo& recv, const int& ty, const int& am, const string& info)
	: Pack(id, sTime, send, recv, ty, am, info)
{}

Fragile::~Fragile()
{}

//�����ݵ��˷�
void Fragile::getPrice()
{
	//����Ʒ��8Ԫ/kg
	price = amount * 8;
}

Book::Book()
{}

//���ڼļ��Ĺ��캯��
Book::Book(const string& id, const SYSTEMTIME& sTime, const PackUserInfo& send, const PackUserInfo& recv, const int& ty, const int& am, const string& info)
	: Pack(id, sTime, send, recv, ty, am, info)
{}

Book::~Book()
{}

//�����ݵ��˷�
void Book::getPrice()
{
	//�鼮��3Ԫ/��
	price = amount * 3;
}
