#include <stdio.h>
#include <stdlib.h>
#include <string.h>

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

typedef struct {
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
	double netAmtPd;
	Deductions deduct;
} EmployeeInfo;

typedef struct {
	EmployeeInfo employees[15];
	int count;
}EmpList;

void viewEmpList(EmpList *L);
void calculatePayroll();
void dayInputs();
void settings();

void initList(EmpList *L);
EmpList populateList();

void displayPersonalInfo(PersonalInfo e);
void displayDeductions(Deductions d);
void displayEmployeeInfo(EmployeeInfo e);
void displayEmpList(EmpList L);

void insertLast(EmpList *L, EmployeeInfo info);

void createNewEmp(EmpList *L);
void updateEmp(EmpList *L);
void deleteEmp(EmpList *L);

void selectionSort(EmpList *L);
void insertSorted(EmpList *L, EmployeeInfo e);

void writeEmpInfo(EmployeeInfo e);
void writeEmpList(EmpList L);
PersonalInfo getPersonalInfo();

int main()
{
    int a = 0;
    EmpList L;
    
	L = populateList();

    while(a != 5) {
    	a = 0;
    	printf("\n\n1. View Employee List\n");
	    printf("2. Calculate Payroll\n");
	    printf("3. End of day inputs\n");
	    printf("4. Settings\n");
	    printf("5. Exit\n");
    
        printf("\nWhat do you want to do? Enter number: ");
        scanf("%d", &a);

        switch (a) {
            case 1: 
                viewEmpList(&L);
                break;
            case 2:
                calculatePayroll();
                break;
            case 3:
                dayInputs();
                break;
            case 4:
                settings();
                break;
            case 5:
            	printf("Exiting!");
            	break;
            default:
                printf("Error, please input correctly!");
                break;
        }
    }

    return 0;
}


void initList(EmpList *L)
{
	L->count = 0;
}

void selectionSort(EmpList *L)
{
	int curr, smol, ctr;
	int end = L->count - 1;
	EmployeeInfo temp;
	
	for(curr = 0; curr < end; curr++) {
		smol = curr;
		
		for(ctr = curr + 1; ctr < L->count; ctr++) {
			if(strcmp(L->employees[ctr].info.empName.lname, L->employees[smol].info.empName.lname) == 0) {
				if(strcmp(L->employees[ctr].info.empName.fname, L->employees[smol].info.empName.fname) < 1) {
					smol = ctr;	
				}
			} else 	if(strcmp(L->employees[ctr].info.empName.lname, L->employees[smol].info.empName.lname) < 1) {
				smol = ctr;	
			}
		}
		
		if(smol != curr) {
			temp = L->employees[curr];
			L->employees[curr] = L->employees[smol];
			L->employees[smol] = temp;
		}
	}
}

void insertSorted(EmpList *L, EmployeeInfo e)
{
	int ctr;
	
	if(L->count < SIZE) {
		for(ctr = 0; ctr < L->count && strcmp(e.info.empName.lname, L->employees[ctr].info.empName.lname) > 0; ctr++) {}
		for(;strcmp(e.info.empName.lname, L->employees[ctr].info.empName.lname) == 0 && (e.info.empName.fname, L->employees[ctr].info.empName.fname) > 0; ctr++) {}
		
		L->count++;
		memcpy(L->employees + ctr + 1, L->employees + ctr, sizeof(EmployeeInfo) * L->count - ctr - 1);
		L->employees[ctr] = e;
		
	} else {
		printf("No space left!");
	}
}

EmpList populateList()
{
	FILE *fp;
	EmployeeInfo e;
	EmpList L;
	
	initList(&L);
	
	fp = fopen("employeeInfo.txt", "rb");
	if(fp != NULL) {
		while(L.count < SIZE && fread(&e, sizeof(EmployeeInfo), 1, fp) == 1) {
			insertSorted(&L, e);
		}
		
		fclose(fp);
	} else {
		printf("\nFile not found!");
	}
	return L;	
}

void displayHeader()
{
	printf("%10s", "ID");
	printf("%10s", "Name");
	printf("%10s", "Address");
	printf("%10s", "Age");
	printf("%10s", "DoB");
	printf("%10s", "Gender");
	printf("%10s", "Contact No.");
	printf("%10s", "Email");
	printf("%10s", "Department");
	printf("%10s", "Position");
	printf("%10s", "Min. Late");
	printf("%10s", "Absence");
	printf("%10s", "Position Rate");
	printf("%10s", "Basic Salary");
	printf("%10s", "Overtime pay");
	printf("%10s", "Allowance");
	printf("%10s", "Less");
	printf("%15s", "Net amount paid");
	printf("%10s", "SSS");
	printf("%10s", "Philhealth");
	printf("%10s", "PagIbig");
	printf("%10s", "Withholding");
	printf("%10s", "VALE");
	printf("%10s", "SSSLoan");
	printf("%10s", "PagIbigLoan");
	printf("%10s", "others");
	
}

void displayDeductions(Deductions d)
{
	printf("%10.2f", d.SSS);
	printf("%10.2f", d.Philhealth);
	printf("%10.2f", d.PagIbig);
	printf("%10.2f", d.Withholding);
	printf("%10.2f", d.VALE);
	printf("%10.2f", d.SSSLoan);
	printf("%10.2f", d.PagIbigLoan);
	printf("%10.2f", d.others);
}

void displayPersonalInfo(PersonalInfo e)
{
	printf("%10s, %s %s", e.empName.lname, e.empName.fname, e.empName.mname);
	printf("%10s", e.Address);
	printf("%10d", e.Age);
	printf("%10d-%d-%d", e.DoB.month, e.DoB.day, e.DoB.year);
	printf("%10c", e.Sex);
	printf("%10d", e.ContactNo);
	printf("%10s", e.email);
}

void displayEmployeeInfo(EmployeeInfo e)
{
	printf("\n%10d", e.empID);
	displayPersonalInfo(e.info);
	printf("%10s", e.Department);
	printf("%10s", e.Position);
	printf("%10d", e.minLate);
	printf("%10d", e.Absence);
	printf("%10.2f", e.PositionRate);
	printf("%10.2f", e.BasicSalary);
	printf("%10.2f", e.OvertimePay);
	printf("%10.2f", e.Allowance);
	printf("%10.2f", e.Less);
	printf("%10.2f", e.netAmtPd);
	displayDeductions(e.deduct);
}

void displayEmpList(EmpList L)
{
	int ctr;
	
	displayHeader();
	
	for(ctr = 0; ctr < L.count; ctr++) {
		displayEmployeeInfo(L.employees[ctr]);
	}
}

void viewEmpList(EmpList *L)
{
    int a = 0;
    PersonalInfo e;
    
    displayEmpList(*L);

    while(a != 4) {
    	a = 0;
    	printf("\n\n1. Create New Employee\n");
	    printf("2. Update Employee\n");
	    printf("3. Delete Employee\n");
	    printf("4. Exit\n");
    
        printf("\nWhat do you want to do? Enter number: ");
        scanf("%d", &a);

        switch (a) {
            case 1: 
                createNewEmp(L);
                break;
            case 2:
                updateEmp(L);
                break;
            case 3:
                deleteEmp(L);
                break;
            case 4:
                printf("Exiting!");
                break;
            default:
                printf("Error, please input correctly!");
                break;
        }
    }
}

void createNewEmp(EmpList *L) 
{
	char confirm;
	EmployeeInfo e;

	printf("Last name: "); 						scanf("%s", &e.info.empName.lname); 	strupr(e.info.empName.lname);		fflush(stdin); 
	printf("\nFirst name: "); 					scanf("%s", &e.info.empName.fname);		strupr(e.info.empName.fname);		fflush(stdin);
	printf("\nMiddle name: "); 					scanf("%s", &e.info.empName.mname);		strupr(e.info.empName.mname);		fflush(stdin);
	printf("\nAddress: "); 						scanf("%s", &e.info.Address);			strupr(e.info.Address);				fflush(stdin);
	printf("\nAge: "); 						 	scanf("%d", &e.info.Age);				fflush(stdin);
	printf("\nDate of Birth: [mm-dd-yyyy] "); 	scanf("%2d-%2d-%4d", &e.info.DoB.month, &e.info.DoB.day, &e.info.DoB.year);	fflush(stdin);
	printf("\nSex: [F/M] "); 					scanf("%c", &e.info.Sex);				e.info.Sex = toupper(e.info.Sex);	fflush(stdin);
	printf("\nContact number: "); 				scanf("%lf", &e.info.ContactNo);			fflush(stdin);
	printf("\nEmail: "); 						scanf("%s", &e.info.email);				fflush(stdin);
	printf("\nDepartment: "); 					scanf("%s", &e.Department); 			strupr(e.Department);				fflush(stdin);
	printf("\nPosition: "); 					scanf("%s", &e.Position); 				strupr(e.Position);					fflush(stdin);	
	printf("\nMin. Late: "); 					scanf("%d", &e.minLate);  				fflush(stdin);
	printf("\nAbsences: "); 					scanf("%d", &e.Absence);  				fflush(stdin);
	printf("\nPosition Rate: "); 				scanf("%lf", &e.PositionRate);  		fflush(stdin);
	printf("\nBasic Salary: "); 				scanf("%lf", &e.BasicSalary); 	 		fflush(stdin);
	printf("\nOvertime Pay: "); 				scanf("%lf", &e.OvertimePay); 			fflush(stdin);
	printf("\nAllowance: "); 					scanf("%lf", &e.Allowance); 			fflush(stdin);
	printf("\nLess: "); 						scanf("%lf", &e.Less); 					fflush(stdin);
	printf("\nNet Amount Paid: "); 				scanf("%lf", &e.netAmtPd); 				fflush(stdin);
	printf("\nSSS: "); 							scanf("%lf", &e.deduct.SSS); 			fflush(stdin);
	printf("\nPhilHealth: "); 					scanf("%lf", &e.deduct.Philhealth); 	fflush(stdin);
	printf("\nPag-Ibig: "); 					scanf("%lf", &e.deduct.PagIbig); 		fflush(stdin);
	printf("\nWithholding: "); 					scanf("%lf", &e.deduct.Withholding); 	fflush(stdin);
	printf("\nVALE: "); 						scanf("%lf", &e.deduct.VALE); 			fflush(stdin);
	printf("\nSSS loan: "); 					scanf("%lf", &e.deduct.SSSLoan); 		fflush(stdin);
	printf("\nPag-ibig Loan: "); 				scanf("%lf", &e.deduct.PagIbigLoan); 	fflush(stdin);
	printf("\nothers: "); 						scanf("%lf", &e.deduct.others);			fflush(stdin);
	
	printf("Are all the details correct? [Y/N] ");	
	fflush(stdin);
	scanf("%c", &confirm);
	confirm = toupper(confirm);
	
	if(confirm == 'Y') {
		e.empID = L->count + 1;
		insertSorted(L, e);
		writeEmpList(*L);
	}	
}

void writeEmpList(EmpList L)
{
	FILE *fp;
	
	fp = fopen("employeeInfo.txt", "wb+");
	
	if(fp != NULL) {
		fwrite(L.employees, sizeof(EmployeeInfo), L.count, fp);
		
		fclose(fp);
	} else {
		printf("\nFile not found!");
	}
}

void updateEmp(EmpList *L)
{
	int ctr;
	char ln[25], fn[25];
	int choice = 0;
	char ch = 'N';
	
	printf("\nLast name: "); fflush(stdin);	scanf("%s", &ln); strupr(ln);
	printf("\nFirst name: "); fflush(stdin); scanf("%s", &fn); strupr(fn);
	
	for(ctr = 0; ctr < L->count && strcmp(ln, L->employees[ctr].info.empName.lname) != 0; ctr++) {}
	if(ctr < L->count) {
		if(strcmp(ln, L->employees[ctr].info.empName.lname) == 0) {
			for( ; ctr < L->count && strcmp(fn, L->employees[ctr].info.empName.fname) != 0; ctr++){}
		}
		if(ctr < L->count) {
			displayHeader();
			displayEmployeeInfo(L->employees[ctr]);
			
			printf("\n\n1. Employee name");
			printf("\n2. Address");
			printf("\n3. Age");
			printf("\n4. Date of Birth");
			printf("\n5. Sex");
			printf("\n6. Contact Number");
			printf("\n7. Email");
			printf("\n8. Department");
			printf("\n9. Position");
			printf("\n10. Min. late");
			printf("\n11. Absence");
			printf("\n12. Position rate");
			printf("\n13. Basic salary");
			printf("\n14. Overtime pay");
			printf("\n15. Allowance");
			printf("\n16. Less");
			printf("\n17. Net amount paid");
			printf("\n18. SSS");
			printf("\n19. Philhealth");
			printf("\n20. Pag-Ibig");
			printf("\n21. Withholding");
			printf("\n22. VALE");
			printf("\n23. SSS Loan");
			printf("\n24. Pag-Ibig Loan");
			printf("\n25. Others");
			printf("\n26. Exit");
			
			while(choice != 26 ) {
				choice = 0;
				printf("\n\nWhat do you want to edit? "); fflush(stdin); scanf("%d", &choice);
				
				switch(choice) {
					case 1:
						printf("\n\nFirst name: "); fflush(stdin); scanf("%s", &L->employees[ctr].info.empName.fname);	strupr(L->employees[ctr].info.empName.fname);
						printf("\nMiddle name: "); fflush(stdin); scanf("%s", &L->employees[ctr].info.empName.mname);	strupr(L->employees[ctr].info.empName.mname);
						printf("\nLast name: "); 	fflush(stdin); scanf("%s", &L->employees[ctr].info.empName.lname);	strupr(L->employees[ctr].info.empName.lname);
						break;
					case 2:
						printf("\n\nAddress: "); fflush(stdin); scanf("%s", &L->employees[ctr].info.Address);	strupr(L->employees[ctr].info.Address);
						break;
					case 3:
						printf("\n\nAge: "); fflush(stdin); scanf("%d", &L->employees[ctr].info.Age);
						break;
					case 4:
						printf("\n\nDate of Birth: [mm-dd-yyyy] "); fflush(stdin); scanf("%d-%d-%d", &L->employees[ctr].info.DoB.month, &L->employees[ctr].info.DoB.day, &L->employees[ctr].info.DoB.year);
						break;
					case 5:
						printf("\n\nSex: [F/M] "); fflush(stdin); scanf("%c", &L->employees[ctr].info.Sex); L->employees[ctr].info.Sex = toupper(L->employees[ctr].info.Sex);
						break;
					case 6:
						printf("\n\nContact number: "); fflush(stdin); scanf("%lf", &L->employees[ctr].info.ContactNo);
						break;
					case 7:
						printf("\n\nEmail: "); fflush(stdin); scanf("%s", &L->employees[ctr].info.email);
						break;	
					case 8:
						printf("\n\nDepartment: "); fflush(stdin); scanf("%s", &L->employees[ctr].Department); strupr(L->employees[ctr].Department);
						break;
					case 9:
						printf("\n\nPosition: "); fflush(stdin); scanf("%s", &L->employees[ctr].Position);	strupr(L->employees[ctr].Position);
						break;
					case 10:
						printf("\n\nMin. Late: "); fflush(stdin); scanf("%d", &L->employees[ctr].minLate);
						break;
					case 11:
						printf("\n\nAbsence: "); fflush(stdin); scanf("%d", &L->employees[ctr].Absence);
						break;
					case 12:
						printf("\n\nPosition rate: "); fflush(stdin); scanf("%lf", &L->employees[ctr].PositionRate);
						break;
					case 13:
						printf("\n\nBasic salary: "); fflush(stdin); scanf("%lf", &L->employees[ctr].BasicSalary);
						break;
					case 14:
						printf("\n\nOvertime pay: "); fflush(stdin); scanf("%lf", &L->employees[ctr].OvertimePay);
						break;
					case 15:
						printf("\n\nAllowance: "); fflush(stdin); scanf("%lf", &L->employees[ctr].Allowance);
						break;
					case 16:
						printf("\n\nLess: "); fflush(stdin); scanf("%lf", &L->employees[ctr].Less);
						break;
					case 17:
						printf("\n\nNet amount paid: "); fflush(stdin); scanf("%lf", &L->employees[ctr].netAmtPd);
						break;
					case 18:
						printf("\n\nSSS: "); fflush(stdin); scanf("%lf", &L->employees[ctr].deduct.SSS);
						break;
					case 19:
						printf("\n\nPhilHealth: "); fflush(stdin); scanf("%lf", &L->employees[ctr].deduct.Philhealth);
						break;
					case 20:
						printf("\n\nPag-Ibig: "); fflush(stdin); scanf("%lf", &L->employees[ctr].deduct.PagIbig);
						break;
					case 21:
						printf("\n\nWithholding: "); fflush(stdin); scanf("%lf", &L->employees[ctr].deduct.Withholding);
						break;
					case 22:
						printf("\n\nVALE: "); fflush(stdin); scanf("%lf", &L->employees[ctr].deduct.VALE);
						break;
					case 23:
						printf("\n\nSSS Loan: "); fflush(stdin); scanf("%lf", &L->employees[ctr].deduct.SSSLoan);
						break;
					case 24:
						printf("\n\nPag-Ibig loan: "); fflush(stdin); scanf("%lf", &L->employees[ctr].deduct.PagIbigLoan);
						break;
					case 25:
						printf("\n\nOthers: "); fflush(stdin); scanf("%lf", &L->employees[ctr].deduct.others);
						break;
					case 26:
						printf("Exiting!");
						break;
					default:
						printf("Invalid input. Please try again!");
						break;
				}
				
				if(choice != 26) {
					printf("\n\nConfirm edit? [Y/N] "); fflush(stdin); scanf("%c", &ch);	ch = toupper(ch);
					
					if(ch == 'Y') {
						writeEmpList(*L);
						printf("\nUpdated successfully!\n\n");
						break;
					} else {
						printf("\nNot updated!\n\n");
						break;
					}
				}
			}
		} else {
			printf("\nEmployee not found!");
		}
	} else {
		printf("\n\nEmployee not found!");
	}
}

void deleteEmp(EmpList *L)
{
	int ctr;
	char ln[25], fn[25];
	char choice = 'N';
	
	printf("\nLast name: "); fflush(stdin);	scanf("%s", &ln); strupr(ln);
	printf("\nFirst name: "); fflush(stdin); scanf("%s", &fn); strupr(fn);
	
	for(ctr = 0; ctr < L->count && strcmp(ln, L->employees[ctr].info.empName.lname) != 0; ctr++) {}
	if(ctr < L->count) {
		if(strcmp(ln, L->employees[ctr].info.empName.lname) == 0) {
			for( ; ctr < L->count && strcmp(fn, L->employees[ctr].info.empName.fname) != 0; ctr++){}
		}
		if(ctr < L->count) {
			displayEmployeeInfo(L->employees[ctr]);
			
			printf("\n\nConfirm deletion? [Y/N] "); fflush(stdin); scanf("%c", &choice); choice = toupper(choice);
			
			if(choice == 'Y') {
				memcpy(L->employees + ctr, L->employees + ctr + 1, sizeof(EmployeeInfo) * L->count-ctr);
				L->count--;
				writeEmpList(*L);
			} else {
				printf("\n\nNot deleted!");
			}	
		} else {
			printf("\nEmployee not found!");
		}
	} else {
		printf("\nEmployee not found!");
	}
}

void calculatePayroll()
{
	printf("\nCalculate!");
}

void dayInputs()
{
	printf("\nDay Inputs!");
}

void settings()
{
	printf("\nSettings!");
}

