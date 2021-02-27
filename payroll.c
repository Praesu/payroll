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

typedef struct{
	Rate rate;
	char username[10];
	char password[10];
}PayrollSetting;

void viewEmpList(EmpList *L, Rate r);
void calculatePayroll();
void dayInputs(EmpList *L);
void settings(EmpList *L, PayrollSetting *R);

void initList(EmpList *L); 
EmpList populateList(); 

void displayPersonalInfo(PersonalInfo e);
void displayDeductions(Deductions d);
void displayEmployeeInfo(EmployeeInfo e);
void displayEmpList(EmpList L);
void displayDayInput(EmpList L,int emp);

void createNewEmp(EmpList *L, Rate r);
void updateEmp(EmpList *L, Rate r);
void deleteEmp(EmpList *L);

void selectionSort(EmpList *L); 
void insertSorted(EmpList *L, EmployeeInfo e); 
void insertLast(EmpList *L, EmployeeInfo info);

void writeEmpInfo(EmployeeInfo e);
void writeEmpList(EmpList L);
void calculationOvertime(EmpList *e,int count,float percentOvertime);
void writeSetting(PayrollSetting r);
PayrollSetting readSetting();

int main()
{
	EmpList L;
    int a = 0;
    int loginFlag = 0;
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
    	printf("1. View Employee List\n");
	    printf("2. Calculate Payroll\n");
	    printf("3. End of day inputs\n");
	    printf("4. Settings\n");
	    printf("5. Exit\n");

		printf("\nWhat do you want to do? Enter number: ");
		fflush(stdin);
		scanf("%d", &a);

		switch (a) {
		    case 1: 
			viewEmpList(&L, r.rate);
			break;
		    case 2:
			calculatePayroll();
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
		for(;strcmp(e.info.empName.lname, L->employees[ctr].info.empName.lname) == 0 && strcmp(e.info.empName.fname, L->employees[ctr].info.empName.fname) > 0; ctr++) {}
		
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
	printf("%-8s", "ID");
	printf("%-40s", "Name");
	printf("%-23s", "Address");
	printf("%-5s", "Age");
	printf("%-15s", "DoB");
	printf("%-5s", "Sex");
	printf("%-17s", "Contact No.");
	printf("%-20s", "Email");
	printf("%-17s", "Department");
	printf("%-17s", "Position");
	printf("%-10s", "Position Rate");	
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
	printf("%-s, %-s %-21s", e.empName.lname, e.empName.fname, e.empName.mname);
	printf("%-20s", e.Address);
	printf("%5d", e.Age);
	printf("%3d-%d-%d", e.DoB.month, e.DoB.day, e.DoB.year);
	printf("%-5c", e.Sex);
	printf("%-15d", e.ContactNo);
	printf("%-20s", e.email);
}

void displayEmployeeInfo(EmployeeInfo e)
{
	printf("\n%-8d", e.empID);
	displayPersonalInfo(e.info);
	printf("%-10s", e.Department);
	printf("%-10s", e.Position);
	printf("%-10.2f", e.PositionRate);
}

void displayEmpList(EmpList L)
{
	int ctr;
	
	displayHeader();
	
	for(ctr = 0; ctr < L.count; ctr++) {
		displayEmployeeInfo(L.employees[ctr]);
	}
}

void viewEmpList(EmpList *L, Rate r)
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
		fflush(stdin); 
		scanf("%d", &a);

        switch (a) {
            case 1: 
                createNewEmp(L, r);
                break;
            case 2:
                updateEmp(L, r);
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

void createNewEmp(EmpList *L, Rate r) 
{
	char confirm;
	EmployeeInfo e;

	printf("Last name: "); 						scanf("%[^\n]s", &e.info.empName.lname); 		strupr(e.info.empName.lname);		fflush(stdin); 
	printf("\nFirst name: "); 					scanf("%[^\n]s", &e.info.empName.fname);		strupr(e.info.empName.fname);		fflush(stdin);
	printf("\nMiddle name: "); 					scanf("%[^\n]s", &e.info.empName.mname);		strupr(e.info.empName.mname);		fflush(stdin);
	printf("\nAddress: "); 						scanf("%[^\n]s", &e.info.Address);				strupr(e.info.Address);				fflush(stdin);
	printf("\nAge: "); 						 	scanf("%d", &e.info.Age);															fflush(stdin);
	printf("\nDate of Birth: [mm-dd-yyyy] "); 	scanf("%2d-%2d-%4d", &e.info.DoB.month, &e.info.DoB.day, &e.info.DoB.year);			fflush(stdin);
	printf("\nSex: [F/M] "); 					scanf("%c", &e.info.Sex);						e.info.Sex = toupper(e.info.Sex);	fflush(stdin);
	printf("\nContact number: "); 				scanf("%lf", &e.info.ContactNo);													fflush(stdin);
	printf("\nEmail: "); 						scanf("%[^\n]s", &e.info.email);													fflush(stdin);
	printf("\nDepartment: "); 					scanf("%[^\n]s", &e.Department); 				strupr(e.Department);				fflush(stdin);
	printf("\nPosition: "); 					scanf("%[^\n]s", &e.Position); 					strupr(e.Position);					fflush(stdin);	
	printf("\nPosition Rate: "); 				scanf("%lf", &e.PositionRate);  													fflush(stdin);
	
	while(1) {
		printf("Are all the details correct? [Y/N] ");	
		fflush(stdin);
		scanf("%c", &confirm);
		confirm = toupper(confirm);
	
		if(confirm == 'Y') {
			e.empID = L->count + 1;
			
			e.minLate = 0;				e.Absence = 0;		e.BasicSalary=e.PositionRate/r;		e.OvertimePay=0;   		e.Less=0;
			e.Allowance=0; 				e.netAmtPd=0; 		e.deduct.others=0;					e.deduct.PagIbig=0; 	e.deduct.PagIbigLoan=0;
			e.deduct.Philhealth=0;		e.deduct.SSS=0;		e.deduct.SSSLoan=0;					e.deduct.VALE=0;
			e.deduct.Withholding=0;		e.minOvertime=0;
			
			insertSorted(L, e);
			writeEmpList(*L);
			break;
		} else if(confirm == 'N'){
			printf("\nEmployee not created!");
			break;
		} else {
			printf("\nInvalid input. Please try again.\n");
		}
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

void updateEmp(EmpList *L, Rate r)
{
	int ctr;
	char ln[25], fn[25];
	int choice = 0;
	char ch = 'N';
	EmployeeInfo temp;
	
	printf("\nLast name: ");  fflush(stdin);	scanf("%[^\n]s", &ln); strupr(ln);
	printf("\nFirst name: "); fflush(stdin);    scanf("%[^\n]s", &fn); strupr(fn);
	
	for(ctr = 0; ctr < L->count && strcmp(ln, L->employees[ctr].info.empName.lname) != 0; ctr++) {}
	if(ctr < L->count) {
		if(strcmp(ln, L->employees[ctr].info.empName.lname) == 0) {
			for( ; ctr < L->count && strcmp(fn, L->employees[ctr].info.empName.fname) != 0; ctr++){}
		}
		if(ctr < L->count) {
			displayHeader();
			
			temp = L->employees[ctr];
			displayEmployeeInfo(temp);
			
			while(choice != 14 ) {
				choice = 0;
				
				printf("\n\n1. Employee name");
				printf("\n2. Address");
				printf("\n3. Age");
				printf("\n4. Date of Birth");
				printf("\n5. Sex");
				printf("\n6. Contact Number");
				printf("\n7. Email");
				printf("\n8. Department");
				printf("\n9. Position");
				printf("\n10. Position");
				printf("\n11. Min. late");
				printf("\n12. Absence");
				printf("\n13. Position rate");
				printf("\n14. Exit");
				
				printf("\n\nWhat do you want to edit? "); fflush(stdin); scanf("%d", &choice);
				
				switch(choice) {
					case 1:
						printf("\n\nFirst name: ");	 fflush(stdin); scanf("%[^\n]s", &temp.info.empName.fname);	strupr(temp.info.empName.fname);
						printf("\nMiddle name: ");	 fflush(stdin); scanf("%[^\n]s", &temp.info.empName.mname);	strupr(temp.info.empName.mname);
						printf("\nLast name: "); 	 fflush(stdin); scanf("%[^\n]s", &temp.info.empName.lname);	strupr(temp.info.empName.lname);
						break;
					case 2:
						printf("\n\nAddress: "); fflush(stdin); scanf("%[^\n]s", &temp.info.Address);	strupr(temp.info.Address);
						break;
					case 3:
						printf("\n\nAge: "); fflush(stdin); scanf("%d", &temp.info.Age);
						break;
					case 4:
						printf("\n\nDate of Birth: [mm-dd-yyyy] "); fflush(stdin); scanf("%d-%d-%d", &temp.info.DoB.month, &temp.info.DoB.day, &temp.info.DoB.year);
						break;
					case 5:
						printf("\n\nSex: [F/M] "); fflush(stdin); scanf("%c", &temp.info.Sex); temp.info.Sex = toupper(temp.info.Sex);
						break;
					case 6:
						printf("\n\nContact number: "); fflush(stdin); scanf("%s", &temp.info.ContactNo);
						break;
					case 7:
						printf("\n\nEmail: "); fflush(stdin); scanf("%s", &temp.info.email);
						break;	
					case 8:
						printf("\n\nDepartment: "); fflush(stdin); scanf("%[^\n]s", &temp.Department); strupr(temp.Department);
						break;
					case 9:
						printf("\n\nPosition: "); fflush(stdin); scanf("%[^\n]s", &temp.Position);	strupr(temp.Position);
						break;
					case 10:
						printf("\n\nMin. Late: "); fflush(stdin); scanf("%d", &temp.minLate);
						break;
					case 11:
						printf("\n\nMin. overtime: "); fflush(stdin); scanf("%d", &temp.minLate);
						break;
					case 12:
						printf("\n\nAbsence: "); fflush(stdin); scanf("%d", &temp.Absence);
						break;
					case 13:
						printf("\n\nPosition rate: "); fflush(stdin); scanf("%lf", &temp.PositionRate);
						temp.BasicSalary = temp.PositionRate/r;
						break;
					case 14:
						printf("Exiting!");
						break;
					default:
						printf("Invalid input. Please try again!");
						break;
				}
				
				if(choice != 14) {
					while(1) {
						printf("\n\nConfirm edit? [Y/N] "); fflush(stdin); scanf("%c", &ch);	ch = toupper(ch);
						
						if(ch == 'Y') {
							L->employees[ctr] = temp;
							writeEmpList(*L);
							printf("\nUpdated successfully!\n\n");
							break;
						} else if(ch == 'N') {
							printf("\nNot updated!\n\n");
							break;
						} else {
							printf("\nInvalid input. Please try again!\n");
						}
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
	
	printf("\nLast name: "); fflush(stdin);	scanf("%[^\n]s", &ln); strupr(ln);
	printf("\nFirst name: "); fflush(stdin); scanf("%[^\n]s", &fn); strupr(fn);
	
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
