/*
* ignore(LLONG_MAX, end)用于跳过缓冲区大小为LLONG_MAX的空间或直到下一个end字符，包括该字符
* seek()函数之前需要用clear()函数清除文件流状态，文件流到达EOF无法使用seek()
*/

#include "head2.h"

User* user = NULL;				//存储登录中的用户的信息
Admin* admin = NULL;			//存储登录中的管理员的信息
Postman* postman = NULL;		//存储登录中的快递员的信息
vector<Pack> packList;			//相关快递列表
vector<User> allUserList;		//所有用户列表，仅管理员可用
vector<Postman> allPostmanList;	//所有快递员列表，仅管理员可用
fstream userInfoFile;			//读写用户信息文件流
fstream adminInfoFile;			//读写管理员信息文件流
fstream packInfoFile;			//读写快递信息文件流
fstream postmanInfoFile;		//读写快递员信息文件流

int main(void)
{
	//打开所需文件
	userInfoFile.open(".\\data\\userInfo.txt", ios::in | ios::out);
	if (!userInfoFile.is_open())
	{
		cout << "打开用户信息文件失败" << endl;
		system("pause");
		exit(1);
	}
	adminInfoFile.open(".\\data\\adminInfo.txt", ios::in | ios::out);
	if (!adminInfoFile.is_open())
	{
		cout << "打开管理员信息文件失败" << endl;
		system("pause");
		exit(1);
	}
	packInfoFile.open(".\\data\\packInfo.txt", ios::in | ios::out);
	if (!packInfoFile.is_open())
	{
		cout << "打开物流信息文件失败" << endl;
		system("pause");
		exit(1);
	}
	postmanInfoFile.open(".\\data\\postmanInfo.txt", ios::in | ios::out);
	if (!postmanInfoFile.is_open())
	{
		cout << "打开快递员信息文件失败" << endl;
		system("pause");
		exit(1);
	}

	char input;

	do {
		cout << "登录请按1" << endl << "新用户注册请按2" << endl << "快递员登录请按3" << endl << "管理员登录请按4" << endl << "输入q退出" << endl;;
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
			cout << "输入错误，请重新输入" << endl;
			break;
		}
	} while (input != 'q');

	userInfoFile.close();
	adminInfoFile.close();
	packInfoFile.close();

	system("pause");
	return 0;
}
