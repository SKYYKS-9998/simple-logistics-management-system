/*
* ignore(LLONG_MAX, end)����������������СΪLLONG_MAX�Ŀռ��ֱ����һ��end�ַ����������ַ�
* seek()����֮ǰ��Ҫ��clear()��������ļ���״̬���ļ�������EOF�޷�ʹ��seek()
*/

#include "head2.h"

User* user = NULL;				//�洢��¼�е��û�����Ϣ
Admin* admin = NULL;			//�洢��¼�еĹ���Ա����Ϣ
Postman* postman = NULL;		//�洢��¼�еĿ��Ա����Ϣ
vector<Pack> packList;			//��ؿ���б�
vector<User> allUserList;		//�����û��б�������Ա����
vector<Postman> allPostmanList;	//���п��Ա�б�������Ա����
fstream userInfoFile;			//��д�û���Ϣ�ļ���
fstream adminInfoFile;			//��д����Ա��Ϣ�ļ���
fstream packInfoFile;			//��д�����Ϣ�ļ���
fstream postmanInfoFile;		//��д���Ա��Ϣ�ļ���

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
			break;
		default:
			cout << "�����������������" << endl;
			break;
		}
	} while (input != 'q');

	userInfoFile.close();
	adminInfoFile.close();
	packInfoFile.close();

	system("pause");
	return 0;
}
