#pragma once
#include <WinSock2.h>
#include <Ws2tcpip.h>
#pragma comment(lib,"WS2_32.lib")
#include <iostream>
#include <string>
#include <fstream>
#include <windows.h>
#include <sstream>

using namespace std;

#define RECEIVE 2		//�ѽ���
#define NOT_RECEIVE 1	//δ����
#define NOT_PICK 0		//δ����
#define SPLIT '.'		//�ļ����ݷָ���
#define SEND_SPLIT '|'	//�������ݷָ���
#define NORMAL 0
#define FRAGILE 1
#define BOOK 2

//���巢�ͻ������ͽ��ܻ�����
extern char send_buf[1000];
extern char recv_buf[1000];
//��������socket����������socket
extern SOCKET server;
extern SOCKET client;

/*
* �����ʽ
* x.y.z
* x=0����Ա��1�û���2���Ա
* y��������
* z��������
*/

/*
* ��ݼ�/�ռ�����Ϣ
* �û�ID
* name�û�����
* addr��ַ
* phone�绰
* */
typedef struct
{
	string ID;
	string name;
	string addr;
	string phone;
}PackUserInfo;

/*
* �������Ŀ��Ա��Ϣ
* ���ԱID
* name���Ա����
* phone���Ա�绰
*/
typedef struct 
{
	string ID;
	string name;
	string phone;
}PackPostmanInfo;

/*
* ָ������룬�������ͣ���������
* 0��id�Ƿ���ڣ�id
* 1���������룬id
* 2���û���¼��ɼ�����Ϣ��id
* 3���������룬������
* 4��������������
* 5�����Ϳ�ݣ��˷�.�����Ϣ
* 6�����տ�ݣ���ݺ�.����ʱ��
* 7�������ռ�����Ϣ��id
* 8�����û����û���Ϣ
* 9�������û���ؿ�ݣ�id
* locInFile��2��8��ֵ
*/
//�û���
class User
{
public:
	User();
	~User();

	void recvFromClient(const string& req);			//�ӿͻ����յ��û���ָ��
	
private:
	void checkExistId(const string& id) const;			//����û�ID id�Ƿ���ڣ����ڷ���1��������0
	void sendPw(const string& id) const;				//�����û�ID id������
	void getInfo(const string& id);						//�����û�ID id���û���Ϣ
	void changePW(const string& pw) const;				//�����û�ID id������
	void changeBalance(const string& balance) const;	//�����û�ID id�����
	void sendPack(const string& packInfo) const;		//���Ϳ��
	void recvPack(const string& packInfo) const;		//���տ��
	void loadPacketUserInfo(const string& id) const;	//�����û�IDΪid���ռ�����Ϣ
	void newUser(const string& userInfo);				//���ļ�������û�
	void loadUserPackList(const string& id) const;		//���ļ��ж�ȡ���û�IDΪid���û���صĿ�ݲ�����
	streampos searchID(const string& id) const;			//�����û�IDΪid���û���Ϣ���ļ��е�λ��
	
	void writeToFile(int mode, const string& info) const;			//���û���Ϣд���ļ���mode=0д�����û���Ϣ��mode=1����ĳ�û������룬mode=1����ĳ�û������

public:
	streampos locInFile;		//��ǰ��¼�ͻ��˵��û�����Ϣ���ļ���λ��
};

/*
* ָ������룬�������ͣ���������
* 0��id�Ƿ���ڣ�id
* 1���������룬id
* 2����¼��ɺ������Ϣ��id
* 3���¿��Ա�����Ա��Ϣ
* 4��ɾ����ݵĿ��Ա��Ϣ��packid
* 5��ɾ�����Ա��postid
* 6��������Ա��packid��post��Ϣ
* 7�����������û���Ϣ
* 8���������п��Ա��Ϣ
* 9���������п����Ϣ
*/
//����Ա��
class Admin
{
public:
	Admin();
	~Admin();

	void recvFromClient(const string& req) const;		//�ӿͻ����յ�����Ա��ָ��
	
private:
	void checkExistId(const string& id) const;				//������ԱID id�Ƿ���ڣ����ڷ���1��������0
	void sendPw(const string& id) const;					//���͹���ԱID id������
	void getInfo(const string& id) const;					//���͹���ԱID id����Ϣ
	void addPostman(const string& postmanInfo) const;		//���ļ��������Ա��Ϣ
	void deletePostmanFromPack(const string& packId) const;	//ɾ���ļ��п�ݵĿ��Ա��Ϣ
	void deletePostman(const string& id) const;				//���ļ���ɾ�����Ա
	void assignPack(const string& info) const;				//Ϊ��ݷ�����Ա
	void loadAllUserList() const;				//����Ա��Ȩ�鿴�����û�����ǰ���ļ��ж�ȡ���������û���Ϣ������
	void loadAllPostmanList() const;			//����Ա��Ȩ�鿴���п��Ա����ǰ���ļ��ж�ȡ�������п��Ա��Ϣ������
	void loadAllPackList() const;				//����Ա��Ȩ�鿴���п�ݣ���ǰ���ļ��ж�ȡ�������п����Ϣ������
	streampos searchID(const string& id) const;	//����IDΪid�Ĺ���Ա��Ϣ���ļ��е�λ��
};

/*
* ָ������룬�������ͣ���������
* 0��id�Ƿ���ڣ�id
* 1���������룬id
* 2����ɵ�¼������Ϣ��id
* 3�����տ�ݣ�packID.����ʱ��
* 4�������������
* 5��������ؿ�ݣ�id
* locInFile��2��ֵ
*/
//���Ա��
class Postman 
{
public:
	Postman();
	~Postman();

	void recvFromClient(const string& req);			//�ӿͻ����յ����Ա��ָ��

private:
	void checkExistId(const string& id) const;		//�����ԱID id�Ƿ���ڣ����ڷ���1��������0
	void sendPw(const string& id) const;			//���Ϳ��ԱID id������
	void getInfo(const string& id);					//���Ϳ��ԱID id���û���Ϣ
	void pickUp(const string& info) const;			//���տ��
	void updateBalanceInFile(const string& balance) const;		//�ڿ��Ա��Ϣ�ļ��и������
	void loadPostmanPackList(const string& id) const;			//���ļ���ȡ��ǰ���Ա����Ŀ�ݲ�����
	streampos searchID(const string& id) const;					//���ص�ǰ��¼�Ŀ��Ա����Ϣ���ļ��е�λ��

public:
	streampos locInFile;	//��ǰ��¼�Ŀ��Ա����Ϣ���ļ��е�λ��
};
