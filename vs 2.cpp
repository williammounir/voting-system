
#include <iostream>
#include<fstream>
#include<cstring>
#include<vector>
#include <string>
#include<iomanip>
using namespace std;

const string AdminFile = "AdminsFile.txt";
const string ElectorFile = "ElectorFile.txt";
const string ElectionFile = "ElectionFile.txt";

const string bigDelma = "#//#";

const string smallDelma = ",";


//we have declared welcome window again in this part bec we are going to use it in the authenticationimplamitation for both the one for admin and elector so it has to be above it
void WelcomeWindow();
void AdminMainWindow();
void ElectorMainWindow();
void AddAdminsDataToFile();


struct stElection {

	string ID;
	string Name;
	string Description; 
	string Nominees[7];
	int numberofnominees=0;
	//votes will be freq array and its size will be related to the number of Nominees
	int Votes[7] = { 0 };



	//only 6 codes are allowed from 0 to 5
	string AllowedVotersCode[6] = {};



	bool finished = 0;


};

stElection elections[10];
int numberofelections = 0;




struct stElectionAdmin {

	string id;
	string name;
	string password;
	string email;
	string address;
	string phone;
	vector<string> ElectionIds;
	int permissions = -1;
	


}currentadmin;

stElectionAdmin admins[20];
int numberofadmins = 0;

struct stelector {
	string id;
	string name;
	string password;
	string email;
	string address;
	string phone;
	string PElections[10];
	int NumberOfPElections = 0;
	string code;

}currentelector;
int NumberOfElectors = 0;
stelector electorsdata[20];

//utility functions section//////////////////////////////////////////////////////////

bool IsNumberBetween(short Number, short From, short To)
{
	if (Number >= From && Number <= To)
		return true;
	else
		return false;
}

int ReadIntNumberBetween(int From, int To, string ErrorMessage = "Number is not within range, Enter again:\n")
{
	int Number;
	cin >> Number;

	while (!IsNumberBetween(Number, From, To))
	{
		cout << ErrorMessage;
		cin >> Number;
		
	}
	return Number;
}



void SplitString(string splitedstring[],string delma,string line) {
	//A1#//#marly
	string s2 = "";
	int pos = 0;
	short indx = 0;

	while ((pos = line.find(delma)) != std::string::npos) {

		s2 = line.substr(0, pos);

		if (s2 != "") {

			splitedstring[indx] = s2;

		}

		line.erase(0, pos + delma.length());

		indx++;

	}

	if (line != "")
		splitedstring[indx] = line;


}

vector<string> VSplitString( string delma, string line) {
	string s2 = "";
	int pos = 0;
	vector<string>splitedstring;

	while ((pos = line.find(delma)) != std::string::npos) {

		s2 = line.substr(0, pos);

		if (s2 != "") {
			
			splitedstring.push_back(s2);

		}

		line.erase(0, pos + delma.length());

		

	}

	if (line != "")
		splitedstring.push_back(line);

	return splitedstring;

}

enum enCharType {
	SamallLetter = 1, CapitalLetter = 2,
	Digit = 3, MixChars = 4, SpecialCharacter = 5,
};



int RandomNumber(int From, int To)
{
	

	int randNum = rand() % (To - From + 1) + From;
	return randNum;
}

char GetRandomCharacter(enCharType CharType)
{


	if (CharType == MixChars)
	{

		CharType = (enCharType)RandomNumber(1, 3);

	}

	switch (CharType)
	{

	case enCharType::SamallLetter:
	{
		return char(RandomNumber(97, 122));
		break;
	}
	case enCharType::CapitalLetter:
	{
		return char(RandomNumber(65, 90));
		break;
	}
	case enCharType::SpecialCharacter:
	{
		return char(RandomNumber(33, 47));
		break;
	}
	case enCharType::Digit:
	{
		return char(RandomNumber(48, 57));
		break;
	}

defualt:
	{
		return char(RandomNumber(65, 90));
		break;
	}
	}
}



string GenerateWord(enCharType CharType, short Length)

{
	string Word;

	for (int i = 1; i <= Length; i++)

	{

		Word = Word + GetRandomCharacter(CharType);

	}
	return Word;
}


string  GenerateKey(enCharType CharType = CapitalLetter)
{

	string Key = "";


	Key = GenerateWord(CharType, 4) + "-";
	Key = Key + GenerateWord(CharType, 4) + "-";
	Key = Key + GenerateWord(CharType, 4) + "-";
	Key = Key + GenerateWord(CharType, 4);


	return Key;
}


////////////////////////////////////////////////////////////////////////////////




//election functions section/////////////////////////////////////////////////////

void SplitStringfornominees(string splitedstring[], string delma, string line,int &indx) {
	//  ,
	string s2 = "";
	int pos = 0;

	//william#//#mounir
	while ((pos = line.find(delma)) != std::string::npos) {

		s2 = line.substr(0, pos);

		if (s2 != "") {

			splitedstring[indx] = s2;

		}

		line.erase(0, pos + delma.length());

		indx++;

	}

	if (line != "") {
		splitedstring[indx] = line;
		indx++;
	}

	



}
void SplitStringforelectionINT(int splitedstring[], string delma, string line) {
	string s2 = "";
	int pos = 0;
	int indx = 0;

	while ((pos = line.find(delma)) != std::string::npos) {

		s2 = line.substr(0, pos);

		if (s2 != "") {
			

			splitedstring[indx] = stoi(s2);

		}

		line.erase(0, pos + delma.length());

		indx++;

	}

	if (line != "") {
		splitedstring[indx] = stoi(line);
		indx++;
	}


}



stElection convertLineIntoRecordForElection(string line) {

	stElection election;
	string splitedstring[8];
	// so in here we have three different kinds of splitstring first one is the typical one we splited the whole string with it the the second one we used for 
	//spliting election.Nominees which have an array of string third split string is the same as the second one just an array of in for votes instead of array of string
	
	SplitString(splitedstring, bigDelma, line);


	
	election.ID = splitedstring[0];
	election.Name = splitedstring[1];
	election.Description = splitedstring[2];
    SplitStringfornominees(election.Nominees, smallDelma,splitedstring[3], election.numberofnominees);
	SplitStringforelectionINT(election.Votes, smallDelma, splitedstring[4]);
	SplitString(election.AllowedVotersCode, smallDelma, splitedstring[5]);
	

	election.finished = stoi(splitedstring[6]);

	return election;
}



void ReadElections() {

	fstream myfile;

	myfile.open(ElectionFile, ios::in);
	string line;
	numberofelections = 0;
	if (myfile.is_open()) {

		while (getline(myfile, line)) {

			elections[numberofelections] = convertLineIntoRecordForElection(line);
			numberofelections++;
		}
		

		myfile.close();
	}
	



}


string ElectionConvertRecordToLine(stElection election) {

	string line = "";


	line += election.ID + bigDelma;
	line += election.Name + bigDelma;
	line += election.Description + bigDelma;
	
	
	

	
	for (int i = 0; i < election.numberofnominees;i++) {
		if (election.numberofnominees-1 > i)
			line += election.Nominees[i] + smallDelma;
		else
			line += election.Nominees[i] + bigDelma;
		
	}

	for (int i = 0; i < election.numberofnominees; i++) {
		if (election.numberofnominees - 1 > i)
			line += to_string(election.Votes[i]) + smallDelma;
		else
			line += to_string(election.Votes[i]) + bigDelma;

	}

	for (int i = 0; i < 6; i++) {
		if (5 > i)
			line += election.AllowedVotersCode[i] + smallDelma;
		else
			line += election.AllowedVotersCode[i] + bigDelma;

	}

	line += to_string(election.finished);

    
	return line;

}

void AddElectionDataToFile() {



	fstream myfile;
	myfile.open(ElectionFile, ios::out);


	if (myfile.is_open()) {
		for (int i = 0; i < numberofelections; i++) {
		    

			myfile << ElectionConvertRecordToLine(elections[i]) << "\n";


		}
		myfile.close();
	}

}

bool checkIdExistanceoForElection(string id){

	for (int i = 0; i < numberofelections; i++) 
		if (id == elections[i].ID)
			return 1;

    return 0;

}


void CreateNewElection() {

	
		while (true) {
			system("cls");

			cout << "\n=== CREATE NEW ELECTION ===\n";
			cout << "\nEnter Election ID: ";
			getline(cin, elections[numberofelections].ID);
			if (checkIdExistanceoForElection(elections[numberofelections].ID)) {

				cout << "\n\nthis Id already exists! press any key to try again......\n";
				system("pause>0");

			}
			else
				break;
		}
	 


	cout << "Enter Election Name: ";
	getline(cin, elections[numberofelections].Name);

	cout << "Enter Election Description: ";
	getline(cin, elections[numberofelections].Description);


	cout << "\n=== ADD NOMINEES (MAX 5) ===";
	for (int i = 0; i < 5; i++) {
		cout << "\nEnter Nominee #" << i + 1 << " name (or 'done' to finish): ";
		string nominee;
		getline(cin, nominee);

		if (nominee == "done") {
			if (i == 0) {
				cout << "You must add at least one nominee!\n";
				i--; 
				continue;
			}
			break;
		}

		elections[numberofelections].Nominees[i] = nominee;
		elections[numberofelections].numberofnominees++;
	}


	cout << "\n=== ADD ALLOWED VOTER CODES (MAX 6) ===";
	for (int i = 0; i < 6; i++) {
		cout << "\nis code number " << i  << " Allowed? (y/n)";
		string code;
		getline(cin, code);

		
		if (code == "y"||code=="Y")
			elections[numberofelections].AllowedVotersCode[i] = "1";
		else
			elections[numberofelections].AllowedVotersCode[i] = "0";

	}
	currentadmin.ElectionIds.push_back(elections[numberofelections].ID);
	numberofelections++;
	AddElectionDataToFile();
	

	for (int i = 0; i < numberofadmins; i++) {
		if (currentadmin.id == admins[i].id) {
			admins[i] = currentadmin;
			break;
		}
	}
	AddAdminsDataToFile();


	cout << "\nElection created successfully!\n";
	cout << "\n\nPRESS ANY KEY TO GO BACK TO MAIN MENU......\n\n";
	system("pause>0");
	
	

}







//electors functions section/////////////////
stelector storedata(string line) {
	stelector e;
	string splitedstring[10];
	SplitString(splitedstring,  bigDelma,line);

	e.id = splitedstring[0];
	e.name = splitedstring[1];
	e.password = splitedstring[2];
	e.email = splitedstring[3];
	e.address = splitedstring[4];
	e.phone = splitedstring[5];
	SplitStringfornominees(e.PElections, smallDelma, splitedstring[6], e.NumberOfPElections);
	e.code = splitedstring[7];
	return e;
};


void readdata() {
	fstream myfile;
	myfile.open(ElectorFile, ios::in);
	string line;

	NumberOfElectors = 0;
	if (myfile.is_open()) {
		while (getline(myfile, line)) {
			electorsdata[NumberOfElectors] = storedata(line);
			NumberOfElectors++;
		}
		myfile.close();
	}
};

bool E_CheckLogin() {
	string enteredID;
	string enteredPass;
	system("cls");
	cout << "*\n\n";
	cout << "Elector Login\n\n";
	cout << "*\n\n";
	cout << "ID : ";
	cin >> enteredID;
	cout << "\n\nPassword : ";
	cin >> enteredPass;
	cout << "\n__";
	for (int i = 0; i < NumberOfElectors; i++) {

		if (enteredID == electorsdata[i].id && enteredPass == electorsdata[i].password) {
			//we did this part to store the current elector data bec we will need it in the code next
			currentelector = electorsdata[i];
			return true;
		}
	}
	return false;
}



void E_Login()
{
	if (E_CheckLogin()) {
		system("color 2F");
		cout << "\n\n\nLogin Successful. Welcome!" << endl;
		cout << "\n\n PRESS ANY KEY TO CONTINOUE.......\n";
		system("pause>0");
		
		system("color 07");
		ElectorMainWindow();
	}

	else {
		system("color 4F");
		cout << "\n\n\nIncorrect ID or Password. Please try again.\nPress any Key to Retry" << endl;
		cin.get();
		cin.ignore();
		system("color 07");
		E_Login();

	}
}

bool ElecatorIdCheck(string id) {
	for (int i = 0; i < NumberOfElectors; i++) {
		if (id == electorsdata[i].id) {
			return true;
		}
	}
	return false;
}

string ConvertElectorToLine(stelector elector) {
	string line = "";
	line += elector.id + bigDelma;
	line += elector.name + bigDelma;
	line += elector.password + bigDelma;
	line += elector.email + bigDelma;
	line += elector.address + bigDelma;
	line += elector.phone + bigDelma;
	for (int i = 0; i < elector.NumberOfPElections; i++) {
		if (elector.NumberOfPElections - 1 > i)
			line += elector.PElections[i] + smallDelma;
		else
			line += elector.PElections[i] + bigDelma;

	}
	line += elector.code;
	return line;
}
void SaveElectorDataToFile() {
	ofstream myfile(ElectorFile, ios::out);
	if (myfile.is_open()) {
		for (int i = 0; i < NumberOfElectors; i++) {
			string line = ConvertElectorToLine(electorsdata[i]);
			myfile << line << "\n";
		}
		myfile.close();
	}
}



void E_signup() {
	
	stelector newElector;
	
	do {
		system("cls");
		cout << "elector sign up:\n";
		cout << "------------------\n\n";
		cout << "enter your ID:\n";
		cin >> newElector.id;
		if (ElecatorIdCheck((newElector.id))) {
			cout << "\nThis ID already exists.\n press any key to try again.....\n";
			system("pause>0");
		}
	} while (ElecatorIdCheck(newElector.id));
	cout << "Enter Name: ";
	cin.ignore();
	getline(cin, newElector.name);
	cout << "do you want us to generate a random password for you?(y/n)\n";
	string choice;
	getline(cin, choice);
	if (choice == "y" || choice == "Y") {

		 newElector.password = GenerateKey(enCharType::MixChars);
		 cout << "password generated successfully...\n";
		 cout << "Password = " << newElector.password << endl;

	}
	else {
		cout << "Enter Password: ";
		getline(cin, newElector.password);
	}
	cout << "Enter Email: ";
	getline(cin, newElector.email);
	cout << "Enter Address: ";
	getline(cin, newElector.address);
	cout << "Enter Phone: ";
	getline(cin, newElector.phone);

	for (int i = 0; i < 10; i++) {
		cout << "\nEnter the elections id you have participate at #" << i + 1 << " id (or 'done' to finish): ";
		string id;
		getline(cin, id);

		if (id == "done") 
			break;
		

		newElector.PElections[i] = id;
		newElector.NumberOfPElections++;
	}


	cout << "Enter Code: ";
	getline(cin, newElector.code);
	electorsdata[NumberOfElectors] = newElector;
	NumberOfElectors++;
	SaveElectorDataToFile();

	cout << "\n\n YOUR ELECTOR REGESTERATION IS DONE SUCCSEFULLY....\n\n";
	cout << " PRESS L TO TRANSFER TO LOGIN WINDOW\n";
	system("pause>0");
	


}


void E_AuthenticationMainMenu() {
	short int choice;

	cout << "\n\n";
	cout << "[1]  Login Elector" << endl;
	cout << "[2]  Sign up Elector" << endl;
	cout << "[3]  Return To The Previous Window" << endl;
	cout << "\n\n**\n";
	cout << "Your Choice : ";

	cin >> choice;
	while (choice < 1 || choice>3) {
		cout << "Invalid Choice Enter Correct One 1/2/3\n";
		cout << "Your Choice : ";
		cin >> choice;
	}

	switch (choice) {
	case 1:
		E_Login(); break;

	case 2:
		E_signup();
		E_Login();
		break;
	case 3:
		system("cls");
		WelcomeWindow();
		break;

	}
}


void EditElectorData() {
	system("cls");
	cout << "============================================\n";
	cout << "          EDIT YOUR PROFILE INFORMATION\n";
	cout << "============================================\n\n";


	cout << "Current Profile Details:\n";
	cout << "------------------------\n";
	cout << "[1] Name:     " << currentelector.name << endl;
	cout << "[2] Password: " << currentelector.password << endl; 
	cout << "[3] Address:  " << currentelector.address << endl;
	cout << "[4] Email:    " << currentelector.email << endl;
	cout << "[5] Phone:    " << currentelector.phone << endl;
	cout << "------------------------\n\n";

	char continueEditing;
	do {
		cout << "What would you like to edit? (1-5):\n";
		cout << "1. Name\n";
		cout << "2. Password\n";
		cout << "3. Address\n";
		cout << "4. Email\n";
		cout << "5. Phone\n";
		cout << "0. Cancel and return to menu\n";
		cout << "Your choice: ";

		int choice = ReadIntNumberBetween(0, 5);
		if (choice == 0) {
			return; 
		}

		cin.ignore(); 

		switch (choice) {
		case 1:
			cout << "\nEnter new name: ";
			getline(cin, currentelector.name);
			break;
		case 2:
			cout << "\nEnter new password: ";
			getline(cin, currentelector.password);
			break;
		case 3:
			cout << "\nEnter new address: ";
			getline(cin, currentelector.address);
			break;
		case 4:
			cout << "\nEnter new email: ";
			getline(cin, currentelector.email);
			break;
		case 5:
			cout << "\nEnter new phone number: ";
			getline(cin, currentelector.phone);
			break;
		}

		
		for (int i = 0; i < NumberOfElectors; i++) {
			if (currentelector.id == electorsdata[i].id) {
				electorsdata[i] = currentelector;
				break;
			}
		}

		cout << "\nChange saved successfully!\n";
		cout << "Would you like to edit another field? (y/n): ";
		cin >> continueEditing;
		cin.ignore();
		cout << endl;

	} while (tolower(continueEditing) == 'y');


	SaveElectorDataToFile();

	cout << "\nAll changes have been saved.\n";
	cout << "Press any key to return to the main menu...";
	system("pause>0");
}

void switchaccountatelector() {
	system("cls");
	cout << "\n=== SWITCH ACOUNT  ===\n";
	char choice;

	for (int i = 0; i < numberofadmins; i++) {

		if (currentelector.id == admins[i].id) {

			cout << "\nyou indeed have an admin account, are you sure you want to switch the account ?(y/n) ";
			cin >> choice;
			cin.ignore();

			if (tolower(choice) == 'y') {

				currentadmin = admins[i];
				cout << "SWITCHING WILL BE DONE IN A FEW SECONDS......\n\n";
				cout << "THE SWITCH IS DONE SUCESSFULLY PLEASE PRESS ANY KEY TO GO TO MAIN MENU OF THE admin.......\n\n";
				system("pause>0");
				//when you make the main menu for the admin you have to put it in here DO NOT FORGET
				AdminMainWindow();
			}
			else
				break;
		}



	}
	cout << "\n\nSWITCHING FAILED\n\n";
	cout << "\n\nPRESS ANY KEY TO GO BACK TO elector MAIN MENU.....\n\n";
	system("pause>0");
	


}





void ShowElections(string electorCode) {
	cin.ignore();
	system("cls");
	bool found = false;
	cout << "Elections you are allowed to vote in:\n";
	cout << "-------------------------------------\n\n";
	for (int i = 0; i < numberofelections; i++) {
		if (elections[i].AllowedVotersCode[stoi(electorCode)] == "1" && !elections[i].finished) {
			bool hasVoted = false;
			for (int j = 0; j < currentelector.NumberOfPElections; j++) {
				if (currentelector.PElections[j] == elections[i].ID) {
					hasVoted = true;
					break;
				}
			}

			if (!hasVoted) {

				cout << "Election ID: " << elections[i].ID << endl;
				cout << "Name: " << elections[i].Name << endl;
				cout << "Description: " << elections[i].Description << endl;
				cout << "----------------------------------------\n";
				found = true;
			}
		}
	}
	if (!found)
		cout << "No elections available for you to vote in at the moment.\n";
}

void vote(stElection& e) {
	system("cls");
	cout << "   Election Details:\n ";
	cout << "----------------------\n";
	cout << "Name: " << e.Name << "\n";
	cout << "ID: " << e.ID << "\n";
	cout << "Description: " << e.Description << "\n\n";


	cout << "Vote for one of the following nominees:\n";
	for (int i = 0; i < e.numberofnominees; ++i) {
		cout << "[" << i + 1 << "] " << e.Nominees[i] << endl;
	}
	int choice;
	cout << "your choice: ";
	cin >> choice;

	
	e.Votes[choice - 1]++;
	cout << "\n\n THE VOTE IS DONE SUCESSFULY....\n\n";
	cout << "PRESS ANY KEY TO GO BACK TO ELECTION SELECTION MENU.......\n\n";
	system("pause>0");
}


void StartVoting() {
	system("cls");
	string electorCode = currentelector.code;
	
	while (true) {
		ShowElections(electorCode);
		if (numberofelections == 0) {
			return;
		}
		cout << "Enter Election ID to vote (or write cancel): ";
		string electionId;
		getline(cin, electionId);
		if (electionId == "cancel") {
			break;
		}

		bool hasVoted = false;
		int electionIndex = -1;
		for (int i = 0; i < numberofelections; i++) {
			if (elections[i].ID == electionId) {

				//check election id da mwgod fe elarray bta3 el elections elle 3mlha vote	
				for (int j = 0; j < currentelector.NumberOfPElections; j++) {

					if (currentelector.PElections[j] == electionId) {
						cout << "You have already voted in this election.\n";
						hasVoted = true;
						break;
					}
				}
				if (!hasVoted) {
					electionIndex = i;
				}
				break;
			}
		}



		if (electionIndex != -1) {

			vote(elections[electionIndex]);
			currentelector.PElections[currentelector.NumberOfPElections] = electionId;
			currentelector.NumberOfPElections++;

			for (int i = 0; i < NumberOfElectors; i++) {
				if (electorsdata[i].id == currentelector.id) {
					electorsdata[i] = currentelector;
					SaveElectorDataToFile();
					AddElectionDataToFile();
					break;
				}

			}
		
			
		}
		

	}

}

void ShowElectionsResult() {


	system("cls");

	cout << "\n\n_ELECTION RESULTS_\n\n\n";

	for (int i = 0; i < currentelector.NumberOfPElections; i++) {

		string ElectionID = currentelector.PElections[i];

		for (int j = 0; j < numberofelections; j++)
		{

			if (ElectionID == elections[j].ID && elections[j].finished) {

				cout << "\n\n";

				cout << "\n**\n\n";
				cout << setw(20) << left << "Election ID:" << elections[j].ID << endl << endl;

				cout << setw(20) << left << "Election Name:" << elections[j].Name << endl;


				cout << endl << setw(20) << left << "Nominees:";


				for (int k = 0; k < elections[j].numberofnominees; k++)
				{
					if (k != elections[j].numberofnominees - 1) {
						cout << elections[j].Nominees[k] << " - ";
					}
					else
						cout << elections[j].Nominees[k];

				}

				cout << endl << endl << setw(20) << left << "Votes:";

				int winnervote = 0;
				int winnervoteindex = 0;

				for (int l = 0; l < elections[j].numberofnominees; l++)
				{
					if (l != elections[j].numberofnominees - 1) {
						cout << elections[j].Votes[l] << " - ";
					}
					else
						cout << elections[j].Votes[l];

					if (winnervote < elections[j].Votes[l]) {

						winnervote = elections[j].Votes[l];
						winnervoteindex = l;

					}





				}

				cout << endl << endl << setw(20) << left << "Winner is: " <<
					elections[j].Nominees[winnervoteindex] << " with number of votes: " << winnervote;


				cout << "\n\n**\n\n";

			}


		}

		cout << endl << endl << endl;
	}

	cout << "THE PROCESS IS DONE SUCESSFULY.....\n\n";
	cout << "PRESS ANY KEY TO GO BACK TO MAIN MENU\n\n";
	system("pause>0");
}






//////////////////////////////////////////////////////////////////////////////////












//ADMIN FUNCTIONS PART////////////////////////////////////////////////////////////

stElectionAdmin ConvertLineIntoStructForAdmin(string line) {

	stElectionAdmin admin;

	string splitedstring[10];
	SplitString(splitedstring, bigDelma, line);

	admin.id = splitedstring[0];
	admin.name = splitedstring[1];
	admin.password = splitedstring[2];
	admin.email = splitedstring[3];
	admin.address = splitedstring[4];
	admin.phone = splitedstring[5];

	
	admin.ElectionIds=VSplitString( smallDelma, splitedstring[6]);
	
	admin.permissions = stoi(splitedstring[7]);

	return admin;
}

void ReadAdminsData() {

	int indx = 0;
	fstream myfile;
	myfile.open(AdminFile, ios::in);
	numberofadmins = 0;

	if (myfile.is_open()) {
		string line = "";
		while (getline(myfile, line)) {

			admins[indx] = ConvertLineIntoStructForAdmin(line);
			indx++;
			numberofadmins++;

		}

		myfile.close();
	}




}

bool ACheckLogIn(string Id,string Password) {

	for (int i = 0; i < numberofadmins; i++) {

		if (admins[i].id == Id) {
			if (admins[i].password == Password) {
				//we did this part to store the current admin data bec we will need it in the code next
				currentadmin = admins[i];
				return 1;
			}
			else
				return 0;
		}

	}

	return 0;
}


void ALogInWindow() {
	system("cls");
	cout << " + -------------------------------------- - +\n";
	cout << " |  ADMIN LOGIN   | \n";
	cout << " + -------------------------------------- - +\n";
	string Id;
	cout << " | Id: ";
	getline(cin, Id);
	cout << " | Password: ";
	string password;
	getline(cin, password);
	cout << " + -------------------------------------- - +\n\n\n";

	if (ACheckLogIn(Id,password)) {
		system("color 27");
		cout << "\nLog in is done successfully press any key to transfer to main menu\n";
		cin.ignore();
		
		system("color 07");
		AdminMainWindow();
	}
	else {
		system("color 4F");
		cout << "Wrong Id or Password please try again\npress any key to try again\n";
		system("pause>0");
		ALogInWindow();

	}


}

string AConvertRecordToLine(stElectionAdmin Admin, string Delma,string smallDelma) {

    string line = "";
	

    line += Admin.id + Delma;
    line += Admin.name + Delma;
    line += Admin.password+Delma ;
	line += Admin.email + Delma;
	line += Admin.address + Delma;
	line += Admin.phone + Delma;
	int countelectionids = 1;


	for (string ElectionId : Admin.ElectionIds) {
		if (Admin.ElectionIds.size() >countelectionids )
			line += ElectionId + smallDelma;
		else
			line += ElectionId + Delma;
		countelectionids++;
	}

    line += to_string(Admin.permissions);
    
     
	return line;

}

void AddAdminsDataToFile() {



	fstream myfile;
	myfile.open(AdminFile, ios::out);


	if (myfile.is_open()) {
		for (int i = 0; i < numberofadmins;i++) {


			myfile << AConvertRecordToLine(admins[i], "#//#",",") << "\n";


		}
		myfile.close();
	}

}



void ASignUpWindow() {
	stElectionAdmin NewAdmin;

	system("cls");
	cout << " + -------------------------------------- - +\n";
	cout << " |  ADMIN SIGN UP   | \n";
	cout << " + -------------------------------------- - +\n";
	
	cout << " | Id: ";
	getline(cin, NewAdmin.id);
	for (int i = 0; i < numberofadmins; i++) {

		if (NewAdmin.id == admins[i].id) {
			cout << "THIS ID ALREADY EXISTS\n\n";
			cout << "PRESS ENTER TO TRY AGAIN......\n";
			system("pause>0");
			ASignUpWindow();
			break;

		}
	}
	cout << "\n | do you want us to generate a random password for you?(y/n) ";
	string choice1;
	getline(cin, choice1);
	if (choice1 == "y" || choice1 == "Y") {

		NewAdmin.password = GenerateKey(enCharType::MixChars);
		cout << " | password generated successfully...\n\n";
		cout << " | Password: " << NewAdmin.password << endl;

	}
	else {
		cout << " | Password: ";
		getline(cin, NewAdmin.password);
	}
	
	cout << " | Name: ";
	getline(cin , NewAdmin.name);
	cout << " | Email: ";
	getline(cin, NewAdmin.email);
	cout << " | Address: ";
	getline(cin, NewAdmin.address);
	cout << " | Phone: ";
	getline(cin, NewAdmin.phone);
	


		NewAdmin.ElectionIds.push_back("");
	


	NewAdmin.permissions=-1;


	admins[numberofadmins] = NewAdmin;
	numberofadmins++;

	AddAdminsDataToFile();


	cout << "\n\n YOUR ADMIN REGESTERATION IS DONE SUCCSEFULLY....\n\n";
	cout << " PRESS L TO TRANSFER TO LOGIN WINDOW\n";
	system("pause>0");
	ALogInWindow();


}

void ShowCreatedElection() {

	for (string id : currentadmin.ElectionIds) {
		for (int i = 0; i < numberofelections; i++) {
			if (elections[i].ID == id) {
				cout << "election id:" << elections[i].ID << "\n\n";
				cout << "name:" << elections[i].Name << "\n\n";
				cout << "description:" << elections[i].Description << "\n\n";
				cout << "number of nominees:" << elections[i].numberofnominees << "\n\n";
				cout << "nominees:\n";
				for (int j = 0; j < elections[i].numberofnominees; j++) {
					cout << "[" << j + 1 << "] " << elections[i].Nominees[j] << "(votes : " << elections[i].Votes[j] << endl<<endl;
				}
				cout << "status: ";
				if (elections[i].finished == '1') {
					cout << "finished\n\n";
				}
				else {
					cout << "not finished\n\n";
				}
				cout << "--------------------------------\n\n";

			}

		}
	}

}


void Editelections() {
	cin.ignore();
	system("cls");
	cout << "\n=== EDIT ELECTION DESCRIBTION ===\n\n";
	ShowCreatedElection();
	
	cout << "please enter the election id you want to change: ";
	string chossenID;
	getline(cin, chossenID);
	cout << "\n\n";
	system("cls");
	for (string id : currentadmin.ElectionIds) {
		if (chossenID == id) {
			for (int i = 0; i < numberofelections; i++) {
				if (chossenID == elections[i].ID) {
					cout << "information of this chosen id before editing\n\n";
					cout << "[1] ID : " << elections[i].ID << "\n\n";
					cout << "[2]Name:" << elections[i].Name << "\n\n";
					cout << "[3]Description:" << elections[i].Description << "\n\n";
					cout << "[4]number of nominees:" << elections[i].numberofnominees << endl;
					cout << "[5]nominees:\n";
					for (int j = 0; j < elections[i].numberofnominees; j++) {
						cout << "[" << j + 1 << "] " << elections[i].Nominees[j] << "(votes : " << elections[i].Votes[j] << endl;
					}




					cout << "\nEnter new description : ";
					getline(cin, elections[i].Description);





					AddElectionDataToFile();
					cout << "\n\nTHE DESCRIBTION EDIT IS DONE SUCESSFULY......\n\n";
					cout << "PRESS ANY KEY TO GO BACK TO MAIN MENU......\n\n";
					system("pause>0");

					return;
				}
			}
		}
	}
	cout << "\n\nTHE ID YOU HAVE ENTERED IS WRONG....\n\n";
	cout << "PRESS ANY KEY TO GO BACK TO MAIN MENU......\n\n";
	system("pause>0");

}

enum eAuthenticationSellection{Alogin=1,Asignup=2,backtoEorA};


void AauthenticationImplementation(int choice) {

	switch (eAuthenticationSellection(choice)) {

	case eAuthenticationSellection::Alogin:
		ALogInWindow();
		break;
	case eAuthenticationSellection::Asignup:
		ASignUpWindow();
		break;
	case eAuthenticationSellection::backtoEorA:
		//remmember when you make function that selects between elector and admin to return to it from here!!!!!!!!
		system("cls");
		WelcomeWindow();
		break;



     

	}


}


void switchaccountatadmin() {
	
	system("cls");
	

	cout << "\n=== SWITCH ACOUNT  ===\n";
	char choice;
	
	for (int i = 0; i < NumberOfElectors; i++) {

		if (currentadmin.id == electorsdata[i].id) {

			cout << "\nyou indeed have an elector account, are you sure you want to switch the account ?(y/n) ";
			cin >> choice;
			cin.ignore();

			if (tolower(choice) == 'y') {

				currentelector = electorsdata[i];
				cout << "SWITCHING WILL BE DONE IN A FEW SECONDS......\n\n";
				cout << "THE SWITCH IS DONE SUCESSFULLY PLEASE PRESS ANY KEY TO GO TO MAIN MENU OF THE ELECTOR.......\n\n";
				system("pause>0");
				//when you make the main menu for the ELECTOR you have to put it in here DO NOT FORGET
				ElectorMainWindow();
			}
			else {
			
				break;
			}
		}
		
		

	}
	cout << "\nSWITCHING FAILED\n\n";
	cout << "\n\nPRESS ANY KEY TO GO BACK TO ADMIN MAIN MENU.....\n\n";
	cin.ignore();
	system("pause>0");
	




}

void EditAdminData() {
	system("cls");
	cout << "\n=== EDIT ADMIN INFORMATION ===\n\n";
	cout << "[1] Name: " << currentadmin.name << endl;
	cout << "[2] Password: " << currentadmin.password << endl;
	cout << "[3] Email: " << currentadmin.email << endl;
	cout << "[4] Address: " << currentadmin.address << endl;
	cout << "[5] Phone: " << currentadmin.phone << endl;

	cout << "\nSelect what you want to edit (1-5) or 0 to cancel: ";
	int choice = ReadIntNumberBetween(0, 5);
	cin.ignore();

	if (choice == 0) {
		AdminMainWindow();
		return;
	}

	switch (choice) {
	case 1: {
		cout << "Enter new name: ";
		getline(cin, currentadmin.name);
		break;
	}
	case 2: {
		cout << "Enter new password: ";
		getline(cin, currentadmin.password);
		break;
	}
	case 3: {
		cout << "Enter new email: ";
		getline(cin, currentadmin.email);
		break;
	}
	case 4: {
		cout << "Enter new address: ";
		getline(cin, currentadmin.address);
		break;
	}
	case 5: {
		cout << "Enter new phone: ";
		getline(cin, currentadmin.phone);
		break;
	}
	}


	for (int i = 0; i < numberofadmins; i++) {
		if (admins[i].id == currentadmin.id) {
			admins[i] = currentadmin;
			break;
		}
	}


	AddAdminsDataToFile();

	cout << "\nInformation updated successfully!\n";
	cout << "Press any key to go back to main menu...\n\n";
	system("pause>0");

}


void finishedelection() {
	system("cls");
	char finish;
	cin.ignore();
	do {
		bool foundAny = false;

		cout << "Elections you can finish:\n";
		cout << "--------------------------\n";


		for (string id : currentadmin.ElectionIds) {
			for (int i = 0; i < numberofelections; i++) {
				if (elections[i].ID == id && !elections[i].finished) {
					cout << "Election ID: " << elections[i].ID << " | Name: " << elections[i].Name << endl << endl;
					foundAny = true;
				}
			}
		}

		if (!foundAny) {
			cout << "No open elections available to finish.\n";
			break;
		}

		cout << "\nChoose the election you want to end (Enter ID): ";
		string wantedID;
		getline(cin, wantedID);

		bool electionFound = false;

		for (int i = 0; i < numberofelections; i++) {
			if (wantedID == elections[i].ID && !elections[i].finished) {
				elections[i].finished = 1;
				AddElectionDataToFile();
				cout << "\n\nYou finished this election successfully.\n";
				electionFound = true;
				break;
			}
		}

		if (!electionFound) {
			cout << "Invalid Election ID or Election already finished.\n";
		}

		cout << "\nDo you want to finish another election? (y/n): ";
		cin >> finish;
		cin.ignore();

	} while (tolower(finish) == 'y');

	cout << "\n\nTHE PROCESS IS DONE SUCSEFULLY.....\n\n";
	cout << "PLEASE ENTER ANY KEY TO GO BACK TO MAIN MENU.....\n\n";
	system("pause");
}









void Deletenominee() {
	system("cls");

	string id;

	cout << "\n\n==============================================\n\n";

	cout << "Enter ID Of Election You Want To Delete Nominee from\n ";

	cout << "\n\n==============================================\n\n";
	cout << "Election ID : ";
	cin >> id;
	int indx = -1;

	for (int i = 0; i < currentadmin.ElectionIds.size(); i++)
	{

		if (currentadmin.ElectionIds[i] == id)
		{
			for (int j = 0; j < numberofelections; j++)
				if (id == elections[j].ID) {
					indx = j;
					break;
				}

			break;
		}
	}

	if (indx != -1) {


		cout << "\nCorrect Id, You Are Allowed To Delete Any Nominee in This Election";

		cout << "\n\n_LIST OF NOMINEES\n\n";
		for (int k = 0; k < elections[indx].numberofnominees; k++)
		{
			cout << "[" << k + 1 << "] " << elections[indx].Nominees[k] << endl;
		}
		int choice;
		cout << "Enter Nominee Number To Delete : ";
		cin >> choice;

		for (int k = choice - 1; k < elections[indx].numberofnominees - 1; k++)
		{
			elections[indx].Nominees[k] = elections[indx].Nominees[k + 1];
			elections[indx].Votes[k] = elections[indx].Votes[k + 1];
		}

		elections[indx].numberofnominees--;
		AddElectionDataToFile();
		
		cout << "\nNominee Deleted Successfully\n";
		cout << "Press Any Key To See The Remaining Nominees \n";

		system("pause>0");

		cout << "\n\nRemaining Nominees:\n";
		for (int i = 0; i < elections[indx].numberofnominees; i++) {
			cout << "[" << i + 1 << "] " << elections[indx].Nominees[i] << "\n";
		}

	}

	else {

		cout << "\nInCorrect Id, You Are Not Allowed To Delete Any Nominee in This Election\n";
		cout << "\n\nPRESS ANY KEY TO GO BACK TO MAIN MEUN\n\n";
		system("pause>0");
		return;
		
	}
	cout << "\n\nPROCESS IS DONE........\n\n";
	cout << "\n\nPRESS ANY KEY TO GO BACK TO MAIN MEUN..........\n\n";
	system("pause>0");
}









void AuthenticationWindowForAdmin() {

	int choice;

	cout << "===========================================\n";
	cout << "===========================================\n";
	cout << "    [1] Login Admin\n";
	cout << "    [2] sign up new Admin\n";
	cout << "    [3] return to the welcome window\n";
	cout << "-------------------------------------------\n";
	cout << "Enter your choice: ";
	cin >> choice;
	cin.ignore(numeric_limits<streamsize>::max(), '\n');

	AauthenticationImplementation(choice);

}




/////////////////////////////////////////////////////////////////////////////////////////






//THE MAIN FUNCTIONS FOR THE PROJECT//////////////////////////////////////////////////////

void programends() {
	cout << "\n------------------------------------------\n";
	cout << "\tProgram Ends :)";
	cout << "\n------------------------------------------\n\n";

}


enum enAdminChoice {
	EditInformation = 1,
	CreateElection = 2,
	EditElectionDescription = 3,
	DeleteNominee = 4,
	EndElection = 5,
	SwitchAccounttoelector=6,
	Logout = 7
};

void AdminWindowImplementation(int choice) {
	switch (enAdminChoice(choice)) {
	case enAdminChoice::EditInformation:
		system("cls");
		EditAdminData();
		AdminMainWindow();
		break;

	case enAdminChoice::CreateElection:
		system("cls");
		cin.ignore();
		CreateNewElection();
	
		AdminMainWindow();
		break;

	case enAdminChoice::EditElectionDescription:
		system("cls");
		Editelections();
		AdminMainWindow();
		break;

	case enAdminChoice::DeleteNominee:
		system("cls");
		Deletenominee();
		AdminMainWindow();
		break;

	case enAdminChoice::EndElection:
		system("cls");
		finishedelection();
		AdminMainWindow();
		break;
	case enAdminChoice::SwitchAccounttoelector:
	
		switchaccountatadmin();
		AdminMainWindow();
		break;

	case enAdminChoice::Logout:
		system("cls");
		WelcomeWindow();
		break;
	}
}

void AdminMainWindow() {
	system("cls");

	cout << "+------------------------------------------------+\n";
	cout << "|             VOTING SYSTEM - ADMIN PANEL        |\n";
	cout << "+------------------------------------------------+\n";
	cout << "|                                                |\n";
	cout << "|  Please select an option:                      |\n";
	cout << "|                                                |\n";
	cout << "|  1. Edit Information                           |\n";
	cout << "|  2. Create an Election                         |\n";
	cout << "|  3. Edit Election's Description                |\n";
	cout << "|  4. Delete a Nominee                           |\n";
	cout << "|  5. End the Election                           |\n";
	cout << "|  6. Switch Account                             |\n";
	cout << "|  7. Log Out                                    |\n";
	cout << "|                                                |\n";
	cout << "+------------------------------------------------+\n";
	cout << "|  Enter your choice (1-7):                      |\n";
	cout << "+------------------------------------------------+\n";

	int choice = ReadIntNumberBetween(1, 7);

	AdminWindowImplementation(choice);
}







enum enElectorChoice {
	eEditInformation = 1,
	VoteInElection = 2,
	ViewResults = 3,
	SwitchAccounttoadmin=4,
	LogOut = 5
};

void ElectorWindowImplementation(int choice) {

	switch (enElectorChoice(choice)) {

	case enElectorChoice::eEditInformation:
		system("cls");
		EditElectorData();
		ElectorMainWindow();
		break;

	case enElectorChoice::VoteInElection:
		system("cls");
		StartVoting();
		ElectorMainWindow();
		break;

	case enElectorChoice::ViewResults:
		system("cls");
		ShowElectionsResult();
		ElectorMainWindow();
		break;
	case enElectorChoice::SwitchAccounttoadmin:
		system("cls");

		switchaccountatelector();
		ElectorMainWindow();
		break;


	case enElectorChoice::LogOut:
		system("cls");
		WelcomeWindow(); 
		break;
	}
}









void ElectorMainWindow() {
	system("cls");

	cout << "+------------------------------------------------+\n";
	cout << "|            VOTING SYSTEM - ELECTOR PANEL       |\n";
	cout << "+------------------------------------------------+\n";
	cout << "|                                                |\n";
	cout << "|  Please select an option:                      |\n";
	cout << "|                                                |\n";
	cout << "|  1. Edit Information                           |\n";
	cout << "|  2. Vote in an Election                        |\n";
	cout << "|  3. View Results of Past Elections             |\n";
	cout << "|  4. Switch Account                             |\n";
	cout << "|  5. Log Out                                    |\n";
	cout << "|                                                |\n";
	cout << "+------------------------------------------------+\n";
	cout << "|  Enter your choice (1-5):                      |\n";
	cout << "+------------------------------------------------+\n";

	int choice = ReadIntNumberBetween(1, 5);
	ElectorWindowImplementation(choice);
}








enum enWelcomeChoice { Adminstrator = 1, Elector = 2, Exit = 3 };

void WelcomeWindowImplementation(int choice) {

	switch (enWelcomeChoice(choice)) {

	case enWelcomeChoice::Adminstrator:
		system("cls");
		AuthenticationWindowForAdmin();

		break;
	case enWelcomeChoice::Elector:
		system("cls");
		E_AuthenticationMainMenu();

		break;
	case enWelcomeChoice::Exit:
		system("cls");
		programends();
		break;



	}



}











void WelcomeWindow() {

	readdata();
	ReadAdminsData();
	ReadElections();

	
	cout << "+---------------------------------------+\n";
	cout << "|       VOTING SYSTEM - WELCOME         |\n";
	cout << "+---------------------------------------+\n";
	cout << "|                                       |\n";
	cout << "|  Please select your role:             |\n";
	cout << "|                                       |\n";
	cout << "|  1. Administrator                     |\n";
	cout << "|  2. Elector/Voter                     |\n";
	cout << "|  3. Exit                              |\n";
	cout << "|                                       |\n";
	cout << "+---------------------------------------+\n";
	cout << "|  Enter your choice (1-3):             |\n";
	cout << "+---------------------------------------+\n";
	int choice = ReadIntNumberBetween(1, 3);
	WelcomeWindowImplementation(choice);


}



////////////////////////////////////////////////////////////////////////////////


int main()
{
	srand((unsigned)time(NULL));

	ios_base::sync_with_stdio(false);
	cin.tie(nullptr);


	WelcomeWindow();
	



















	return 0;
}





