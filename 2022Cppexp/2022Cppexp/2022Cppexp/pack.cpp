#include "head1.h"

extern vector<Pack> packList;
extern fstream packInfoFile;
extern fstream adminInfoFile;


Pack::Pack()
	: state(NOT_RECEIVE)
	, sendTime()
	, recvTime()
{}

//���ڶ�ȡ�ļ�����Ĺ��캯��
Pack::Pack(const string& ipackID, const int& istate, const SYSTEMTIME& isendTime, const SYSTEMTIME& irecvTime, const PackUserInfo& isender, const PackUserInfo& irecver, const string& ipackInfo)
	: packID(ipackID)
	, state(istate)
	, sendTime(isendTime)
	, recvTime(irecvTime)
	, sender(isender)
	, recver(irecver)
	, packInfo(ipackInfo)
{}

//���ڼļ��Ĺ��캯��
Pack::Pack(const string& ipackID, const SYSTEMTIME& isendTime, const PackUserInfo& isender, const PackUserInfo& irecver, const string& ipackInfo)
	: packID(ipackID)
	, state(NOT_RECEIVE)
	, sendTime(isendTime)
	, recvTime()
	, sender(isender)
	, recver(irecver)
	, packInfo(ipackInfo)
{}

Pack::~Pack()
{}

//��������Ϣ
void Pack::output() const
{
	cout << "��ݺţ�" << packID << endl;
	cout << "״̬��" << ((state == RECEIVE) ? "�ѽ���" : "δ����") << endl;
	cout << "����ʱ�䣺" << sendTime.wYear << "��" << sendTime.wMonth << "��" << sendTime.wDay << "��" << sendTime.wHour << "ʱ" << sendTime.wMinute << "��" << endl;
	//ֻ�е�����ѱ�����ʱ�����н���ʱ��
	if (state == RECEIVE)
		cout << "����ʱ�䣺" << recvTime.wYear << "��" << recvTime.wMonth << "��" << recvTime.wDay << "��" << recvTime.wHour << "ʱ" << recvTime.wMinute << "��" << endl;
	cout << "�ļ��ˣ�" << endl;
	cout << "�ļ���ID��" << sender.ID << endl;
	cout << "�ļ���������" << sender.name << endl;
	cout << "�ļ��˵绰��" << sender.phone << endl;
	cout << "�ļ��˵�ַ��" << sender.addr << endl;
	cout << "�ռ��ˣ�" << endl;
	cout << "�ռ���ID��" << recver.ID << endl;
	cout << "�ռ���������" << recver.name << endl;
	cout << "�ռ��˵绰��" << recver.phone << endl;
	cout << "�ռ��˵�ַ��" << recver.addr << endl;
	cout << "������飺" << endl;
	cout << packInfo << endl;
}

//���Ϳ��
void Pack::sendPack() const
{
	//��packList������¿��
	packList.push_back(*this);

	//������Ϣ�ļ�ĩβд���¿����Ϣ
	//�շ��͵Ŀ��һ����δ���յ�״̬�������������ʱ��
	packInfoFile.clear();
	packInfoFile.seekg(0, ios::end);
	packInfoFile << packID << SPLIT << state << SPLIT;
	packInfoFile << sendTime.wYear << SPLIT << sendTime.wMonth << SPLIT << sendTime.wDay << SPLIT << sendTime.wHour << SPLIT << sendTime.wMinute << SPLIT;
	packInfoFile << sender.ID << SPLIT << sender.name << SPLIT << sender.addr << SPLIT << sender.phone << SPLIT;
	packInfoFile << recver.ID << SPLIT << recver.name << SPLIT << recver.addr << SPLIT << recver.phone << SPLIT;
	packInfoFile << packInfo << endl;

	//���Ĺ���Ա�����
	fstream tmpFile;
	string tmp;
	int balance;
	tmpFile.open(".\\data\\tmp.txt", ios::in | ios::out | ios::trunc);
	adminInfoFile.clear();
	adminInfoFile.seekg(0);
	//��ȡ�����+15��д����ʱ�ļ�
	adminInfoFile >> balance;
	adminInfoFile.ignore(LLONG_MAX, '\n');
	balance += 15;
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
		if(tmp.length())
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
		//������ǰ������˳�ѭ��
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
