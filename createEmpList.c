#include <stdio.h>
#include <stdlib.h>


typedef struct {
	char fname[25];
	char mname[25];
	char lname[25];
}name;

typedef struct {
	int day;
	int month;
	int year;
}date;

typedef struct {             
	name empName;
	char Address[50];
	int Age;
	date DoB;
	char Sex;
	long ContactNo;
	char email[25];
} PersonalInfo;

typedef struct {
	double SSS;
	double Philhealth;
	double PagIbig;
	double Withholding;
	double VALE;
	double SSSLoan;
	double PagIbigLoan;
	double others;
} Deductions;

typedef struct {	// {1, {{"Luke", "Abram", "Colina"}, "LapuLapu", 21, {25,7,2000}, 'M', 123123123, "colina@gmail.com"}, "HR", "Manager", 1, 1, 1, 1, 1, 1, 1, {1, 2, 3, 4, 5, 6, 7, 8}};
	int empID;
	PersonalInfo info;
	char Department[15];
	char Position[15];
	int minLate;
	int Absence;
	double PositionRate;
	double BasicSalary;
	double OvertimePay;
	double Allowance;
	double Less;
	Deductions deduct;
} EmployeeInfo;

typedef struct {
	EmployeeInfo employees[15];
	int count;
}EmpList;


void initList(EmpList* L);
void writeEmpList(EmpList L);
void insertLast(EmpList* L, EmployeeInfo info);

int main()
{
	EmpList L;
	
	initList(&L);
	
	EmployeeInfo a = {1, {{"Luke", "Abram", "Colina"}, "LapuLapu", 21, {25,7,2000}, 'M', 123123123, "colina@gmail.com"}, "HR", "Manager", 1, 1, 1, 1, 1, 1, 1, {1, 2, 3, 4, 5, 6, 7, 8}};	
	EmployeeInfo b = {2, {{"Janie", "Lane", "Sabado"}, "Cabancalan", 21, {1,1,2000}, 'F', 321321321, "sabado@gmail.com"}, "HR", "Manager", 10, 0, 1000, 20000, 1500, 300, 4000, {1,2,3,4,5,6,7,8}};
	EmployeeInfo c = {3, {{"Hans", "Hansel", "Cesa"}, "Cabancalan", 22, {2,2,2000}, 'M', 222222222, "cesa@gmail.com"}, "HR", "Manager", 10, 0, 1000, 20000, 1500, 300, 4000, {1,2,3,4,5,6,7,8}};
	EmployeeInfo d = {4, {{"Natalie", "Natalie", "Sagnoy"}, "Cebu", 22, {3,3,2000}, 'F', 333333333, "sagnoy@gmail.com"}, "HR", "Manager", 10, 0, 1000, 20000, 1500, 300, 4000, {1,2,3,4,5,6,7,8}};
	EmployeeInfo e = {5, {{"Cate", "Frances", "Zamora"}, "Talamban", 23, {4,4,2000}, 'F', 444444444, "zamora@gmail.com"}, "HR", "Manager", 10, 0, 1000, 20000, 1500, 300, 4000, {1,2,3,4,5,6,7,8}};
	
	insertLast(&L, a);
	insertLast(&L, b);
	insertLast(&L, c);
	insertLast(&L, d);
	insertLast(&L, e);
	
	writeEmpList(L);
	
	return 0;
}

void insertLast(EmpList* L, EmployeeInfo info)
{
	L->employees[L->count] = info;
	L->count++;
}

void initList(EmpList *L)
{
	L->count = 0;
}

void writeEmpList(EmpList L)
{
	FILE *fp;
	
	fp = fopen("employeeInfo.txt", "wb");
	if(fp != NULL) {
		fwrite(L.employees, sizeof(EmployeeInfo), L.count, fp);
		fclose(fp);
	}
}
