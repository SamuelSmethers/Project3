#include <iostream>
#include <cstring>
#include <stdio.h>
#include <fstream>
#include <cstdlib>
using namespace std;

const int MAX_TXN = 100;
int gTXNCount=0;
enum AccountType{Checking=1,Savings=2,Student=3};

struct Record{
	char typeOfTrans;
	double ammountChanged;
	string comments;
};
Record history[MAX_TXN];

void printHeader()
{
	cout << "+-----------------------------------------------------+"<< endl;
    cout<< "|\t     Computer Science and Engineering \t      |" << endl;
    cout<< "|\t      CSCE 1030 - Computer Science I \t      |"<< endl;
    cout<< "|  Samuel,Smethers-SAS0993-SamuelSmethers@my.unt.edu  |" << endl;
    cout<< "| Cristian,Mireles-COM0066-CristianMireles@my.unt.edu |" << endl;
    cout << "+-----------------------------------------------------+"<< endl;
    cout<<endl;
    cout<<endl;
    cout<<endl;	
}

bool isValidName(const string& name)
{
	bool valid=false;
	
		for(int i=0; i<name.length(); ++i)
		{
			if(isalnum(name.at(i)) || isspace(name.at(i))) 
			{
				valid=true; 
			}
			else //if its not a number an alphabetic character or a space it will do the else
			{
				valid = false; //set acceptable to false
				break; //takes program out of for loop 
			}		
		}

	return valid;
}

string readValidName()
{
	string name;
	cout<<"Enter your name using letters, numbers, and spaces: " << endl;
	getline(cin,name);
	
	while(isValidName(name)==false)
	{
		cout<<"Name can only contain letters, digits, and spaces. Try again:"<<endl;
		getline(cin, name);
		
	}
	
	return name;
}

bool isValidPin(const string&  p)
{
	bool validPin;
	int pinLength = p.length();

		if(p.length() > 3 && p.length() < 7)
		{
			for(int i = 0; i < pinLength; i++)
			{
				if(isdigit(p.at(i)))
				{
					validPin = true;
				}
				else
				{
					cout << "Invalid PIN: Try again"<<endl;
					validPin = false;
					break;
				}
			}
		}
		else 
		{
			cout << "Invalid PIN: Try again"<<endl;
			validPin=false;
		}

	return validPin;
}

bool setOrChangePin(string& pin, bool& pinSet)
{
	bool validPIN=false;
	string tempPIN;
	string blank;
	bool confirm=false;
	do
	{
		cout << "Set PIN(4-6 digits): " ;
		cin>>pin;
	}while(isValidPin(pin)==false);
	
	while(confirm==false)
	{
		cout<<"Confirm PIN: ";
		cin>>tempPIN;
		if(tempPIN==pin)
		{
			confirm=true;
			pinSet=true;
			cout<<"Pin Set"<<endl;
		}
		else
		{
			cout<<"Doesn't Match. Try again."<<endl;
		}
	}

	return pinSet;
}

bool requirePinForAction(string& pin, bool& pinSet)
{
	string tempPin;
	string blank;
	bool correctEntry;
	if(pinSet==false)
	{
		cout<<"No PIN set. Set now."<<endl;
		setOrChangePin(pin, pinSet);
	}
	
		cout<< "Enter PIN:";
		getline(cin,blank);
		getline(cin,tempPin);
		if(tempPin!=pin)
		{
			cout<<"Incorrect PIN."<<endl;
			correctEntry= false;
		}
		else
		{
			correctEntry= true;
		}

	return correctEntry;
}

AccountType chooseAccountType()
{
	int accountTypeSelection;
	AccountType t;
	cout<<"Chose account type:"<<endl;
	cout<<"1) Checking"<<endl;
	cout<<"2) Savings"<<endl;
	cout<<"3) Student"<<endl;
	do{
		cout<<"Enter 1-3:";
		cin>>accountTypeSelection;
		cout<<endl;
	}while(!(accountTypeSelection>0 && accountTypeSelection<4));

	switch(accountTypeSelection)
	{
		case 1:
			t = Checking;
			break;
		case 2:
			t =  Savings;
			break;
		case 3:
			t =  Student;
			break;
		default:
			cout<<"Error."<<endl;
			break;
	}

	return t;
}

string accountTypeToString(AccountType t)
{
	string AccountChosen;
	switch(t)
	{
		case Checking:
			AccountChosen = "Checking";
			break;
		case Savings:
			AccountChosen = "Savings";
			break;
		case Student:
			AccountChosen = "Student";
			break;
		default:
		break;
	}

	return AccountChosen;
}

void recordTransaction(char type,double amount,const string& memo)
{
	if(gTXNCount<MAX_TXN)
	{
		history[gTXNCount].typeOfTrans=type;
		history[gTXNCount].ammountChanged=amount;
		history[gTXNCount].comments=memo;
		++gTXNCount;
	}
	else
	{
		for(int i = 99;i>0;--i)
		{
			history[i-1]=history[i];
			history[i-1]=history[i];
			history[i-1]=history[i];
		}
		history[99].typeOfTrans=type;
		history[99].ammountChanged=amount;
		history[99].comments=memo;
	}
}
void printRecentTransaction(int maxToShow)
{
	cout << "+----------------- Recent Transactions ----------------+"<< endl;
	if(maxToShow<=0)
	{
		maxToShow=10;
	}
	else if(gTXNCount==0)
	{
		cout<<"(No transactions yet)"<<endl;
	}

	for(int i=0; i<maxToShow; ++i)
	{
		if(history[i].typeOfTrans=='D')
		{
			cout<<"[Deposit ] ";
		}
		else if(history[i].typeOfTrans=='W')
		{
			cout<<"[Withdraw] ";
		}
		else if(history[i].typeOfTrans=='F')
		{
			cout<<"[  Fee   ] ";
		}

		cout.setf(ios::fixed);
		cout.setf(ios::showpoint);
		cout.precision(2);
		cout<<"$"<<history[i].ammountChanged<<" | "<< history[i].comments <<endl;
	}
	cout << "+-----------------------------------------------------+"<< endl;
}

void ViewTransactionsByType()
{
	char selection;
	bool again;
	do
	{
		cout<<"Show which transactions? (D=Deposit, W=Withdraw, F=Fee): ";
		cin>>selection;
		selection=toupper(selection);
		if(selection=='D')
		{
			again=false;
		}
		else if(selection=='W')
		{
			again=false;
		}
		else if(selection=='F')
		{
			again=false;
		}
		else
		{
			again=true;
		}
	}while(again==true);

	for(int i=MAX_TXN-1;i>=0;--i)
	{
		if(history[i].typeOfTrans==selection)
		{
			if(history[i].typeOfTrans=='D')
		{
			cout<<"[Deposit ] ";
		}
		else if(history[i].typeOfTrans=='W')
		{
			cout<<"[Withdraw] ";
		}
		else if(history[i].typeOfTrans=='F')
		{
			cout<<"[  Fee   ] ";
		}

		cout.setf(ios::fixed);
		cout.setf(ios::showpoint);
		cout.precision(2);
		cout<<"$"<<history[i].ammountChanged<<" | "<< history[i].comments <<endl;
		}
	}
}

void showDetails(const string& holder, AccountType type, double balance,bool pinSet)
{
	cout<<"============== Account Details =============="<<endl;
	cout<< "Holder: "<< holder<< endl;
	cout<<"Type: " << accountTypeToString(type)<<endl;
	cout<< "Balance: "<< balance <<endl;
	if(pinSet==true)
	{
		cout<< "PIN: (set) " << endl;	
	}
	else if(pinSet==false)
	{
		cout<< "PIN: (not set) " << endl;
	}
	printRecentTransaction(10);
	cout<<"============================================="<<endl;
}

void deposit(double& balance, string& pin, bool& pinSet)
{
	double amountToAdd=0.00;
	string memo;
	string blank;
	if(requirePinForAction(pin, pinSet)==false)
	{
		cout<<"Deposit canceled."<<endl;
		return;
	}
	cout<<"Enter deposit amount: ";
	cin>>amountToAdd;
	cout<<"Enter memo: ";
	getline(cin,blank);
	getline(cin,memo);
	cout<<endl;
	balance+=amountToAdd;
	recordTransaction('D',amountToAdd,memo);
	cout<<"Deposited $"<<amountToAdd<<". New Balance: $"<<balance<<endl;
}

void withdraw(double& balance, AccountType type, string& pin, bool& pinSet)
{
	double amount=0.00;
	string blank;
	string memo;
	double newBal;
	if(requirePinForAction(pin, pinSet)==false)
	{
		cout<<"Withdrawal canceled."<<endl;
		return;
	}
	cout<<"Enter Withdrawal amount: ";
	cin>>amount;
	cout<<"Enter memo: ";
	getline(cin,blank);
	getline(cin,memo);
	cout<<endl;
	newBal= balance-amount;

	if(newBal>=0)
	{
		balance=newBal;
		recordTransaction('W',amount,memo);
		cout<<"Withdrew $"<<amount<<". New Balance: $"<<balance<<endl;
	}
	else if((newBal<0) && (type==Checking))
	{
		balance=newBal-35.0;
		recordTransaction('W',amount,memo);
		recordTransaction('F',35.0,"Overdraft Fee");
		cout<<"Withdrew $"<<amount<<" and $35.0 for overdraft fee. New Balance: $"<<balance<<endl;
	}
	else if((newBal<0) && ((type==Student) || (type==Savings)))
	{
		cout<<"Withdrawal Denied."<<endl;
	}

}

void saveToFile()
{

}

void loadFromFile()
{

}

string makeFileName(string holder)
{
	string fileTitle;
	fileTitle = holder + ".txt";
	cout<<fileTitle;
	return fileTitle;
}

int main()
{
	ifstream i_f;
	ofstream o_f;

	string pin="empty"; 
	bool pinSet=false;
	double balance;
	string name;
	string file_name;

	/*
	history[0].typeOfTrans='D';
	history[0].ammountChanged=250.00;
	history[0].comments="Test Transaction";

	cout<< "type: "<< history[0].typeOfTrans<< endl;
	cout<< "ammount: "<< history[0].ammountChanged<< endl;
	cout<< "memo: "<< history[0].comments << endl;

	history[1].typeOfTrans='W';
	history[1].ammountChanged=250.00;
	history[1].comments="Test Transaction #2";

	cout<< "type: "<< history[1].typeOfTrans<< endl;
	cout<< "ammount: "<< history[1].ammountChanged<< endl;
	cout<< "memo: "<< history[1].comments << endl;
	*/

	printHeader();

	bool exit=false;

	name = readValidName();
	file_name=makeFileName(name);
	/*
	i_f.open(file_name);
	if(i_f.fail())
	{
		cout<<"File open failure"<<endl;
		exit(EXIT_FAILURE);
	}

	o_f.open(file_name);
	if(o_f.fail())
	{
		cout<<"File open failure"<<endl;
		exit(EXIT_FAILURE);
	}
	*/

	cout<<"Enter initial balance:";
	cin>>balance;
	cout<<endl;

	AccountType t;
	string AccountChosen;
	t=chooseAccountType();
	AccountChosen=accountTypeToString(t);

	cout << "+-----------------------------------------------------+"<< endl;
	cout<< "Account holder: " << name << endl;
	cout<<"Account Type: " << AccountChosen <<endl;
	cout<<"Balance: "<<balance<<endl;
	
	int menuSelection;
	enum menuOptions{Deposit=1,Withdraw=2,ShowAccount=3,Set_ChangePIN=4, EXIT=5,ViewTransaction=6};
do{
	cout << "+-----------------------------------------------------+"<< endl;
	cout<<"1) Deposit (PIN Required)"<<endl;
	cout<<"2) Withdraw (PIN Required)"<<endl;
	cout<<"3) Show Account Detials"<<endl;
	cout<<"4) Set/Change PIN (4-6 Digits)"<<endl;
	cout<<"5) EXIT"<<endl;
	cout<<"6) View Transaction by type"<<endl;
	cout << "+-----------------------------------------------------+"<< endl;

	do
	{
		cout<<"Select an option 1-6:";
	 	cin>>menuSelection;
		cout<<endl;
	} while (!(menuSelection>0) && (menuSelection<6));

	switch(menuSelection)
	{
		case Deposit:
			deposit(balance, pin, pinSet);
			break;
		case Withdraw:
			withdraw(balance, t, pin, pinSet);
			break;
		case ShowAccount:
			showDetails(name, t, balance, pinSet);
			break;
		case Set_ChangePIN:
			setOrChangePin(pin,pinSet);
			break;
		case EXIT:
			char tempExit;
			cout<<"Are you sure you want to exit y/n:";
			cin>>tempExit;
			if(tempExit=='y' || tempExit=='Y')
			{
				exit=true;
			}
			break;
		case ViewTransaction:
			ViewTransactionsByType();
			break;
		default:
		;

	}

}while(exit==false);

	i_f.close();
	o_f.close();

	return 0;
}