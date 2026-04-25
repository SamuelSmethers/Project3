#include <iostream>
#include <cstring>
#include <stdio.h>
#include <fstream>
#include <cstdlib>
#include <iomanip>
#include <cmath>
using namespace std;

int gTXNCount=0;
enum AccountType{Checking=1,Savings=2,Student=3};


struct Record{
	char typeOfTrans;
	double ammountChanged;
	string comments;
};
Record* history; //create pointer for transaction history


struct UserInfoStorage{ //creates the account parameters to store in
	string accountHolder;
	double accountBalance;
	AccountType typeCheckSaveStud;
	bool pinStatus;
	unsigned long hashedPIN;
	string fileName;
	
};

// Hash PIN
unsigned long hashPin(const string& pin) {
    unsigned long h = 0;
    for (int i = 0; i < (int)pin.size(); i++) {
        h = h * 31 + pin.at(i);
    }
    return h;
}

void saveToFile(const UserInfoStorage& acc,const Record *history)
{
	ofstream o_f;
	o_f.open(acc.fileName);
	if(o_f.fail())
	{
		cout<<"File open failure"<<endl;
		exit(EXIT_FAILURE);
	}

	o_f<<acc.accountHolder<<","<<acc.accountBalance<<","<<acc.typeCheckSaveStud<<","<<acc.pinStatus<<","<<acc.hashedPIN<<endl;
	if(gTXNCount==0)
	{
		o_f<<gTXNCount<<endl;
		o_f<<"No transactions"<<endl;
	}
	else
	{
		o_f<<gTXNCount<<endl;
		for(int i=0; i<gTXNCount; ++i)
		{
			//cout<<"Saving to file history: "<<"{"<<(*(history+i)).typeOfTrans<<","<<(*(history+i)).ammountChanged<<","<<(*(history+i)).comments<<"}"<<endl;
			o_f<<"{"<<(*(history+i)).typeOfTrans<<","<<(*(history+i)).ammountChanged<<","<<(*(history+i)).comments<<"}"<<endl;
		}
	}


	o_f.close(); //close file stream
}

bool loadFromFile(UserInfoStorage& acc, Record history[])
{
	bool historyLoaded=false;
	ifstream i_f;
	string inputToBeConverted;
	i_f.open(acc.fileName);
	if(i_f.fail())
	{
		cout<<"File open failure or no account found."<<endl;
		i_f.close(); //close file stream
		return historyLoaded;
	}
	else
	{
		getline(i_f,acc.accountHolder, ',');
		cout<<"Read: "<<acc.accountHolder<<endl;

		getline(i_f, inputToBeConverted , ',');
		acc.accountBalance = std::stod(inputToBeConverted);
		cout<<"Read: "<<acc.accountBalance<<endl;
		
		getline(i_f,inputToBeConverted, ',');
		int typecheck = std::stoi(inputToBeConverted);

		switch(typecheck)
		{
			case 1:
				acc.typeCheckSaveStud = Checking;
				break;
			case 2:
				acc.typeCheckSaveStud = Savings;
				break;
			case 3:
				acc.typeCheckSaveStud = Student;
				break;
			default:
				break;
		}
		//cout<<"Read: "<<accountTypeToString(acc)<<endl;
		cout<<"read: "<<acc.typeCheckSaveStud<<endl;

		getline(i_f, inputToBeConverted , ',');
		acc.pinStatus =std::stoi(inputToBeConverted);
		cout<<"Read: "<<acc.pinStatus<<endl;

		getline(i_f, inputToBeConverted , ',');
		acc.hashedPIN = std::stoul(inputToBeConverted);
		cout<<"Read: "<<acc.hashedPIN<<endl;

		
		historyLoaded=true;
	}



	i_f.close(); //close file stream	
	return historyLoaded;
}

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

bool isValidPin(UserInfoStorage& acc, string p)
{
	bool validPin=false;
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

bool setOrChangePin(UserInfoStorage& acc)
{
	bool validPIN=false;
	string tempPIN;
	string confirmPIN;
	bool confirm=false;
	do
	{
		cout << "Set PIN(4-6 digits): " ;
		cin>>tempPIN;
	}while(isValidPin(acc, tempPIN)==false);

	

	
	cout<<"Confirm PIN: ";
	cin>>confirmPIN;
	if(confirmPIN==tempPIN)
	{
		confirm=true;
		acc.hashedPIN = hashPin(tempPIN);
		acc.pinStatus=true;
		cout<<"Pin Set"<<endl;
	}
	else
	{
		cout<<"Doesn't Match."<<endl;
	}


	return acc.pinStatus;
}

bool requirePinForAction(UserInfoStorage& acc)
{
	string tempPin;
	string blank;
	bool correctEntry;
	if(acc.pinStatus==false)
	{
		cout<<"No PIN set. Set now."<<endl;
		setOrChangePin(acc);
	}
	
	cout<< "Enter PIN:";
	getline(cin,blank);
	getline(cin,tempPin);
	if(hashPin(tempPin)!=acc.hashedPIN)
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

string accountTypeToString(UserInfoStorage& acc)
{
	string AccountChosen;
	switch(acc.typeCheckSaveStud)
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

void recordTransaction(UserInfoStorage& acc, char type,double amount,const string& memo)
{
	Record* tempArray;
	tempArray=new Record[gTXNCount+1];
	if(history!=nullptr)
	{
		for(int i =0; i<gTXNCount;++i) //add old transactions to new transaction array in same locations
		{
			tempArray[i].typeOfTrans=history[i].typeOfTrans;
			tempArray[i].ammountChanged=history[i].ammountChanged;
			tempArray[i].comments=history[i].comments;

		}
	}
	delete [] history; //delete old array

	//append new transaction to the new place
	tempArray[gTXNCount].typeOfTrans=type;
	tempArray[gTXNCount].ammountChanged=amount;
	tempArray[gTXNCount].comments=memo;

	++gTXNCount;

	history=new Record[gTXNCount];
	for(int i =0; i<gTXNCount;++i) //add new transactions to new array in same locations
	{
		history[i].typeOfTrans=tempArray[i].typeOfTrans;
		history[i].ammountChanged=tempArray[i].ammountChanged;
		history[i].comments=tempArray[i].comments;

	}

	
	saveToFile(acc,history);
	delete [] tempArray; //delete temp to resize next time

}

void printRecentTransaction(const UserInfoStorage& acc,int maxToShow=10)
{
	cout << "+----------------- Recent Transactions ----------------+"<< endl;
	int lowTXN=min(gTXNCount, maxToShow);

	if(lowTXN==0)
	{
		cout<<"(No transactions yet)"<<endl;
	}
	else
	{
		for(int i=0; i<lowTXN; ++i)
		{
			if(history[i].typeOfTrans=='D')
			{
				cout<<"[Deposit ] ";
				cout<<"$"<<history[i].ammountChanged<<" | "<< history[i].comments <<endl;
			}
			else if(history[i].typeOfTrans=='W')
			{
				cout<<"[Withdraw] ";
				cout<<"$"<<history[i].ammountChanged<<" | "<< history[i].comments <<endl;
			}
			else if(history[i].typeOfTrans=='F')
			{
				cout<<"[  Fee   ] ";
				cout<<"$"<<history[i].ammountChanged<<" | "<< history[i].comments <<endl;
			}
			
		}
		
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

	int printCounter=0; //track how many were output
	for(int i=gTXNCount-1;i>=0;--i)
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

			cout<<"$"<<history[i].ammountChanged<<" | "<< history[i].comments <<endl;
			++printCounter;
		}
	}
	if(printCounter==0)
	{
		cout<<"No matching transaction type"<<endl;
	}
}

void showDetails(UserInfoStorage& acc)
{
	cout<<"============== Account Details =============="<<endl;
	cout<< "Holder: "<< acc.accountHolder<< endl;
	cout<<"Type: " << accountTypeToString(acc)<<endl;
	cout<< "Balance: "<< acc.accountBalance <<endl;
	if(acc.pinStatus==true)
	{
		cout<< "PIN: (set) " << endl;	
	}
	else if(acc.pinStatus==false)
	{
		cout<< "PIN: (not set) " << endl;
	}
	printRecentTransaction(acc,10);
	cout<<"============================================="<<endl;
}

void deposit(UserInfoStorage& acc)
{
	double amountToAdd=0.00;
	string memo;
	string blank;
	if(requirePinForAction(acc)==false)
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
	acc.accountBalance+=amountToAdd;
	recordTransaction(acc,'D',amountToAdd,memo);
	cout<<"Deposited $"<<amountToAdd<<". New Balance: $"<<acc.accountBalance<<endl;
}

void withdraw(UserInfoStorage& acc)
{
	double amount=0.00;
	string blank;
	string memo;
	double newBal;
	if(requirePinForAction(acc)==false)
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
	newBal= acc.accountBalance-amount;

	if(newBal>=0)
	{
		acc.accountBalance=newBal;
		recordTransaction(acc, 'W',amount,memo);
		cout<<"Withdrew $"<<amount<<". New Balance: $"<<acc.accountBalance<<endl;
	}
	else if((newBal<0) && (acc.typeCheckSaveStud==Checking))
	{
		acc.accountBalance=newBal-35.0;
		recordTransaction(acc, 'W',amount,memo);
		recordTransaction(acc, 'F',35.0,"Overdraft Fee");
		cout<<"Withdrew $"<<amount<<" and $35.0 for overdraft fee. New Balance: $"<<acc.accountBalance<<endl;
	}
	else if((newBal<0) && ((acc.typeCheckSaveStud==Student) || (acc.typeCheckSaveStud==Savings)))
	{
		cout<<"Withdrawal Denied."<<endl;
	}

}
/*
void saveToFile(const UserInfoStorage& acc,const Record history[])
{
	ofstream o_f;
	o_f.open(acc.fileName);
	if(o_f.fail())
	{
		cout<<"File open failure"<<endl;
		exit(EXIT_FAILURE);
	}

	o_f<<acc.accountHolder<<","<<acc.accountBalance<<","<<acc.typeCheckSaveStud<<","<<acc.pinStatus<<","<<acc.hashedPIN<<","<<endl;
	o_f<<gTXNCount<<endl;
	for(int i=0; i<gTXNCount+1; ++i)
	{
		o_f<<"{"<<history[i].typeOfTrans<<","<<history[i].ammountChanged<<","<<history[i].comments<<"}";
	}
	


	o_f.close(); //close file stream
}

void loadFromFile(UserInfoStorage& acc, Record history[])
{
	ifstream i_f;
	i_f.open(acc.fileName);
	if(i_f.fail())
	{
		cout<<"File open failure"<<endl;
		exit(EXIT_FAILURE);
	}


	i_f.close(); //close file stream	
}
*/

string makeFileName(UserInfoStorage& acc)
{
	string fileTitle; //name of file
	fileTitle = acc.accountHolder + ".txt"; //append .txt to holder name and put into name of file
	//cout<<fileTitle; //check for correct append
	return fileTitle;
}

int main()
{
	cout.setf(ios::fixed);
	cout.setf(ios::showpoint);
	cout.precision(2);
	
	history=new Record[gTXNCount]; //initialize

	UserInfoStorage acc;
	acc.pinStatus=false; //initialize variable for no pin set yet  
	
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

	acc.accountHolder = readValidName();
	acc.fileName=makeFileName(acc);
	if(loadFromFile(acc,history)==false)
	{
	cout<<"Enter initial balance:";
	cin>>acc.accountBalance; 
	cout<<endl;
	acc.typeCheckSaveStud=chooseAccountType(); //store account type into user storage
	}

	string AccountChosen; //string for the eventual display of account type
	AccountChosen=accountTypeToString(acc); //convert type to a string

	//initial output of user provided information to start account
	cout << "+-----------------------------------------------------+"<< endl;
	cout<< "Account holder: " << acc.accountHolder << endl; 
	cout<<"Account Type: " << AccountChosen <<endl; 
	cout<<"Balance: "<<acc.accountBalance<<endl; 
	
	int menuSelection; // variable for menu selection input
	enum menuOptions{Deposit=1,Withdraw=2,ShowAccount=3,Set_ChangePIN=4, EXIT=5,ViewTransaction=6}; //create options for menu
	do{
		cout << "+-----------------------------------------------------+"<< endl;
		cout<<"1) Deposit (PIN Required)"<<endl;
		cout<<"2) Withdraw (PIN Required)"<<endl;
		cout<<"3) Show Account Detials"<<endl;
		cout<<"4) Set/Change PIN (4-6 Digits)"<<endl;
		cout<<"5) EXIT"<<endl;
		cout<<"6) View Transaction by type"<<endl;
		cout << "+-----------------------------------------------------+"<< endl;

		//loop to check if input is number between 0 and 7
		do
		{
			cout<<"Select an option 1-6:";
			cin>>menuSelection;
			cout<<endl;
		} while (!(menuSelection>0) && (menuSelection<7));

		// find menu selction and perform action based on selection
		switch(menuSelection)
		{
			case Deposit:
				deposit(acc);
				break;
			case Withdraw:
				withdraw(acc);
				break;
			case ShowAccount:
				showDetails(acc);
				break;
			case Set_ChangePIN:
				setOrChangePin(acc);
				break;
			case EXIT:
				char tempExit; // temp input variable
				cout<<"Are you sure you want to exit y/n:";
				cin>>tempExit;
				if(tempExit=='y' || tempExit=='Y') //check temp variable to see if they confirmed they want to exit the program
				{
					exit=true; //update main loop boolean to stop program
				}
				break;
			case ViewTransaction:
				ViewTransactionsByType();
				break;
			default:
			;

		}

	}while(exit==false);

	delete[] history;
	return 0;
}