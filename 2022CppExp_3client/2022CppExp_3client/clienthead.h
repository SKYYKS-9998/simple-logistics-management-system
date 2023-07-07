#pragma once
#include <WinSock2.h>
#include <Ws2tcpip.h>
#pragma comment(lib, "WS2_32.lib")
#include <iostream>
#include <string>
#include <windows.h>
#include <vector>
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

/*
* ��ݼ�/�ռ�����Ϣ
* �û�ID
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

//�����
class Pack
{
public:
	Pack();
	//���ڶ�ȡ�ļ�����Ĺ��캯��
	Pack(const string& ipackID, const int& istate, const SYSTEMTIME& isendTime, const SYSTEMTIME& irecvTime, const PackUserInfo& isender, const PackUserInfo& irecver, const PackPostmanInfo& ipostman, const SYSTEMTIME& ipickUpTime, const int& itype, const int& iamount, const int& iprice, const string& ipackInfo);
	//���ڼļ��Ĺ��캯��
	Pack(const string& ipackID, const SYSTEMTIME& isendTime, const PackUserInfo& isender, const PackUserInfo& irecver, const int& itype, const int& iamount, const string& ipackInfo);
	~Pack();

	void output() const;	//��������Ϣ
	void sendPack() const;	//���Ϳ��
	void recvPack();		//���տ��
	void pickUp();			//���տ��
	void assignPostman(const PackPostmanInfo& postmanInfo);	//Ϊ��ݷ�����Ա������Ϊ���Ա��Ϣ
	void deletePostman();	//ɾ����ݵĿ��Ա��Ϣ
	virtual void getPrice();//�����ݵ��˷�
public:
	string packID;			//���ID���ɼļ����û�ID�ͷ���ʱ�乹��
	int state;				//���״̬����Ϊδ���ա�δ���ա��ѽ�������״̬
	SYSTEMTIME sendTime;	//����ʱ��
	SYSTEMTIME recvTime;	//����ʱ��
	PackUserInfo sender;	//�ļ�����Ϣ
	PackUserInfo recver;	//�ռ�����Ϣ
	PackPostmanInfo postman;//���Ա��Ϣ
	SYSTEMTIME pickUpTime;	//����ʱ��
	int type;				//�������
	int amount;				//�������/����
	int price;				//����˷�
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
	void loadUserPackList() const;				//���ļ��ж�ȡ�뵱ǰ�û���صĿ�ݣ���������packList��
	void userInterface();						//�û�����
	void userInfoInterface();					//�û����������Ϣ����
	void userPackInterface();					//�û�����������Ϣ����

	void changePW();							//��������
	void checkBalance() const;					//�鿴���
	void topUp();								//��ֵ
	void regis(const string& id);				//���û���¼ʱʹ�ò������û���ʱʹ�ã�ע�������ID
	void login(const string& id);				//���û�ע��ʱʹ���Ѵ����û���ʱʹ�ã���¼�����ID
	int checkPwValid(const string& newPw) const;//���newPw�Ƿ���������ʽҪ�����򷵻�1�����򷵻�0

	void sendPack();							//���Ϳ��
	void recvPack() const;						//���տ��
	void searchPack() const;					//��ѯ���
	void loadSenderInfo(PackUserInfo& sender) const;	//���ؼļ�����Ϣ
	void loadRecverInfo(PackUserInfo& recver) const;	//�����ռ�����Ϣ
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
	void searchPackByState() const;				//���ݿ��״̬��ѯ���
	int checkUnrecvPackIn() const;				//����ĸ���ǰ�û���δ���տ��
	int checkUnpickPackIn() const;				//����ĸ���ǰ�û���δ���տ��
	int checkRecvPackIn() const;				//����ĸ���ǰ�û����ѽ��տ��
	int checkUnrecvPackOut() const;				//�����ǰ�û��ĳ���δ���տ��
	int checkUnpickPackOut() const;				//�����ǰ�û��ĳ���δ���տ��
	int checkRecvPackOut() const;				//�����ǰ�û��ĳ����ѽ��տ��

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

	void login();		//��¼
	
private:
	void loadAllUserList() const;		//����Ա��Ȩ�鿴�����û�����ǰ���ļ��ж�ȡ���������û���Ϣ�洢������allUserList
	void loadAllPackList() const;		//����Ա��Ȩ�鿴���п�ݣ���ǰ���ļ��ж�ȡ�������п����Ϣ�洢������packList
	void loadAllPostmanList() const;	//����Ա��Ȩ�鿴���п��Ա����ǰ���ļ��ж�ȡ�������п��Ա��Ϣ�洢��allPostmanList
	void adminInterface() const;		//����Ա����
	void adminUserInterface() const;	//����Ա�����û���Ϣ����
	void adminPackInterface() const;	//����Ա��������Ϣ����
	void adminPostmanInterface() const;	//����Ա������Ա��Ϣ����

	void checkAllUser() const;			//�鿴�����û�
	void searchUser() const;			//��ѯ�û�
	void searchUserByID() const;		//���û�ID�����û�
	void searchUserByName() const;		//���û����������û�
	void searchUserByPhone() const;		//���û��绰�����û�
	void searchUserByAddr() const;		//���û���ַ�����û�

	void checkAllPack() const;				//�鿴���п��
	void checkBalance() const;				//��ѯ���
	void searchPack() const;				//��ѯ���
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
	void searchPackByState() const;			//���ݿ��״̬��ѯ���
	int outputUnrecvPack() const;			//���δ���տ��
	int outputUnpickPack() const;			//���δ���տ��
	int outputRecvPack() const;				//����ѽ��տ��

	void checkAllPostman() const;						//�鿴���п��Ա
	void addPostman() const;							//�������Ա
	void deletePostman() const;							//ɾ�����Ա
	int searchPostman(const string& id) const;			//�鿴IDΪ����id�Ŀ��Ա�Ƿ���ڣ����򷵻���allPostman�������±꣬���򷵻�-1
	int checkPwValid(const string& newPw) const;		//���������������Ƿ���ϸ�ʽҪ�����򷵻�1�����򷵻�0
	void addPostmanToFile() const;						//����Ա��Ϣ�ļ���д���¿��Ա��Ϣ
	void deletePostmanFromFile(const string& id) const;	//�ӿ��Ա��Ϣ�ļ�ɾ��IDΪ����id�Ŀ��Ա��Ϣ
	void assignPack() const;							//Ϊ��ݷ�����Ա
	int checkUnassignPack() const;						//���δ������Ա�Ŀ��
	
	void searchPostman() const;				//��ѯ���Ա
	void searchPostmanByID() const;			//��ID��ѯ���Ա
	void searchPostmanByName() const;		//��������ѯ���Ա
	void searchPostmanByPhone() const;		//�Ե绰��ѯ���Ա

	void showAllPackAssignedToOne() const;						//��������ָ��һ�����Ա�����п��
	void searchPackAssignedToOne() const;						//��ָ����һ�����Ա����Ŀ���в��ҿ��
	void searchAssignPackBySender(const string& id) const;		//���ݼļ�����Ϣ��ѯ���
	void searchAssignPackBySenderID(const string& id) const;	//���ݼļ����û�ID��ѯ���
	void searchAssignPackBySenderName(const string& id) const;	//���ݼļ���������ѯ���
	void searchAssignPackBySenderPhone(const string& id) const;	//���ݼļ��˵绰��ѯ���
	void searchAssignPackBySenderAddr(const string& id) const;	//���ݼļ��˵�ַ��ѯ���
	void searchAssignPackByRecver(const string& id) const;		//�����ռ�����Ϣ��ѯ���
	void searchAssignPackByRecverID(const string& id) const;	//�����ռ����û�ID��ѯ���
	void searchAssignPackByRecverName(const string& id) const;	//�����ռ���������ѯ���
	void searchAssignPackByRecverPhone(const string& id) const;	//�����ռ��˵绰��ѯ���
	void searchAssignPackByRecverAddr(const string& id) const;	//�����ռ��˵�ַ��ѯ���
	void searchAssignPackBySendTime(const string& id) const;	//���ݷ���ʱ���ѯ���
	void searchAssignPackByPackID(const string& id) const;		//���ݿ�ݺŲ�ѯ���
	void searchAssignPackByState(const string& id) const;		//���ݿ��״̬��ѯ���
	int outputUnrecvAssignPack(const string& id) const;			//���δ���տ��
	int outputUnpickAssignPack(const string& id) const;			//���δ���տ��
	int outputRecvAssignPack(const string& id) const;			//����ѽ��տ��

public:
	string ID;		//����ԱID
	string name;	//����Ա����
	int balance;	//���

private:
	string pw;		//����Ա����
};

//���Ա��
class Postman 
{
public:
	Postman();
	//���ļ���������Ա��ӿ��Աʱʹ�õĹ��캯��
	Postman(const string& id, const string& ipw, const string& iname, const string& iphone, const int& ibalance);
	~Postman();

	void login();		//��¼
	void output() const;//������Ա��Ϣ

private:
	void loadPostmanPackList() const;	//���ļ���ȡ��ǰ���Ա����Ŀ�ݣ��洢��packList��
	void postmanInterface();			//���Ա����

	void pickUp();				//���տ��
	void checkBalance() const;	//�鿴���

	void searchPack() const;					//��ѯ���
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
	void searchPackByState() const;				//���ݿ��״̬��ѯ���
	int outputUnrecvPack() const;				//���δ���տ��
	int outputUnpickPack() const;				//���δ���տ��
	int outputRecvPack() const;					//����ѽ��տ��

	void updateBalanceInFile() const;			//���տ�ݺ��ڿ��Ա��Ϣ�ļ��и������

public:
	string ID;		//���ԱID
	string pw;		//����
	string name;	//����
	string phone;	//�绰
	int balance;	//���
};

//����Ʒ��
class Fragile : public Pack
{
public:
	Fragile();
	//���ڼļ��Ĺ��캯��
	Fragile(const string& id, const SYSTEMTIME& sTime, const PackUserInfo& send, const PackUserInfo& recv, const int& ty, const int& am, const string& info);
	~Fragile();

	void getPrice();	//�����ݵ��˷�
};

//�鼮��
class Book : public Pack
{
public:
	Book();
	//���ڼļ��Ĺ��캯��
	Book(const string& id, const SYSTEMTIME& sTime, const PackUserInfo& send, const PackUserInfo& recv, const int& ty, const int& am, const string& info);
	~Book();

	void getPrice();	//�����ݵ��˷�
};