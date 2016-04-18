#include "StudentDatabase.h"

void StudentDatabase::CreateDatabase()
{
	char p;
	system("cls");
	cout << "\nВи впевнені, що хочете створити нову базу даних? Стара база даних буде знищена.";
	cout << "\n\n (1 - ТАК, 0 - НІ) -> "; cin >> p;
	if (p == '1')
	{
		database.open("database", ios_base::out | ios_base::binary);
		for (int i = 0; i < numberOfRecords; i++)
			database.write((char*)&emptyStud, sizeOf);
		database.close();
		index.open("index", ios_base::out | ios_base::binary);
		index.close();
		temp_stud.id = 0;
		student.id = 0;
		ind.record = 0;
	}
}

void StudentDatabase::Insert()
{
	system("cls");
	cout << "Будь ласка, введіть інформацію про нового студента...";
	cout << "\nID -> "; cin >> student.id;
	if (Exist(student.id) != -1)
	{
		cout << "\nСтудент з таким ID вже існує...\n\n";
		system("pause");
		return;
	}
	cout << "\nПрізвище -> "; cin >> student.surname;
	cout << "Ім\'я -> "; cin >> student.name;
	cout << "По-батькові -> "; cin >> student.father;
	cout << "\nДень народження (ДД.ММ.РРРР) -> "; cin >> student.birthday;
	getchar();
	cout << "Адреса -> "; gets_s(student.address);
	cout << "Телефон -> "; cin >> student.telephone;
	cout << "E-mail -> "; cin >> student.email;
	cout << "\nГрупа -> "; cin >> student.group;
	cout << "\nЧи має він/вона стипендію? (1 - ТАК, 0 - НІ) -> "; cin >> student.hasScolarship;

	InsertToBlock(GetBlockFromIndex(student.id) * sizeOf);
}

void StudentDatabase::Edit()
{
	system("cls");
	cout << "Введіть ID студента, дані про якого ви хочете редагувати...";
	cout << "\n\nID -> "; cin >> student.id;
	if ((position1 = Exist(student.id)) == -1)
	{
		cout << "\nСтудента з таким ID в базі даних не виявлено...\n\n";
		system("pause");
		return;
	}
	cout << "\nБудь ласка, введіть нову інформацію про студента...";
	cout << "\n\nПрізвище -> "; cin >> student.surname;
	cout << "Ім\'я -> "; cin >> student.name;
	cout << "По-батькові -> "; cin >> student.father;
	cout << "\nДень народження (ДД.ММ.РРРР) -> "; cin >> student.birthday;
	getchar();
	cout << "Адреса -> "; gets_s(student.address);
	cout << "Телефон -> "; cin >> student.telephone;
	cout << "E-mail -> "; cin >> student.email;
	cout << "\nГрупа -> "; cin >> student.group;
	cout << "\nЧи має він/вона стипендію? (1 - ТАК, 0 - НІ) -> "; cin >> student.hasScolarship;

	database.open("database", ios_base::in | ios_base::out | ios_base::binary);
	database.seekp(position1, ios_base::beg);
	database.write((char*)&student, sizeOf);
	database.close();
}

void StudentDatabase::Delete()
{
	system("cls");
	cout << "Введіть ID студента, дані про якого ви хочете видалити...";
	cout << "\n\nID -> "; cin >> student.id;
	if ((position1 = Exist(student.id)) == -1)
	{
		cout << "\nСтудента з таким ID в базі даних не виявлено...\n\n";
		system("pause");
		return;
	}
	database.open("database", ios_base::in | ios_base::out | ios_base::binary);
	database.seekp(position1, ios_base::beg);
	database.write((char*)&emptyStud, sizeOf);
	database.close();
	RepairBlock(GetBlockFromIndex(student.id) * sizeOf);
}

int StudentDatabase::GetBlockFromIndex(int id)
{
	index.open("index", ios_base::in | ios_base::binary);
	index.read((char*)&ind, sizeof(ind));
	while (!index.eof())
	{
		if (ind.id > id)
		{
			position1 = index.tellg();
			position1 -= sizeof(ind);
			if (position1 == 0)
			{
				index.close();
				return ind.record;
			}
			else
			{
				index.clear();
				index.seekg(position1 - sizeof(ind), ios_base::beg);
				index.read((char*)&ind, sizeof(ind));
				index.close();
				return ind.record;
			}
		}
		index.read((char*)&ind, sizeof(ind));
	}
	if (index.eof())
	{
		index.close();
		return ind.record;
	}
}

void StudentDatabase::InsertToBlock(long startPosition)
{
	database.open("database", ios_base::in | ios_base::out | ios_base::binary);
	if (CountRecordsInBlock(startPosition) != numberOfRecords)
	{
		database.clear();
		database.seekg(startPosition, ios_base::beg);
		st.erase(st.begin(), st.end());
		database.read((char*)&temp_stud, sizeOf);
		while (temp_stud.id != 0)
		{
			st.push_back(temp_stud);
			database.read((char*)&temp_stud, sizeOf);
		}
		st.push_back(student);
		sort(st.begin(), st.end(), [](const Student &st1, const Student &st2) { return st1.id < st2.id; });
		database.clear();
		database.seekp(startPosition, ios_base::beg);
		for (short i = 0; i < st.size(); i++)
		{
			database.write((char*)&st[i], sizeOf);
		}
		if(st[0].id == student.id)
			ChangeIdInIndex(st[0].id);
		database.close();
	}
	else
	{
		position1 = AddEmptyBlock();
		position2 = startPosition + (numberOfRecords / 2 * sizeOf);

		for (int i = 0; i <= numberOfRecords / 2; i++)
		{
			database.clear();
			database.seekg(position2 + i * sizeOf, ios_base::beg);
			database.read((char*)&temp_stud, sizeOf);
			database.seekp(position1 + i * sizeOf, ios_base::beg);
			database.write((char*)&temp_stud, sizeOf);
		}
		database.clear();
		database.seekp(position2, ios_base::beg);
		for (int i = 0; i <= numberOfRecords / 2; i++)
			database.write((char*)&emptyStud, sizeOf);
		database.clear();
		database.seekg(position1, ios_base::beg);
		database.read((char*)&temp_stud, sizeOf);
		InsertIndex(temp_stud.id, position1 / sizeOf);
		database.close();
		InsertToBlock(GetBlockFromIndex(student.id) * sizeOf);
	}
}

short StudentDatabase::CountRecordsInBlock(long startPosition)
{
	short i = 0;
	database.seekg(startPosition, ios_base::beg);
	database.read((char*)&temp_stud, sizeOf);
	while (temp_stud.id != 0 && !database.eof())
	{
		i++;
		database.read((char*)&temp_stud, sizeOf);
	}
	if (i == 0)
	{
		database.read((char*)&temp_stud, sizeOf);
		if (temp_stud.id != 0)
			i++;
	}
	return i;
}

void StudentDatabase::InsertIndex(int id, int record)
{
	bool p = false;
	index.open("index", ios_base::in | ios_base::binary);
	tmp.open("tmp", ios_base::out | ios_base::binary);
	index.read((char*)&ind, sizeof(ind));
	while (!index.eof())
	{
		if (ind.id < id)
		{
			tmp.write((char*)&ind, sizeof(ind));
			index.read((char*)&ind, sizeof(ind));
		}
		else
		{
			p = true;
			ind.id = id;
			ind.record = record;
			tmp.write((char*)&ind, sizeof(ind));
			position1 = index.tellg();
			position1 -= sizeof(ind);
			index.read((char*)&ind, sizeof(ind));
			while (!index.eof())
			{
				tmp.write((char*)&ind, sizeof(ind));
				index.read((char*)&ind, sizeof(ind));
			}
		}
	}
	if (!p)
	{
		ind.id = id;
		ind.record = record;
		tmp.write((char*)&ind, sizeof(ind));
	}
	index.close();
	tmp.close();
	system("del index");
	system("rename tmp index");
}

void StudentDatabase::ChangeIdInIndex(int id)
{
	index.open("index", ios_base::in | ios_base::out | ios_base::binary);
	index.read((char*)&ind, sizeof(ind));
	while (!index.eof())
	{
		if (ind.id > id)
		{
			position1 = index.tellg();
			position1 -= sizeof(ind);
			if (position1 == 0)
			{
				ind.id = id;
				index.clear();
				index.seekp(position1, ios_base::beg);
				index.write((char*)&ind, sizeof(ind));
				index.close();
				return;
			}
			else
			{
				index.clear();
				index.seekg(position1 - sizeof(ind), ios_base::beg);
				index.read((char*)&ind, sizeof(ind));
				ind.id = id;
				index.clear();
				index.seekp(position1 - sizeof(ind), ios_base::beg);
				index.write((char*)&ind, sizeof(ind));
				index.close();
				return;
			}
		}
		index.read((char*)&ind, sizeof(ind));
	}
	if (index.eof())
	{
		ind.id = id;
		index.clear();
		index.seekp(0, ios_base::beg);
		index.write((char*)&ind, sizeof(ind));
		index.close();
		return;
	}
}

void StudentDatabase::PrintToFile()
{
	int i = 0;
	tmp.open("database.txt", ios_base::out);
	tmp << left << setw(6) << "Номер"
		<< left << setw(4) << "ID"
		<< left << setw(20) << "Прізвище"
		<< left << setw(20) << "Ім\'я"
		<< left << setw(20) << "По-батькові"
		<< left << setw(17) << "День народження"
		<< left << setw(40) << "Адреса"
		<< left << setw(16) << "Телефон"
		<< left << setw(25) << "E-mail"
		<< left << setw(10) << "Група"
		<< left << setw(10) << "Стипендія\n\n";
	database.open("database", ios_base::in | ios_base::binary);
	database.read((char*)&student, sizeOf);
	while (!database.eof())
	{
		tmp << left << setw(6) << i
			<< left << setw(4) << student.id
			<< left << setw(20) << student.surname
			<< left << setw(20) << student.name
			<< left << setw(20) << student.father
			<< left << setw(17) << student.birthday
			<< left << setw(40) << student.address
			<< left << setw(16) << student.telephone
			<< left << setw(25) << student.email
			<< left << setw(10) << student.group
			<< left << setw(10) << student.hasScolarship << "\n";
		database.read((char*)&student, sizeOf);
		i++;
	}
	tmp.close();
	database.close();
	system("explorer.exe database.txt");
}

void StudentDatabase::PrintIndex()
{
	system("cls");
	index.open("index", ios_base::in | ios_base::binary);
	index.read((char*)&ind, sizeof(ind));
	while (!index.eof())
	{
		cout << "\n\nID: " << ind.id
			<< "\nAddress: " << ind.record;
		index.read((char*)&ind, sizeof(ind));
	}
	index.close();
	cout << "\n\n";
	system("pause");
}

long StudentDatabase::AddEmptyBlock()
{
	database.clear();
	database.seekp(0, ios_base::end);
	position1 = database.tellp();
	for (int i = 0; i < numberOfRecords; i++)
		database.write((char*)&emptyStud, sizeOf);
	return position1;
}

long StudentDatabase::Exist(int id)
{
	position1 = -1;
	position1 = isInBlock(GetBlockFromIndex(id) * sizeOf, id);
	return position1;
}

long StudentDatabase::isInBlock(long startPosition, int id)
{
	database.open("database", ios_base::in | ios_base::binary);
	position1 = -1;
	database.seekg(startPosition, ios_base::beg);
	for (int i = 0; i < numberOfRecords; i++)
	{
		database.read((char*)&temp_stud, sizeOf);
		if (temp_stud.id == id)
		{
			position1 = database.tellg();
			position1 -= sizeOf;
			database.close();
			return position1;
		}
	}
	database.close();
	return position1;
}

void StudentDatabase::RepairBlock(long startPosition)
{
	database.open("database", ios_base::in | ios_base::out | ios_base::binary);
	if (CountRecordsInBlock(startPosition) != 0)
	{
		database.clear();
		database.seekg(startPosition, ios_base::beg);
		st.erase(st.begin(), st.end());
		database.read((char*)&temp_stud, sizeOf);
		while (temp_stud.id != 0)
		{
			st.push_back(temp_stud);
			database.read((char*)&temp_stud, sizeOf);
		}
		database.read((char*)&temp_stud, sizeOf);
		if (database.eof())
		{
			database.close();
			return;
		}
		else if (temp_stud.id != 0)
		{
			while (temp_stud.id != 0 && !database.eof())
			{
				st.push_back(temp_stud);
				database.read((char*)&temp_stud, sizeOf);
			}
		}
		database.clear();
		database.seekp(startPosition, ios_base::beg);
		for (short i = 0; i < st.size(); i++)
		{
			database.write((char*)&st[i], sizeOf);
		}
		database.write((char*)&emptyStud, sizeOf);
		if (st[0].id > student.id)
			ChangeIdInIndex(st[0].id, startPosition / sizeOf);
		database.close();
	}
	else
	{
		database.close();
		DeleteEmptyBlock();
		RepairIndex();
	}
}

void StudentDatabase::ChangeIdInIndex(int id, int record)
{
	index.open("index", ios_base::in | ios_base::out | ios_base::binary);
	position1 = 0;
	index.read((char*)&ind, sizeof(ind));
	while (!index.eof())
	{
		if (ind.record == record)
		{
			ind.id = id;
			index.seekp(position1, ios_base::beg);
			index.write((char*)&ind, sizeof(ind));
			index.close();
			return;
		}
		position1 = index.tellg();
		index.read((char*)&ind, sizeof(ind));
	}
}

void StudentDatabase::DeleteEmptyBlock()
{
	database.open("database", ios_base::in | ios_base::binary);
	tmp.open("tmp", ios_base::out | ios_base::binary);
	position1 = 0;
	database.read((char*)&temp_stud, sizeOf);
	while (!database.eof())
	{
		if (temp_stud.id != 0 && (position1 == 0 || (position1 / sizeOf) % numberOfRecords == 0))
		{
			for (int i = 0; i < numberOfRecords; i++)
			{
				tmp.write((char*)&temp_stud, sizeOf);
				database.read((char*)&temp_stud, sizeOf);
			}
			if (database.eof())
				break;
			position1 = database.tellg();
			position1 -= sizeOf;
			database.clear();
			database.seekg(position1, ios_base::beg);
		}
		position1 = database.tellg();
		database.read((char*)&temp_stud, sizeOf);
	}
	database.close();
	tmp.close();
	system("del database");
	system("rename tmp database");
}

void StudentDatabase::RepairIndex()
{
	database.open("database", ios_base::in | ios_base::binary);
	tmp.open("tmp", ios_base::out | ios_base::binary);
	position1 = 0;
	database.read((char*)&temp_stud, sizeOf);
	while (!database.eof())
	{
		ind.id = temp_stud.id;
		ind.record = position1 / sizeOf;
		tmp.write((char*)&ind, sizeof(ind));

		database.clear();
		position1 += numberOfRecords * sizeOf;
		database.seekg(position1, ios_base::beg);
		database.read((char*)&temp_stud, sizeOf);
	}
	database.close();
	tmp.close();
	system("del index");
	system("rename tmp index");
}

void StudentDatabase::Search()
{
	system("cls");
	cout << "\nВведіть ID студента, якого хочете знайти -> "; cin >> student.id;
	database.open("database", ios_base::in | ios_base::binary);
	if ((position1 = Exist(student.id)) == -1)
	{
		cout << "\nСтудента з таким ID в базі даних не виявлено...\n\n";
		system("pause");
		return;
	}
	database.seekg(position1, ios_base::beg);
	database.read((char*)&temp_stud, ios_base::beg);
	PrintExact(temp_stud);
	database.close();
	system("pause");

}

void StudentDatabase::PrintExact(Student student)
{
	cout << "\nID:\t\t" << student.id
		<< "\nПрізвище:\t" << student.surname
		<< "\nІм\'я:\t\t" << student.name
		<< "\nПо-батькові:\t" << student.father
		<< "\nДень народження:" << student.birthday
		<< "\nАдреса:\t\t"; puts(student.address);
	cout << "Телефон:\t" << student.telephone
		<< "\nE-mail:\t\t" << student.email
		<< "\nГрупа:\t\t" << student.group
		<< "\nСтипендія:\t" << student.hasScolarship
		<< "\n";
}