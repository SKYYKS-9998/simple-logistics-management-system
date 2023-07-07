/*
* ignore(LLONG_MAX, end)����������������СΪLLONG_MAX�Ŀռ��ֱ����һ��end�ַ����������ַ�
* seek()����֮ǰ��Ҫ��clear()��������ļ���״̬���ļ�������EOF�޷�ʹ��seek()
*/

#include "clienthead.h"

User* user = NULL;				//�洢��¼�е��û�����Ϣ
Admin* admin = NULL;			//�洢��¼�еĹ���Ա����Ϣ
Postman* postman = NULL;		//�洢��¼�еĿ��Ա����Ϣ
vector<Pack> packList;			//��ؿ���б�
vector<User> allUserList;		//�����û��б�������Ա����
vector<Postman> allPostmanList;	//���п��Ա�б�������Ա����
//���巢�ͻ������ͽ��ܻ�����
char send_buf[1000];
char recv_buf[1000];
//��������socket����������socket
SOCKET server;

int main(void)
{
	//���峤�ȱ���
	int send_len = 0;
	//����˵�ַ�ͻ��˵�ַ
	SOCKADDR_IN server_addr;
	//��ʼ��socket��
	WORD w_req = MAKEWORD(2, 2);//�汾��
	WSADATA wsadata;
	int err;
	err = WSAStartup(w_req, &wsadata);
	if (err != 0) 
	{
		cout << "��ʼ��socket��ʧ��" << endl;
	}
	else 
	{
		cout << "��ʼ��socket��ɹ�" << endl;
	}
	//���汾��
	if (LOBYTE(wsadata.wVersion) != 2 || HIBYTE(wsadata.wHighVersion) != 2) 
	{
		cout << "socket��汾�Ų���" << endl;
		WSACleanup();
	}
	else 
	{
		cout << "socket��汾��ȷ" << endl;
	}
	//���������Ϣ
	server_addr.sin_family = AF_INET;
	inet_pton(AF_INET, "127.0.0.1", &server_addr.sin_addr.S_un.S_addr);
	server_addr.sin_port = htons(5000);
	//����socket
	server = socket(AF_INET, SOCK_STREAM, 0);
	if (connect(server, (SOCKADDR*)&server_addr, sizeof(SOCKADDR)) == SOCKET_ERROR) 
	{
		cout << "����������ʧ��" << endl;
		WSACleanup();
	}
	else 
	{
		cout << "���������ӳɹ�" << endl;
		char input;
		do {
			cout << "��¼�밴1" << endl << "���û�ע���밴2" << endl << "���Ա��¼�밴3" << endl << "����Ա��¼�밴4" << endl << "����q�˳�" << endl;;
			input = getchar();
			cin.ignore(LLONG_MAX, '\n');
			switch (input)
			{
			case '1':
				user = new User;
				user->login();
				delete user;
				break;
			case '2':
				user = new User;
				user->regis();
				delete user;
				break;
			case '3':
				postman = new Postman;
				postman->login();
				delete postman;
				break;
			case '4':
				admin = new Admin;
				admin->login();
				delete admin;
				break;
			case 'q':
				strcpy_s(send_buf, "q");
				send_len = send(server, send_buf, 1000, 0);
				if (send_len < 1)
				{
					cout << "����ʧ��" << endl;
					break;
				}
				break;
			default:
				cout << "�����������������" << endl;
				break;
			}
		} while (input != 'q');
	}
	
	//�ر�socket
	closesocket(server);
	//�ͷ�DLL��Դ
	WSACleanup();

	system("pause");
	return 0;
}
