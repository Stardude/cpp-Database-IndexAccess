#include<iostream>
#include<Windows.h>
#include"StudentDatabase.h"
using namespace std;

int main()
{
	SetConsoleCP(1251);
	SetConsoleOutputCP(1251);
	short choice, M;
	while (1)
	{
		/*Головне Меню*/
		system("cls");
		cout << "\n\t1.Створити базу даних"
			<< "\n\n\t2.Додати дані про нового студента"
			<< "\n\t3.Редагувати дані про студента"
			<< "\n\t4.Видалити дані про студента"
			<< "\n\n\t5.Пошук"
			<< "\n\t6.Друкувати у файл"
			<< "\n\t7.Вивести індексний файл"
			<< "\n\n\t0.Вихід";
		cout << "\n\n\tОберіть операцію:\n";
		M:	cout << "\t -> "; cin >> choice;
		switch (choice)
		{
		case 1: StudentDatabase::CreateDatabase(); break;
		case 2:	StudentDatabase::Insert(); break;
		case 3:	StudentDatabase::Edit(); break;
		case 4:	StudentDatabase::Delete(); break;
		case 5:	StudentDatabase::Search(); break;
		case 6:	StudentDatabase::PrintToFile(); break;
		case 7: StudentDatabase::PrintIndex(); break;
		case 0:	exit(1); break;
		default: cout << "\nБудь ласка, повторіть спробу...\n"; goto M; break;
		}
	}
	cout << "\n\n";
	return 1;
}