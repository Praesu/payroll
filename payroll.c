#include <stdio.h>

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
	int empID;
	name empName;
	char Address[50];
	int Age;
	date DoB;
	char Sex;
	int ContactNo;
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
	PersonalInfo info;
	char Department[15];
	char Position[15];
	double PositionRate;
	double BasicSalary;
	double OvertimePay;
	double Allowance;
	double Less;
	Deductions deduct;
} EmployeeInfo;

void viewEmpList();
void calculatePayroll();
void dayInputs();
void settings();

int main()
{
    int a = 0;
    int loginFlag = 0;
    char username[10] ="\0";
    char password[10] ="\0";
    
    
    do{
	printf("**************** LOGIN ******************\n");
	if(username[0]!='\0' && password[0]!='\0' && loginFlag==0){
		printf("Incorrent Username or Password.\n");
		}
		printf("\nEnter username:");
	    scanf("%s",username);
	    printf("Enter password:");
	    scanf("%s",password);
	    loginFlag = (strcmp(username,"admin")==0 && strcmp(password,"12345")==0)? 1:0;

	    system("CLS");

    }while(loginFlag==0);
    
    if(loginFlag==1){
    	
    	    printf("*************** PAYROLL SYSTEM ****************\n\n");
	    printf("1. View Employee List\n");
	    printf("2. Calculate Payroll\n");
	    printf("3. End of day inputs\n");
	    printf("4. Settings\n");
	    printf("5. Exit\n");

	    while(a != 5) {
		printf("\nWhat do you want to do? Enter number: ");
		scanf("%d", &a);

		switch (a) {
		    case 1: 
			viewEmpList();
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
    	
     }
   

    return 0;
}

void viewEmpList()
{
	printf("\nView employee!");
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
