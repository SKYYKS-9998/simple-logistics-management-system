#include "clienthead.h"

extern vector<Pack> packList;

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
	string tmp, req;
	//��packList������¿��
	packList.push_back(*this);

	//�����˷����¿����Ϣ
	tmp = to_string(price) + SPLIT + packID + SPLIT + to_string(state) + SPLIT + to_string(sendTime.wYear) + SPLIT + to_string(sendTime.wMonth) + SPLIT + to_string(sendTime.wDay) + SPLIT + to_string(sendTime.wHour) + SPLIT + to_string(sendTime.wMinute) 
		+ SPLIT + sender.ID + SPLIT + sender.name + SPLIT + sender.phone + SPLIT + sender.addr + SPLIT + recver.ID + SPLIT + recver.name + SPLIT + recver.phone + SPLIT + recver.addr + SPLIT + postman.ID + SPLIT + postman.name + SPLIT + postman.phone + SPLIT 
		+ to_string(type) + SPLIT + to_string(amount) + SPLIT + to_string(price) + SPLIT + packInfo;
	req = string("1") + SPLIT + '5' + SPLIT + tmp;
	strcpy_s(send_buf, req.c_str());
	send(server, send_buf, 1000, 0);
	recv(server, recv_buf, 1000, 0);
	if (recv_buf[0] != '1')
		cout << "���ʹ���" << endl;
	else
		cout << "���ͳɹ�" << endl;
}

//���տ��
void Pack::recvPack()
{
	string tmp, req;
	state = RECEIVE;
	GetSystemTime(&recvTime);
	send(server, packID.c_str(), packID.size(), 0);

	//�����˷��ͽ��յĿ�ݺźͽ���ʱ��
	tmp = to_string(recvTime.wYear) + SPLIT + to_string(recvTime.wMonth) + SPLIT + to_string(recvTime.wDay) + SPLIT + to_string(recvTime.wHour) + SPLIT + to_string(recvTime.wMinute);
	req = string("1") + SPLIT + '6' + SPLIT + packID + SPLIT + tmp;
	strcpy_s(send_buf, req.c_str());
	send(server, send_buf, 1000, 0);
	recv(server, recv_buf, 1000, 0);
	if (recv_buf[0] != '1')
		cout << "����ʧ��" << endl;
	else
		cout << "���ճɹ�" << endl;
}

//���տ��
void Pack::pickUp()
{
	string tmp, req;
	state = NOT_RECEIVE;
	GetSystemTime(&pickUpTime);

	//�����˷������յĿ�ݺź�����ʱ��
	tmp = to_string(pickUpTime.wYear) + SPLIT + to_string(pickUpTime.wMonth) + SPLIT + to_string(pickUpTime.wDay) + SPLIT + to_string(pickUpTime.wHour) + SPLIT + to_string(pickUpTime.wMinute);
	req = string("2") + SPLIT + '3' + SPLIT + packID + SPLIT + tmp;
	strcpy_s(send_buf, req.c_str());
	send(server, send_buf, 1000, 0);

	recv(server, recv_buf, 1000, 0);
	if (recv_buf[0] != '1')
		cout << "����ʧ��" << endl;
	else
		cout << "���ճɹ�" << endl;
}

//Ϊ��ݷ�����Ա������Ϊ���Ա��Ϣ
void Pack::assignPostman(const PackPostmanInfo& postmanInfo)
{
	string tmp, req;
	//�Ŀ�ݵĿ��Ա��Ϣ
	postman.ID = postmanInfo.ID;
	postman.name = postmanInfo.name;
	postman.phone = postmanInfo.phone;
	//�����˷���Ҫ����Ŀ�ݺͿ��Ա��Ϣ
	tmp = packID + SPLIT + postmanInfo.ID + SPLIT + postmanInfo.name + SPLIT + postmanInfo.phone;
	req = string("0") + SPLIT + '6' + SPLIT + tmp;
	strcpy_s(send_buf, req.c_str());
	send(server, send_buf, 1000, 0);
	recv(server, recv_buf, 1000, 0);
	if (recv_buf[0] != '1')
		cout << "����ʧ��" << endl;
	else
		cout << "����ɹ�" << endl;
}

//ɾ����ݵĿ��Ա��Ϣ
void Pack::deletePostman()
{
	state = NOT_PICK;
	postman.ID = "???";
	postman.name = "???";
	postman.phone = "???";
	
	//�����˷���Ҫ�޸ĵĿ�ݺ�
	string req = string("0") + SPLIT + '4' + SPLIT + packID;
	strcpy_s(send_buf, req.c_str());
	send(server, send_buf, 1000, 0);
	recv(server, recv_buf, 1000, 0);
	if (recv_buf[0] != '1')
		cout << "�޸�ʧ��" << endl;
	else
		cout << "�޸ĳɹ�" << endl;
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
