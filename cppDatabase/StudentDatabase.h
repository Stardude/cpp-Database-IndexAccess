#define _CRT_SECURE_NO_WARNINGS
#define BLOCKS 5

#include <iostream>
#include <iomanip>
#include <fstream>
#include <vector>
#include <algorithm>

#include "Student.h" 
using namespace std;

static Student student;
static Student temp_stud;
static Student emptyStud;
static Index ind;
static fstream database;
static fstream index;
static fstream tmp;
static long position1;
static long position2;
static int sizeOf = sizeof(student);
static int numberOfRecords = 5;
static int sizeOfFile = numberOfRecords * BLOCKS;
static vector<Student> st;


class StudentDatabase
{
public:
	static void CreateDatabase();
	static void Insert();
	static void Edit();
	static void Delete();
	static void Search();
	static void PrintToFile();
	static void PrintIndex();
private:
	static int GetBlockFromIndex(int);
	static void InsertToBlock(long);
	static short CountRecordsInBlock(long);
	static void ChangeIdInIndex(int);
	static void ChangeIdInIndex(int, int);
	static void InsertIndex(int, int);
	static void RepairBlock(long);
	static void DeleteEmptyBlock();
	static void RepairIndex();

	static long Exist(int);
	static void PrintExact(Student);
	
	static long AddEmptyBlock();
	static long isInBlock(long, int);
};

