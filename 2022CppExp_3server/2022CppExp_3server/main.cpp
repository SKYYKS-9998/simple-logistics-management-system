/*
* ignore(LLONG_MAX, end)����������������СΪLLONG_MAX�Ŀռ��ֱ����һ��end�ַ����������ַ�
* seek()����֮ǰ��Ҫ��clear()��������ļ���״̬���ļ�������EOF�޷�ʹ��seek()
*/

#include "serverhead.h"

fstream userInfoFile;			//��д�û���Ϣ�ļ���
fstream adminInfoFile;			//��д����Ա��Ϣ�ļ���
fstream packInfoFile;			//��д�����Ϣ�ļ���
fstream postmanInfoFile;		//��д���Ա��Ϣ�ļ���
//���巢�ͻ������ͽ��ܻ�����
char send_buf[1000];
char recv_buf[1000];
//��������socket����������socket
SOCKET server;
SOCKET client;

int main(void)
{
	//�������ļ�
	userInfoFile.open(".\\data\\userInfo.txt", ios::in | ios::out);
	if (!userInfoFile.is_open())
	{
		cout << "���û���Ϣ�ļ�ʧ��" << endl;
		system("pause");
		exit(1);
	}
	adminInfoFile.open(".\\data\\adminInfo.txt", ios::in | ios::out);
	if (!adminInfoFile.is_open())
	{
		cout << "�򿪹���Ա��Ϣ�ļ�ʧ��" << endl;
		system("pause");
		exit(1);
	}
	packInfoFile.open(".\\data\\packInfo.txt", ios::in | ios::out);
	if (!packInfoFile.is_open())
	{
		cout << "��������Ϣ�ļ�ʧ��" << endl;
		system("pause");
		exit(1);
	}
	postmanInfoFile.open(".\\data\\postmanInfo.txt", ios::in | ios::out);
	if (!postmanInfoFile.is_open())
	{
		cout << "�򿪿��Ա��Ϣ�ļ�ʧ��" << endl;
		system("pause");
		exit(1);
	}

	//���峤�ȱ���
	int recv_len = 0;
	int len = 0;
	//����˵�ַ�ͻ��˵�ַ
	SOCKADDR_IN server_addr;
	SOCKADDR_IN client_addr;
	//��ʼ��socket��
	WORD w_req = MAKEWORD(2, 2);//�汾��
	WSADATA wsadata;
	int err;
	err = WSAStartup(w_req, &wsadata);
	if (err != 0) 
		cout << "��ʼ��socket��ʧ��" << endl;
	else 
		cout << "��ʼ��socket��ɹ�" << endl;
	//���汾��
	if (LOBYTE(wsadata.wVersion) != 2 || HIBYTE(wsadata.wHighVersion) != 2) 
	{
		cout << "socket��汾�Ų���" << endl;
		WSACleanup();
	}
	else 
		cout << "socket��汾��ȷ" << endl;
	//���������Ϣ
	server_addr.sin_family = AF_INET;
	server_addr.sin_addr.S_un.S_addr = htonl(INADDR_ANY);
	server_addr.sin_port = htons(5000);
	//����socket
	server = socket(AF_INET, SOCK_STREAM, 0);
	if (bind(server, (SOCKADDR*)&server_addr, sizeof(SOCKADDR)) == SOCKET_ERROR) 
	{
		cout << "socket��ʧ��" << endl;
		WSACleanup();
	}
	else 
		cout << "socket�󶨳ɹ�" << endl;
	//����socketΪ����״̬
	if (listen(server, SOMAXCONN) < 0) 
	{
		cout << "���ü���״̬ʧ��" << endl;
		WSACleanup();
	}
	else 
		cout << "���ü���״̬�ɹ�" << endl;
	cout << "��������ڼ������ӣ����Ժ�...." << endl;
	//������������
	len = sizeof(SOCKADDR);
	
	//��������
	while (1) 
	{
		User user;
		Postman postman;
		Admin admin;
		int flag = 1;
		client = accept(server, (SOCKADDR*)&client_addr, &len);	//���տͻ�������
		if (client == SOCKET_ERROR)
		{
			cout << "����ʧ��" << endl;
			WSACleanup();
			break;
		}
		cout << "���ӽ�����׼�����տͻ�����Ϣ" << endl;
		while (1)
		{
			recv_len = recv(client, recv_buf, 1000, 0);		//�ӿͻ��˽�������ָ��
			if (recv_len < 1)
			{
				cout << "����ʧ��" << endl;
				break;
			}
			else
			{
				istringstream iss(recv_buf);
				string id, req;
				getline(iss, id, SPLIT);	//��ȡ�ͻ����û����
				getline(iss, req);			//��ȡָ��󲿷�
				cout << "�ͻ����û����:";
				switch (id[0])
				{
				case '1':
					cout << "�û�" << endl;
					user.recvFromClient(req);
					break;
				case '2':
					cout << "���Ա" << endl;
					postman.recvFromClient(req);
					break;
				case '0':
					cout << "����Ա" << endl;
					admin.recvFromClient(req);
					break;
				case 'q':
					cout << "�˳�" << endl;
					flag = 0;
					break;
				default:
					break;
				}
			}
			if (!flag)
				break;
		}
		
		closesocket(client);
	}
	//�ر�socket
	closesocket(server);
	//�ͷ�DLL��Դ
	WSACleanup();

	userInfoFile.close();
	adminInfoFile.close();
	packInfoFile.close();

	system("pause");
	return 0;
}
