#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

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
/*-------*/
	double emprShareSSS;
	double emprSharePagIbig;
	double emprSharePhilhealth;
/*-------*/
	double totVALE;
	double totSSSLoan;
	double totPagIbigLoan;
/*-------*/
	double tempVALE;
	double tempSSSLoan;
	double tempPagIbigLoan;
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
	double BasicSalary; /*positionrate/rate*/
	double OvertimePay;
	double Allowance;
	double Less;
	double grossSalary; /*BasicSalary+OvertimePay+Allowance-Less*/
	double netAmtPd; /*grossSalary-deductions*/
	Deductions deduct;
} EmployeeInfo;

typedef struct {
	EmployeeInfo employees[15];
	int count;
}EmpList;

typedef int Rate;

typedef struct{
	Rate rate;
	char username[10];
	char password[10];
}PayrollSetting;

void viewEmpList(EmpList *L,Rate r);
void calculatePayroll(EmpList *L);
void dayInputs(EmpList *L);
void settings(EmpList *L, PayrollSetting *r);

void initList(EmpList *L); 
EmpList populateList(); 

void displayPersonalInfo(PersonalInfo e);
void displayDeductions(Deductions d);
void displayEmployeeInfo(EmployeeInfo e);
void displayEmpList(EmpList L);
void displayDayInput(EmpList L,int emp);

void createNewEmp(EmpList *L,Rate r);
void updateEmp(EmpList *L,Rate r);
void deleteEmp(EmpList *L);

void selectionSort(EmpList *L); 
void insertSorted(EmpList *L, EmployeeInfo e); 
void insertLast(EmpList *L, EmployeeInfo info);

void writeEmpInfo(EmployeeInfo e);
void writeEmpList(EmpList L);
void writeSetting(PayrollSetting r);
void calculationOvertime(EmpList *e,int count,float percentOvertime);
PayrollSetting readSetting();

void displayHeaderforCP();
void displayEmployeeInfoforCP(EmployeeInfo e);
void displayEmployeeInfoforCPv2(EmployeeInfo e);
void displayEmpListforCP(EmpList L);

void initialPayroll(EmpList *L,char yon);

void calculateSSS(EmployeeInfo *e);
void calculatePagIbig(EmployeeInfo *e);
void calculatePhilhealth(EmployeeInfo *e);
void calculateWithholdingTax(EmployeeInfo *e);


int main()
{
    int a = 0;
    int loginFlag = 0;
    EmpList L;
    char username[10] ="\0";
    char password[10] ="\0";
    PayrollSetting r;
    r = readSetting();

	do{
	printf("**************** LOGIN ******************\n");
	if(username[0]!='\0' && password[0]!='\0' && loginFlag==0){
		printf("Incorrent Username or Password.\n");
		}
		printf("\nEnter username:");
	    scanf("%s",username);
	    printf("Enter password:");
	    scanf("%s",password);
	    loginFlag = (strcmp(username,r.username)==0 && strcmp(password,r.password)==0)? 1:0;

	    system("CLS");

    }while(loginFlag==0);

    if(loginFlag==1){
    	
    printf("*************** PAYROLL SYSTEM ****************\n\n");

	L = populateList();

    while(a != 5) {    	
    	a = 0;
    	printf("\n\n1. View Employee List\n");
	    printf("2. Calculate Payroll\n");
	    printf("3. End of day inputs\n");
	    printf("4. Settings\n");
	    printf("5. Exit\n");
    
        printf("\nWhat do you want to do? Enter number: ");
        fflush(stdin);
        scanf("%d", &a);

        switch (a) {
            case 1: 
    			system("cls");
                viewEmpList(&L,r.rate);
                break;
            case 2:
    			system("cls");
                calculatePayroll(&L);
                break;
            case 3:
                dayInputs(&L);
                break;
            case 4:
                settings(&L, &r);
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
	printf("%-5s", "ID");
	printf("%22s %-21s", "Name"," ");
	printf("%-23s", "Address");
	printf("%-4s", "Age");
	printf("%-10s", "DoB");
	printf("%-4s", "Sex");
	printf("%-15s", "Contact No.");
	printf("%-25s", "Email");
	printf("%-8s", "Dept");
	printf("%-12s", "Position");
	printf(" %-10s", "Position Rate");
}

void displayPersonalInfo(PersonalInfo e)
{
	printf("%24s,",e.empName.lname);
	printf("%-16s",e.empName.fname);
	printf("%-3c",e.empName.mname[0]);
	printf("%-23s", e.Address);
	printf("%-4d", e.Age);
	printf("%02d-%02d-%04d", e.DoB.month, e.DoB.day, e.DoB.year);
	printf("%3c", e.Sex);
	printf(" %-14s", e.ContactNo);
	printf(" %-24s", e.email);
}

void displayEmployeeInfo(EmployeeInfo e)
{
	char position[12];
	char department[8];
	
	strcpy(position,e.Position);
	strcpy(department,e.Department);
	
	printf("\n%-5d", e.empID);
	displayPersonalInfo(e.info);
	printf(" %-8s", department);
	printf("%-12s ", position);
	printf("P%10.2f", e.PositionRate);
}

void displayEmpList(EmpList L)
{
	int ctr;
	
	displayHeader();
	
	for(ctr = 0; ctr < L.count; ctr++) {
		displayEmployeeInfo(L.employees[ctr]);
	}
}

void viewEmpList(EmpList *L,Rate r)
{
    int a = 0;
    PersonalInfo e;
    
    while(a != 4) {
		printf("View Employee List\n\n");
		
   		displayEmpList(*L);
    	a = 0;
    	printf("\n\n1. Create New Employee\n");
	    printf("2. Update Employee\n");
	    printf("3. Delete Employee\n");
	    printf("4. Exit\n");
    
        printf("\nWhat do you want to do? Enter number: ");
        scanf("%d", &a);

        switch (a) {
            case 1: 
                createNewEmp(L,r);
                system("cls");
                break;
            case 2:
                updateEmp(L,r);
                system("cls");
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

void createNewEmp(EmpList *L,Rate r) 
{
	char confirm;
	EmployeeInfo e;
	fflush(stdin);
	printf("Last name: "); 						scanf("%[^\n]s", &e.info.empName.lname); 	strupr(e.info.empName.lname);		fflush(stdin); 
	printf("\nFirst name: "); 					scanf("%[^\n]s", &e.info.empName.fname);		strupr(e.info.empName.fname);		fflush(stdin);
	printf("\nMiddle name: "); 					scanf("%[^\n]s", &e.info.empName.mname);		strupr(e.info.empName.mname);		fflush(stdin);
	printf("\nAddress: "); 						scanf("%[^\n]s", &e.info.Address);			strupr(e.info.Address);				fflush(stdin);
	printf("\nAge: "); 						 	scanf("%d", &e.info.Age);				fflush(stdin);
	printf("\nDate of Birth: [mm-dd-yyyy] "); 	scanf("%2d-%2d-%4d", &e.info.DoB.month, &e.info.DoB.day, &e.info.DoB.year);	fflush(stdin);
	printf("\nSex: [F/M] "); 					scanf("%c", &e.info.Sex);				e.info.Sex = toupper(e.info.Sex);	fflush(stdin);
	printf("\nContact number: "); 				scanf("%[^\n]s", &e.info.ContactNo);			fflush(stdin);
	printf("\nEmail: "); 						scanf("%[^\n]s", &e.info.email);				fflush(stdin);
	printf("\nDepartment: "); 					scanf("%[^\n]s", &e.Department); 			strupr(e.Department);				fflush(stdin);
	printf("\nPosition: "); 					scanf("%[^\n]s", &e.Position); 				strupr(e.Position);	
	printf("\nPosition Rate: "); 				scanf("%lf", &e.PositionRate);  		fflush(stdin);
	
	printf("Are all the details correct? [Y/N] ");	
	fflush(stdin);
	scanf("%c", &confirm);
	confirm = toupper(confirm);
	
	if(confirm == 'Y') {
		e.empID = L->count + 1;
		e.minLate = 0;	e.Absence = 0;	e.BasicSalary=e.PositionRate/r;	e.OvertimePay=0;   e.Less=0;
		e.Allowance=0; 	e.netAmtPd=0; 	e.deduct.others=0;	e.deduct.PagIbig=0; 	e.deduct.PagIbigLoan=0;
		e.deduct.Philhealth=0;			e.deduct.SSS=0;		e.deduct.SSSLoan=0;		e.deduct.VALE=0;
		e.deduct.Withholding=0;			e.minOvertime=0;	e.grossSalary=0;		e.deduct.emprSharePagIbig=0;
		e.deduct.emprSharePhilhealth=0;	e.deduct.emprShareSSS=0;	e.deduct.totPagIbigLoan=0;
		e.deduct.totSSSLoan=0;			e.deduct.totVALE=0;			e.deduct.tempVALE=0;
		e.deduct.tempSSSLoan=0;			e.deduct.tempPagIbigLoan=0;
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

void updateEmp(EmpList *L,Rate r)
{
	int ctr;
	char ln[25], fn[25];
	int choice = 0;
	char ch = 'N';
	char hold;
	
	printf("\nLast name: "); fflush(stdin);	scanf("%[^\n]s", &ln); strupr(ln);
	printf("\nFirst name: "); fflush(stdin); scanf("%[^\n]s", &fn); strupr(fn);
	
	system("cls");
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
			printf("\n13. Allowance");	
			printf("\n14. VALE Total");
			printf("\n15. SSS Loan Total");
			printf("\n16. Pag-Ibig Loan Total");
			printf("\n17. Other Deductions");
			printf("\n18. Exit");
			
			while(choice != 18 ) {
				choice = 0;
				printf("\n\nWhat do you want to edit? "); fflush(stdin); scanf("%d", &choice);
				
				switch(choice) {
					case 1:
						printf("\n\nFirst name: "); fflush(stdin); scanf("%[^\n]s", &L->employees[ctr].info.empName.fname);	strupr(L->employees[ctr].info.empName.fname);
						printf("\nMiddle name: "); fflush(stdin); scanf("%[^\n]s", &L->employees[ctr].info.empName.mname);	strupr(L->employees[ctr].info.empName.mname);
						printf("\nLast name: "); 	fflush(stdin); scanf("%[^\n]s", &L->employees[ctr].info.empName.lname);	strupr(L->employees[ctr].info.empName.lname);
						break;
					case 2:
						printf("\n\nAddress: "); fflush(stdin); scanf("%[^\n]s", &L->employees[ctr].info.Address);	strupr(L->employees[ctr].info.Address);
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
						printf("\n\nDepartment: "); fflush(stdin); scanf("%[^\n]s", &L->employees[ctr].Department); strupr(L->employees[ctr].Department);
						break;
					case 9:
						printf("\n\nPosition: "); fflush(stdin); scanf("%[^\n]s", &L->employees[ctr].Position);	strupr(L->employees[ctr].Position);
						break;
					case 10:
						printf("\n\nMin. Late: "); fflush(stdin); scanf("%d", &L->employees[ctr].minLate);
						break;
					case 11:
						printf("\n\nAbsence: "); fflush(stdin); scanf("%d", &L->employees[ctr].Absence);
						break;
					case 12:
						printf("\n\nPosition rate: "); fflush(stdin); scanf("%lf", &L->employees[ctr].PositionRate);
						L->employees[ctr].BasicSalary=L->employees[ctr].PositionRate/r;
						break;
					case 13:
						printf("\n\nAllowance: "); fflush(stdin); scanf("%lf", &L->employees[ctr].Allowance);
						break;
					case 14:
						printf("Previous VALE Total: %12.2f",L->employees[ctr].deduct.totVALE);
						printf("\n\nVALE Total: "); fflush(stdin); scanf("%lf", &L->employees[ctr].deduct.totVALE);
						
						printf("\nRecommended Amount: %10.2f",(L->employees[ctr].deduct.totVALE/12));
						printf("\nPay by what amount? (Taken monthly):");	fflush(stdin); 
						scanf("%lf", &L->employees[ctr].deduct.VALE);
						L->employees[ctr].deduct.tempVALE=L->employees[ctr].deduct.VALE;
						break;
						
					case 15:
						printf("Previous SSS Loan Total: %12.2f",L->employees[ctr].deduct.totSSSLoan);
						printf("\n\nSSS Loan Total: "); fflush(stdin); scanf("%lf", &L->employees[ctr].deduct.totSSSLoan);
						
						printf("\nRecommended Amount: %10.2f",(L->employees[ctr].deduct.totSSSLoan/12));
						printf("\nPay by what amount? (Taken monthly):");	fflush(stdin); 
						scanf("%lf", &L->employees[ctr].deduct.SSSLoan);
						L->employees[ctr].deduct.tempSSSLoan=L->employees[ctr].deduct.SSSLoan;
						break;
					case 16:
						printf("Previous Pag-Ibig Loan Total: %12.2f",L->employees[ctr].deduct.totPagIbigLoan);
						printf("\n\nPag-Ibig loan Total: "); fflush(stdin); scanf("%lf", &L->employees[ctr].deduct.totPagIbigLoan);
						
						printf("\nRecommended Amount: %10.2f",(L->employees[ctr].deduct.totPagIbigLoan/12));
						printf("\nPay by what amount? (Taken monthly):");	fflush(stdin); 
						scanf("%lf", &L->employees[ctr].deduct.PagIbigLoan);
						L->employees[ctr].deduct.tempPagIbigLoan=L->employees[ctr].deduct.PagIbigLoan;
						break;
					case 17:
						printf("\n\nOthers: "); fflush(stdin); scanf("%lf", &L->employees[ctr].deduct.others);
						break;
					case 18:
						printf("Exiting!");
						break;
					default:
						printf("Invalid input. Please try again!");
						break;
				}
				
				if(choice != 18) {
					printf("\n\nConfirm edit? [Y/N] "); fflush(stdin); scanf("%c", &ch);	ch = toupper(ch);
					
					if(ch == 'Y') {
						writeEmpList(*L);
						printf("\nUpdated successfully!\n\n");
						printf("\nPress Enter to continue "); fflush(stdin); scanf("%c", &hold);
						break;
					} else {
						printf("\nNot updated!\n\n");
						printf("\nPress Enter to continue "); fflush(stdin); scanf("%c", &hold);
						break;
					}
				}
				
			}
		} else {
			printf("\nEmployee not found!");
			printf("\nPress Enter to continue "); fflush(stdin); scanf("%c", &hold);
		}
	} else {
		printf("\n\nEmployee not found!");
		printf("\nPress Enter to continue "); fflush(stdin); scanf("%c", &hold);
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

void displayDeductions(Deductions d)
{
	printf(" P%-12.2f", d.SSS+d.Philhealth+d.PagIbig+d.Withholding+d.tempVALE+d.tempSSSLoan+d.tempPagIbigLoan+d.others);
/*	printf("%10.2f", d.Philhealth);
	printf("%10.2f", d.PagIbig);
	printf("%10.2f", d.Withholding);
	printf("%10.2f", d.VALE);
	printf("%10.2f", d.SSSLoan);
	printf("%10.2f", d.PagIbigLoan);
	printf("%10.2f", d.others);*/
}

void displayHeaderforCP()
{
	printf("%-5s", "ID");
	printf("%20s %-19s", "Name"," ");
	printf("%-8s", "Dept");
	printf("%-12s", "Position");
	printf("%-15s", "Pos Rate");
	printf("%-15s", "Basic Salary");
	printf("%-15s", "Overtime pay");
	printf("%-15s", "Allowance");
	printf("%-15s", "Less");
	printf("%-15s", "Deductions");
	printf("%-15s", "NetAmtPd");
}

void displayEmployeeInfoforCP(EmployeeInfo e)
{
	char position[12];
	char department[8];
	
	strcpy(position,e.Position);
	strcpy(department,e.Department);
	
	printf("\n%5d", e.empID);
	printf("%24s,",e.info.empName.lname);
	printf("%-16s",e.info.empName.fname);
	printf("%-8s", department);
	printf("%-12s", position);
	printf(" P%-12.2f", e.PositionRate);
	printf(" P%-12.2f", e.BasicSalary);
	printf(" P%-12.2f", e.OvertimePay);
	printf(" P%-12.2f", e.Allowance);
	printf(" P(%-11.2f)", e.Less);
	displayDeductions(e.deduct);
	printf(" P%-12.2f", e.netAmtPd);
}

void displayEmployeeInfoforCPv2(EmployeeInfo e)
{
	
	printf("\n%-16s: %-15d", "Emp ID",e.empID);			printf("%-14s: %s, %s","Emp Name",e.info.empName.lname,e.info.empName.fname);
	printf("\n%-16s: %-15s","Emp Dept" ,e.Department);	printf("%-14s: %s","Emp Position" , e.Position);
	printf("\n\n%16s: P%12.2f","PositionRate" , e.PositionRate);
	printf("\n%16s: P%12.2f","Basic Salary", e.BasicSalary);
	printf("\n%16s: P%12.2f","Overtime Pay", e.OvertimePay);
	printf("\n%16s: P%12.2f","Allowance", e.Allowance);
	printf("\n%16s: P(%10.2f)","Less", e.Less);
	
	printf("\n\nDeductions:");
	printf("\n%16s: P%12.2f","SSS", e.deduct.SSS);
	printf("\n%16s: P%12.2f","PhilHealth", e.deduct.Philhealth);
	printf("\n%16s: P%12.2f","PagIbig", e.deduct.PagIbig);
	printf("\n%16s: P%12.2f","Withholding Tax", e.deduct.Withholding);
	printf("\n%16s: P%12.2f","VALE", e.deduct.tempVALE);
	printf("\n%16s: P%12.2f","SSS Loan", e.deduct.tempSSSLoan);
	printf("\n%16s: P%12.2f","PagIbig Loan", e.deduct.tempPagIbigLoan);
	printf("\n%16s: P%12.2f","Others", e.deduct.others);
	
	printf("\n\n%16s: %12.2f","Net Amount Paid", e.netAmtPd);
}

void displayEmpListforCP(EmpList L)
{
	int ctr;
	
	displayHeaderforCP();
	
	for(ctr = 0; ctr < L.count; ctr++) {
		displayEmployeeInfoforCP(L.employees[ctr]);
	}
}

void calculatePayroll(EmpList *L)
{
	int ctr,choice=0;
	char yon,cor,rep='N';
	char hold;
	char today[25],fileformat[10]=".txt";
	FILE *fp;
	
	EmployeeInfo e;
	EmpList PayrollList;
		
	printf("\nCalculate!");
	printf("\nIs it the end of the month? Y/N ");
	fflush(stdin);
	scanf("%c",&yon);
	
	initialPayroll(L,yon);
	
	while(choice!=4){
		printf("\n\nCurrent Payroll\n\n");
		displayEmpListforCP(*L);
		choice = 0;
		printf("\n\n1. Create Payroll File\n");
	    printf("2. View Payroll File\n");
	    printf("3. View Total VALE\n");
	    printf("4. Exit\n");
	    
		printf("\n\nWhat do you want to do? "); fflush(stdin); scanf("%d", &choice);
		
		switch(choice){
			case 1:
				rep='N';
				while(rep=='N'||rep=='n'){
				
					for(ctr = 0; ctr < L->count; ctr++) {
						system("cls");
						printf("%d out of %d Employees",ctr+1,L->count);
						printf("\n\n");
						displayEmployeeInfoforCPv2(L->employees[ctr]);
						
						printf("\n\nCorrect Information? Y/N?");	fflush(stdin);	scanf("%c",&cor);
						fflush(stdin);
						
						if(cor=='N'||cor=='n'){
							
							L->employees[ctr].deduct.tempVALE=L->employees[ctr].deduct.VALE;
							L->employees[ctr].deduct.tempSSSLoan=L->employees[ctr].deduct.SSSLoan;
							L->employees[ctr].deduct.tempPagIbigLoan=L->employees[ctr].deduct.PagIbigLoan;
							while(cor=='N'||cor=='n'){
								
								printf("Update Deductions: If no update to that deduction, copy the initial calculation\n");
								
								printf("\nInitial SSS: %.2f",L->employees[ctr].deduct.SSS);
								printf("\nUpdated SSS: "); 				fflush(stdin);	scanf("%lf", &L->employees[ctr].deduct.SSS); 	
								
								printf("\nInitial PhilHealth: %.2f",L->employees[ctr].deduct.Philhealth);
								printf("\nUpdated PhilHealth: "); 		fflush(stdin);	scanf("%lf", &L->employees[ctr].deduct.Philhealth);	
								
								printf("\nInitial Pag-Ibig: %.2f",L->employees[ctr].deduct.PagIbig);
								printf("\nUpdated Pag-Ibig: "); 		fflush(stdin);	scanf("%lf", &L->employees[ctr].deduct.PagIbig); 		
								
								printf("\nInitial Withholding Tax: %.2f",L->employees[ctr].deduct.Withholding);	
								printf("\nUpdated Withholding Tax: "); 	fflush(stdin);	scanf("%lf", &L->employees[ctr].deduct.Withholding); 	
								
								printf("\nInitial VALE: %.2f",L->employees[ctr].deduct.VALE);	
								printf("\nUpdated VALE: "); 			fflush(stdin);	scanf("%lf", &L->employees[ctr].deduct.tempVALE); 		
								
								printf("\nInitial SSS Loan: %.2f",L->employees[ctr].deduct.SSSLoan);		
								printf("\nUpdated SSS Loan: "); 		fflush(stdin);	scanf("%lf", &L->employees[ctr].deduct.tempSSSLoan); 		
								
								printf("\nInitial Pag-ibig Loan: %.2f",L->employees[ctr].deduct.PagIbigLoan);	
								printf("\nUpdated Pag-ibig Loan: "); 	fflush(stdin);	scanf("%lf", &L->employees[ctr].deduct.tempPagIbigLoan); 	
								
								printf("\nInitial Others: %.2f",L->employees[ctr].deduct.others);	
								printf("\nUpdated Others: "); 			fflush(stdin);	scanf("%lf", &L->employees[ctr].deduct.others);
								
								printf("Correct Information? Y/N? "); 	fflush(stdin);	scanf("%c",&cor);
							}
							
							
							L->employees[ctr].netAmtPd = L->employees[ctr].grossSalary - (L->employees[ctr].deduct.SSS + L->employees[ctr].deduct.PagIbig + L->employees[ctr].deduct.Philhealth + L->employees[ctr].deduct.Withholding + L->employees[ctr].deduct.tempVALE + L->employees[ctr].deduct.tempPagIbigLoan + L->employees[ctr].deduct.tempSSSLoan + L->employees[ctr].deduct.others);
							printf("Continuing list, please press enter"); 	fflush(stdin);	scanf("%c",&hold);
						}
						
					}
					
					printf("\n\nConfirm Payroll is complete? "); fflush(stdin); scanf("%c", &rep);
					
				}
				
				printf("\nDate Today: [mm-dd-yyyy] "); 	fflush(stdin);	scanf("%[^\n]s", &today);
				
				strcat(today,fileformat);
	
				fp = fopen(today, "wb+");
				
				if(fp != NULL) {
					fwrite(L->employees, sizeof(EmployeeInfo), L->count, fp);
					
					for(ctr = 0; ctr < L->count; ctr++) {
						L->employees[ctr].Allowance=0;
						
						if((L->employees[ctr].deduct.tempVALE)!=0){
							L->employees[ctr].deduct.totVALE= L->employees[ctr].deduct.totVALE - L->employees[ctr].deduct.tempVALE;
							L->employees[ctr].deduct.tempVALE=L->employees[ctr].deduct.VALE;
							
							if((L->employees[ctr].deduct.totVALE)<=0){
								L->employees[ctr].deduct.tempVALE=0;
								L->employees[ctr].deduct.VALE=0;
								if((L->employees[ctr].deduct.totVALE)<0){
									L->employees[ctr].Allowance=(L->employees[ctr].deduct.totVALE)*-1;
									L->employees[ctr].deduct.totVALE=0;
								}
							}
							
						}	
						if((L->employees[ctr].deduct.PagIbigLoan)!=0){
							L->employees[ctr].deduct.totPagIbigLoan= L->employees[ctr].deduct.totPagIbigLoan - L->employees[ctr].deduct.tempPagIbigLoan;
							L->employees[ctr].deduct.tempPagIbigLoan=L->employees[ctr].deduct.PagIbigLoan;
							
							if((L->employees[ctr].deduct.totPagIbigLoan)<=0){
								L->employees[ctr].deduct.tempPagIbigLoan=0;
								L->employees[ctr].deduct.PagIbigLoan=0;
								if((L->employees[ctr].deduct.totPagIbigLoan)<0){
									L->employees[ctr].Allowance=(L->employees[ctr].deduct.totPagIbigLoan)*-1;
									L->employees[ctr].deduct.totPagIbigLoan=0;
								}
							}
						}
						if((L->employees[ctr].deduct.SSSLoan)!=0){
							L->employees[ctr].deduct.totSSSLoan= L->employees[ctr].deduct.totSSSLoan - L->employees[ctr].deduct.tempSSSLoan;
							L->employees[ctr].deduct.tempSSSLoan=L->employees[ctr].deduct.SSSLoan;
							
							if((L->employees[ctr].deduct.totSSSLoan)<=0){
								L->employees[ctr].deduct.tempSSSLoan=0;
								L->employees[ctr].deduct.SSSLoan=0;
								if((L->employees[ctr].deduct.totSSSLoan)<0){
									L->employees[ctr].Allowance=(L->employees[ctr].deduct.totSSSLoan)*-1;
									L->employees[ctr].deduct.totSSSLoan=0;
								}
							}
						}
							
						
					}
					
					writeEmpList(*L);
					fclose(fp);
					printf("File created!");
					printf("Please press enter to continue"); 	fflush(stdin);	scanf("%c",&hold);
					system("cls");
				} else {
					printf("\nOperation Failed!");
				}
				
				break;
				
			case 2:
				/*search for file with payroll date*/
				/*displays the thingy needed for Payroll*/
				system("cls");
				printf("View Payroll File\n");
				printf("\nFilename? [mm-dd-yyyy] "); 	fflush(stdin);	scanf("%[^\n]s", &today);
				
				strcat(today,fileformat);
	
				initList(&PayrollList);
				
				fp = fopen(today, "rb");
				
				if(fp != NULL) {
					printf("From File: %s\n\n",today);
					while(PayrollList.count < SIZE && fread(&e, sizeof(EmployeeInfo), 1, fp) == 1) {
						insertSorted(&PayrollList, e);
					}
					
					fclose(fp);
				} else {
					printf("\nFile not found!");
				}
				
				displayEmpListforCP(PayrollList);
				printf("\n\n");
				
				break;
			case 3:
				/*display ra of VALE of Emps*/
				system("cls");
				printf("Show current VALE of all Emps\n\n");
				
				printf("%-5s","ID");
				printf("%22s %-21s", "Name"," ");
				printf("%-15s","Total Vale");
				printf("\n\n");
				
				for(ctr = 0; ctr < L->count; ctr++) {
					if((L->employees[ctr].deduct.totVALE)!=0){
						printf("%5d",L->employees[ctr].empID);
						printf("%24s,",L->employees[ctr].info.empName.lname);
						printf("%-16s",L->employees[ctr].info.empName.fname);
						printf("%-3c",L->employees[ctr].info.empName.mname[0]);
						printf("%-13.2f",L->employees[ctr].deduct.totVALE);
						printf("\n");
					}
				}
				
				printf("\n\n");
				break;
			case 4:
				printf("Exiting!");
				break;
		
			default:
				printf("Invalid input. Please try again!");
				break;
		}
	}
	
}

void initialPayroll(EmpList *L,char yon)
{
	int ctr;
	float monthly,daily,minly,total,grossSalary;
	
	for(ctr = 0; ctr < L->count; ctr++) {
		monthly=L->employees[ctr].PositionRate/12;
		daily=L->employees[ctr].PositionRate/312;
		minly=(daily/8)/60;
		
		total=L->employees[ctr].Absence*daily;
		total=total+(L->employees[ctr].minLate*minly);
		L->employees[ctr].Less=total;
		
		L->employees[ctr].grossSalary= L->employees[ctr].BasicSalary + L->employees[ctr].OvertimePay - L->employees[ctr].Less + L->employees[ctr].Allowance;
		
		if(yon=='Y'||yon=='y'){
			calculateSSS(&(L->employees[ctr]));
			calculatePagIbig(&(L->employees[ctr]));
			calculatePhilhealth(&(L->employees[ctr]));
			calculateWithholdingTax(&(L->employees[ctr]));
			L->employees[ctr].deduct.tempVALE=L->employees[ctr].deduct.VALE;
			L->employees[ctr].deduct.tempSSSLoan=L->employees[ctr].deduct.SSSLoan;
			L->employees[ctr].deduct.tempPagIbigLoan=L->employees[ctr].deduct.PagIbigLoan;
		}else{
			L->employees[ctr].deduct.SSS=0;
			L->employees[ctr].deduct.Philhealth=0;
			L->employees[ctr].deduct.PagIbig=0;
			L->employees[ctr].deduct.Withholding=0;
			L->employees[ctr].deduct.tempVALE=0;
			L->employees[ctr].deduct.tempSSSLoan=0;
			L->employees[ctr].deduct.tempPagIbigLoan=0;
		}
		
		L->employees[ctr].netAmtPd = L->employees[ctr].grossSalary - (L->employees[ctr].deduct.SSS + L->employees[ctr].deduct.PagIbig + L->employees[ctr].deduct.Philhealth + L->employees[ctr].deduct.Withholding + L->employees[ctr].deduct.VALE + L->employees[ctr].deduct.PagIbigLoan + L->employees[ctr].deduct.SSSLoan + L->employees[ctr].deduct.others);
		
	}
}

void calculateSSS(EmployeeInfo *e)
{
	float monthly=e->PositionRate/12;
	
	if(monthly<2250){
		e->deduct.SSS=170;
		e->deduct.emprShareSSS=80;
	}else if(monthly>=2250&&monthly<2750){
		e->deduct.SSS=210;
		e->deduct.emprShareSSS=100;
	}else if(monthly>=2750&&monthly<3250){
		e->deduct.SSS=250;
		e->deduct.emprShareSSS=120;
	}else if(monthly>=3250&&monthly<3750){
		e->deduct.SSS=290;
		e->deduct.emprShareSSS=140;
	}else if(monthly>=3750&&monthly<4250){
		e->deduct.SSS=330;
		e->deduct.emprShareSSS=160;
	}else if(monthly>=4250&&monthly<4750){
		e->deduct.SSS=370;
		e->deduct.emprShareSSS=180;
	}else if(monthly>=4750&&monthly<5250){
		e->deduct.SSS=410;
		e->deduct.emprShareSSS=200;
	}else if(monthly>=5250&&monthly<5750){
		e->deduct.SSS=450;
		e->deduct.emprShareSSS=220;
	}else if(monthly>=5750&&monthly<6250){
		e->deduct.SSS=490;
		e->deduct.emprShareSSS=240;
	}else if(monthly>=6250&&monthly<6750){
		e->deduct.SSS=530;
		e->deduct.emprShareSSS=260;
	}else if(monthly>=6750&&monthly<7250){
		e->deduct.SSS=570;
		e->deduct.emprShareSSS=280;
	}else if(monthly>=7250&&monthly<7750){
		e->deduct.SSS=610;
		e->deduct.emprShareSSS=300;
	}else if(monthly>=7750&&monthly<8250){
		e->deduct.SSS=650;
		e->deduct.emprShareSSS=320;
	}else if(monthly>=8250&&monthly<8750){
		e->deduct.SSS=690;
		e->deduct.emprShareSSS=340;
	}else if(monthly>=8750&&monthly<9250){
		e->deduct.SSS=730;
		e->deduct.emprShareSSS=360;
	}else if(monthly>=9250&&monthly<9750){
		e->deduct.SSS=770;
		e->deduct.emprShareSSS=380;
	}else if(monthly>=9750&&monthly<10250){
		e->deduct.SSS=810;
		e->deduct.emprShareSSS=400;
	}else if(monthly>=10250&&monthly<10750){
		e->deduct.SSS=850;
		e->deduct.emprShareSSS=420;
	}else if(monthly>=10750&&monthly<11250){
		e->deduct.SSS=890;
		e->deduct.emprShareSSS=440;
	}else if(monthly>=11250&&monthly<11750){
		e->deduct.SSS=930;
		e->deduct.emprShareSSS=460;
	}else if(monthly>=11750&&monthly<12250){
		e->deduct.SSS=970;
		e->deduct.emprShareSSS=480;
	}else if(monthly>=12250&&monthly<12750){
		e->deduct.SSS=1010;
		e->deduct.emprShareSSS=500;
	}else if(monthly>=12750&&monthly<13250){
		e->deduct.SSS=1050;
		e->deduct.emprShareSSS=520;
	}else if(monthly>=13250&&monthly<13750){
		e->deduct.SSS=1090;
		e->deduct.emprShareSSS=540;
	}else if(monthly>=13750&&monthly<14250){
		e->deduct.SSS=1130;
		e->deduct.emprShareSSS=560;
	}else if(monthly>=14250&&monthly<14750){
		e->deduct.SSS=1170;
		e->deduct.emprShareSSS=580;
	}else if(monthly>=14750&&monthly<15250){
		e->deduct.SSS=1230;
		e->deduct.emprShareSSS=600;
	}else if(monthly>=15250&&monthly<15750){
		e->deduct.SSS=1270;
		e->deduct.emprShareSSS=620;
	}else if(monthly>=15750&&monthly<16250){
		e->deduct.SSS=1310;
		e->deduct.emprShareSSS=640;
	}else if(monthly>=16250&&monthly<16750){
		e->deduct.SSS=1350;
		e->deduct.emprShareSSS=660;
	}else if(monthly>=16750&&monthly<17250){
		e->deduct.SSS=1390;
		e->deduct.emprShareSSS=680;
	}else if(monthly>=17250&&monthly<17750){
		e->deduct.SSS=1430;
		e->deduct.emprShareSSS=700;
	}else if(monthly>=17750&&monthly<18250){
		e->deduct.SSS=1470;
		e->deduct.emprShareSSS=720;
	}else if(monthly>=18250&&monthly<18750){
		e->deduct.SSS=1510;
		e->deduct.emprShareSSS=740;
	}else if(monthly>=18750&&monthly<19250){
		e->deduct.SSS=1550;
		e->deduct.emprShareSSS=760;
	}else if(monthly>=19250&&monthly<19750){
		e->deduct.SSS=1590;
		e->deduct.emprShareSSS=780;
	}else if(monthly>=19750){
		e->deduct.SSS=1630;
		e->deduct.emprShareSSS=800;
	}
}

void calculatePagIbig(EmployeeInfo *e)
{
	float monthly=e->PositionRate/12;
	
	if(monthly<1500){
		e->deduct.PagIbig=e->grossSalary*.01;
		e->deduct.emprSharePagIbig=e->grossSalary*.02;
	}else{
		e->deduct.PagIbig=e->grossSalary*.02;
		e->deduct.emprSharePagIbig=e->grossSalary*.02;
	}
}

void calculatePhilhealth(EmployeeInfo *e)
{
	float monthly=e->PositionRate/12;
	
	if(monthly<9000){
		e->deduct.Philhealth=100;
		e->deduct.emprSharePhilhealth=100;
		
	}else if(monthly>=9000&&monthly<10000){
		e->deduct.Philhealth=112.5;
		e->deduct.emprSharePhilhealth=112.5;
		
	}else if(monthly>=10000&&monthly<11000){
		e->deduct.Philhealth=125;
		e->deduct.emprSharePhilhealth=125;	
	}else if(monthly>=11000&&monthly<12000){
		e->deduct.Philhealth=137.5;
		e->deduct.emprSharePhilhealth=137.5;
	}else if(monthly>=12000&&monthly<13000){
		e->deduct.Philhealth=150;
		e->deduct.emprSharePhilhealth=150;
	}else if(monthly>=13000&&monthly<14000){
		e->deduct.Philhealth=162.5;
		e->deduct.emprSharePhilhealth=162.5;
	}else if(monthly>=14000&&monthly<15000){
		e->deduct.Philhealth=175;
		e->deduct.emprSharePhilhealth=175;
	}else if(monthly>=15000&&monthly<16000){
		e->deduct.Philhealth=187.5;
		e->deduct.emprSharePhilhealth=187.5;
	}else if(monthly>=16000&&monthly<17000){
		e->deduct.Philhealth=200;
		e->deduct.emprSharePhilhealth=200;
	}else if(monthly>=17000&&monthly<18000){
		e->deduct.Philhealth=212.5;
		e->deduct.emprSharePhilhealth=212.5;
	}else if(monthly>=18000&&monthly<19000){
		e->deduct.Philhealth=225;
		e->deduct.emprSharePhilhealth=225;
	}else if(monthly>=19000&&monthly<20000){
		e->deduct.Philhealth=237.5;
		e->deduct.emprSharePhilhealth=237.5;
	}else if(monthly>=20000&&monthly<21000){
		e->deduct.Philhealth=250;
		e->deduct.emprSharePhilhealth=250;
	}else if(monthly>=21000&&monthly<22000){
		e->deduct.Philhealth=262.5;
		e->deduct.emprSharePhilhealth=262.5;
	}else if(monthly>=22000&&monthly<23000){
		e->deduct.Philhealth=275;
		e->deduct.emprSharePhilhealth=275;
	}else if(monthly>=23000&&monthly<24000){
		e->deduct.Philhealth=287;
		e->deduct.emprSharePhilhealth=287;
	}else if(monthly>=24000&&monthly<25000){
		e->deduct.Philhealth=300;
		e->deduct.emprSharePhilhealth=300;
	}else if(monthly>=25000&&monthly<26000){
		e->deduct.Philhealth=312.5;
		e->deduct.emprSharePhilhealth=312.5;
	}else if(monthly>=26000&&monthly<27000){
		e->deduct.Philhealth=325;
		e->deduct.emprSharePhilhealth=325;
	}else if(monthly>=27000&&monthly<28000){
		e->deduct.Philhealth=337.5;
		e->deduct.emprSharePhilhealth=337.5;
	}else if(monthly>=28000&&monthly<29000){
		e->deduct.Philhealth=350;
		e->deduct.emprSharePhilhealth=350;
	}else if(monthly>=29000&&monthly<30000){
		e->deduct.Philhealth=362.5;
		e->deduct.emprSharePhilhealth=362.5;
	}else if(monthly>=30000&&monthly<31000){
		e->deduct.Philhealth=375;
		e->deduct.emprSharePhilhealth=375;
	}else if(monthly>=31000&&monthly<32000){
		e->deduct.Philhealth=387.5;
		e->deduct.emprSharePhilhealth=387.5;
	}else if(monthly>=32000&&monthly<33000){
		e->deduct.Philhealth=400;
		e->deduct.emprSharePhilhealth=400;
	}else if(monthly>=33000&&monthly<34000){
		e->deduct.Philhealth=412.5;
		e->deduct.emprSharePhilhealth=412.5;
	}else if(monthly>=34000&&monthly<35000){
		e->deduct.Philhealth=425;
		e->deduct.emprSharePhilhealth=425;
	}else if(monthly>=35000){
		e->deduct.Philhealth=437.5;
		e->deduct.emprSharePhilhealth=437.5;
	}
}

void calculateWithholdingTax(EmployeeInfo *e)
{
	
	if(e->grossSalary<=10000){
		e->deduct.Withholding=e->grossSalary*.05;
	}else if(e->grossSalary>10000&&e->grossSalary<=30000){
		e->deduct.Withholding=500+(e->grossSalary*.1);
	}else if(e->grossSalary>30000&&e->grossSalary<=70000){
		e->deduct.Withholding=2500+(e->grossSalary*.15);
	}else if(e->grossSalary>70000&&e->grossSalary<=140000){
		e->deduct.Withholding=8500+(e->grossSalary*.2);
	}else if(e->grossSalary>140000&&e->grossSalary<=250000){
		e->deduct.Withholding=22500+(e->grossSalary*.25);
	}else if(e->grossSalary>250000&&e->grossSalary<=500000){
		e->deduct.Withholding=50000+(e->grossSalary*.3);
	}else if(e->grossSalary>500000){
		e->deduct.Withholding=125000+(e->grossSalary*.32);
	}
}

void displayDayInput(EmpList L,int emp){
	int count;
	printf("********************************************** DAY INPUT *************************************************");
	printf("\n%5s %15s %15s %15s %10s %10s %10s %15s","ID","LASTNAME","FIRSTNAME","PositionRate","OVERTIME(Min)","LATE(Min)","ABSENCES","OvertimePay");
	if(emp==-1){
		for(count=0;count<L.count;count++){
		printf("\n%5d %15s %15s %15.2lf %10d %10d %10d %15.2lf",L.employees[count].empID,L.employees[count].info.empName.lname,
		L.employees[count].info.empName.fname,L.employees[count].PositionRate,L.employees[count].minOvertime,L.employees[count].minLate,
		L.employees[count].Absence,L.employees[count].OvertimePay);	
		}
	}else{
		printf("\n%5d %15s %15s %15.2lf %10d %10d %10d %15.2lf",L.employees[emp].empID,L.employees[emp].info.empName.lname,
		L.employees[emp].info.empName.fname,L.employees[emp].PositionRate,L.employees[emp].minOvertime,L.employees[emp].minLate,
		L.employees[emp].Absence,L.employees[emp].OvertimePay);	
	}
}

void calculationOvertime(EmpList *e,int count,float percentOvertime)
{

	int y,z;
	int totMins;
	double totPay,x;
	
	printf("\nHours: "); fflush(stdin);	scanf("%d", &y);
	y *= 60;
	
	printf("Minutes: "); fflush(stdin);	scanf("%d", &z);

	totMins=y+z;
	
	x=(((e->employees[count].PositionRate/312)*percentOvertime)/8)/60;
	/*x is salary per minute*/
	
	totPay=x*totMins;
	e->employees[count].minOvertime += totMins;
	e->employees[count].OvertimePay += totPay;


}

void dayInputs(EmpList *L)
{
	int id, count,hour,minute,day;
	int opt,overtimeOpt;
	char flag = '\0';
	char proceed = '\0';
	char ch = '\0';
	char absentOpt = '\0';
	
	system("CLS");
	displayDayInput(*L,-1);
	
	printf("\n\nProceed?(Y|N):"); fflush(stdin);	scanf("%c", &flag); strupr(&flag);
	
	switch(flag){
		case 'Y':
		
			printf("\nEnter Employee ID: "); fflush(stdin);	scanf("%d", &id);
		
			for(count=0;count<L->count && L->employees[count].empID!=id;count++){}
			if(count<L->count){
			
					system("CLS");
					displayDayInput(*L,count);
					printf("\n\n1. ADD Late");
					printf("\n2. ADD Overtime");
					printf("\n3. Absent");
					printf("\n4. Exit");	
					printf("\n\n\nEnter option: "); fflush(stdin);	scanf("%d", &opt);
					
					switch(opt){
						case 1:
			
							printf("\nHours:"); fflush(stdin);	scanf("%d", &hour);
							printf("Minutes:"); fflush(stdin); scanf("%d", &minute);
							printf("\nConfirm? (Y|N):"); fflush(stdin); scanf("%c", &proceed); strupr(&proceed);
							
							switch(proceed){
								case 'Y':
									minute = L->employees[count].minLate + minute + (hour*60);
									L->employees[count].minLate = minute;
							
									break;
								case 'N':
									printf("\nExiting!");
									break;
								default:
									printf("\nInvalid input. Please try again!");
									break;
							}
							system("CLS");
							displayDayInput(*L,-1);
							break;
						case 2:
							system("CLS");
							displayDayInput(*L,count);
							printf("\n\n1. Regular");
							printf("\n2. Non-Legal");
							printf("\n3. Legal");
							printf("\n4. Exit");	
							printf("\n\nType of Overtime?:"); scanf(" %d", &overtimeOpt);
						
							if(overtimeOpt==1){
								calculationOvertime(L,count,1.25);
							}else if(overtimeOpt==2){
								calculationOvertime(L,count,1.35);
							}else if(overtimeOpt==3){
								calculationOvertime(L,count,2);
							}else{
								printf("Exit!");
							}
							system("CLS");	
							displayDayInput(*L,-1);
							break;
						case 3:
							
							printf("\nAre you sure?(Y|N):"); scanf(" %c", &absentOpt); strupr(&absentOpt);
							if(absentOpt=='Y'){
								++L->employees[count].Absence;
								printf("\n%s %s is absent.",L->employees[count].info.empName.fname,L->employees[count].info.empName.lname);
							}else if(absentOpt=='N'){
								printf("Incomplete Process");
							}
							break;
						case 4:
							printf("\nExiting!");
							break;
						default:
							printf("\nInvalid input. Please try again!");
							break;
					}
					
					if(opt!= 4) {
					printf("\n\nConfirm edit? [Y/N] "); fflush(stdin); scanf("%c", &ch);	ch = toupper(ch);
					
					if(ch == 'Y') {
						writeEmpList(*L);
						system("CLS");
						printf("\nUpdated successfully!\n\n");
						break;
					} else {
						printf("\nNot updated!\n\n");
						break;
					}
				}
			
			}else{
				printf("\nEmployee not found.");
			}
		
			break;
		case 'N':
			printf("\nExiting!");
			break;
		default:
			printf("\nInvalid input. Please try again!");
			break;
	}
}

void settings(EmpList *L, PayrollSetting *r)
{
	int ctr, choice, wage,login; 

	while (choice != 4) {
		system("CLS");
		printf("************************* SETTING ***************************");
		printf("\n1. Edit rate of wage");
		printf("\n2. View current settings");
		printf("\n3. Edit Login Credentials");
		printf("\n4. Exit");
		
		printf("\n\nWhat do you want to do? "); fflush(stdin); scanf("%d", &choice);
		
		switch(choice) {
			case 1:		
			//	while(wage != 5){
					wage = 0;
					system("CLS");
					printf("************************* WAGE SETTING ***************************");
					printf("\n1. Daily");
					printf("\n2. Weekly");
					printf("\n3. Bi-monthly");
					printf("\n4. Monthly");
					printf("\n5. Exit");
					
					printf("\n\nHow often do yo want to pay? "); fflush(stdin); scanf("%d", &wage);
					switch(wage) {
						case 1:
							r->rate = 312;
							printf("\nChanged successfully!\n\nPress any key to continue...");
							getch();
							break;
						case 2:
							r->rate = 96;
							printf("\nChanged successfully!\n\nPress any key to continue...");
							getch();
							break;
						case 3:
							r->rate = 24;
							printf("\nChanged successfully!\n\nPress any key to continue...");
							getch();
							break;
						case 4:
							r->rate = 12;
							printf("\nChanged successfully!\n\nPress any key to continue...");
							getch();
							break;
						case 5:
							break;
						default:
							printf("\nInvalid input. Please try again.");
							break;	
					}
					
			//	}
				
				for(ctr = 0; ctr < L->count; ctr++) {
					L->employees[ctr].BasicSalary = L->employees[ctr].PositionRate / r->rate;
				}
				printf("Rate has been changed successsfully!");
				writeSetting(*r);
				writeEmpList(*L);
				break;
			case 2:
				system("CLS");
				printf("************************* CURRENT SETTING ***************************");
				printf("\nCurrent rate of wage is: %d", r->rate);
				printf("\nUsername: %s", r->username);
				printf("\nPassword: %s\n", r->password);
				printf("\n\nPress any key to exit....");
				getch();
				break;
			case 3:
				system("CLS");
				printf("************************* LOGIN SETTING ***************************");
				printf("\n1. Change Username");
				printf("\n2. Change Password");
				printf("\n3. Exit");
				printf("\n\nEnter Option: "); fflush(stdin); scanf("%d", &login);
				switch(login){
					case 1:
						printf("\nNew Username: "); fflush(stdin); scanf("%s", r->username);
						printf("\nChanged successfully!\n\nPress any key to continue...");
						getch();
						break;
					case 2:
						printf("\nNew Password: "); fflush(stdin); scanf("%s", r->password);
						printf("\nChanged successfully!\n\nPress any key to continue...");
						getch();
						break;
					case 3:
						break;
					default:
						printf("\nInvalid input. Please try again.");
						break;
				}
				writeSetting(*r);
				break;
			case 4:
				break;
			default:
				printf("\nInvalid input. Please try again.");
				break;
		}
	}
}

void writeSetting(PayrollSetting r)
{
	FILE  *fp;
	
	fp = fopen("settings.txt", "wb");
	
	if(fp != NULL) {
		fwrite(&r, sizeof(PayrollSetting), 1, fp);
		fclose(fp);
	} else {
		printf("File not found!");
	}
}

PayrollSetting readSetting()
{
	FILE *fp;
	PayrollSetting r;
	
	if( access("settings.txt", F_OK ) == 0 ) {
	    fp = fopen("settings.txt", "rb");
	    if(fp != NULL) {
		fread(&r, sizeof(PayrollSetting),1 , fp);
		fclose(fp);
		}
	} else {
	    strcpy(r.username,"admin");
		strcpy(r.password,"12345");
		writeSetting(r);
	}
	
	return r;
}

