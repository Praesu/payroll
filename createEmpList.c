#include <stdio.h>
#include <stdlib.h>
#define SIZE 15

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
	char ContactNo[15];
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

typedef struct {
	int empID;
	PersonalInfo info;
	char Department[15];
	char Position[15];
	int minOvertime;
	int minLate;
	int Absence;
	double PositionRate; /*annual rate*/
	double BasicSalary;	/*rate*positionrate*/
	double OvertimePay;
	double Allowance;
	double Less;
	double netAmtPd;
	Deductions deduct;
} EmployeeInfo;

typedef struct {
	EmployeeInfo employees[SIZE];
	int count;
}EmpList;

typedef int Rate;


void initList(EmpList* L);
void writeEmpList(EmpList L);
void insertLast(EmpList* L, EmployeeInfo info);

int main()
{
	EmpList L;
	
	initList(&L);
	
	EmployeeInfo a = {1, {{"LUKE", "ABRAM", "COLINA"}, "LAPULAPU", 21, {25,7,2000}, 'M', "09191919191", "colina@gmail.com"}, "HR", "MANAGER", 0, 1, 1, 1, 1, 1, 1, 1, 5, {1, 2, 3, 4, 5, 6, 7, 8}};	
	EmployeeInfo b = {2, {{"JANIE", "LANE", "SABADO"}, "CABANCALAN", 21, {1,1,2000}, 'F', "09292929292", "sabado@gmail.com"}, "HR", "MANAGER", 0, 0, 0, 100000, 20000, 0, 300, 0, 2333, {1,2,3,4,5,6,7,8}};
	EmployeeInfo c = {3, {{"HANS", "HANSEL", "CESA"}, "CABANCALAN", 22, {2,2,2000}, 'M', "09393939393", "cesa@gmail.com"}, "HR", "MANAGER", 0, 0, 0, 100000, 20000, 0, 300, 0, 4444, {1,2,3,4,5,6,7,8}};
	EmployeeInfo d = {4, {{"NATALIE", "NATALIE", "SAGNOY"}, "CEBU", 22, {3,3,2000}, 'F', "09484848484", "sagnoy@gmail.com"}, "HR", "MANAGER", 0, 0, 0, 100000, 20000, 0, 300, 0, 3535, {1,2,3,4,5,6,7,8}};
	EmployeeInfo e = {5, {{"CATE", "FRANCES", "ZAMORA"}, "TALAMBAN", 23, {4,4,2000}, 'F', "09575757575", "zamora@gmail.com"}, "HR", "MANAGER", 0, 0, 0, 100000, 20000, 0, 300, 0, 1234, {1,2,3,4,5,6,7,8}};
	
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
