#pragma once
#include <iostream>
#include <string>
#include <fstream>
#include <windows.h>
#include <vector>
using namespace std;

#define RECEIVE 1		//�ѽ���
#define NOT_RECEIVE 0	//δ����
#define SPLIT '.'		//�ļ����ݷָ���

/*
* ��ݼ�/�ռ�����Ϣ
* ID�û���
* name�û�����
* addr��ַ
* phone�绰
*/
typedef struct
{
	string ID;
	string name;
	string addr;
	string phone;
}PackUserInfo;

//�����
class Pack
{
public:
	Pack();
	//���ڶ�ȡ�ļ�����Ĺ��캯��
	Pack(const string& ipackID, const int& istate, const SYSTEMTIME& isendTime, const SYSTEMTIME& irecvTime, const PackUserInfo& isender, const PackUserInfo& irecver, const string& ipackInfo);
	//���ڼļ��Ĺ��캯��
	Pack(const string& ipackID, const SYSTEMTIME& isendTime, const PackUserInfo& isender, const PackUserInfo& irecver, const string& ipackInfo);
	~Pack();

	void output() const;	//��������Ϣ
	void sendPack() const;	//���Ϳ��
	void recvPack();		//���տ��
public:
	string packID;			//��ݺţ��ɼļ����û��źͷ���ʱ�乹��
	int state;				//����״̬
	SYSTEMTIME sendTime;	//����ʱ��
	SYSTEMTIME recvTime;	//����ʱ��
	PackUserInfo sender;	//�ļ�����Ϣ
	PackUserInfo recver;	//�ռ�����Ϣ
	string packInfo;		//�������
};

//�û���
class User
{
public:
	User();
	//����ע�����ļ���ȡ����Ĺ��캯��
	User(const string& id, const string& ipw, const string& iname, const string& iphone, const string& iaddr, const int& ibalance);
	~User();

	void regis();				//ע��
	void login();				//��¼
	void output() const;		//����û���Ϣ

private:
	void loadUserPacklist();	//��ȡ����û���صĿ�ݣ���������packList��
	void userInterface();		//�û�����
	void userInfoInterface();	//�û�������Ϣ�������
	void userPackInterface();	//�û�������Ϣ�������

	void changePW();							//��������
	void checkBalance() const;					//�鿴���
	void topUp();								//��ֵ
	void regis(const string& id);				//�ڵ�¼ʱʹ�ò������û�IDʱʹ�ã�ע��id
	void login(const string& id);				//��ע��ʱʹ���Ѵ����û�IDʱʹ�ã���¼id
	int checkPwValid(const string& newPw) const;//���newPw�Ƿ���������ʽҪ�󣬷����򷵻�1�����򷵻�0

	void sendPack();							//���Ϳ��
	void recvPack() const;						//���տ��
	void searchPack() const;					//��ѯ���
	void loadPacketUserInfo(PackUserInfo& sender) const;				//���ؼļ�����Ϣ
	void loadPacketUserInfo(const streampos& loc, PackUserInfo& recver) const;	//�����ռ�����Ϣ��streampos loc�Ǽļ�����Ϣ���û���Ϣ�ļ���λ��
	void searchPackBySender() const;			//���ݼļ�����Ϣ��ѯ���
	void searchPackBySenderID() const;			//���ݼļ����û�ID��ѯ���
	void searchPackBySenderName() const;		//���ݼļ���������ѯ���
	void searchPackBySenderPhone() const;		//���ݼļ��˵绰��ѯ���
	void searchPackBySenderAddr() const;		//���ݼļ��˵�ַ��ѯ���
	void searchPackByRecver() const;			//�����ռ�����Ϣ��ѯ���
	void searchPackByRecverID() const;			//�����ռ����û�ID��ѯ���
	void searchPackByRecverName() const;		//�����ռ���������ѯ���
	void searchPackByRecverPhone() const;		//�����ռ��˵绰��ѯ���
	void searchPackByRecverAddr() const;		//�����ռ��˵�ַ��ѯ���
	void searchPackBySendTime() const;			//���ݷ���ʱ���ѯ���
	void searchPackByPackID() const;			//���ݿ�ݺŲ�ѯ���
	int checkUnrecvPack() const;				//���δ���տ��

	streampos searchID() const;					//���ļ��в��Ҹö���id���û���Ϣ���������ļ��е�λ�ã��������򷵻�-1
	streampos searchID(const string& id) const;	//���ļ��в����û�IDΪid���û���Ϣ���������ļ��е�λ�ã��������򷵻�-1
	void writeToFile(int mode) const;			//���û���Ϣд���ļ���mode=0��ʾд�����û���Ϣ��mode=1��ʾ����ԭ���û�����Ϣ

public:
	string ID;		//�û�ID
	string name;	//�û�����
	string phone;	//�û��绰
	string addr;	//�û���ַ
	int balance;	//���

private:
	string pw;		//�û�����
};

//����Ա��
class Admin
{
public:
	Admin();
	~Admin();

	void login();				//��¼
	
private:
	void adminInterface() const;		//����Ա����
	void loadAllPacklist() const;		//����Ա��Ȩ�������п�ݣ����ļ���ȡ���п��
	void loadAllUserList() const;		//����Ա��Ȩ���������û������ļ���ȡ�����û�
	void adminUserInterface() const;	//����Ա�����û���Ϣ����
	void adminPackInterface() const;	//����Ա����������Ϣ����

	void checkAllUser() const;			//�鿴�����û�
	void checkAllPack() const;			//�鿴���п��
	void checkBalance() const;			//��ѯ���
	void searchUser() const;			//��ѯ�û�
	void searchPack() const;			//��ѯ���

	void searchUserByID() const;			//���û�ID��ѯ�û�
	void searchUserByName() const;			//���û�������ѯ�û�
	void searchUserByPhone() const;			//���û��绰��ѯ�û�
	void searchUserByAddr() const;			//���û���ַ��ѯ�û�

	void searchPackBySender() const;		//���ݼļ�����Ϣ��ѯ���
	void searchPackBySenderID() const;		//���ݼļ����û�ID��ѯ���
	void searchPackBySenderName() const;	//���ݼļ���������ѯ���
	void searchPackBySenderPhone() const;	//���ݼļ��˵绰��ѯ���
	void searchPackBySenderAddr() const;	//���ݼļ��˵�ַ��ѯ���
	void searchPackByRecver() const;		//�����ռ�����Ϣ��ѯ���
	void searchPackByRecverID() const;		//�����ռ����û�ID��ѯ���
	void searchPackByRecverName() const;	//�����ռ���������ѯ���
	void searchPackByRecverPhone() const;	//�����ռ��˵绰��ѯ���
	void searchPackByRecverAddr() const;	//�����ռ��˵�ַ��ѯ���
	void searchPackBySendTime() const;		//���ݷ���ʱ���ѯ���
	void searchPackByPackID() const;		//���ݿ�ݺŲ�ѯ���

public:
	string ID;		//����ԱID
	string name;	//����Ա����
	int balance;	//����������й���Ա���õģ������洢���ļ��ĵ�һ��

private:
	string pw;		//����Ա����
};
