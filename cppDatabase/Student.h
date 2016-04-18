#pragma once
class Student
{
public:
	int id;
	char surname[20];
	char name[20];
	char father[20];
	char address[40];
	char birthday[12];
	char telephone[15];
	char email[25];
	char group[8];
	bool hasScolarship;
};

class Index
{
public:
	int id;
	long record;
};